#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include<iomanip>
#include<ctime>
using namespace std;


int MainMenu();
bool isValid(char DOB[]) {
	return (
        DOB[2] == '/' && DOB[5] == '/' &&
		DOB[0] >= '0' && DOB[0] <= '3' &&
		DOB[1] >= '0' && DOB[1] <= '9' &&
		DOB[3] >= '0' && DOB[3] <= '1' &&
		DOB[4] >= '0' && DOB[4] <= '9' &&
		DOB[6] >= '1' && DOB[6] <= '2' &&
		DOB[7] >= '0' && DOB[7] <= '9' &&
		DOB[8] >= '0' && DOB[8] <= '9' &&
		DOB[9] >= '0' && DOB[9] <= '9');
}
void Newstudent()
{
	char name[5000];

	int i = 0, spacecounter = 0;

	while (spacecounter < 2)
	{
		cout << "Enter Student Name: ";
		cin.ignore();
		cin.getline(name, 30, '\n');
		for (int i = 0;name[i] != '\0';i++)
		{
			if (name[i] == ' ')
				spacecounter += 1;
		}
		if (spacecounter >= 2)
		{
			cout << "Enter only 2 names (first and last)" << endl;
			spacecounter = 0;
		}
		else if (spacecounter < 2)
		{
			break;
		}

	}
	cout << "Enter National ID: ";
	char id[30];
	cin.getline(id, 30, '\n');

	while (strlen(id) != 14 || id[0] == '0')
	{
		cout << "Invalid, enter valid input only (14 digits with no leading zeros)" << endl;
		cout << "Enter National ID: ";
		cin.getline(id, 30, '\n');
	}

	char Gnd = 'n';
	cout << "Enter your gender (M/F): ";
	while (Gnd != 'M' && Gnd != 'm' && Gnd != 'f' && Gnd != 'F')
	{
		cin >> Gnd;
		if (Gnd == 'M' || Gnd == 'm' || Gnd == 'f' || Gnd == 'F')
			break;
		cout << "Invalid input" << endl << "Please enter M or F: ";
	}

	char DOB[11]; // safer (room for '\0')
	time_t now = time(0);

	tm* ltm = localtime(&now);
	int year = 0, cyear = 0, age = 0;

	do {
		cout << "Enter DOB (DD/MM/YYYY): ";
		cin.getline(DOB, 11);
		year = (DOB[6] - '0') * 1000 + (DOB[7] - '0') * 100 + (DOB[8] - '0') * 10 + (DOB[9] - '0');
		cyear = 1900 + ltm->tm_year;
		age = cyear - year;

	} while (!isValid(DOB) || (DOB[3] == '1' && DOB[4] > '2') || (DOB[0] == '3' && DOB[1] > '1') || (DOB[3] == '0' && DOB[4] == '2' && (DOB[0] > '2' || (DOB[0] == '2' && DOB[1] > '8'))) || age < 17);

	cout << "Your date of birth is: " << DOB << endl;
	cout << "Your birth year is: " << year << endl;
	cout << "Current year is: " << cyear << endl;
	cout << "Your age is: " << age << endl;

	char phone[30];
	cout << "Enter Phone Number (11 digits starting with 01): ";
	cin.ignore();
	cin.getline(phone, 30);

	while (true) {



		if (strlen(phone) == 11 && phone[0] == '0' && phone[1] == '1') {
			break;
		}
		else {
			cout << "Invalid, must be 11 digits and start with '01'.)\n";
			cout << "Enter Phone Number (11 digits starting with 01):";
			cin.getline(phone, 30);

		}
	}

	char program[50];

	cout << "Enter Program (CSE/CCE/MCT): ";
	cin.getline(program, 50, '\n');

	while (strcmp(program, "CSE") && strcmp(program, "CCE") && strcmp(program, "MCT"))
	{
		cout << "Invalid input, enter (CSE/CCE/MCT):" << endl;
		cout << "Enter Program (CSE/CCE/MCT): ";
		cin.getline(program, 50);
	}

	int level;
	cout << "Enter Academic Level (1-4): ";
	cin >> level;
	while (level < 1 || level>4)
	{
		cout << "Invalid, Enter from range 1-4" << endl;
		cout << "Enter Academic Level (1-4): ";
		cin >> level;
	}

	cout << "Student added successfully!\n" << endl;
	cout << "Student ID: ";
}
	

void studentManagement()

{
	cout << "\033[1m=== Student Management ===\033[0m " << endl;
	cout << "1. Add New Student" << endl << "2. Search Student" << endl << "3. Update Student" << endl << "4. Delete Student" << endl << "5. List All Students" << endl << "6. Back to Main Menu" << endl << "Enter your choice: ";
	int choice1 = 0;
	cin >> choice1;
	if (choice1 > 6 || choice1 < 1)
	{
		cout << "Invalid, please enter within range of 1-6" << endl;
		cout << "1. Add New Student" << endl << "2. Search Student" << endl << "3. Update Student" << endl << "4. Delete Student" << endl << "5. List All Students" << endl << "6. Back to Main Menu" << endl << "Enter your choice: ";
		cin >> choice1;
	}

	switch (choice1)
	{
	case 1: {
		Newstudent();

		break;
	}
	
	case 6: {
		MainMenu();
		break;
	}
	}
}


void courseManagement()
{
	cout << "\033[1m=== Course Management ===\033[0m " << endl;
	cout << "1. Add New course" << endl << "2. View All Courses" << endl << "3. Update Course" << endl << "4. Delete Course" << endl << "5. Back to Main Menue" << endl << "Enter your choice: ";

}
void gradesManagement()
{
	cout<< "\033[1m=== Grades Management ===\033[0m " << endl;
	cout << "1. Enter Student Grades" << endl << "2. View Student Grades" << endl << "3. Calculate GPA" << endl << "4. Generate Transcript" << endl << "5. Back to Main Menue" << endl << "Enter your choice: ";
}

int MainMenu()
{
	cout << "=======================================" << endl;
	cout << "||    STUDENT INFORMATION SYSTEM     ||" << endl;
	cout << "=======================================" << endl;
	cout << "1. Student Management" << endl << "2.Course Management" << endl <<
		"3.Grades Management " << endl << "4. Exist" << endl << "Enter your choice: ";
	int number;
	cin >> number;

	if (number > 4 || number < 1)
	{
		cout << "Invalid, please enter within range of 1-4" << endl;
		MainMenu();
		cin >> number;
	}

	switch (number)
	{
	case 1: {
		studentManagement();

		break;
	}
	case 2: {
		courseManagement();
		break;
	}
	case 3: {
		gradesManagement();
		break;
	}
	case 4: {
		return 0;
		break;
	}
    
	}
    return 0;
}
int main()
{
	MainMenu();
	


}