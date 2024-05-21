#include <iostream>
#include <string>
#include <windows.h>  // for console visuals
#include <algorithm> // sorting and shuffling
#include <random>    // for std::default_random_engine and std::random_device
#include <fstream>

#include <cstdlib> // for std::srand and std::rand
#include <ctime>   // for std::time

void setTxtColor(int colorValue);
void toLowercase(std::string& input);
std::string addSpaces(int amt);
std::string addSpacesButCool(int amt, int visualSize);
int getNumberDigitCount(int number) { return std::to_string(number).length(); }

int compareStrings(std::string& str1, std::string& str2) {
  std::sort(str1.begin(), str1.end());
  std::sort(str2.begin(), str2.end());

  int i = 0, j = 0;
  int similarities = 0;

  while (i < str1.length() && j < str2.length()) {
    if (str1[i] == str2[j]) {
      ++similarities;
      ++i;
      ++j;
    } else if (str1[i] < str2[j]) {
      ++i;
    } else {
      ++j;
    }
  }

  int lessStrengthOnSize = (str1.length() + str2.length()) / 2;
  int result = static_cast<int>(
      (static_cast<double>(similarities) / lessStrengthOnSize) * 100);
  return result;
}


// Globals
static unsigned int global_seed_counter = 0;
const std::string firstNameLastNameSeperator = ", ";

std::mt19937 createRNG() {
  // Increment the global seed counter
  unsigned int counter_value = global_seed_counter++;

  // Seed the random number generator with the current time and the counter value
  std::srand(static_cast<unsigned int>(std::time(nullptr)) + counter_value);

  // Create a random number generator
  std::random_device rd;               // Obtain a random number from hardware
  std::mt19937 g(rd() + std::rand());  // Seed the generato
  return g;
}

struct Student {
  std::string fullName = "";

  int totalStaffSelected = 0; // amt of staff chosen
  std::string* selectedStaffs = nullptr;  // dynamic array of staff names

  std::string studentVoteForVeritas = ""; // name of vote
  std::string matchedWithStaff = ""; // which staff the student was matched with
};

Student NULLSTUDENT = Student();

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
  
  void shuffleStudList() {
    std::shuffle(studentList, studentList + totalGraduatingStuds, createRNG());
  }

  void createStudentList(int maxSize) {
    totalGraduatingStuds = maxSize;
    studentList = new Student[maxSize];
  }

  void printStudentData() {
    std::cout << "#   | FULLNAME | #STAFF SELECTED | VERIDAS CHOICE\n";
    for (int i = 0; i < totalGraduatingStuds; ++i) {
      Student& thisStudent = studentList[i];
      std::cout << i + 1 << addSpaces(4 - getNumberDigitCount(i + 1))
                << "| " << thisStudent.fullName
                << addSpaces(longestStudentName - thisStudent.fullName.length())
                << " | " << thisStudent.totalStaffSelected
                << addSpaces(4 - getNumberDigitCount(thisStudent.totalStaffSelected))
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

  Student& getStudentAtI(int index) { return studentList[index]; };
  Student& getStudentFromName(std::string name) {
    for (int i = 0; i < totalGraduatingStuds; ++i) {
      Student& thisStudent = studentList[i];
      if (thisStudent.fullName == name) {
        return thisStudent;
      }
    }
    return NULLSTUDENT;
  };
  int getListSize() { return totalGraduatingStuds; };
};

class Staff {
 private:
  int popularity = 0; // how many times they are found on student list
  std::string name = "";

  int studentListMaxSize = 0;
  int currentStudentCount = 0;
  std::string* studentList = nullptr;  // dynamic array of wanted student names


  int maxStudentsThisPersonCanStole = 100;
  // max size should be ^ maxStudentsThisPersonCanStole
  int currentStudentMatchCount = 0;
  std::string* matchedStudentsList = nullptr;  // dynamic array of matched students

 public:

  // Constructor
   Staff() {}

  // Destructor
  ~Staff() {
    // strings deallocate themselves already so no need to delete string list
  }

  void shuffleStudList() {
    // Shuffle the array
    std::shuffle(studentList, studentList + currentStudentCount, createRNG());
  }

  void updateMatchedStudentsList(int matchStudentListSize, StudentData &studentData) {
    if (matchedStudentsList != nullptr){
      for (int i = 0; i < currentStudentMatchCount; ++i) {
        Student& foundStud = studentData.getStudentFromName(matchedStudentsList[i]);
        foundStud.matchedWithStaff = ""; // clear matched staff
      }
      delete[] matchedStudentsList;
      matchedStudentsList = nullptr;
    }
    maxStudentsThisPersonCanStole = 0;  //  reset
    currentStudentMatchCount = 0;

    maxStudentsThisPersonCanStole = matchStudentListSize;
    matchedStudentsList = new std::string[maxStudentsThisPersonCanStole];
    //std::cout << "set " << name << " maxcanstole: " << maxStudentsThisPersonCanStole << '\n';
  }

  void addStudent(std::string name) {
    studentList[currentStudentCount] = name;
    ++currentStudentCount;
  }

  std::string getStudentAtI(int index) {
    return studentList[index];
  }

  void addMatchedStudent(std::string name) {
    if (currentStudentMatchCount < maxStudentsThisPersonCanStole) { // teacher is full prevent overflow
      if (matchedStudentsList == nullptr) {
          matchedStudentsList = new std::string[maxStudentsThisPersonCanStole];
      }
      for (int i = 0; i < currentStudentMatchCount; ++i) {
        if (name == matchedStudentsList[i]) { // if already in match list
          return;
        }
      }
      matchedStudentsList[currentStudentMatchCount] = name;
      ++currentStudentMatchCount;
    }
  }

  bool teacherIsFullMatched() {
    return currentStudentMatchCount >= maxStudentsThisPersonCanStole;
  }

  void createStudentStringList(int maxSize) {
    studentListMaxSize = maxSize;
    studentList = new std::string[maxSize];
  }

  void setName(std::string &newName) { name = newName; };
  void setPopularity(int newpop) {
    popularity = newpop;
  };
  int getPopularity() { return popularity; };
  int getMaxSelectedStuds() { return studentListMaxSize; };
  int getMaxMatchingStudents() { return maxStudentsThisPersonCanStole; };
  int getAmtOfMatchedStudents() { return currentStudentMatchCount; };
  std::string getMatchedStudentAtI(int index) { return matchedStudentsList[index]; };
  std::string getName() { return name; };
};


Staff NULLSTAFF = Staff();

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

  void sortStaffIndexs() {
    std::sort(staffList, staffList + currentSize, [](Staff& a, Staff& b) {
      return (a.getAmtOfMatchedStudents() - a.getPopularity() - a.getMaxMatchingStudents()) <
            (b.getAmtOfMatchedStudents() - b.getPopularity() - b.getMaxMatchingStudents());
    });
  }

  void sortStaffIndexsForEquality() {
    std::sort(staffList, staffList + currentSize, [](Staff& a, Staff& b) {
      return a.getAmtOfMatchedStudents() + a.getPopularity() <
             b.getAmtOfMatchedStudents() + b.getPopularity();
    });
  }

  void sortStaffIndexsByPopularity() { // sort by popularity first
    std::sort(staffList, staffList + currentSize, [](Staff& a, Staff& b) {
        return a.getPopularity() > b.getPopularity();
    });
  }

  void sortByAmtStolling() {
    std::sort(staffList, staffList + currentSize, [](Staff& a, Staff& b) {
      return a.getAmtOfMatchedStudents() > b.getAmtOfMatchedStudents();
    });
  }

  void addStaff(Staff &newStaff) {
    staffList[currentSize] = newStaff;
    if (newStaff.getName().empty()) {
      std::cout << "[ERROR PARSING] staff csv, STAFF NAME IS EMPTY?" << '\n';
      exit(1);
    }
    if (newStaff.getName().length() > longestStaffName) {
      longestStaffName = newStaff.getName().length();
    }
    ++currentSize;
  }

  void printPairings(StudentData& studentData) {
    int totalStudentsCount = studentData.getListSize();
    int pairedStudentsCounter = 0;
    std::cout << "\ntotal student count: " << totalStudentsCount << "\n";
    std::cout << "total staff count: " << currentSize << "\n\n";
    sortByAmtStolling();
    for (int i = 0; i < currentSize; ++i) {
      Staff& thisStaff = staffList[i];
      int thisStaffMaxMatchedStudents = thisStaff.getMaxMatchingStudents();
      int thisStaffAmtOfMatchStudents = thisStaff.getAmtOfMatchedStudents();
      setTxtColor(8);
      std::cout << "staff #" << i + 1;
      setTxtColor(15);
      std::cout << addSpaces(3 - getNumberDigitCount(i + 1)) << "[ ";
      if (thisStaffAmtOfMatchStudents <= 0) {
        setTxtColor(12);
      } else if (thisStaffAmtOfMatchStudents == thisStaffMaxMatchedStudents) {
        setTxtColor(1);
      } else {
        setTxtColor(11);
      }
      std::cout << thisStaff.getName();
      setTxtColor(15);
      std::cout << " ]: ";
      setTxtColor(8);
      //std::cout << thisStaffAmtOfMatchStudents << "/" << thisStaffMaxMatchedStudents << " students to stole: \n";
      std::cout << "\n";
      int previousStaffStudentNameLength = 10;
      int studentsPerRow = 3;
      if (thisStaffAmtOfMatchStudents < 5) {
        studentsPerRow = 2;
      }
      setTxtColor(7);
      for (int j = 0; j < thisStaffAmtOfMatchStudents; ++j) {
        std::cout << addSpaces(20 - previousStaffStudentNameLength);
        setTxtColor(8);
        std::cout << "- ";
        setTxtColor(7);
        std::cout << thisStaff.getMatchedStudentAtI(j);
        previousStaffStudentNameLength = thisStaff.getMatchedStudentAtI(j).length();
        ++pairedStudentsCounter;
        if (((j+1)%studentsPerRow) == 0 || j == thisStaffAmtOfMatchStudents-1) {
          std::cout << "\n";
          previousStaffStudentNameLength = 10;
        }
      }
      setTxtColor(15);
      std::cout << "\n";
    }

    std::cout << "\ntotal students paired = (";
    setTxtColor(10);
    std::cout << pairedStudentsCounter << " / " << totalStudentsCount;
    setTxtColor(15);
    double percentage = (static_cast<double>(pairedStudentsCounter) / totalStudentsCount) * 100.0;
    std::cout << ") " << percentage << "%\n ";

    int totalUnairedStudentCount = 0;
    // loop to count total unpaired students
    for (int k = 0; k < studentData.getListSize(); ++k) {  // for each of all graduating students
      Student& thisStud = studentData.getStudentAtI(k);
      if (thisStud.matchedWithStaff == "" || thisStud.matchedWithStaff.empty()) {
        ++totalUnairedStudentCount;
      }
    }

    std::cout << "\n - [" << totalUnairedStudentCount << " UNPAIRED STUDENTS] - \n";
    for (int k = 0; k < studentData.getListSize(); ++k) {  // for each of all graduating students
      Student& thisStud = studentData.getStudentAtI(k);
      if (thisStud.matchedWithStaff == "" || thisStud.matchedWithStaff.empty()) {
        std::cout << "[";
        setTxtColor(12);
        std::cout << thisStud.fullName;
        setTxtColor(15);
        std::cout << "] ";
        setTxtColor(8);
        std::cout << "chose " << thisStud.totalStaffSelected << " staff members " << '\n';
        setTxtColor(15);
      }
    }
  }

  void printAllStaff() {
    std::cout << "________________________________________________________________________________________________________\n";
    setTxtColor(14);
    std::cout << " STAFF LIST";
    setTxtColor(15);
    std::cout << " | ";
    setTxtColor(14);
    std::cout << "staff full name";
    setTxtColor(15);
    std::cout << addSpaces(25-longestStaffName);
    std::cout << " | ";
    setTxtColor(14);
    std::cout << "#selected students | max can stole | #students stolling | popularity";
    setTxtColor(15);
    std::cout << "\n";

    sortByAmtStolling();
    for (int i = 0; i < currentSize; ++i) {
      Staff& thisStaff = staffList[i];
      std::cout << "  staff #" << i + 1 << addSpaces(3 - getNumberDigitCount(i + 1)) << "| ";
      int thisStaffAmtOfMatchStudents = thisStaff.getAmtOfMatchedStudents();
      int thisStaffMaxMatchedStudents = thisStaff.getMaxMatchingStudents();
      if (thisStaffAmtOfMatchStudents <= 0) {
        setTxtColor(12);
      } else if (thisStaffMaxMatchedStudents == thisStaffAmtOfMatchStudents) {
        setTxtColor(1);
      } else {
        setTxtColor(11);
      }
      std::cout << thisStaff.getName();
      setTxtColor(15);
      std::cout << addSpaces(longestStaffName - thisStaff.getName().length());
      std::cout << " | " << thisStaff.getMaxSelectedStuds() << addSpaces(18-getNumberDigitCount(thisStaff.getMaxSelectedStuds())) << " | "
                << thisStaffMaxMatchedStudents << addSpaces(13-getNumberDigitCount(thisStaffMaxMatchedStudents)) << " | "
                << thisStaffAmtOfMatchStudents << " " << addSpacesButCool(17-getNumberDigitCount(thisStaffAmtOfMatchStudents), thisStaffAmtOfMatchStudents) << " | "
                << thisStaff.getPopularity();
      std::cout << '\n';
    };
  };

  // gets
  int getListSize() const { return currentSize; }
  Staff& getStaff(int index) const { return staffList[index]; };
  Staff& getStaffFromName(std::string staffFullName) const {
    for (int i = 0; i < currentSize; ++i) {
      Staff& thisStaff = staffList[i];
      if (thisStaff.getName() == staffFullName) {
        return thisStaff;
      }
    }
    return NULLSTAFF;
  };
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

void replaceHyphens(std::string& str) {
  size_t pos = 0;
  while ((pos = str.find('-')) != std::string::npos) {
    str[pos] = ' ';
  }
}

void keepFirstWord(std::string& str) {
    // find the position of the first space in the string
    size_t pos = str.find(' ');
    if (pos != std::string::npos) {
        // erase everything after the first space
        str.erase(pos);
    }
}

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

        replaceHyphens(studentFirstName);
        keepFirstWord(studentFirstName);
        replaceHyphens(studentLastName);
        keepFirstWord(studentLastName);

        if (!studentFirstName.empty() && !studentLastName.empty()) {
          ++wantedStudentsCount;
        }
      }

      Staff newStaff;
      removeWhitespace(firstName);
      removeQuotes(firstName);
      removeWhitespace(lastName);
      removeQuotes(lastName);

      replaceHyphens(firstName);
      keepFirstWord(firstName);
      replaceHyphens(lastName);
      keepFirstWord(lastName);
      std::string staffFullName = lastName + firstNameLastNameSeperator + firstName;
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

        replaceHyphens(studentFirstName);
        keepFirstWord(studentFirstName);
        replaceHyphens(studentLastName);
        keepFirstWord(studentLastName);
        if (!studentFirstName.empty() && !studentLastName.empty()) {
          std::string studentFullName = studentLastName + firstNameLastNameSeperator + studentFirstName;
          newStaff.addStudent(studentFullName);
          ++countingStuds;
        }
      }

      staffData.addStaff(newStaff);
      // not needed
      //delete[] words;  // free
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

      replaceHyphens(firstName);
      keepFirstWord(firstName);
      replaceHyphens(lastName);
      keepFirstWord(lastName);

      if (!firstName.empty() && !lastName.empty()) {
        ++studentCounter;
      }

      // not needed
      // delete[] words;  // free
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

      replaceHyphens(firstName);
      keepFirstWord(firstName);
      replaceHyphens(lastName);
      keepFirstWord(lastName);

      if (!firstName.empty() && !lastName.empty()) {
        std::string studentFullName = lastName + firstNameLastNameSeperator + firstName;
        Student newStudent;
        newStudent.fullName = studentFullName;

        std::string veritasFirstName = words[dynamicArrSz - 1];
        std::string veritasLastName = words[dynamicArrSz - 2];
        removeWhitespace(veritasFirstName);
        removeQuotes(veritasFirstName);
        removeWhitespace(veritasLastName);
        removeQuotes(veritasLastName);

        replaceHyphens(veritasFirstName);
        keepFirstWord(veritasFirstName);
        replaceHyphens(veritasLastName);
        keepFirstWord(veritasLastName);
        std::string veritasWielder = veritasLastName + firstNameLastNameSeperator + veritasFirstName;
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
          //std::string staffFullName = staffLastName + firstNameLastNameSeperator + staffFirstName;
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

          replaceHyphens(staffFirstName);
          keepFirstWord(staffFirstName);
          replaceHyphens(staffLastName);
          keepFirstWord(staffLastName);
          if (!staffFirstName.empty() && !staffLastName.empty()) {
            std::string staffFullName = staffLastName + firstNameLastNameSeperator + staffFirstName;
            if (studentChoseStaffAmt > 0) {
              newStudent.selectedStaffs[incrementer] = staffFullName;
            }
            ++incrementer;
          }
        }

        studentData.addStudent(newStudent);
      }

      // not needed
      // delete[] words;  // free
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

bool existsInStringList(std::string fullName, std::string *theList, int size) {
  for (int i = 0; i < size; ++i) {
    if (fullName == theList[i]) {
      return true;
    }
  }
  return false;
}

std::string findSimilarLowCaseInStringList(std::string fullName, std::string* theList, int size) {
  for (int i = 0; i < size; ++i) {
    std::string listIFullName = theList[i];
    toLowercase(listIFullName);
    if (fullName == listIFullName) {
      return theList[i];
    }
  }
  return "";
}

std::string findSimilarLastNameInStringList(std::string lastName, std::string* theList, int size) {
  for (int i = 0; i < size; ++i) {
    std::string listIFullName = theList[i];
    std::string IlastName = listIFullName;
    keepFirstWord(IlastName);
    if (lastName == IlastName) {
      return theList[i];
    }
  }
  return "";
}

std::string findSimilarFirstNameInStringList(std::string firstName, std::string* theList, int size) {
  for (int i = 0; i < size; ++i) {
    std::string listIFullName = theList[i];
    std::string IFirstName = listIFullName.substr(listIFullName.find(' ') + 1);;
    keepFirstWord(IFirstName);
    if (firstName == IFirstName) {
      return theList[i];
    }
  }
  return "";
}

std::string finstClosestSimilarInList(std::string fullName, std::string* theList, int size) {
  int closest = 0;
  std::string mostSim = "";
  for (int i = 0; i < size; ++i) {
    std::string lowercaseIFullName = theList[i];
    toLowercase(lowercaseIFullName);
    int thisClosest = compareStrings(fullName, lowercaseIFullName);
    if (thisClosest > closest) {
      mostSim = theList[i];
      closest = thisClosest;
    }
  }
  return mostSim;
}

void locateMistakes(StaffData& staffData, StudentData& studentData) {
  std::cout << "\n[finding mistakes]...\n";
  int currentStudI = 0;
  int currentStafI = 0;
  int maxStudListSize = studentData.getListSize();
  int maxStaffListSize = staffData.getListSize();

  std::string* allOfTeacherWantedStudents = new std::string[maxStudListSize];
  std::string* allOfStudentwantedTeachers = new std::string[maxStudListSize];

  for (int i = 0; i < maxStaffListSize; ++i) {  // for each (staff)
    Staff& thisStaff = staffData.getStaff(i);
    for (int j = 0; j < thisStaff.getMaxSelectedStuds(); ++j) {  // for each wanted student
      std::string attemptStudent = thisStaff.getStudentAtI(j);
      if (!existsInStringList(attemptStudent, allOfTeacherWantedStudents, currentStudI)) {
        allOfTeacherWantedStudents[currentStudI] = attemptStudent;
        ++currentStudI;
      }
    };
  }

  for (int i = 0; i < maxStudListSize; ++i) {  // for each (student)
    Student& thisStudent = studentData.getStudentAtI(i);
    for (int j = 0; j < thisStudent.totalStaffSelected; ++j) {  // for each (wanted staff)
      std::string selectedStaff = thisStudent.selectedStaffs[j];
      Staff& actualSelectedStaff = staffData.getStaffFromName(selectedStaff);
      if (actualSelectedStaff.getName() != NULLSTAFF.getName()) {
        actualSelectedStaff.setPopularity(actualSelectedStaff.getPopularity() + 1);
      } 
      if (!existsInStringList(selectedStaff, allOfStudentwantedTeachers, currentStafI) && currentStafI < maxStudListSize) {
        if (actualSelectedStaff.getName() == NULLSTAFF.getName()) { // was not found on staff list
          std::cout << "(";
          setTxtColor(12);
          std::cout << selectedStaff;
          setTxtColor(15);
          std::cout << ") was not found on staff list\n";
        }
        allOfStudentwantedTeachers[currentStafI] = selectedStaff;
        ++currentStafI;
      }
    }
  }

  for (int k = 0; k < maxStudListSize; ++k) {  // for each of all (graduating students)
    Student thisStud = studentData.getStudentAtI(k);
    std::string studFullName = thisStud.fullName;
    if (!existsInStringList(studFullName, allOfTeacherWantedStudents, currentStudI)) {
      std::string firstName = studFullName.substr(studFullName.find(' ') + 1);
      std::string lastName = studFullName;
      keepFirstWord(lastName);

      std::cout << "[name error]: (";
      setTxtColor(12);
      std::cout << thisStud.fullName;
      setTxtColor(15);
      std::cout << ") from list of students was not found on any of the "
                   "teachers selected students list\n";

      std::string lowCaseFullName = studFullName;
      toLowercase(lowCaseFullName);

      std::string foundSimilarToLowerCase = findSimilarLowCaseInStringList(lowCaseFullName, allOfTeacherWantedStudents, currentStudI);
      std::string foundSimilarLastName = findSimilarLastNameInStringList(lastName, allOfTeacherWantedStudents, currentStudI);
      if (foundSimilarToLowerCase != "" && !foundSimilarToLowerCase.empty()) {
        std::cout << "  - same last names different cases: ";
        setTxtColor(3);
        std::cout << foundSimilarToLowerCase;
        setTxtColor(15);
        std::cout << "\n\n";
      } else if (foundSimilarLastName != "" && !foundSimilarLastName.empty()) {
        std::cout << "  - found student with same last name: ";
        setTxtColor(3);
        std::cout << foundSimilarLastName;
        setTxtColor(15);
        std::cout << "\n\n";
      } else {
        // idk
        std::string foundSimilarFirstName = findSimilarFirstNameInStringList( firstName, allOfTeacherWantedStudents, currentStudI);
        if (foundSimilarFirstName != "" && !foundSimilarFirstName.empty()) {
          std::cout << "  - found student with same first name: ";
          setTxtColor(3);
          std::cout << foundSimilarFirstName;
          setTxtColor(15);
          std::cout << "\n\n";
        } else {
          std::string MostSimilar = finstClosestSimilarInList( lowCaseFullName, allOfTeacherWantedStudents, currentStudI);
          std::cout << "  ~ similar? to: ";
          setTxtColor(8);
          std::cout << MostSimilar;
          setTxtColor(15);
          std::cout << "\n\n";
        }
      }
    }
  }

  for (int k = 0; k < maxStaffListSize; ++k) { // for each teachers student selected
    Staff& thisStaff = staffData.getStaff(k);
    std::string staffName = thisStaff.getName();
    if (!existsInStringList(staffName, allOfStudentwantedTeachers, currentStafI)) {
      std::string lastName = staffName;
      std::string firstName = staffName.substr(staffName.find(' ') + 1);
      keepFirstWord(lastName);
      std::string foundWithSameLastName = findSimilarLastNameInStringList(lastName, allOfStudentwantedTeachers, currentStafI);
      std::string foundWithSameFirstName = findSimilarFirstNameInStringList(firstName, allOfStudentwantedTeachers, currentStafI);
      if (!foundWithSameLastName.empty()) {
        std::cout << "(";
        setTxtColor(4);
        std::cout << staffName;
        setTxtColor(15);
        std::cout << ") was not found on students lists\n";
        std::cout << "  - found same last names: ";
        setTxtColor(3);
        std::cout << foundWithSameLastName;
        setTxtColor(15);
        std::cout << "\n\n";
      } else if (!foundWithSameFirstName.empty()) {
        std::cout << "(";
        setTxtColor(4);
        std::cout << staffName;
        setTxtColor(15);
        std::cout << ") was not found on students lists\n";
        std::cout << "  - found same first names: ";
        setTxtColor(3);
        std::cout << foundWithSameFirstName;
        setTxtColor(15);
        std::cout << "\n\n";
      } else {
        std::cout << "(";
        setTxtColor(6);
        std::cout << staffName;
        setTxtColor(15);
        std::cout << ") noone chose this staff\n";
      }
    }
  }
}

void setupWaves(StaffData& staffData, StudentData& studentData) {
  int maxStaffPerLine = 0;
  system("CLS");
  std::cout << "enter max staff per line: ";
  std::cin >> maxStaffPerLine;
}

void pairStudentAndStaff(StaffData& staffData, StudentData& studentData) {
  int listSize = staffData.getListSize();
  for (int i = 0; i < listSize; ++i) {
    Staff& thisStaff = staffData.getStaff(i);
    thisStaff.updateMatchedStudentsList(thisStaff.getMaxMatchingStudents(), studentData);
  };

  std::string secondInput = "";
  do {
    system("CLS");
    std::cout << "  - student/staff pairing -\n\n"
              << "[1] force pair specified teacher with specified student\n"
              << "[2] set #of students of staff by POPULARITY\n"
              << "[3] manually set #of students for each staff can stole";
    setTxtColor(8);
    std::cout << " (tedious)\n";
    setTxtColor(15);
    setTxtColor(10);
    std::cout << "[s]";
    setTxtColor(15);
    std::cout << " start pairing process\n "
              << "\ninput: ";
    std::cin >> secondInput;
    if (secondInput == "3") {
      int listSize = staffData.getListSize();
      for (int i = 0; i < listSize; ++i) {
        Staff &thisStaff = staffData.getStaff(i);
        std::string firstThingy = std::to_string(i + 1) + "/" + std::to_string(listSize);
        std::cout << "#" << firstThingy;
        std::cout << addSpaces(6 - firstThingy.length()) << "| #of students ";
        setTxtColor(3);
        std::cout << thisStaff.getName();
        setTxtColor(15);
        std::cout << " can stole: ";
        int askedSize = 0;
        std::cin >> askedSize;
        thisStaff.updateMatchedStudentsList(askedSize, studentData);
      };
      system("PAUSE");
    } else if (secondInput == "1") {
      std::string staffInput = "";
      std::string studentInput = "";
      std::cout << "enter staff full name(last, first): ";
      std::cin.ignore(1000, '\n');  // flush the buffer
      std::getline(std::cin, staffInput);
      Staff& thisStaff = staffData.getStaffFromName(staffInput);
      if (thisStaff.getName() != NULLSTAFF.getName()) {
        std::cout << "staff #selected studs: " << thisStaff.getMaxSelectedStuds() << "\n";
        std::cout << "enter student full name(last, first): ";
        std::getline(std::cin, studentInput);
        Student& thisStudent = studentData.getStudentFromName(studentInput);
        if (thisStudent.fullName != NULLSTUDENT.fullName) {
          thisStudent.matchedWithStaff = thisStaff.getName();
          thisStaff.addMatchedStudent(studentInput);
        } else {
          std::cout << "[Error] student (" << studentInput << ") was not found\n";
        }
      } else {
        std::cout << "[Error] staff (" << staffInput << ") was not found\n";
      }
      system("PAUSE");
    } else if (secondInput == "2") {
      int listSize = staffData.getListSize();
      int top = 0;
      int next = 0;
      int bottom = 0;

      int topIndex = 0;
      int nextIndex = 0;
      std::cout << " - Top ";
      std::cin >> topIndex;
      std::cout << " requested staff will stole #of students: ";
      std::cin >> top;
      std::cout << " - Next ";
      std::cin >> nextIndex;
      std::cout << " requested staff will stole #of students: ";
      std::cin >> next;
      std::cout << " - Bottom " << (listSize-(topIndex+nextIndex)) << "\n requested staff will stole #of students: ";
      std::cin >> bottom;

      staffData.sortStaffIndexsByPopularity();
      for (int i = 0; i < listSize; ++i) {
        Staff& thisStaff = staffData.getStaff(i);
        if (i < topIndex) {  // top
          thisStaff.updateMatchedStudentsList(top, studentData);
        } else if (i < topIndex + nextIndex) {  // next
          thisStaff.updateMatchedStudentsList(next, studentData);
        } else { // last
          thisStaff.updateMatchedStudentsList(bottom, studentData);
        }
      }
    }
  } while (secondInput != "s");
  
  system("CLS");
  int equalityBias = 1;
  std::cout << "1 = prioratizes popular staff\n"
            << "20 = equally gives staff same amt of students\n"
            << "(1-20) - 15 is preffered\ninput (equality bias): ";
  std::cin >> equalityBias;
  if (equalityBias < 1) {
    equalityBias = 1;
  }

  int cycleIncrement = 1;
  // multiple cycles to make sure all possible pairs are paired
  for (int cycle = 0; cycle < 1000; cycle+=cycleIncrement) { // two cycles, first only fits <= minPossiblNeededStudsForEachStaff then second cycle fills in all the other students
  if (equalityBias > 14) {
    staffData.sortStaffIndexsForEquality();
  } else {
    staffData.sortStaffIndexs();
  }
  //++cycleIncrement;
  int matchedStudentsI = 0;
  std::string* alreadyMatchedStudens = new std::string[studentData.getListSize()]; // keep track of already matched students, prevent student repeats
  std::cout << "cycle " << cycle << ": ";
  for (int i = 0; i < staffData.getListSize(); ++i) {  // for each (staff)
    std::cout << ".";
    Staff &thisStaff = staffData.getStaff(i);
    std::string thisStaffName = thisStaff.getName();

    if (thisStaff.teacherIsFullMatched()) { // staff is full skup
      continue;
    }

    // shuffle students to iterate through to prevent alphabetical prioratization
    thisStaff.shuffleStudList();
    for (int j = 0; j < thisStaff.getMaxSelectedStuds(); ++j) { // for each wanted student
      std::string attemptStudent = thisStaff.getStudentAtI(j);

      if (thisStaff.getAmtOfMatchedStudents() > cycle - (i / equalityBias)) {
        continue;
      }

      // makes this faster
      if (existsInStringList(attemptStudent, alreadyMatchedStudens, matchedStudentsI) || thisStaff.teacherIsFullMatched()) {
        continue;  // skip, student was already taken
      }

      // shuffle to prevent prioratizing alphabetical order of student list
      studentData.shuffleStudList();
      for (int k = 0; k < studentData.getListSize(); ++k) { // for each of all (graduating students)
        Student& thisStud = studentData.getStudentAtI(k);
        if (thisStud.fullName == attemptStudent && !thisStaff.teacherIsFullMatched() && thisStud.matchedWithStaff.empty()) { // if student is wanted by the staff & staff is free

          // makes sure student wants this staff too
          bool matchFound = false; // make sure student wants that staff member too
          for (int l = 0; l < thisStud.totalStaffSelected; ++l) { // for each staff student selected
            if (thisStud.selectedStaffs[l] == thisStaffName) {
              matchFound = true;
              break;
            }
          }
          if (matchFound && !existsInStringList(thisStud.fullName, alreadyMatchedStudens, matchedStudentsI)) {
            thisStaff.addMatchedStudent(thisStud.fullName);
            thisStud.matchedWithStaff = thisStaffName;

            alreadyMatchedStudens[matchedStudentsI] = thisStud.fullName;
            ++matchedStudentsI;
          }
        }
      }
    }
  }
  std::cout << "#of students paired: " << matchedStudentsI << "\n";
  if (matchedStudentsI == 0 && cycle > 1) {
    break;
  }
  }
  std::cout << "\n\n";
  system("PAUSE");
  system("CLS");
  staffData.printPairings(studentData);

  // not needed to free cuz strings free themselve prolly through their dconstruct
  //delete alreadyMatchedStudens;
  system("PAUSE");
}

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
  locateMistakes(staffData, studentData);

  system("PAUSE");

  std::string input = "";
  do {
    system("CLS");
    std::cout << " - grad halper menu -                       #of students: " << studentData.getListSize() << "\n"
              << "                                            #of staff:    " << staffData.getListSize() << "\n"
              << "[1] display all staff\n"
              << "[2] display all students\n"
              << "[3] calculate veritas results\n";
    setTxtColor(14);
    std::cout << "[4] pair student & staff\n";
    std::cout << "[5] setup waves\n";
    setTxtColor(15);
    std::cout << "[6] view pairings\n"
              << "[q] quit program\n"
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
    } else if (input == "5") {
      setupWaves(staffData, studentData);
    } else if (input == "6") {
      system("CLS");
      staffData.printPairings(studentData);
      system("PAUSE");
    }
  } while (input != "q");
  //writeToCSV(staffData);

  system("PAUSE");
  return 0;
}

void setTxtColor(int colorValue) {
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, colorValue);
}

void toLowercase(std::string& input) {
  for (char& c : input) {
    if (c >= 'A' && c <= 'Z') {
      c += 32;  // Convert uppercase letter to lowercase
    }
  }
}

// return a string of spaces of length amt given
std::string addSpaces(int amt) {  // pseudo set w so i get to control it more
  std::string spaces = "";
  for (int i = 0; i < amt; ++i) {
    spaces += ' ';
  }
  return spaces;
}

std::string addSpacesButCool(int amt, int visualSize) {
  std::string spaces = "";
  for (int i = 0; i < amt; ++i) {
    if (i < visualSize) {
      spaces += '*';
    } else {
      spaces += ' ';
    }
  }
  return spaces;
}
