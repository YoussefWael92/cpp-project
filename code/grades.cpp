#include "grades.h"
#include "student.h"


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

    while (file >> g.studentID)
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


void viewStudentGrades()
{
    int id;

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


void calculateGPA()
{
    int id;

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


void generateTranscript()
{
    int id;

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
