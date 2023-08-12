#include <bits/stdc++.h>
using namespace std;

const string usersFile = "users.txt";

class FileDataBase {
private:
	string filename;
	vector<string> records;

	void loadFile() {
		ifstream fin(usersFile);

		string record;
		while (getline(fin, record))
			records.push_back(record);

		fin.close();
	}

public:
	FileDataBase(string filename) {
		this->filename = filename;
		loadFile();
	}

	const vector<string>& read() {
		return this->records;
	}

	void write(const string &record) {
		auto status = ios::in | ios::out | ios::app;
		ofstream fout(usersFile, status);
		fout << record;
		fout.close();
		
		records.push_back(record);
	}
};

class Login {
private:
	FileDataBase fileDataBase = FileDataBase(usersFile);

public:
	int login(const string &username, const string &password) {
		return -1;
	}
};

class Signup {
private:
	FileDataBase fileDataBase = FileDataBase(usersFile);

	bool userAlreadyExist(const string &username) {
		return false;
	}

	bool noSpace(const string &str) {
		return false;
	}

public:
	int signup(const string &username, const string &password, const string &name, const string &email, int AQ) {
		return -1;
	}
};

class LoginMenu {
private:
	Login login;
	Signup signup;

	int menu() {
		cout << "Menu:\n";
		cout << "\t\t1: Login\n";
		cout << "\t\t2: Sign Up\n\n";
		cout << "Enter number in range 1 - 2: ";

		int choice;
		cin >> choice;

		return choice;
	}

	int loginMenu() {
		cout << "Enter user name & password: ";
		string username, password;
		cin >> username >> password;
		
		return login.login(username, password);
	}

	int signupMenu() {
		string username, password, name, email;
		int AQ;

		cout << "Enter user name. (No spaces): ";
		cin >> username;

		cout << "Enter password: ";
		cin >> password;

		cout << "Enter name: ";
		cin >> name;

		cout << "Enter email: ";
		cin >> email;

		cout << "Allow anonymous questions?: (0 or 1) ";
		cin >> AQ;

		return signup.signup(username, password, name, email, AQ);
	}

public:
	LoginMenu() {
		int loggedId = -1;
		while (loggedId == -1) {
			int menuOption = menu();

			if (menuOption == 1)
				loggedId = loginMenu();
			else if (menuOption == 2)
				loggedId = signupMenu();

			cout << endl;
		}
	}
};

class Ask {
private:
	LoginMenu loginMenu;

public:
	Ask() {	
		loginMenu = LoginMenu();
	}
};

int main() {
	cout << boolalpha;
	Ask ask = Ask();

	return 0;
}