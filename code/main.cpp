#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include <fstream>

using namespace std;

int MainMenu();

struct Student
{
    char name[50];
    char nationalID[15];
    char gender;
    char DOB[11];
    char phone[12];
    char program[10];
    int level;
    float GPA;
    int ID;
};

vector<Student> students;

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


void saveToFile(vector<Student>& students)
{
    ofstream file("students.csv");

    for (int i = 0; i < students.size(); i++)
    {
        file << students[i].name << ","
             << students[i].nationalID << ","
             << students[i].gender << ","
             << students[i].DOB << ","
             << students[i].phone << ","
             << students[i].program << ","
             << students[i].level << ","
             << students[i].GPA << ","
             << students[i].ID << "\n";
    }

    file.close();
}

void loadFromFile(vector<Student>& students)
{
    ifstream file("students.csv");

    if (!file) return; // file doesn't exist yet

    Student s;

    while (file.getline(s.name, 50, ',') &&

       file.getline(s.nationalID, 15, ',') &&

       file >> s.gender &&

       file.ignore() &&

       file.getline(s.DOB, 11, ',') &&

       file.getline(s.phone, 12, ',') &&

       file.getline(s.program, 10, ',') &&

       file >> s.level &&

       file.ignore() &&

       file >> s.GPA &&

       file.ignore() &&      

       file >> s.ID)
    {
        file.ignore(); 
        students.push_back(s);
    }

    file.close();
}

void Newstudent(vector<Student>& students)
{
    Student newStudent;

    while (true)
    {
        cout << "Enter Student Name: ";
        cin.ignore();
        cin.getline(newStudent.name, 50);

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

            if (!((newStudent.name[i] >= 'a' && newStudent.name[i] <= 'z') || (newStudent.name[i] >= 'A' && newStudent.name[i] <= 'Z') || newStudent.name[i] == ' ')) 
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
            cout << "Enter only 2 names (first and last)\n";
        }

        }

        cout << "Enter National ID: ";
        cin.getline(newStudent.nationalID, 15);

        while (strlen(newStudent.nationalID) != 14 || newStudent.nationalID[0] == '0')
        {
            cout << "Invalid input. Must be 14 digits and no leading zero.\n";
            cout << "Enter National ID: ";
            cin.getline(newStudent.nationalID, 15);
        }

        newStudent.gender = 'x';

        cout << "Enter Gender (M/F): ";

        while (newStudent.gender != 'M' &&
            newStudent.gender != 'm' &&
            newStudent.gender != 'F' &&
            newStudent.gender != 'f')
        {
            cin >> newStudent.gender;

            if (newStudent.gender != 'M' &&
                newStudent.gender != 'm' &&
                newStudent.gender != 'F' &&
                newStudent.gender != 'f')
            {
                cout << "Invalid input. Enter M or F only: ";
            }
        }

        int currentYear;
        int birthYear;
        int age;


        time_t now = time(NULL);

        struct tm timeinfo;
        localtime_r(&now, &timeinfo);

        currentYear = timeinfo.tm_year + 1900;

        cin.ignore();

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

            if (!isValid(newStudent.DOB) || age < 17)
            {
                cout << "Invalid DOB or age must be at least 17.\n";
            }

        } while (!isValid(newStudent.DOB) || age < 17);

        cout << "Enter Phone Number (11 digits starting with 01): ";
        cin.getline(newStudent.phone, 12);

        while (!(strlen(newStudent.phone) == 11 &&
            newStudent.phone[0] == '0' &&
            newStudent.phone[1] == '1'))
        {
            cout << "Invalid phone number.\n";
            cout << "Enter Phone Number: ";
            cin.getline(newStudent.phone, 12);
        }

        cout << "Enter Program (CSE / CCE / MCT): ";
        cin.getline(newStudent.program, 10);

        while (strcmp(newStudent.program, "CSE") &&
            strcmp(newStudent.program, "CCE") &&
            strcmp(newStudent.program, "MCT"))
        {
            cout << "Invalid input.\n";
            cout << "Enter Program (CSE / CCE / MCT): ";
            cin.getline(newStudent.program, 10);
        }

        cout << "Enter Academic Level (1 - 4): ";
        cin >> newStudent.level;

        while (newStudent.level < 1 || newStudent.level > 4)
        {
            cout << "Invalid level. Enter from 1 to 4: ";
            cin >> newStudent.level;
        }

        newStudent.GPA = 0.0;
        int idfcode = currentYear - 2000;
        
        if (students.empty())

            newStudent.ID = idfcode * 10000 + 1;

        else

        newStudent.ID = students.back().ID + 1;
   ;


        students.push_back(newStudent);
        saveToFile(students); 


        cout << "\nStudent added successfully!\n";

        MainMenu();
    }
    


void ListAllStudents(vector<Student>& students)
{ 
    if (students.empty())
    {
        cout << "No students found.\n";
        return;
    }

    cout << "\n===== All Students =====\n";

    for (int i = 0; i < students.size(); i++)
    {
        cout << "\nStudent #" << i + 1 << endl;
        cout << "Name: " << students[i].name << endl;
        cout << "National ID: " << students[i].nationalID << endl;
        cout << "Gender: " << students[i].gender << endl;
        cout << "DOB: " << students[i].DOB << endl;
        cout << "Phone: " << students[i].phone << endl;
        cout << "Program: " << students[i].program << endl;
        cout << "Level: " << students[i].level << endl;
        cout << "GPA: " << students[i].GPA << endl;
        cout << "Student ID: " << students[i].ID << endl;
    }
}

void PrintStudentBox(const Student& s)
{
    cout << "╔═════════════════════════════════════════════════════╗\n";
    cout << "║ Student ID: " << s.ID << " ║\n";
    cout << "║ Name: " << s.name << " ║\n";
    cout << "║ National ID: " << s.nationalID << " ║\n";
    cout << "║ Gender: " << s.gender << " | DOB: " << s.DOB << " ║\n";
    cout << "║ Phone: " << s.phone << " ║\n";
    cout << "║ Program: " << s.program << " | Level: " << s.level << " | GPA: " << s.GPA << " ║\n";
    cout << "╚═════════════════════════════════════════════════════╝\n";
}



void SearchStudents(vector<Student>& students)
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
    case 1: // Search by ID
    {
        int id;
        cout << "Enter Student ID: ";
        cin >> id;

        bool found = false;

        for (int i = 0; i < students.size(); i++)
        {
            if (students[i].ID == id)
            {
                PrintStudentBox(students[i]);
                found = true;
                break;
            }
        }

        if (!found)
            cout << "Student not found.\n";

        break;
    }

    case 2: // Search by Name
    {
        cin.ignore();
        char name[50];

        cout << "Enter student name: ";
        cin.getline(name, 50);

        int count = 0;

        // count first
        for (int i = 0; i < students.size(); i++)
        {
            if (strcmp(students[i].name, name) == 0)
                count++;
        }

        if (count == 0)
        {
            cout << "No students found with this name.\n";
        }
        else
        {
            cout << "\nFound " << count << " student(s):\n";

            for (int i = 0; i < students.size(); i++)
            {
                if (strcmp(students[i].name, name) == 0)
                {
                    PrintStudentBox(students[i]);
                }
            }
        }

        break;
    }

    case 3: // Search by National ID
    {
        cin.ignore();
        char nid[15];

        cout << "Enter National ID: ";
        cin.getline(nid, 15);

        bool found = false;

        for (int i = 0; i < students.size(); i++)
        {
            if (strcmp(students[i].nationalID, nid) == 0)
            {
                PrintStudentBox(students[i]);
                found = true;
                break;
            }
        }

        if (!found)
            cout << "Student not found.\n";

        break;
    }

    case 4:
        return;

    default:
        cout << "Invalid choice\n";
    }
}


void studentManagement()
{
    int choice;

    cout << "\n=== Student Management ===\n";
    cout << "1. Add New Student\n";
    cout << "2. List All Students\n";
    cout << "3. Search Students\n";
    cout << "4. Back to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        Newstudent(students);
        break;

    case 2:
        ListAllStudents(students);
        break;

    case 3:
        SearchStudents(students);
        break;

    case 4:
        MainMenu();
        break;

    default:
        cout << "Invalid choice\n";
    }
}






int MainMenu()
{
    int choice;

    cout << "\n=======================================\n";
    cout << "||    STUDENT INFORMATION SYSTEM     ||\n";
    cout << "=======================================\n";

    cout << "1. Student Management\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        studentManagement();
        break;

    case 2:
        return 0;

    default:
        cout << "Invalid choice\n";
    }

    return 0;
}

int main()
{

    loadFromFile(students);  
    MainMenu();
    return 0;
}