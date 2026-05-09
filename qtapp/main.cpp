#include <QApplication>
#include "mainwindow.h"
#include "globals.h"
#include "student.h"
#include "course.h"
#include "grades.h"

Student students[1200];
int studentCount = 0;

Course courses[200];
int courseCount = 0;

Grade grades[5000];
int gradeCount = 0;

// The original console files still call MainMenu() after some operations.
// In the GUI version we do not return to a console menu, so this small stub
// satisfies those old references without changing the original project logic.
int MainMenu()
{
    return 0;
}


int main(int argc, char *argv[])
{
    loadFromFile(students, studentCount);
    loadCoursesFromFile();
    loadGradesFromFile();

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
