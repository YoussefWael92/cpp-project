#include "student.h"


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
        s.ID = stoi(temp);

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
    cout << "|" << left << setw(50) << ("Student ID: " + to_string(s.ID)) << "\n";
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

        if (newStudent.nationalID.length() != 14 || newStudent.nationalID[0] == '0')
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

    int idfcode = currentYear - 2000;

    if (studentCount == 0)
        newStudent.ID = idfcode * 10000 + 1;
    else
        newStudent.ID = students[studentCount - 1].ID + 1;

    students[studentCount] = newStudent;
    studentCount++;

    saveToFile(students, studentCount);

    cout << "\nStudent added successfully!\n";
    cout << "Student ID: " << newStudent.ID << endl;

    MainMenu();
}


void ListAllStudents(Student students[], int studentCount)
{
    if (studentCount == 0)
    {
        cout << "No students found.\n";
        return;
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
        int id;

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

            if (id < 100000 || id > 999999)
            {
                cout << "Invalid ID (must be 6 digits)\n";
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


void updatestudent()
{
    int id;

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


void deleteStudent()
{
    int id;

    cout << "\n=== Delete Student ===\n";
    cout << "Enter Student ID to delete: ";
    cin >> id;

    if (id <= 0)
    {
        cout << "ERROR: Invalid Student ID format\n";
        return;
    }

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
        MainMenu();
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
