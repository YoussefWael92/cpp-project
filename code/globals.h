#pragma once
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

    string registeredCourses[10];
    int registeredCourseCount;
};

extern Student students[1200];
extern int studentCount;


struct Course
{
    string code;
    string title;
    int creditHours;
};

extern Course courses[200];
extern int courseCount;


struct Grade
{
    int studentID;
    string courseCode;
    float midterm;
    float finalExam;
    float total;
    char letterGrade;
    float points;
};

extern Grade grades[5000];
extern int gradeCount;


// Forward declarations of menu functions
bool isValidCourseCode(string code);
int MainMenu();
void studentManagement();
void courseManagement();
void gradesManagement();
