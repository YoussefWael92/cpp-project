#pragma once
#include "globals.h"

bool isValid(char DOB[]);
string cleanExcelText(string value);
void saveToFile(Student students[], int studentCount);
void loadFromFile(Student students[], int& studentCount);
void PrintStudentBox(const Student& s);
void Newstudent(Student students[], int& studentCount);
void ListAllStudents(Student students[], int studentCount);
void SearchStudents(Student students[], int studentCount);
void updatestudent();
void deleteStudent();
void studentManagement();
