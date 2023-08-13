#include <bits/stdc++.h>
using namespace std;

struct userInfo {
	int id;
	string username;
	string password;
	string name;
	string email;
	int AQ;
};

class FileDataBase {
private:
	string filename;
	vector<string> records;

	void loadFile() {
		ifstream fin(filename);

		string record;
		while (getline(fin, record))
			records.push_back(record);

		fin.close();
	}

public:
	FileDataBase() {}
	FileDataBase(string filename) {
		this->filename = filename;
		loadFile();
	}

	const vector<string>& read() {
		return this->records;
	}

	void write(const string &record) {
		auto status = ios::in | ios::out | ios::app;
		ofstream fout(filename, status);
		fout << record;
		fout.close();
		
		records.push_back(record);
	}
};

class Login {
private:
	map<string, pair<string, int>> usernamePasswordMapping;

public:
	Login() {}
	Login(const vector<userInfo> &users) {
		for (auto &user: users)
			usernamePasswordMapping[user.username] = {user.password, user.id};
	}

	int login(const string &username, const string &password) {
		if (!usernamePasswordMapping.count(username))
			return -1;
	
		if (usernamePasswordMapping[username].first != password)
			return -1;

		return usernamePasswordMapping[username].second;
	}
};

class Signup {
private:
	bool userAlreadyExist(const string &username) {
		return false;
	}

	bool noSpace(const string &str) {
		return false;
	}

public:
	Signup() {}
	Signup(const vector<userInfo> &allUsers) {

	}

	int signup(const string &username, const string &password, const string &name, const string &email, int AQ) {
		return -1;
	}
};

class Register {
private:
	const string file = "users.txt";
	FileDataBase fileDataBase;
	Login login;
	Signup signup;
	vector<userInfo> users;

	void encodeInfo() {
		vector<string> records = fileDataBase.read();

		for (auto &record: records) {
			istringstream iss(record);
			userInfo user;
			iss >> user.id >> user.username >> user.password >> user.name >> user.email >> user.AQ;
			users.push_back(user);
		}
	}

public:
	Register() {
		fileDataBase = FileDataBase(file);

		encodeInfo();
		this->login = Login(users);
		this->signup = Signup(users);
	}

	int signUp(const string &username, const string &password, const string &name, const string &email, int AQ) {
		return signup.signup(username, password, name, email, AQ);
	}

	int logIn(const string &username, const string &password) {
		return login.login(username, password);
	}
};

class LoginMenu {
private:
	Register reg;

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
		
		int userId = reg.logIn(username, password);
		if (userId == -1)
			cout << "Invalid Username or Passowrd. Try Again\n";

		return userId;
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

		int userId = reg.signUp(username, password, name, email, AQ);
		if (userId == -1)
			cout << "Invalid Signup Try Again\n";

		return userId;
	}

public:
	LoginMenu() {}

	void run() {
		int loggedId = -1;
		while (loggedId == -1) {
			int menuOption = menu();

			if (menuOption == 1)
				loggedId = loginMenu();
			else if (menuOption == 2)
				loggedId = signupMenu();

			cout << endl;
		}

		cout << loggedId << endl;
	}
};

class Ask {
private:
	LoginMenu loginMenu;

public:
	Ask() {	
		loginMenu.run();
	}
};

int main() {
	cout << boolalpha;
	Ask ask = Ask();

	return 0;
}