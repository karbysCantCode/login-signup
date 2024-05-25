#define RAPIDJSON_HAS_STDSTRING 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <string>
#include "rapidjson/document.h" 
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>

using namespace std;
using namespace rapidjson;

Document userData;

void openMenu();
void initSaveFile();
FILE* createFileIfNotExist();
void writeToJson();
bool isUserInSave(string username);
bool isPasswordValid(string username, string password);
void createUser(string username, string password);
int registration();
int forgot(string username);
int login();

class UserVerifier {
public:
	struct UserVerificationResult
	{
		bool hasUsername;
		bool has;
	};
};

class userProfile {
private:
	Value profile;
public:
	struct UserVerificationResult
	{
		bool hasUsername;
		bool hasPassword;
		bool hasLogins;
		bool hasUserId;

	};
	Value& BuildProfile(const std::string& username, const std::string& password, int logins, int userId, Document::AllocatorType& allocator) {
		profile.SetObject();
		profile.AddMember("Username", Value().SetString(username.c_str(), username.length(), allocator), allocator);
		profile.AddMember("Password", Value().SetString(password.c_str(), password.length(), allocator), allocator);
		profile.AddMember("Logins", logins, allocator);
		profile.AddMember("UserId", userId, allocator);
		return profile;
	}

	static UserVerificationResult VerifyProfile(string username) {
		UserVerificationResult currentVerif;

		currentVerif.hasUsername = userData[username].HasMember("Username");
		currentVerif.hasPassword = userData[username].HasMember("Password");
		currentVerif.hasLogins = userData[username].HasMember("Logins");
		currentVerif.hasUserId = userData[username].HasMember("UserId");

		return currentVerif;
	}

};

FILE* createFileIfNotExist() {
	FILE* fp = fopen("SaveFile.json", "w");
	if (!fp) {
		cerr << "Error creating file: " << "SaveFile.json" << endl << "program will likely not function as intended, no fail safe for this :3";
	}

	return fp;
}

void initSaveFile() {
	FILE* fp = fopen("SaveFile.json", "r");

	if (!fp) {
		fp = createFileIfNotExist();
	}
	userData.SetObject();

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* readBuffer = new char[fileSize];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	userData.ParseStream(is);

	if (userData.HasParseError()) {
		cerr << "Error parsing JSON: "
			<< userData.GetParseError() << endl;
	}
	fclose(fp);
}

void writeToJson() {
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	userData.Accept(writer);

	FILE* fp = fopen("SaveFile.json", "w");
	fputs(buffer.GetString(), fp);
	fclose(fp);
}

void verifyExistingAccounts(bool correct) {
	cout << "hasUser,hasPass,hasLogins,hasUserId\n";
	for (Value::MemberIterator itr = userData.MemberBegin(); itr != userData.MemberEnd(); ++itr) {
		const char* key = itr->name.GetString();
		Value& currentValue = itr->value;

		userProfile::UserVerificationResult currentVerifyResult = userProfile::VerifyProfile(key);
		if (correct) {
			Document::AllocatorType& allocator = userData.GetAllocator();

			if (!currentVerifyResult.hasUsername) {
				Value usernameKey;
				usernameKey.SetString("Username", allocator);

				Value usernameValue;
				usernameValue.SetString(key, allocator);

				currentValue.AddMember(usernameKey,usernameValue, allocator);
			}

			if (!currentVerifyResult.hasPassword) {
				Value passwordKey;
				passwordKey.SetString("Password", allocator);

				Value passwordValue;
				passwordValue.SetString("password", allocator);

				currentValue.AddMember(passwordKey, passwordValue, allocator);
			}

			if (!currentVerifyResult.hasLogins) {
				Value loginKey;
				loginKey.SetString("Password", allocator);

				Value loginValue;
				loginValue.SetInt(0);

				currentValue.AddMember(loginKey, loginValue, allocator);
			}

			if (!currentVerifyResult.hasUserId) {
				Value userIdKey;
				userIdKey.SetString("UserId", allocator);

				currentValue.AddMember(userIdKey, Value().SetInt(userData.MemberCount() + 1), allocator);
			}
			if (!currentVerifyResult.hasUsername|| !currentVerifyResult.hasPassword|| !currentVerifyResult.hasLogins|| !currentVerifyResult.hasUserId) {
				writeToJson();
			}
		}
		cout << key << ':' << currentVerifyResult.hasUsername << currentVerifyResult.hasPassword << currentVerifyResult.hasLogins << currentVerifyResult.hasUserId << endl;
	}
}

bool isUserInSave(string username) {

	if (userData.HasMember(username)) {
		return true;
	}
	return false;
}

bool isPasswordValid(string username, string password) {

	if (userData.HasMember(username) && userData[username].IsObject()) {
		if (password == userData[username]["Password"].GetString()) {

			Value& logins = userData[username]["Logins"];
			logins.SetInt(userData[username]["Logins"].GetInt() + 1);

			writeToJson();

			return true;
		}

	}
	return false;
}

void createUser(string username, string password) {
	Document::AllocatorType& allocator = userData.GetAllocator();

	Value newProfile(kObjectType);
	newProfile.AddMember("Username", Value().SetString(username.c_str(), username.length(), allocator), allocator);
	newProfile.AddMember("Password", Value().SetString(password.c_str(), password.length(), allocator), allocator);
	newProfile.AddMember("Logins", 0, allocator);
	newProfile.AddMember("UserId", userData.MemberCount()+1, allocator);

	Value key;
	key.SetString(username.c_str(), username.length(), allocator);
	userData.AddMember(key, newProfile, allocator);

	writeToJson();
}

void openAdminPanel() {
	short choice = 0;
	while (cin.fail() || choice < 1 || choice > 5) {
		if (cin.fail()) {
			cin.clear(); // Clear the error state
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		cout << "------------------------------------------" << endl;
		cout << "             ADMIN PANEL YOOO             " << endl;
		cout << "------------------------------------------" << endl;
		cout << "   __\n";
		cout << "  /##\\ \n";
		cout << "  \\##/ \n";
		cout << "   --" << "   yeah hi okay so uhuydeifr in admin profile you can do cool things:\n";
		cout << "     " << "   [1] delete other profiles\n";
		cout << "   []" << "   [2] edit other profiles\n";
		cout << "   []" << "   [3] check some cool statistrics\n";
		cout << "   []" << "   [4] verify all profiles\n";
		cout << "   []" << "   [5] verify and correct all profiles\n";
		cout << "   []" << "   (enter the the number on what one you weant to do)\n";
		cout << "   []\n";
		cin >> choice;
	}

	switch (choice)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		verifyExistingAccounts(false);
		openAdminPanel();
		break;
	case 5:
		verifyExistingAccounts(true);
		openAdminPanel();
		break;
	default:
		cout << "YOU BROKE IT HOW";
		break;
	}
}

int registration() {
	cout << "------------------------------------------" << endl;
	cout << "                  Sign up                 " << endl;
	cout << "------------------------------------------" << endl;
	cout << "Enter a username:";
	string username;
	cin >> username;
	bool userTaken = isUserInSave(username);

	while (userTaken) {
		cout << endl << "Username already registered, please pick another:";
		cin >> username;
		userTaken = isUserInSave(username);
	}

	cout << "Choose your password:";
	string password;
	cin >> password;

	createUser(username, password);

	openMenu();

	return 0;
}

int forgot(string username) {
	if (username == "") {
		cout << "Enter a username:";
		cin >> username;

		bool usernameValid = isUserInSave(username);
		while (!usernameValid) {
			cout << "Username not found, try again or enter 1 to open menu: ";
			cin >> username;
			if (username == "1") {
				openMenu();
				return 0;
			} 
			usernameValid = isUserInSave(username);

		}
	}

	if (username == "admin") {
		cout << endl << "cannot change admin password" << endl;
		openMenu();
		return 0;
	}

	cout << "Enter a new password for \"" + username + "\": \n";
	string password;
	cin >> password;

	Value& passwordIndex = userData[username]["Password"];
	passwordIndex.SetString(password, userData.GetAllocator());

	writeToJson();
	cout << "successfully changed password\n";
	openMenu();

	return 0;
}

int login() {
	string username;
	cout << "------------------------------------------" << endl;
	cout << "                Login Page                " << endl;
	cout << "------------------------------------------" << endl;
	cout << "Enter username: ";
	cin >> username;
	bool validUsername = isUserInSave(username);

	while (!validUsername) {
		cout << endl << "Username not found please try again or enter 1 to return to menu: ";
		cin >> username;
		if (username == "1") {
			openMenu();
			return 0;
		}
		else {
			validUsername = isUserInSave(username);
		}
	}

	string password = username;
	cout << "------------------------------------------" << endl;
	cout << "              Enter Password              " << endl;
	cout << "------------------------------------------" << endl;
	cout << ": ";
	cin >> password;
	bool validPassword = isPasswordValid(username, password);

	while (!validPassword) {
		cout << "Incorrect password, please try again or enter 1 to return to menu, or 2 to reset your password: ";
		cin >> password;
		if (password == "1") {
			openMenu();
			return 0;
		}
		else if (password == "2") {
			forgot(username);
			return 0;
		}
		validPassword = isPasswordValid(username, password);
	}

	cout << "Logged in successfully!\n" << "enter to exit\n"; // open a menu or something maybe integrate the invntory system if i do it
	if (username == "admin") {

		openAdminPanel();

	}
	cin >> password;

	return 0;
}

void openMenu() {
	int choice = 0;
	do {
		if (cin.fail()) {
			cin.clear(); // Clear the error state
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		cout << "------------------------------------------" << endl;
		cout << "                   Menu                   " << endl;
		cout << "------------------------------------------" << endl;
		cout << "Enter choice number to proceed." << endl;
		cout << "1: Login" << endl << "2: Signup" << endl << "3: Forgot password" << endl << "4: Exit" << endl;
		cin >> choice;

	} while (cin.fail() || choice > 4 || choice < 1);
	

	switch (choice)
	{
	case 1:
		login();
		break;
	case 2:
		registration();
		break;
	case 3:
		forgot("");
		break;
	case 4:	
		return;
	default:
		cout << "super error what!";
		break;
	}
}

int main() {

	initSaveFile();

	openMenu();

	return 0;
}