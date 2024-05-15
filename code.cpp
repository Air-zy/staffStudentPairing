#include <iostream>
#include <string>
#include <Windows.h> // for PAUSE
#include <fstream>

struct Staff {
  std::string name = "";
  std::string* students; // dynamic array of students
};

class StaffData {
 private:
  Staff* staffList;         // pointer to array of Staff objects
  const int maxSize = 200;  // maximum size of the array
  int currentSize = 0;      // current size of the array or amt of staff

 public:
  // Constructor
  StaffData() {
    staffList = new Staff[maxSize];  // allocate memory for the array
  }

  // Destructor
  ~StaffData() {
    delete[] staffList;  // free memory
  }

  void addStaff(Staff &newStaff) {
    std::cout << '[' << currentSize <<  "] added staff " << newStaff.name << '\n';
    staffList[currentSize] = newStaff;
    ++currentSize;
  }

  // gets
  int getSize() const { return currentSize; }
  Staff getStaff(int index) const { return staffList[index]; };
};

// dynamic array of strings
std::string* stringToWords(std::string& line, int& arraySize) {
  std::string word = "";

  // counting words to allocate the string array to that value
  for (char& c : line) {
    if (c == ' ') {  // the words seperator
      if (!word.empty()) {
        ++arraySize;
        word = "";
      }
    } else {
      word += c;
    }
  }

  std::string* wordsArr = new std::string[arraySize]; // allocate

  // actual setting the array of words
  int wordIndex = 0;
  word = "";
  for (char& c : line) {
    if (c == ' ') { // the words seperatorwordIndex
      if (!word.empty()) {
        if (wordIndex > arraySize) { // overflow
          break;
        }
        if (wordIndex <= arraySize && wordsArr) {
          std::string newWord = word;
          wordsArr[wordIndex].assign(newWord);  // store the word.  TODO: [this might be reading/setting invalid data?]
        }
        ++wordIndex;
        word = ""; // prepare for next word
      }
    } else {
      // add character to the word making
      word += c;
    }
  }
  return wordsArr;
}

void parseStaffFileToData(StaffData& staffData) {
  std::cout << "\n[Reading] in data...\n";
  std::ifstream file("staffPicksData.txt");
  if (!file.is_open()) {
    std::cout << "[ERROR READING] Couldnt open staffPicksData.txt" << '\n';
  }

  std::string line = "";
  // loops through each line
  while (!file.eof()) {
    std::getline(file, line);
    if (!line.empty()) { // making sure line exists
      int dynamicArrSz = 0;
      std::string* words = stringToWords(line, dynamicArrSz); // seperates the into words
      std::string firstName = words[0];
      std::string lastName = "???";
      if (dynamicArrSz > 1) {  // has more than 1 word
        lastName = words[1];
      };
      // > rest of the words are the student names
      Staff newStaff;
      newStaff.name = firstName + ' ' + lastName;
      staffData.addStaff(newStaff);
      delete[] words;  // free
    }
  }

  file.close(); // close the file
}

void writeToCSV(StaffData& staffData) {
  std::cout << "\n[WRITING] in csv...\n";
  std::ofstream file("pairingResults.csv");

  if (!file.is_open()) {
    std::cout << "[ERROR WRITING] Couldnt open pairingResults.txt" << '\n';
    return;
  }

  file << "staff,students\n";
  for (int i = 0; i < staffData.getSize(); ++i) {
    Staff currentStaff = staffData.getStaff(i);
    file << currentStaff.name << ',' << "1'st col stuff" << ',' << "2'nd col stuff" << '\n';
    std::cout << '+';
  }

  std::cout << "\n[SUCCESS]\n";
  file.close(); // close the file
}

int main() {
  std::cout << "\n[Initializing] Program\n";
  StaffData staffData;
  parseStaffFileToData(staffData);
  writeToCSV(staffData);
  return 0;
}
