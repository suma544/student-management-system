// Student Management System
// Console-based, menu-driven C++ program with persistent file storage.
// Features: Add, Display, Search, Update, Delete student records.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

using namespace std;

const string DATA_FILE = "students.txt";

struct Student {
    int roll;
    string name;
    int age;
    string course;
    double marks;
};

class StudentManager {
    vector<Student> students;

    // Fields are stored one record per line, separated by '|'
    void load() {
        students.clear();
        ifstream in(DATA_FILE);
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string roll, name, age, course, marks;
            getline(ss, roll, '|');
            getline(ss, name, '|');
            getline(ss, age, '|');
            getline(ss, course, '|');
            getline(ss, marks, '|');
            try {
                students.push_back({stoi(roll), name, stoi(age), course, stod(marks)});
            } catch (...) {
                // skip corrupted line
            }
        }
    }

    void save() const {
        ofstream out(DATA_FILE, ios::trunc);
        for (const auto &s : students)
            out << s.roll << '|' << s.name << '|' << s.age << '|'
                << s.course << '|' << s.marks << '\n';
    }

    int findIndex(int roll) const {
        for (size_t i = 0; i < students.size(); ++i)
            if (students[i].roll == roll) return (int)i;
        return -1;
    }

    static string grade(double m) {
        if (m >= 90) return "A+";
        if (m >= 80) return "A";
        if (m >= 70) return "B";
        if (m >= 60) return "C";
        if (m >= 40) return "D";
        return "F";
    }

    static void header() {
        cout << left << setw(8) << "Roll" << setw(20) << "Name" << setw(6) << "Age"
             << setw(15) << "Course" << setw(8) << "Marks" << "Grade\n";
        cout << string(63, '-') << '\n';
    }

    static void row(const Student &s) {
        cout << left << setw(8) << s.roll << setw(20) << s.name << setw(6) << s.age
             << setw(15) << s.course << setw(8) << fixed << setprecision(1) << s.marks
             << grade(s.marks) << '\n';
    }

public:
    StudentManager() { load(); }

    static int readInt(const string &prompt) {
        int v;
        while (true) {
            cout << prompt;
            if (cin >> v) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
            if (cin.eof()) exit(0);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid number, try again.\n";
        }
    }

    static double readDouble(const string &prompt) {
        double v;
        while (true) {
            cout << prompt;
            if (cin >> v && v >= 0 && v <= 100) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
            if (cin.eof()) exit(0);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Enter marks between 0 and 100.\n";
        }
    }

    static string readLine(const string &prompt) {
        string s;
        while (true) {
            cout << prompt;
            if (!getline(cin, s)) exit(0);
            s.erase(remove(s.begin(), s.end(), '|'), s.end());
            if (!s.empty()) return s;
            cout << "  Cannot be empty.\n";
        }
    }

    void addStudent() {
        cout << "\n--- Add Student ---\n";
        int roll = readInt("Roll number: ");
        if (findIndex(roll) != -1) { cout << "Student with roll " << roll << " already exists.\n"; return; }
        Student s;
        s.roll = roll;
        s.name = readLine("Name: ");
        s.age = readInt("Age: ");
        s.course = readLine("Course: ");
        s.marks = readDouble("Marks (0-100): ");
        students.push_back(s);
        save();
        cout << "Student added and saved to " << DATA_FILE << ".\n";
    }

    void displayAll() const {
        cout << "\n--- All Students (" << students.size() << ") ---\n";
        if (students.empty()) { cout << "No records found.\n"; return; }
        header();
        for (const auto &s : students) row(s);
    }

    void searchStudent() const {
        cout << "\n--- Search Student ---\n";
        int idx = findIndex(readInt("Enter roll number: "));
        if (idx == -1) { cout << "Student not found.\n"; return; }
        header();
        row(students[idx]);
    }

    void updateStudent() {
        cout << "\n--- Update Student ---\n";
        int idx = findIndex(readInt("Enter roll number to update: "));
        if (idx == -1) { cout << "Student not found.\n"; return; }
        Student &s = students[idx];
        cout << "Current record:\n"; header(); row(s);
        s.name = readLine("New name: ");
        s.age = readInt("New age: ");
        s.course = readLine("New course: ");
        s.marks = readDouble("New marks (0-100): ");
        save();
        cout << "Record updated.\n";
    }

    void deleteStudent() {
        cout << "\n--- Delete Student ---\n";
        int roll = readInt("Enter roll number to delete: ");
        int idx = findIndex(roll);
        if (idx == -1) { cout << "Student not found.\n"; return; }
        students.erase(students.begin() + idx);
        save();
        cout << "Student with roll " << roll << " deleted.\n";
    }
};

int main() {
    StudentManager sm;
    while (true) {
        cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n"
             << "1. Add Student\n"
             << "2. Display All Students\n"
             << "3. Search Student\n"
             << "4. Update Student\n"
             << "5. Delete Student\n"
             << "6. Exit\n";
        int choice = StudentManager::readInt("Enter your choice: ");
        switch (choice) {
            case 1: sm.addStudent(); break;
            case 2: sm.displayAll(); break;
            case 3: sm.searchStudent(); break;
            case 4: sm.updateStudent(); break;
            case 5: sm.deleteStudent(); break;
            case 6: cout << "Exiting... Data saved in " << DATA_FILE << ". Goodbye!\n"; return 0;
            default: cout << "Invalid choice. Please select 1-6.\n";
        }
    }
}
