#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

struct Student {
    string rollNo, name, fatherName, cnic;
};

struct Record {
    string rollNo;
    int semester;
    int subject1, subject2, subject3, subject4, subject5;
    int total;
    float gpa;
};

vector<string> dummyCourses = { "Math", "Physics", "Chemistry", "English", "Programming" };

void showStudentList() {
    ifstream file("students.txt");
    if (!file.is_open()) {
        cout << "File not exist please add record first!\n";
        return;
    }

    string line;
    cout << left << setw(15) << "Roll No" << setw(20) << "Name" << setw(20) << "Father Name" << "CNIC" << endl;
    while (getline(file, line)) {
        stringstream ss(line);
        Student student;
        getline(ss, student.rollNo, '|');
        getline(ss, student.name, '|');
        getline(ss, student.fatherName, '|');
        getline(ss, student.cnic, '|');
        cout << left << setw(15) << student.rollNo << setw(20) << student.name << setw(20) << student.fatherName << student.cnic << endl;
    }
    file.close();
}

void addStudent() {
    ofstream file("students.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening students file!\n";
        return;
    }

    Student student;
    cout << "Enter Roll No: ";
    cin >> student.rollNo;

    ifstream readFile("students.txt");
    string line;
    while (getline(readFile, line)) {
        if (line.find(student.rollNo + "|") == 0) {
            cout << "Roll No already exists!\n";
            return;
        }
    }

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, student.name);

    cout << "Enter Father Name: ";
    getline(cin, student.fatherName);

    cout << "Enter CNIC: ";
    cin >> student.cnic;

    readFile.clear();
    readFile.seekg(0);
    while (getline(readFile, line)) {
        stringstream ss(line);
        string existingCnic;
        getline(ss, existingCnic, '|');
        getline(ss, existingCnic, '|');
        getline(ss, existingCnic, '|');
        getline(ss, existingCnic, '|');
        if (existingCnic == student.cnic) {
            cout << "CNIC already exists!\n";
            return;
        }
    }

    file << student.rollNo << "|" << student.name << "|" << student.fatherName << "|" << student.cnic << "\n";
    cout << "Student added successfully!\n";
    file.close();
}

void showSingleStudent() {
    ifstream file("students.txt");
    if (!file.is_open()) {
        cout << "Error opening students file!\n";
        return;
    }

    string rollNo;
    cout << "Enter Roll No: ";
    cin >> rollNo;

    string line;
    while (getline(file, line)) {
        if (line.find(rollNo + "|") == 0) {
            stringstream ss(line);
            Student student;
            getline(ss, student.rollNo, '|');
            getline(ss, student.name, '|');
            getline(ss, student.fatherName, '|');
            getline(ss, student.cnic, '|');
            cout << left << setw(15) << "Roll No" << setw(20) << "Name" << setw(20) << "Father Name" << "CNIC" << endl;
            cout << left << setw(15) << student.rollNo << setw(20) << student.name << setw(20) << student.fatherName << student.cnic << endl;
            return;
        }
    }

    cout << "Student not found!\n";
    file.close();
}

void showAllRecords() {
    ifstream file("records.txt");
    if (!file.is_open()) {
        cout << "Error opening records file!\n";
        return;
    }

    string line;
    cout << left << setw(10) << "Roll No" << setw(10) << "| Semester  ";
    for (int i = 1; i <= 5; ++i) cout << "| Subject " << i << " ";
    cout << "| Total" << setw(10) << "     | GPA" << endl;
    cout << "------------------------------------------------------------------------------------------------------" << endl;

    while (getline(file, line)) {
        stringstream ss(line);
        Record record;
        ss >> record.rollNo >> record.semester >> record.subject1 >> record.subject2 >> record.subject3 >> record.subject4 >> record.subject5 >> record.total >> record.gpa;
        cout << left << setw(10) << record.rollNo << "| " << setw(10) << record.semester << "| "
            << setw(10) << record.subject1 << "| " << setw(10) << record.subject2 << "| "
            << setw(10) << record.subject3 << "| " << setw(10) << record.subject4 << "| "
            << setw(10) << record.subject5 << "| " << setw(10) << record.total << "| "
            << setw(10) << record.gpa << endl;
    }
    file.close();
}

void addRecord() {
    ofstream file("records.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening records file!\n";
        return;
    }

    ifstream studentFile("students.txt");
    if (!studentFile.is_open()) {
        cout << "Error opening students file!\n";
        return;
    }

    string rollNo;
    cout << "Enter Roll No: ";
    cin >> rollNo;

    string studentLine;
    bool studentExists = false;
    while (getline(studentFile, studentLine)) {
        if (studentLine.find(rollNo + "|") == 0) {
            studentExists = true;
            break;
        }
    }

    if (!studentExists) {
        cout << "Roll No does not exist!\n";
        return;
    }

    ifstream recordFile("records.txt");
    string recordLine;
    int lastSemester = 0;
    while (getline(recordFile, recordLine)) {
        stringstream ss(recordLine);
        string existingRollNo;
        int existingSemester;
        ss >> existingRollNo >> existingSemester;
        if (existingRollNo == rollNo) {
            lastSemester = max(lastSemester, existingSemester);
        }
    }

    if (lastSemester == 8) {
        cout << "All semesters completed for this student!\n";
        return;
    }

    int semester;
    cout << "Enter Semester: ";
    cin >> semester;

    if (semester != lastSemester + 1) {
        cout << "Invalid semester! The next semester should be " << lastSemester + 1 << ".\n";
        return;
    }

    Record record;
    record.rollNo = rollNo;
    record.semester = semester;

    cout << "Enter marks for 5 subjects (out of 100):\n";
    cin >> record.subject1 >> record.subject2 >> record.subject3 >> record.subject4 >> record.subject5;

    if (
        record.subject1 > 100 || record.subject1 < 0 || 
        record.subject2 > 100 || record.subject2 < 0 || 
        record.subject3 > 100 || record.subject3 < 0 || 
        record.subject4 > 100 || record.subject4 < 0 || 
        record.subject5 > 100 || record.subject5 < 0) {
        cout << "Marks cannot be less then 0 and greater 100!" << endl;
        return;
    }

    record.total = record.subject1 + record.subject2 + record.subject3 + record.subject4 + record.subject5;
    record.gpa = (float)record.total / 500 * 4;

    file << record.rollNo << " " << record.semester << " " << record.subject1 << " " << record.subject2 << " "
        << record.subject3 << " " << record.subject4 << " " << record.subject5 << " " << record.total << " " << record.gpa << "\n";
    cout << "Record added successfully!\n";
    file.close();
}

void showSingleStudentRecords() {
    string rollNo;
    cout << "Enter Roll No: ";
    cin >> rollNo;

    ifstream studentFile("students.txt");
    if (!studentFile.is_open()) {
        cout << "Error opening students file!\n";
        return;
    }

    string studentLine;
    bool studentExists = false;
    while (getline(studentFile, studentLine)) {
        if (studentLine.find(rollNo + "|") == 0) {
            studentExists = true;
            stringstream ss(studentLine);
            Student student;
            getline(ss, student.rollNo, '|');
            getline(ss, student.name, '|');
            getline(ss, student.fatherName, '|');
            getline(ss, student.cnic, '|');
            cout << left << setw(15) << "Roll No" << setw(20) << "Name" << setw(20) << "Father Name" << "CNIC" << endl;
            cout << left << setw(15) << student.rollNo << setw(20) << student.name << setw(20) << student.fatherName << student.cnic << endl;
            break;
        }
    }

    if (!studentExists) {
        cout << "Student not found!\n";
        return;
    }

    ifstream recordFile("records.txt");
    if (!recordFile.is_open()) {
        cout << "Error opening records file!\n";
        return;
    }

    string recordLine;
    cout << left << setw(10) << "Roll No" << setw(10) << "| Semester  ";
    for (int i = 1; i <= 5; ++i) cout << "| Subject " << i << " ";
    cout << "| Total" << setw(10) << "     | GPA" << endl;
    cout << "------------------------------------------------------------------------------------------------------" << endl;
    bool hasRecords = false;
    while (getline(recordFile, recordLine)) {
        stringstream ss(recordLine);
        Record record;
        ss >> record.rollNo >> record.semester >> record.subject1 >> record.subject2 >> record.subject3 >> record.subject4 >> record.subject5 >> record.total >> record.gpa;
        if (record.rollNo == rollNo) {
            hasRecords = true;
            cout << left << setw(10) << record.rollNo << "| " << setw(10) << record.semester << "| "
                << setw(10) << record.subject1 << "| " << setw(10) << record.subject2 << "| "
                << setw(10) << record.subject3 << "| " << setw(10) << record.subject4 << "| "
                << setw(10) << record.subject5 << "| " << setw(10) << record.total << "| "
                << setw(10) << record.gpa << endl;
        }
    }

    if (!hasRecords) {
        cout << "No records found for this student!\n";
    }
}

int main() {
    while (true) {
        cout << endl;
        cout << "\nStudent Management System\n";
        cout << "a. Show Student list\n";
        cout << "b. Add Student\n";
        cout << "c. Show single student\n";
        cout << "d. Show all records\n";
        cout << "e. Add record\n";
        cout << "f. Show single student records\n";
        cout << "g. Exit\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;

        switch (choice) {
        case 'a':
            showStudentList();
            break;
        case 'b':
            addStudent();
            break;
        case 'c':
            showSingleStudent();
            break;
        case 'd':
            showAllRecords();
            break;
        case 'e':
            addRecord();
            break;
        case 'f':
            showSingleStudentRecords();
            break;
        case 'g':
            cout << "Exiting the program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
    return 0;
}