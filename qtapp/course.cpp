#include "course.h"
#include "student.h"


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


void registerCourseToStudent()
{
    int studentID;
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
        MainMenu();
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
