#include <stdio.h>
#include<iostream>>
#include"Master.h"
using namespace std;


void Info();

int main()
{
	struct Master master;
	struct Slave slave;
	char err[50];
	int masterId;
	int slaveId;

	while (1)
	{
		int n;
		Info();
		cin >> n;
		switch (n) {
		case 1:
			cout << "enter ID: ";
			cin >> masterId;
			if (getM(&master, masterId, err)) {
				cout << "Master\'s name: " << master.name << endl;
				cout << "Master\'s count of students: " << master.countOfStudents << endl;
			}
			else {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 2:
			cout << "enter master\'s ID: ";
			cin >> masterId;
			if (getM(&master, masterId, err)) {
				cout << "Enter student id: ";
				cin >> slaveId;
				if (getS(master, &slave, slaveId, err)) {
					cout << "Master:" << master.name << endl;
					cout << "Name: " << slave.name << endl;
					cout << "Department: " << slave.department << endl;
				}
				else {
					cout << "ERROR: " << err << endl;
				}

			}
			else {
				cout << "ERROR: " << err << endl;
			}
			
			break;
		case 3:
			cout << "enter ID: ";
			cin >> masterId;
			if (delM(masterId, err)) {
				cout << "Deleted successfully" << endl;
			}
			else {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 4:
			cout << "enter master\'s ID: ";
			cin >> masterId;
			if (getM(&master, masterId, err)) {
				cout << "Enter student id: ";
				cin >> slaveId;
				if (getS(master, &slave, slaveId, err)) {
					delS(master, slave, slaveId, err);
					cout << "Deleted successfully" << endl;
				}
				else {
					cout << "ERROR: " << err << endl;
				}

			}
			else {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 5:
			cout << "Enter master\'s name: ";
			cin >> master.name;

			cout << "Enter master\'s count of students: ";
			cin >> master.countOfStudents;
			insertM(master);
			break;
		case 6:
			cout << "enter master\'s ID: ";
			cin >> masterId;
			if (getM(&master, masterId, err)) {
				cout << "Enter student id: ";
				cin >> slaveId;
				slave.masterId = masterId;
				slave.studentId = slaveId;
				cout << "Enter name: ";
				cin >> slave.name;

				cout << "Enter department: ";
				cin >> slave.department;
				insertS(master, slave, err);

			}
			else {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 7:
			if (!calcM(err)) {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 8:
			if (!calcS(err)) {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 9:
			if (!utM(err)) {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 10:
			if (!utS(err)) {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 11:
			cout << "enter master\'s ID: ";
			cin >> masterId;
			master.id = masterId;
			
			if (getM(&master, masterId, err)) {
				cout << "Enter master\'s name: ";
				cin >> master.name;

				cout << "Enter master\'s count of students: ";
				cin >> master.countOfStudents;
				updateM(master);
				cout << "Updated successfully" << endl;
			}
			else {				
				cout << "ERROR: " << err << endl;
			}
			break;
		case 12:
			cout << "enter master\'s ID: ";
			cin >> masterId;
			if (getM(&master, masterId, err)) {
				cout << "Enter student id: ";
				cin >> slaveId;
				if (getS(master, &slave, slaveId, err)) {
					cout << "Enter name: ";
					cin >> slave.name;

					cout << "Enter department: ";
					cin >> slave.department;
					updateS(slave, slaveId);
					cout << "Updated successfully" << endl;
				}
				else {
					cout << "ERROR: " << err << endl;
				}

			}
			else {
				cout << "ERROR: " << err << endl;
			}
			break;
		case 13:
			return 0;
			break;
		default:
			cout << "Invalid input" << endl;
			Info();

		}
	}
}

void Info() {
	cout << "Options: " << endl;
	cout << "1 - get-m" << endl << "2 - get-s" << endl;
	cout << "3 - del-m" << endl << "4 - del-s" << endl;
	cout << "5 - insert-m" << endl << "6 - insert-s" << endl;
	cout << "7 - calc-m" << endl << "8 - calc-s" << endl;
	cout << "9 - ut-m" << endl << "10 - ut-s" << endl;
	cout << "11 - update-m" << endl << "12 - update-s" << endl;
	cout << "13 - exit" << endl;
}


