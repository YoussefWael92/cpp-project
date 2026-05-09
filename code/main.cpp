#include "globals.h"
#include "student.h"
#include "course.h"
#include "grades.h"

// Global array definitions (declared extern in globals.h)
Student students[1200];
int studentCount = 0;

Course courses[200];
int courseCount = 0;

Grade grades[5000];
int gradeCount = 0;


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