#define _CRT_SECURE_NO_WARNINGS

#include <limits>

#include <iostream>
#include <cstring>
#include <ctime>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;



bool isValidCourseCode(string code);
int MainMenu();
void studentManagement();
void courseManagement();
void gradesManagement();

//*********************STRUCTURES******************

struct Student
{
    string name;
    string nationalID;
    char gender;
    char DOB[11];
    string phone;
    string program;
    int level;
    float GPA;
    string ID;

    string registeredCourses[10];
    int registeredCourseCount;
};

Student students[1200];
int studentCount = 0;


struct Course
{
    string code;
    string title;
    int creditHours;
};

Course courses[200];
int courseCount = 0;


struct Grade
{
    string studentID;
    string courseCode;
    float midterm;
    float finalExam;
    float total;
    char letterGrade;
    float points;
};

Grade grades[5000];
int gradeCount = 0;

//************************************VALIDATIONS FUNCTIONS*************************

bool isValidCourseCode(string code)
{
    if (code.length() != 6)
        return false;

    for (int i = 0; i < 3; i++)
    {
        if (code[i] < 'A' || code[i] > 'Z')
        {
            return false;
        }
    }

    for (int i = 3; i < 6; i++)
    {
        if (code[i] < '0' || code[i] > '9')
        {
            return false;
        }
    }

    return true;
}

bool isValid(char DOB[])
{
    return (DOB[2] == '/' && DOB[5] == '/' &&

        DOB[0] >= '0' && DOB[0] <= '3' &&

        DOB[1] >= '0' && DOB[1] <= '9' &&

        DOB[3] >= '0' && DOB[3] <= '1' &&

        DOB[4] >= '0' && DOB[4] <= '9' &&

        DOB[6] >= '1' && DOB[6] <= '2' &&

        DOB[7] >= '0' && DOB[7] <= '9' &&

        DOB[8] >= '0' && DOB[8] <= '9' &&

        DOB[9] >= '0' && DOB[9] <= '9');
}

bool isValidStudentID(string id)
{
    if (id.length() != 7)
        return false;

    if (!(id[0] >= '0' && id[0] <= '9'))
        return false;

    if (!(id[1] >= '0' && id[1] <= '9'))
        return false;

    if (id[2] != 'P')
        return false;

    for (int i = 3; i < 7; i++)
    {
        if (!(id[i] >= '0' && id[i] <= '9'))
            return false;
    }

    return true;
}
//*************************************FILE FUNCTIONS***************************
string cleanExcelText(string value)
{
    if (value.length() >= 4 &&
        value[0] == '=' &&
        value[1] == '"' &&
        value[value.length() - 1] == '"')
    {
        return value.substr(2, value.length() - 3);
    }

    return value;
}


void saveToFile(Student students[], int studentCount)
{
    ofstream file("students.csv");

    for (int i = 0; i < studentCount; i++)
    {
        file << students[i].name << ","
            << "=\"" << students[i].nationalID << "\"" << ","
            << students[i].gender << ","
            << students[i].DOB << ","
            << "=\"" << students[i].phone << "\"" << ","
            << students[i].program << ","
            << students[i].level << ","
            << students[i].GPA << ","
            << students[i].ID << ","
            << students[i].registeredCourseCount;

        for (int j = 0; j < students[i].registeredCourseCount; j++)
        {
            file << "," << students[i].registeredCourses[j];
        }

        file << "\n";
    }

    file.close();
}


void loadFromFile(Student students[], int& studentCount)
{
    ifstream file("students.csv");
    if (!file) return;

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        Student s;
        string temp;

        getline(ss, s.name, ',');

        getline(ss, s.nationalID, ',');
        s.nationalID = cleanExcelText(s.nationalID);

        getline(ss, temp, ',');
        s.gender = temp[0];

        getline(ss, temp, ',');
        strncpy(s.DOB, temp.c_str(), 10);
        s.DOB[10] = '\0';

        getline(ss, s.phone, ',');
        s.phone = cleanExcelText(s.phone);

        getline(ss, s.program, ',');

        getline(ss, temp, ',');
        s.level = stoi(temp);

        getline(ss, temp, ',');
        s.GPA = stof(temp);

        getline(ss, temp, ',');
        s.ID = temp;
        s.registeredCourseCount = 0;

        if (getline(ss, temp, ','))
        {
            s.registeredCourseCount = stoi(temp);

            if (s.registeredCourseCount < 0)
                s.registeredCourseCount = 0;

            if (s.registeredCourseCount > 10)
                s.registeredCourseCount = 10;

            for (int j = 0; j < s.registeredCourseCount; j++)
            {
                if (!getline(ss, s.registeredCourses[j], ','))
                {
                    s.registeredCourses[j] = "";
                }
            }
        }

        if (studentCount < 1200)
        {
            students[studentCount] = s;
            studentCount++;
        }
    }

    file.close();
}


void PrintStudentBox(const Student& s)
{
    cout << "_____________________________________________________" << endl;
    cout << "|" << left << setw(50) << ("Student ID: " + s.ID) << "\n";
    cout << "|" << left << setw(50) << ("Name: " + s.name) << "\n";
    cout << "|" << left << setw(50) << ("National ID: " + s.nationalID) << "\n";

    string genderDOB = "Gender: ";
    genderDOB += s.gender;
    genderDOB += " | DOB: ";
    genderDOB += s.DOB;

    cout << "|" << left << setw(50) << genderDOB << "\n";
    cout << "|" << left << setw(50) << ("Phone: " + s.phone) << "\n";

    string programLine = "Program: " + s.program +
        " | Level: " + to_string(s.level) +
        " | GPA: " + to_string(s.GPA);

    cout << "|" << left << setw(50) << programLine << "\n";

    cout << "|" << left << setw(50) << "Registered Courses:" << "\n";

    if (s.registeredCourseCount == 0)
    {
        cout << "|" << left << setw(50) << "None" << "\n";
    }
    else
    {
        for (int i = 0; i < s.registeredCourseCount; i++)
        {
            cout << "|" << left << setw(50) << s.registeredCourses[i] << "\n";
        }
    }

    cout << "_____________________________________________________" << endl;
}

//**************************************************ADD NEW STUDENT**************************************************
void Newstudent(Student students[], int& studentCount)
{
    if (studentCount >= 1200)
    {
        cout << "Storage full!\n";
        return;
    }

    Student newStudent;
    cin.ignore(1000, '\n');

    while (true)
    {
        cout << "Enter Student Name: ";
        getline(cin, newStudent.name);

        int wordCount = 0;
        bool inWord = false;
        bool valid = true;

        for (int i = 0; newStudent.name[i] != '\0'; i++)
        {
            if (newStudent.name[i] != ' ' && newStudent.name[i] != '\t' && newStudent.name[i] != '\n')
            {
                if (!inWord)
                {
                    wordCount++;
                    inWord = true;
                }
            }
            else
            {
                inWord = false;
            }

            if (!((newStudent.name[i] >= 'a' && newStudent.name[i] <= 'z') ||
                (newStudent.name[i] >= 'A' && newStudent.name[i] <= 'Z') ||
                newStudent.name[i] == ' '))
            {
                valid = false;
            }
        }

        if (wordCount == 2 && valid)
        {
            break;
        }
        else
        {
            cout << "Enter only 2 names (first and last) or use only alphabetical letters\n";
        }
    }

    cout << "Enter National ID: ";
    getline(cin, newStudent.nationalID);

    bool unique;

    do
    {
        unique = true;

        while (newStudent.nationalID.length() != 14 || newStudent.nationalID[0] == '0')
        {
            cout << "Invalid input. Enter 14 digits only with no leading zeros: ";
            getline(cin, newStudent.nationalID);
            continue;
        }

        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].nationalID == newStudent.nationalID)
            {
                cout << "Invalid ID. National ID is not unique. Enter again: ";
                getline(cin, newStudent.nationalID);
                unique = false;
                break;
            }
        }

    } while (!unique);

    cout << "Enter Gender (M/F): ";
    cin >> newStudent.gender;

    while (newStudent.gender != 'M' && newStudent.gender != 'm' &&
        newStudent.gender != 'F' && newStudent.gender != 'f')
    {
        cout << "Invalid input. Enter M or F: ";
        cin >> newStudent.gender;
    }

    time_t now = time(NULL);
    struct tm timeinfo;

#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    localtime_r(&now, &timeinfo);
#endif

    int currentYear = timeinfo.tm_year + 1900;
    cin.ignore();

    int birthYear, age;

    do
    {
        cout << "Enter DOB (DD/MM/YYYY): ";
        cin.getline(newStudent.DOB, 11);

        birthYear =
            (newStudent.DOB[6] - '0') * 1000 +
            (newStudent.DOB[7] - '0') * 100 +
            (newStudent.DOB[8] - '0') * 10 +
            (newStudent.DOB[9] - '0');

        age = currentYear - birthYear;

        if (!isValid(newStudent.DOB) ||
            (newStudent.DOB[3] == '1' && newStudent.DOB[4] > '2') ||
            (newStudent.DOB[0] == '3' && newStudent.DOB[1] > '1') ||
            (newStudent.DOB[3] == '0' && newStudent.DOB[4] == '2' &&
                (newStudent.DOB[0] > '2' ||
                    (newStudent.DOB[0] == '2' && newStudent.DOB[1] > '8'))) ||
            age < 17)
        {
            cout << "Invalid DOB or age must be at least 17.\n";
        }

    } while (!isValid(newStudent.DOB) ||
        (newStudent.DOB[3] == '1' && newStudent.DOB[4] > '2') ||
        (newStudent.DOB[0] == '3' && newStudent.DOB[1] > '1') ||
        (newStudent.DOB[3] == '0' && newStudent.DOB[4] == '2' &&
            (newStudent.DOB[0] > '2' ||
                (newStudent.DOB[0] == '2' && newStudent.DOB[1] > '8'))) ||
        age < 17);

    cout << "Enter Phone Number: ";
    getline(cin, newStudent.phone);

    while (newStudent.phone.length() != 11 || newStudent.phone.substr(0, 2) != "01")
    {
        cout << "Invalid phone. Enter 11 digits only starting with 01: ";
        getline(cin, newStudent.phone);
    }

    cout << "Enter Program (CSE / CCE / MCT): ";
    getline(cin, newStudent.program);

    while (newStudent.program != "CSE" && newStudent.program != "CCE" && newStudent.program != "MCT")
    {
        cout << "Invalid. Enter either CSE or CCE or MCT: ";
        getline(cin, newStudent.program);
    }

    cout << "Enter Academic Level (1-4): ";
    cin >> newStudent.level;

    while (newStudent.level < 1 || newStudent.level > 4)
    {
        cout << "Invalid level. Enter between 1 and 4: ";
        cin >> newStudent.level;
    }

    newStudent.GPA = 0.0;
    newStudent.registeredCourseCount = 0;

    string yearPart = to_string(currentYear % 100);

    if (yearPart.length() == 1)
    {
        yearPart = "0" + yearPart;
    }

    int serial = studentCount + 1;

    string serialPart = to_string(serial);

    while (serialPart.length() < 4)
    {
        serialPart = "0" + serialPart;
    }

    newStudent.ID = yearPart + "P" + serialPart;

    students[studentCount] = newStudent;
    studentCount++;

    saveToFile(students, studentCount);

    cout << "\nStudent added successfully!\n";
    cout << "Student ID: " << newStudent.ID << endl;

    MainMenu();
}

//***********************************************************LIST STUDENT***************************************

void ListAllStudents(Student students[], int studentCount)
{
    if (studentCount == 0)
    {
        cout << "No students found.\n";
        studentManagement();
    }

    int choice;

    cout << "\n=== List All Students ===\n";
    cout << "Sort by:\n";
    cout << "1. Student ID\n";
    cout << "2. Name (A-Z)\n";
    cout << "3. GPA (Highest First)\n";
    cout << "4. Back\n";
    cout << "\nEnter your choice: ";
    cin >> choice;

    if (choice == 4)
    {
        studentManagement();
        return;
    }

    if (choice < 1 || choice > 3)
    {
        cout << "Invalid choice.\n";
        return;
    }

    for (int i = 0; i < studentCount - 1; i++)
    {
        for (int j = 0; j < studentCount - i - 1; j++)
        {
            bool condition = false;

            if (choice == 1)
                condition = students[j].ID > students[j + 1].ID;
            else if (choice == 2)
                condition = students[j].name > students[j + 1].name;
            else if (choice == 3)
                condition = students[j].GPA < students[j + 1].GPA;

            if (condition)
            {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    cout << "\nTotal Students: " << studentCount << endl;

    cout << left
        << setw(15) << "ID"
        << setw(25) << "Name"
        << setw(12) << "Program"
        << setw(10) << "Level"
        << setw(8) << "GPA" << endl;

    cout << "---------------------------------------------------------------------\n";

    for (int i = 0; i < studentCount; i++)
    {
        cout << left
            << setw(15) << students[i].ID
            << setw(25) << students[i].name
            << setw(12) << students[i].program
            << setw(10) << students[i].level
            << setw(8) << students[i].GPA
            << endl;
    }

    MainMenu();
}

//***************************************SEARCH STUDENT**********************************//


void SearchStudents(Student students[], int studentCount)
{
    int choice;

    cout << "\n=== Search Students ===\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "3. Search by National ID\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        string id;

        while (true)
        {
            cout << "Enter Student ID: ";
            cin >> id;

            if (cin.fail())
            {
                cout << "Invalid ID (numbers only)\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!isValidStudentID(id))
            {
                cout << "Invalid ID format. Example: 26P0001\n";
                continue;
            }

            break;
        }

        bool found = false;

        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].ID == id)
            {
                PrintStudentBox(students[i]);
                found = true;
                break;
            }
        }

        if (!found)
            cout << "No student found with the given criteria\n";

        break;
    }

    case 2:
    {
        cin.ignore();

        string name;

        while (true)
        {
            cout << "Enter Student Name: ";
            getline(cin, name);

            int wordCount = 0;
            bool inWord = false;
            bool valid = true;

            for (int i = 0; name[i] != '\0'; i++)
            {
                if (name[i] != ' ' && name[i] != '\t' && name[i] != '\n')
                {
                    if (!inWord)
                    {
                        wordCount++;
                        inWord = true;
                    }
                }
                else
                {
                    inWord = false;
                }

                if (!((name[i] >= 'a' && name[i] <= 'z') ||
                    (name[i] >= 'A' && name[i] <= 'Z') ||
                    name[i] == ' '))
                {
                    valid = false;
                }
            }

            if (wordCount == 2 && valid)
            {
                break;
            }
            else
            {
                cout << "Enter only 2 names or use only alphabetical letters\n";
            }
        }

        for (int i = 0; i < name.length(); i++)
        {
            if (name[i] >= 'A' && name[i] <= 'Z')
                name[i] += 32;
        }

        int count = 0;

        for (int i = 0; i < studentCount; i++)
        {
            string temp = students[i].name;

            for (int j = 0; j < temp.length(); j++)
            {
                if (temp[j] >= 'A' && temp[j] <= 'Z')
                    temp[j] += 32;
            }

            if (temp == name)
                count++;
        }

        if (count == 0)
        {
            cout << "No student found with the given criteria.\n";
        }
        else
        {
            cout << "\nFound " << count << " student(s):\n";

            for (int i = 0; i < studentCount; i++)
            {
                string temp = students[i].name;

                for (int j = 0; j < temp.length(); j++)
                {
                    if (temp[j] >= 'A' && temp[j] <= 'Z')
                        temp[j] += 32;
                }

                if (temp == name)
                {
                    PrintStudentBox(students[i]);
                }
            }
        }

        break;
    }

    case 3:
    {
        cin.ignore();
        string nid;

        cout << "Enter National ID: ";
        getline(cin, nid);

        while (nid.length() != 14 || nid[0] == '0')
        {
            cout << "ERROR: Invalid Student ID Format. Enter again: ";
            getline(cin, nid);
        }

        bool found = false;

        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].nationalID == nid)
            {
                PrintStudentBox(students[i]);
                found = true;
                break;
            }
        }

        if (!found)
            cout << "No student found with the given criteria.\n";

        break;
    }

    case 4:
        studentManagement();
        return;

    default:
        cout << "Invalid choice\n";
    }

    MainMenu();
}

//**************************************UPDATE STUDENT***************************//


void updatestudent()
{
    string id;

    cout << "\n=== Student update ===\n";
    cout << "Enter Student ID to be updated: ";
    cin >> id;

    int index = -1;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].ID == id)
        {
            cout << "Student found: " << students[i].name << endl;
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "No student exists with ID: " << id << "\n";
        MainMenu();
        return;
    }

    int choice;

    cout << "\nWhich field do you want to update?\n";
    cout << "1. Phone Number\n";
    cout << "2. Program\n";
    cout << "3. Academic Level\n";
    cout << "4. Cancel\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
    {
        string newPhone;
        cout << "Enter new phone number: ";
        cin >> newPhone;

        while (newPhone.length() != 11 || newPhone.substr(0, 2) != "01")
        {
            cout << "Invalid phone. Enter 11 digits starting with 01: ";
            cin >> newPhone;
        }

        students[index].phone = newPhone;
        cout << "Phone number updated successfully!\n";
        break;
    }

    case 2:
    {
        string newProgram;
        cout << "Enter Program (CSE / CCE / MCT): ";
        cin.ignore();
        getline(cin, newProgram);

        while (newProgram != "CSE" && newProgram != "CCE" && newProgram != "MCT")
        {
            cout << "Invalid. Enter CSE or CCE or MCT: ";
            getline(cin, newProgram);
        }

        students[index].program = newProgram;
        cout << "Program updated successfully!\n";
        break;
    }

    case 3:
    {
        int newLevel;
        cout << "Enter new academic level: ";
        cin >> newLevel;

        while (newLevel < 1 || newLevel > 4)
        {
            cout << "Invalid level (1-4). Enter again: ";
            cin >> newLevel;
        }

        students[index].level = newLevel;
        cout << "Academic level updated successfully!\n";
        break;
    }

    case 4:
        cout << "Update cancelled.\n";
        break;

    default:
        cout << "Invalid choice.\n";
        break;
    }

    saveToFile(students, studentCount);

    MainMenu();
}

//****************************************DELETE STUDENT**************************//


void deleteStudent()
{
    string id;

    cout << "\n=== Delete Student ===\n";
    cout << "Enter Student ID to delete: ";
    cin >> id;


    int index = -1;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].ID == id)
        {
            index = i;
            cout << "Student found: " << students[i].name << endl;
            break;
        }
    }

    if (index == -1)
    {

        cout << "No student exists with ID: " << id << "\n";
        deleteStudent();
        return;
    }

    char confirm;
    cout << "Are you sure you want to delete this student? (Y/N): ";
    cin >> confirm;

    while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n')
    {
        cout << "Invalid choice. Enter Y or N: ";
        cin >> confirm;
    }

    if (confirm == 'N' || confirm == 'n')
    {
        cout << "Operation cancelled.\n";
        MainMenu();
        return;
    }

    for (int i = index; i < studentCount - 1; i++)
    {
        students[i] = students[i + 1];
    }

    studentCount--;

    saveToFile(students, studentCount);

    cout << "Student deleted successfully!\n";

    MainMenu();
}

//********************************************STUDENT MANAGEMENT MENU******************************//


void studentManagement()
{
    int choice;

    cout << "\n=== Student Management ===\n";
    cout << "1. Add New Student\n";
    cout << "2. List All Students\n";
    cout << "3. Search Students\n";
    cout << "4. Update Students\n";
    cout << "5. Delete student\n";
    cout << "6. Back to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        Newstudent(students, studentCount);
        break;

    case 2:
        ListAllStudents(students, studentCount);
        break;

    case 3:
        SearchStudents(students, studentCount);
        break;

    case 4:
        updatestudent();
        break;

    case 5:
        deleteStudent();
        break;

    case 6:
        MainMenu();
        break;

    default:
        cout << "Invalid choice\n";
        studentManagement();
        break;
    }
}


void saveCoursesToFile()
{
    ofstream file("courses.csv");

    for (int i = 0; i < courseCount; i++)
    {
        file << courses[i].code << ","
            << courses[i].title << ","
            << courses[i].creditHours << "\n";
    }

    file.close();
}


void loadCoursesFromFile()
{
    ifstream file("courses.csv");

    if (!file)
        return;

    Course c;

    while (getline(file, c.code, ',') &&
        getline(file, c.title, ',') &&
        file >> c.creditHours)
    {
        file.ignore();

        if (courseCount < 200)
        {
            courses[courseCount] = c;
            courseCount++;
        }
    }

    file.close();
}

//*************************************ADD NEW COURSE******************************//


void addNewCourse()
{
    if (courseCount >= 200)
    {
        cout << "Course storage full!\n";
        return;
    }

    Course newCourse;

    cin.ignore();

    while (true)
    {
        bool valid = true;

        cout << "Enter Course Code: ";
        getline(cin, newCourse.code);

        if (newCourse.code.length() != 6)
        {
            valid = false;
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                if (!(newCourse.code[i] >= 'A' && newCourse.code[i] <= 'Z'))
                    valid = false;
            }

            for (int i = 3; i < 6; i++)
            {
                if (!(newCourse.code[i] >= '0' && newCourse.code[i] <= '9'))
                    valid = false;
            }
        }

        bool unique = true;

        for (int i = 0; i < courseCount; i++)
        {
            if (courses[i].code == newCourse.code)
            {
                unique = false;
                break;
            }
        }

        if (!valid)
        {
            cout << "Invalid code format. Must be 3 letters + 3 digits (Ex: CSE141)\n";
        }
        else if (!unique)
        {
            cout << "Course code already exists.\n";
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        cout << "Enter Course Title: ";
        getline(cin, newCourse.title);

        if (newCourse.title.empty())
            cout << "Course title cannot be empty.\n";
        else
            break;
    }

    cout << "Enter Credit Hours (1-4): ";
    cin >> newCourse.creditHours;

    while (newCourse.creditHours < 1 || newCourse.creditHours > 4)
    {
        cout << "Invalid credit hours. Enter from 1 to 4: ";
        cin >> newCourse.creditHours;
    }

    courses[courseCount] = newCourse;
    courseCount++;

    saveCoursesToFile();

    cout << "\nCourse added successfully!\n";

    MainMenu();
}

//*******************************************VIEW ALL COURSES*************************//


void viewAllCourses()
{
    if (courseCount == 0)
    {
        cout << "No courses found.\n";
        MainMenu();
        return;
    }

    cout << "\n=========== All Courses ===========\n";

    cout << left
        << setw(15) << "Code"
        << setw(35) << "Title"
        << setw(15) << "Credit Hours"
        << endl;

    cout << "----------------------------------------------------------\n";

    for (int i = 0; i < courseCount; i++)
    {
        cout << left
            << setw(15) << courses[i].code
            << setw(35) << courses[i].title
            << setw(15) << courses[i].creditHours
            << endl;
    }

    MainMenu();
}

//*******************************************UPDATE COURSES********************************//


void updateCourse()
{
    cin.ignore();

    string code;

    int index;

    while (true)
    {
        cout << "\nEnter Course Code to update: ";
        getline(cin, code);

        if (isValidCourseCode(code) == false)
        {
            cout << "Incorrect format.\n";
            continue;
        }

        index = -1;

        for (int i = 0; i < courseCount; i++)
        {
            if (courses[i].code == code)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            cout << "Course is not registered.\n";
            continue;
        }

        break;
    }

    int choice;

    cout << "\n1. Update Title\n";
    cout << "2. Update Credit Hours\n";
    cout << "3. Cancel\n";
    cout << "Enter your choice: ";
    cin >> choice;

    cin.ignore();

    switch (choice)
    {
    case 1:
    {
        cout << "Enter New Course Title: ";
        getline(cin, courses[index].title);

        while (courses[index].title.empty())
        {
            cout << "Title cannot be empty. Enter again: ";
            getline(cin, courses[index].title);
        }

        cout << "Course title updated successfully!\n";
        break;
    }

    case 2:
    {
        cout << "Enter New Credit Hours (1-4): ";
        cin >> courses[index].creditHours;

        while (courses[index].creditHours < 1 || courses[index].creditHours > 4)
        {
            cout << "Invalid. Enter from 1 to 4: ";
            cin >> courses[index].creditHours;
        }

        cout << "Credit hours updated successfully!\n";
        break;
    }

    case 3:
        cout << "Operation cancelled.\n";
        break;

    default:
        cout << "Invalid choice.\n";
        break;
    }

    saveCoursesToFile();

    MainMenu();
}

//**********************************DELETE COURSES***************************************//


void deleteCourse()
{
    cin.ignore();

    string code;

    cout << "\nEnter Course Code to delete: ";
    getline(cin, code);

    int index = -1;

    for (int i = 0; i < courseCount; i++)
    {
        if (courses[i].code == code)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Course not found.\n";
        MainMenu();
        return;
    }

    char confirm;

    cout << "Are you sure you want to delete this course? (Y/N): ";
    cin >> confirm;

    while (confirm != 'Y' && confirm != 'y' &&
        confirm != 'N' && confirm != 'n')
    {
        cout << "Invalid choice. Enter Y or N: ";
        cin >> confirm;
    }

    if (confirm == 'Y' || confirm == 'y')
    {
        for (int i = index; i < courseCount - 1; i++)
        {
            courses[i] = courses[i + 1];
        }

        courseCount--;

        saveCoursesToFile();

        cout << "Course deleted successfully!\n";
    }
    else
    {
        cout << "Operation cancelled.\n";
    }

    MainMenu();
}

//********************************REGISTER STUDENT TO COURSE***************************//


void registerCourseToStudent()
{
    string studentID;
    string courseCode;

    cout << "\nEnter Student ID: ";
    cin >> studentID;

    int studentIndex = -1;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].ID == studentID)
        {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1)
    {
        cout << "Student not found.\n";
        registerCourseToStudent();
        return;
    }

    cin.ignore();

    cout << "Enter Course Code: ";
    getline(cin, courseCode);

    int courseIndex = -1;

    for (int i = 0; i < courseCount; i++)
    {
        if (courses[i].code == courseCode)
        {
            courseIndex = i;
            break;
        }
    }

    if (courseIndex == -1)
    {
        cout << "Course not found.\n";
        MainMenu();
        return;
    }

    for (int i = 0; i < students[studentIndex].registeredCourseCount; i++)
    {
        if (students[studentIndex].registeredCourses[i] == courseCode)
        {
            cout << "Student is already registered in this course.\n";
            MainMenu();
            return;
        }
    }

    if (students[studentIndex].registeredCourseCount >= 10)
    {
        cout << "Student cannot register more than 10 courses.\n";
        MainMenu();
        return;
    }

    int pos = students[studentIndex].registeredCourseCount;

    students[studentIndex].registeredCourses[pos] = courseCode;
    students[studentIndex].registeredCourseCount++;

    saveToFile(students, studentCount);

    cout << "Course registered to student successfully.\n";

    MainMenu();
}



//**************************************COURSE MANAGEMENT MENU***************************//


void courseManagement()
{
    int choice;

    cout << "\n=== Course Management ===\n";
    cout << "1. Add New Course\n";
    cout << "2. View All Courses\n";
    cout << "3. Update Course\n";
    cout << "4. Delete Course\n";
    cout << "5. Register Course to Student\n";
    cout << "6. Back to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        addNewCourse();
        break;

    case 2:
        viewAllCourses();
        break;

    case 3:
        updateCourse();
        break;

    case 4:
        deleteCourse();
        break;

    case 5:
        registerCourseToStudent();
        break;

    case 6:
        MainMenu();
        break;

    default:
        cout << "Invalid choice\n";
        courseManagement();
        break;
    }
}

// **************************************GRADES FILE********************************
void saveGradesToFile()
{
    ofstream file("grades.csv");

    for (int i = 0; i < gradeCount; i++)
    {
        file << grades[i].studentID << ","
            << grades[i].courseCode << ","
            << grades[i].midterm << ","
            << grades[i].finalExam << ","
            << grades[i].total << ","
            << grades[i].letterGrade << ","
            << grades[i].points << "\n";
    }

    file.close();
}


void loadGradesFromFile()
{
    ifstream file("grades.csv");

    if (!file)
        return;

    Grade g;

    while (getline(file, g.studentID, ','))
    {
        file.ignore();

        getline(file, g.courseCode, ',');

        file >> g.midterm;
        file.ignore();

        file >> g.finalExam;
        file.ignore();

        file >> g.total;
        file.ignore();

        file >> g.letterGrade;
        file.ignore();

        file >> g.points;
        file.ignore();

        if (gradeCount < 5000)
        {
            grades[gradeCount] = g;
            gradeCount++;
        }
    }

    file.close();
}


//******************************GRADES LETTER*********************//


char getLetterGrade(float total)
{
    if (total >= 90)
        return 'A';
    else if (total >= 80)
        return 'B';
    else if (total >= 70)
        return 'C';
    else if (total >= 60)
        return 'D';
    else
        return 'F';
}

//***************************************GRADES POINT**************************//


float getGradePoints(char grade)
{
    if (grade == 'A')
        return 4.0;
    else if (grade == 'B')
        return 3.0;
    else if (grade == 'C')
        return 2.0;
    else if (grade == 'D')
        return 1.0;
    else
        return 0.0;
}

//*************************************ENTER GRADES***************************//


void enterStudentGrades()
{
    Grade newGrade;

    cout << "\nEnter Student ID: ";
    cin >> newGrade.studentID;

    int studentIndex = -1;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].ID == newGrade.studentID)
        {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1)
    {
        cout << "Student ID does not exist.\n";
        gradesManagement();
        return;
    }

    cin.ignore();

    cout << "Enter Course Code: ";
    getline(cin, newGrade.courseCode);

    int courseIndex = -1;

    for (int i = 0; i < courseCount; i++)
    {
        if (courses[i].code == newGrade.courseCode)
        {
            courseIndex = i;
            break;
        }
    }

    if (courseIndex == -1)
    {
        cout << "Course code does not exist.\n";
        gradesManagement();
        return;
    }

    bool registered = false;

    for (int i = 0; i < students[studentIndex].registeredCourseCount; i++)
    {
        if (students[studentIndex].registeredCourses[i] == newGrade.courseCode)
        {
            registered = true;
            break;
        }
    }

    if (!registered)
    {
        cout << "This student is not registered in this course.\n";
        gradesManagement();
        return;
    }

    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentID == newGrade.studentID &&
            grades[i].courseCode == newGrade.courseCode)
        {
            cout << "Grade already exists for this course.\n";
            gradesManagement();
            return;
        }
    }

    cout << "Enter Midterm Grade (0-40): ";
    cin >> newGrade.midterm;

    while (newGrade.midterm < 0 || newGrade.midterm > 40)
    {
        cout << "Invalid grade. Enter between 0 and 40: ";
        cin >> newGrade.midterm;
    }

    cout << "Enter Final Grade (0-60): ";
    cin >> newGrade.finalExam;

    while (newGrade.finalExam < 0 || newGrade.finalExam > 60)
    {
        cout << "Invalid grade. Enter between 0 and 60: ";
        cin >> newGrade.finalExam;
    }

    newGrade.total = newGrade.midterm + newGrade.finalExam;
    newGrade.letterGrade = getLetterGrade(newGrade.total);
    newGrade.points = getGradePoints(newGrade.letterGrade);

    grades[gradeCount] = newGrade;
    gradeCount++;

    saveGradesToFile();

    cout << "\nTotal: " << newGrade.total << endl;
    cout << "Letter Grade: " << newGrade.letterGrade << endl;
    cout << "Grade recorded successfully!\n";

    MainMenu();
}

//*******************************************VIEW STUDENTS GRADES******************************//


void viewStudentGrades()
{
    string id;

    cout << "\nEnter Student ID: ";
    cin >> id;

    bool found = false;

    cout << "\n============================================================\n";

    cout << left
        << setw(15) << "Course"
        << setw(15) << "Midterm"
        << setw(15) << "Final"
        << setw(15) << "Total"
        << setw(10) << "Grade"
        << endl;

    cout << "============================================================\n";

    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentID == id)
        {
            found = true;

            cout << left
                << setw(15) << grades[i].courseCode
                << setw(15) << grades[i].midterm
                << setw(15) << grades[i].finalExam
                << setw(15) << grades[i].total
                << setw(10) << grades[i].letterGrade
                << endl;
        }
    }

    if (!found)
    {
        cout << "No grades found.\n";
    }

    MainMenu();
}

//***************************************CALCULATE GPA******************************//


void calculateGPA()
{
    string id;

    cout << "\nEnter Student ID: ";
    cin >> id;

    float totalPoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentID == id)
        {
            for (int j = 0; j < courseCount; j++)
            {
                if (courses[j].code == grades[i].courseCode)
                {
                    totalPoints += grades[i].points * courses[j].creditHours;
                    totalCredits += courses[j].creditHours;
                }
            }
        }
    }

    if (totalCredits == 0)
    {
        cout << "No grades available.\n";
        MainMenu();
        return;
    }

    float GPA = totalPoints / totalCredits;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].ID == id)
        {
            students[i].GPA = GPA;
            break;
        }
    }

    saveToFile(students, studentCount);

    cout << fixed << setprecision(2);
    cout << "Calculated GPA: " << GPA << endl;

    MainMenu();
}

//*****************************************GENERATE TRANSCRIPT***********************************//


void generateTranscript()
{
    string id;

    cout << "\nEnter Student ID: ";
    cin >> id;

    int studentIndex = -1;

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].ID == id)
        {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1)
    {
        cout << "Student not found.\n";
        MainMenu();
        return;
    }

    Student s = students[studentIndex];

    cout << "\n============================================================\n";
    cout << "\t\tSTUDENT TRANSCRIPT\n";
    cout << "============================================================\n";

    cout << "Student ID: " << s.ID << endl;
    cout << "Name: " << s.name << endl;
    cout << "Program: " << s.program
        << " | Level: " << s.level << endl;

    cout << "\n";

    cout << left
        << setw(12) << "Course"
        << setw(30) << "Title"
        << setw(10) << "Credits"
        << setw(10) << "Grade"
        << setw(10) << "Pts"
        << endl;

    cout << "------------------------------------------------------------------\n";

    int totalHours = 0;

    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentID == id)
        {
            for (int j = 0; j < courseCount; j++)
            {
                if (courses[j].code == grades[i].courseCode)
                {
                    cout << left
                        << setw(12) << courses[j].code
                        << setw(30) << courses[j].title
                        << setw(10) << courses[j].creditHours
                        << setw(10) << grades[i].letterGrade
                        << setw(10) << grades[i].points
                        << endl;

                    totalHours += courses[j].creditHours;
                }
            }
        }
    }

    cout << "\nTotal Credit Hours: " << totalHours << endl;

    cout << fixed << setprecision(2);
    cout << "Cumulative GPA: " << s.GPA << endl;

    MainMenu();
}

//******************************************GRADES MANAGEMENT MENU***************************//


void gradesManagement()
{
    int choice;

    cout << "\n=== Grades Management ===\n";
    cout << "1. Enter Student Grades\n";
    cout << "2. View Student Grades\n";
    cout << "3. Calculate GPA\n";
    cout << "4. Generate Transcript\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        enterStudentGrades();
        break;

    case 2:
        viewStudentGrades();
        break;

    case 3:
        calculateGPA();
        break;

    case 4:
        generateTranscript();
        break;

    case 5:
        MainMenu();
        break;

    default:
        cout << "Invalid choice\n";
        gradesManagement();
        break;
    }
}

//*********************************************MAIN MENU**************************//


int MainMenu()
{
    int choice;

    cout << "\n=======================================\n";
    cout << "||    STUDENT INFORMATION SYSTEM     ||\n";
    cout << "=======================================\n";

    cout << "1. Student Management\n";
    cout << "2. Courses Management\n";
    cout << "3. Grades Management\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    while (choice > 4 || choice < 1)
    {
        cout << "Invalid entry, enter choice between 1 and 4: ";
        cin >> choice;
    }

    if (choice == 1)
        studentManagement();
    else if (choice == 2)
        courseManagement();
    else if (choice == 3)
        gradesManagement();
    else if (choice == 4)
        return 0;

    return 0;
}


int main()
{
    loadFromFile(students, studentCount);
    loadCoursesFromFile();
    loadGradesFromFile();

    MainMenu();

    return 0;
}