#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h" 

using namespace std;

int initSaveFile() {
	string saveName = "SaveFile.txt";
	ifstream inFile(saveName);
	if (!inFile) {
		ofstream outFile(saveName);
		outFile.close();
		inFile.close();
	}

	
	
	return 0;
}

bool isUserInSave(string username) {
	string saveName = "SaveFile.txt";
	ifstream inFile(saveName);

	string line;
	while (getline(inFile, line)) {
		if (line[0] == 'U') {
			line.erase(0,1);
			if (line == username) {
				inFile.close();
				return true;
			}
		}
	}
	cout << "Username not found, try again or sign up";
	inFile.close();
	return false;
}

bool isPasswordValid(string password) {
	string saveName = "SaveFile.txt";
	ifstream inFile(saveName);

	string line;
	while (getline(inFile, line)) {
		if (line[0] == 'P') {
			line.erase(0, 1);
			if (line == password) {
				inFile.close();
				return true;
			}
		}
	}
	return false;
}

int login() {
	string username;
	cout << "------------------------------------------" << endl;
	cout << "                Login Page                " << endl;
	cout << "------------------------------------------" << endl;
	cout << "Enter username: ";
	cin >> username;
	if (isUserInSave(username)) {
		string password = username;
		cout << "------------------------------------------" << endl;
		cout << "              Enter Password              " << endl;
		cout << "------------------------------------------" << endl;
		cout << ": ";
		cin >> password;
		password = username + password;
		if (isPasswordValid(password)) {
			cout << "Logged in successfully!\n";
		}
		else {
			cout << "Failed to log in :(";
		}
	}

	return 0;
}

int registration() {
	return 0;
}

int forgot() {
	return 0;
}

int main() {
	initSaveFile();
	int choice;

	cout << "------------------------------------------" << endl;
	cout << "                   Menu                   " << endl;
	cout << "------------------------------------------" << endl;
	cout << "Enter choice number to proceed." << endl;
	cout << "1: Login" << endl << "2: Signup" << endl << "3: Forgot password" << endl << "4: Exit" << endl;
	cin >> choice;

	switch (choice)
	{
	case 1:
		login();
		break;
	case 2:
		registration();
		break;
	case 3:
		forgot();
		break;
	default:
		break;
	}

	return 0;
}