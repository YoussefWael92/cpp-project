#include <iostream>
#include <ctime>
using namespace std;

bool isValid(char DOB[]) {
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

int main() {
    char DOB[11]; // safer (room for '\0')
	    time_t now = time(0);

    tm *ltm = localtime(&now);
	int year =0, cyear = 0, age = 0;

    do {
        cout << "Enter DOB (DD/MM/YYYY): ";
        cin.getline(DOB, 11);
		year = (DOB[6] - '0') * 1000 + (DOB[7] - '0') * 100 + (DOB[8] - '0') * 10 + (DOB[9] - '0');
		cyear =  1900 + ltm->tm_year;
		age = cyear - year;
		
    } while (!isValid(DOB) || (DOB[3] == '1' && DOB[4] > '2') || (DOB[0] == '3' && DOB[1] > '1') || (DOB[3] == '0' && DOB[4] == '2' && (DOB[0] > '2' || (DOB[0] == '2' && DOB[1] > '8'))) || age < 17);

    cout << "Your date of birth is: " << DOB << endl;
	cout << "Your birth year is: " << year << endl;
	cout << "Current year is: " << cyear << endl;
	cout << "Your age is: " << age << endl;
return 0;
}
