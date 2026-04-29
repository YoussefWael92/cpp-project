#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

int MainMenu();

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
    int ID;
};

vector<Student> students;

bool isValid(char DOB[])
{
    return (DOB[2]=='/' && DOB[5]=='/' &&

            DOB[0]>='0' && DOB[0]<='3' &&
		
            DOB[1]>='0' && DOB[1]<='9' &&

            DOB[3]>='0' && DOB[3]<='1' &&

            DOB[4]>='0' && DOB[4]<='9' &&

            DOB[6]>='1' && DOB[6]<='2' &&

            DOB[7]>='0' && DOB[7]<='9' &&

            DOB[8]>='0' && DOB[8]<='9' &&

            DOB[9]>='0' && DOB[9]<='9');

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
    if (!file) return;

    Student s;

    while (getline(file, s.name, ',') &&
           getline(file, s.nationalID, ',') &&
           file >> s.gender &&
           file.ignore() &&
           file.getline(s.DOB, 11, ',') &&
           getline(file, s.phone, ',') &&
           getline(file, s.program, ',') &&
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

    cin.ignore();

    // Name

    while (true)

    {

        cout << "Enter Student Name: ";

        getline(cin, newStudent.name);

        int wordCount = 0;

        bool inWord = false;

        bool valid = true;

        for (char c : newStudent.name)

        {

            if (c != ' ')

            {

                if (!inWord)

                {

                    wordCount++;

                    inWord = true;

                }

            }

            else inWord = false;

            if (!isalpha(c) && c != ' ')

                valid = false;

        }

        if (wordCount == 2 && valid) break;

        cout << "Enter only 2 names (first and last)\n";

    }

    // National ID

    cout << "Enter National ID: ";

    getline(cin, newStudent.nationalID);

    while (newStudent.nationalID.length() != 14 || newStudent.nationalID[0] == '0')

    {

        cout << "Invalid input. Enter again: ";

        getline(cin, newStudent.nationalID);

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

            if (!isValid(newStudent.DOB) || (newStudent.DOB[3] == '1' && newStudent.DOB[4] > '2') || 
               (newStudent.DOB[0] == '3' && newStudent.DOB[1] > '1') || (newStudent.DOB[3] == '0' && newStudent.DOB[4] == '2' && (newStudent.DOB[0] > '2' || 
               (newStudent.DOB[0] == '2' && newStudent.DOB[1] > '8'))) || age < 17)
               
            {
                cout << "Invalid DOB or age must be at least 17.\n";
            }

        } while (!isValid(newStudent.DOB) || age < 17);

        cout << "Enter Phone Number: ";

getline(cin, newStudent.phone);

    while (newStudent.phone.length() != 11 ||

           newStudent.phone.substr(0,2) != "01")

    {

        cout << "Invalid phone. Enter again: ";

        getline(cin, newStudent.phone);

    }

    cout << "Enter Program (CSE / CCE / MCT): ";

    getline(cin, newStudent.program);

    while (newStudent.program != "CSE" &&

           newStudent.program != "CCE" &&

           newStudent.program != "MCT")

    {

        cout << "Invalid. Enter again: ";

        getline(cin, newStudent.program);

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

    string name;

    cout << "Enter student name: ";
    getline(cin, name);

    // 🔥 manually lowercase input
    for (int i = 0; i < name.length(); i++)
    {
        if (name[i] >= 'A' && name[i] <= 'Z')
            name[i] = name[i] + 32;
    }

    int count = 0;

    for (int i = 0; i < students.size(); i++)
    {
        string temp = students[i].name;

        // 🔥 manually lowercase stored name
        for (int j = 0; j < temp.length(); j++)
        {
            if (temp[j] >= 'A' && temp[j] <= 'Z')
                temp[j] = temp[j] + 32;
        }

        if (temp == name)
        {
            count++;
        }
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
            string temp = students[i].name;

            for (int j = 0; j < temp.length(); j++)
            {
                if (temp[j] >= 'A' && temp[j] <= 'Z')
                    temp[j] = temp[j] + 32;
            }

            if (temp == name)
            {
                PrintStudentBox(students[i]);
            }
        }
    }

    break;
}

    case 3: // Search by National ID
    {
        cin.ignore(); // clear buffer
        string nid;

        cout << "Enter National ID: ";
        getline(cin, nid);

        bool found = false;

        for (int i = 0; i < students.size(); i++)
        {
            if (students[i].nationalID == nid)
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