#include <iostream>
#include <string>
#include <windows.h>  // for console visuals
#include <fstream>

void setTxtColor(int colorValue);
std::string addSpaces(int amt);

struct Student {
  std::string fullName = "";

  int totalStaffSelected = 0; // amt of staff chosen
  std::string* selectedStaffs = nullptr;  // dynamic array of staff names

  std::string studentVoteForVeritas = ""; // name of vote
};

class StudentData {
 private:
  Student* studentList = nullptr;  // pointer to dyn array of Student objects
  int totalGraduatingStuds = 0; // total of students in the list
  int currentStudentCount = 0; // incrememnter when adding the students

  int longestStudentName = 0;  // helps beutifying the console

  struct veritasCandidate {
    std::string studentFullName = "";
    int votes = 0;
  };
 public:
  StudentData() {};
  ~StudentData() {
    delete[] studentList;  // free memory
    studentList = nullptr;
  };

  void addStudent(Student stud) {
    if (stud.fullName.length() > longestStudentName) {
      longestStudentName = stud.fullName.length();
    }
    studentList[currentStudentCount] = stud;
    ++currentStudentCount;
  }

  void createStudentList(int maxSize) {
    totalGraduatingStuds = maxSize;
    studentList = new Student[maxSize];
  }

  void printStudentData() {
    std::cout << "#   | FULLNAME | #STAFF SELECTED | VERIDAS CHOICE\n";
    for (int i = 0; i < totalGraduatingStuds; ++i) {
      Student& thisStudent = studentList[i];
      std::cout << i+1 <<  addSpaces(4 - std::to_string(i+1).length())<< "| " << thisStudent.fullName
                << addSpaces(longestStudentName - thisStudent.fullName.length())
                << " | " << thisStudent.totalStaffSelected
                << addSpaces(4 - std::to_string(thisStudent.totalStaffSelected).length())
                << "| " << thisStudent.studentVoteForVeritas << '\n';
    }
  }

  void displayVeritasResults() {
    veritasCandidate* candidates = new veritasCandidate[totalGraduatingStuds];
    int candidateCount = 0;
    for (int i = 0; i < totalGraduatingStuds; ++i) {
      std::string votedFor = studentList[i].studentVoteForVeritas;
      bool found = false;
      // check if the candidate is already in the list
      for (int j = 0; j < candidateCount; ++j) {
        if (candidates[j].studentFullName == votedFor) {
          candidates[j].votes++; // increment their vote
          found = true;
          break;
        }
      }

      // if the candidate is not found, add them to the list
      if (!found) {
        candidates[candidateCount].studentFullName = votedFor;
        candidates[candidateCount].votes = 1;
        candidateCount++;
      }
    }

    // bubble sort
    for (int i = 0; i < candidateCount - 1; ++i) {
      for (int j = 0; j < candidateCount - 1 - i; ++j) {
        if (candidates[j].votes < candidates[j + 1].votes) {
          // swap the candidates
          veritasCandidate temp = candidates[j];
          candidates[j] = candidates[j + 1];
          candidates[j + 1] = temp;
        }
      }
    }

    // results
    std::cout << "  - Veritas Results - \n";
    for (int i = 0; i < candidateCount; ++i) {
        double percentage = (candidates[i].votes / static_cast<double>(candidateCount)) * 100;
      std::cout << candidates[i].studentFullName
                << addSpaces(longestStudentName - candidates[i].studentFullName.length())
                << ": " << candidates[i].votes << " votes (" << percentage
                << "%)\n";
    }

    // deallocate  free
    delete[] candidates;
  };

  int getListSize() { return totalGraduatingStuds; };
};

class Staff {
 private:
  std::string name = "";

  int maxStudentsThisPersonCanStole = 0;

  int studentListMaxSize = 0;
  int currentStudentCount = 0;
  std::string* studentList = nullptr;  // dynamic array of student names

 public:

  // Constructor
  Staff() {}

  // Destructor
  ~Staff() {
    // strings deallocate themselves already so no need to delete string list
  }

  void cinHowManyCanStole() {
      std::cin >> maxStudentsThisPersonCanStole;
  }

  void addStudent(std::string name) {
    studentList[currentStudentCount] = name;
    //std::cout << "added student: " << studentList[currentStudentCount] << '\n';
    ++currentStudentCount;
  }

  void createStudentStringList(int maxSize) {
    studentListMaxSize = maxSize;
    studentList = new std::string[maxSize];
  }

  void setName(std::string &newName) { name = newName; };
  int getMaxStudents() { return studentListMaxSize; };
  std::string getName() { return name; };
};

class StaffData {
 private:
  Staff* staffList = nullptr; // pointer to dyn array of Staff objects
  const int maxSize = 200;    // maximum ammount of staff
  int currentSize = 0;        // current size of the array

  int longestStaffName = 0; // helps beutifying the console
 public:
  // Constructor
  StaffData() {
    staffList = new Staff[maxSize];  // allocate memory for the array
  }

  // Destructor
  ~StaffData() {
    delete[] staffList;  // free memory
    staffList = nullptr;
  }

  void addStaff(Staff &newStaff) {
    staffList[currentSize] = newStaff;
    if (newStaff.getName().length() > longestStaffName) {
      longestStaffName = newStaff.getName().length();
    }
    ++currentSize;
  }
  
  void printAllStaff() {
    std::cout << "________________________________________________________\n";
    setTxtColor(12);
    std::cout << " STAFF LIST";
    setTxtColor(15);
    std::cout << " |" << addSpaces(longestStaffName / 2 - 7);
    setTxtColor(12);
    std::cout << "staff full name";
    setTxtColor(15);
    std::cout << addSpaces(longestStaffName / 2 - 7);
    std::cout << "  | ";
    setTxtColor(12);
    std::cout << "#of chosen students ";
    setTxtColor(15);
    std::cout << "\n";
    for (int i = 0; i < currentSize; ++i) {
      Staff& thisStaff = staffList[i];
      std::cout << "  staff #" << i + 1
                << addSpaces(3 - std::to_string(i + 1).length()) << "| "
                << thisStaff.getName()
                << addSpaces(longestStaffName - thisStaff.getName().length());
      std::cout << " |  " << thisStaff.getMaxStudents();
      std::cout << '\n';
    };
  };

  // gets
  int getListSize() const { return currentSize; }
  Staff getStaff(int index) const { return staffList[index]; };
};

// remove quotes
void removeQuotes(std::string& str) {
  if (!str.empty() && str.front() == '"') {
    str.erase(0, 1);
  }
  if (!str.empty() && str.back() == '"') {
    str.pop_back();
  }
};

void removeWhitespace(std::string& str) {
  // remove leading whitespace
  size_t start = 0;
  while (start < str.length() && std::isspace(str[start])) {
    start++;
  }
  str.erase(0, start);

  // remove trailing whitespace
  size_t end = str.length();
  while (end > 0 && std::isspace(str[end - 1])) {
    end--;
  }
  str.erase(end);
}

// dynamic array of strings
std::string* stringToWords(std::string& line, int& arraySize) {
    arraySize = 1; // start with 1 for the case when the line is empty
    for (char c : line) {
        if (c == ',') {
            arraySize++;
        }
    }

    // allocate memory for the array of strings
    std::string* wordsArray = new std::string[arraySize];

    // get words from the line
    int wordIndex = 0;
    size_t startPos = 0; // int too small and length() returns a size_t
    for (size_t i = 0; i <= line.length(); ++i) {
        if (i == line.length() || line[i] == ',') {
            wordsArray[wordIndex++] = line.substr(startPos, i - startPos); // gets slice (word)
            startPos = i + 1;
        }
    }

    return wordsArray;
}

void parseStaffFileToData(StaffData& staffData, std::string staffFilePath) {
  std::ifstream file(staffFilePath);
  if (!file.is_open()) {
    std::cout << "[ERROR READING] staff csv" << '\n';
    exit(1);
  }

  std::string line = "";
  // loops through each line
  while (!file.eof()) {
    std::getline(file, line);
    if (!line.empty()) { // making sure line exists
      int dynamicArrSz = 0;
      std::string* words = stringToWords(line, dynamicArrSz); // seperates the into words
      if (words[0] == "Timestamp") { // ignore first column
        continue;
      }

      if (dynamicArrSz < 4) {
        std::cout << "[ERROR READING] staff csv - LESS THAN 4 COLUMNS" << '\n';
        exit(1);
      }
      std::string firstName = words[3];
      std::string lastName = words[2];

      // count students
      int wantedStudentsCount = 0;
      for (int i = 4; i < dynamicArrSz; i+=2) {
        if (i > dynamicArrSz) {
          std::cout << "[ERROR READING] in staff csv - student name is odd (multiple names?)"<< '\n';
          exit(1);
        }
        std::string studentLastName = words[i];
        std::string studentFirstName = "";
        if (i < dynamicArrSz) {
          studentFirstName = words[i + 1];
        }
        removeWhitespace(studentFirstName);
        removeQuotes(studentFirstName);
        removeWhitespace(studentLastName);
        removeQuotes(studentLastName);
        if (!studentFirstName.empty() && !studentLastName.empty()) {
          ++wantedStudentsCount;
        }
      }

      Staff newStaff;
      std::string staffFullName = firstName + ' ' + lastName;
      newStaff.setName(staffFullName);
      newStaff.createStudentStringList(wantedStudentsCount);

      // set students
      int countingStuds = 0;
      for (int i = 4; i < dynamicArrSz; i += 2) {
        std::string studentLastName = words[i];
        std::string studentFirstName = "";
        if (i < dynamicArrSz) {
          studentFirstName = words[i + 1];
        }
        removeWhitespace(studentFirstName);
        removeQuotes(studentFirstName);
        removeWhitespace(studentLastName);
        removeQuotes(studentLastName);
        if (!studentFirstName.empty() && !studentLastName.empty()) {
          std::string studentFullName = studentLastName + ' ' + studentFirstName;
          newStaff.addStudent(studentFullName);
          ++countingStuds;
        }
      }

      staffData.addStaff(newStaff);
      delete[] words;  // free
    }
  }
  setTxtColor(10);
  std::cout << "Succesfully";
  setTxtColor(15);
  std::cout << " Allocated " << staffData.getListSize() << " staff members\n";
  file.close(); // close the file
}

void parseStudentFileToData(StudentData &studentData, std::string staffFilePath) {
  std::ifstream file(staffFilePath);
  if (!file.is_open()) {
    std::cout << "[ERROR READING] student csv" << '\n';
    exit(1);
  }

  std::string line = "";
  int studentCounter = 0;
  // loops through each line
  while (!file.eof()) {
    std::getline(file, line);
    if (!line.empty()) {  // making sure line exists
      int dynamicArrSz = 0;
      std::string* words = stringToWords(line, dynamicArrSz);  // seperates the into words
      if (words[0] == "Timestamp") {          // ignore first column
        continue;
      }

      if (dynamicArrSz < 4) {
        std::cout << "[ERROR READING] student csv - LESS THAN 4 COLUMNS" << '\n';
        exit(1);
      }

      std::string lastName = "???";
      std::string firstName = "???";
      if (dynamicArrSz > 1) {  // has more than 1 word
        if (dynamicArrSz > 2) {
          firstName = words[3];
        }
        lastName = words[2];
      };

      removeWhitespace(firstName);
      removeQuotes(firstName);
      removeWhitespace(lastName);
      removeQuotes(lastName);
      if (!firstName.empty() && !lastName.empty()) {
        ++studentCounter;
      }
     
      delete[] words;  // free
    }
  }

  studentData.createStudentList(studentCounter);
  // Reset file pointer to the beginning of the file
  file.clear();
  file.seekg(0);

  // second loop for actually setting the studs
  while (!file.eof()) {
    std::getline(file, line);
    if (!line.empty()) {  // making sure line exists
      int dynamicArrSz = 0;
      std::string* words = stringToWords(line, dynamicArrSz);  // seperates the into words
      if (words[0] == "Timestamp") {          // ignore first column
        continue;
      }
      std::string lastName = words[2];
      std::string firstName = "???";
      if (dynamicArrSz > 1) {  // has more than 1 word
        firstName = words[3];
      };
      removeWhitespace(firstName);
      removeQuotes(firstName);
      removeWhitespace(lastName);
      removeQuotes(lastName);
      if (!firstName.empty() && !lastName.empty()) {
        std::string studentFullName = lastName + ' ' + firstName;
        Student newStudent;
        newStudent.fullName = studentFullName;

        std::string veritasFirstName = words[dynamicArrSz - 1];
        std::string veritasLastName = words[dynamicArrSz - 2];
        removeWhitespace(veritasFirstName);
        removeQuotes(veritasFirstName);
        removeWhitespace(veritasLastName);
        removeQuotes(veritasLastName);
        std::string veritasWielder = veritasLastName + ' ' + veritasFirstName;
        int veridasExistanceAdjust = 0;
        if (!veritasFirstName.empty() && !veritasLastName.empty()) {
          veridasExistanceAdjust = 2; // skip the last 2 words for adding to staff list so it wont add veritas wielder into the students staff list
        }

        newStudent.studentVoteForVeritas = veritasWielder;
        int studentChoseStaffAmt = 0;
        for (int i = 4; i < dynamicArrSz-veridasExistanceAdjust; i += 2) { // -veridasExistanceAdjust cuz veritas exists
          std::string staffLastName = words[i];
          std::string staffFirstName = "";
          if (i < dynamicArrSz) {
            staffFirstName = words[i + 1];
          }
          removeWhitespace(staffFirstName);
          removeQuotes(staffFirstName);
          removeWhitespace(staffLastName);
          removeQuotes(staffLastName);
          //std::string staffFullName = staffLastName + ' ' + staffFirstName;
          if (!staffFirstName.empty() && !staffLastName.empty()) {
            ++studentChoseStaffAmt;
          }
        }
        newStudent.totalStaffSelected = studentChoseStaffAmt;
        newStudent.selectedStaffs = new std::string[studentChoseStaffAmt]; // allocate - string dynamic ararys automaticly deallocate cuz strings

        int incrementer = 0; // reset for reloop to set the above dynamic string array
        for (int i = 4; i < dynamicArrSz - veridasExistanceAdjust; i += 2) {
          std::string staffLastName = words[i];
          std::string staffFirstName = "";
          if (i < dynamicArrSz) {
            staffFirstName = words[i + 1];
          }
          removeWhitespace(staffFirstName);
          removeQuotes(staffFirstName);
          removeWhitespace(staffLastName);
          removeQuotes(staffLastName);
          if (!staffFirstName.empty() && !staffLastName.empty()) {
            std::string staffFullName = staffLastName + ' ' + staffFirstName;
            newStudent.selectedStaffs[incrementer] = staffFullName;
            ++incrementer;
          }
        }

        studentData.addStudent(newStudent);
      }

      delete[] words;  // free
    }
  }

  setTxtColor(10);
  std::cout << "Succesfully";
  setTxtColor(15);
  std::cout << " Allocated " << studentData.getListSize() << " graduating students\n";
  file.close();  // close the file
}

std::string askForFilePath() {
  std::string filePath = "";
  //std::cin.ignore(1000, '\n');  // flush the buffer

  std::getline(std::cin, filePath);
  removeWhitespace(filePath);
  removeQuotes(filePath);

  //std::cin.ignore(1000, '\n');  // flush the buffer
  return filePath;
}

void pairStudentAndStaff(StaffData& staffData, StudentData& studentData) {
  int listSize = staffData.getListSize();
  for (int i = 0; i < listSize; ++i) {
    Staff thisStaff = staffData.getStaff(i);
    std::string firstThingy = std::to_string(i+1) + "/" + std::to_string(listSize);
    std::cout << "#" << firstThingy;
    std::cout << addSpaces(6 - firstThingy.length()) << "| #of students ";
    setTxtColor(3);
    std::cout << thisStaff.getName();
    setTxtColor(15);
    std::cout << " can stole: ";
    thisStaff.cinHowManyCanStole();
  };
  system("PAUSE");
};

int main() {
  std::cout << "\n[Initializing] HTA Grad Helper Program\n";
  StaffData staffData;
  StudentData studentData;

  std::cout << "\nenter ";
  setTxtColor(3);
  std::cout << "staff csv ";
  setTxtColor(15);
  std::cout << "file_path: ";
  std::string staffFilePath = askForFilePath();
  parseStaffFileToData(staffData, staffFilePath);

  std::cout << "enter ";
  setTxtColor(3);
  std::cout << "students csv ";
  setTxtColor(15);
  std::cout << "file_path: ";
  std::string stundetsFilePath = askForFilePath();

  parseStudentFileToData(studentData, stundetsFilePath);

  system("PAUSE");

  std::string input = "";
  do {
    system("CLS");
    std::cout << " - grad halper menu - \n\n"
              << "[1] display all staff\n"
              << "[2] display all students\n"
              << "[3] calculate veritas results\n"
              << "[4] pair student & staff\n"
              << "[5] quit program\n"
              << "\ninput: ";
    std::cin >> input;
    if (input == "1") {
      system("CLS");
      staffData.printAllStaff();
      system("PAUSE");
    } else if (input == "2") {
      system("CLS");
      studentData.printStudentData();
      system("PAUSE");
    } else if (input == "3") {
      system("CLS");
      studentData.displayVeritasResults();
      system("PAUSE");
    } else if (input == "4") {
      pairStudentAndStaff(staffData, studentData);
    }
  } while (input != "5");
  //writeToCSV(staffData);
  return 0;
}

void setTxtColor(int colorValue) {
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorValue);
}

// return a string of spaces of length amt given
std::string addSpaces(int amt) {  // pseudo set w so i get to control it more
  std::string spaces = "";
  for (int i = 0; i < amt; ++i) {
    spaces += ' ';
  }
  return spaces;
}
