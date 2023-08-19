#include <bits/stdc++.h>
using namespace std;

struct UserInfo {
	int id;
	string username;
	string password;
	string name;
	string email;
	int AQ;

	string stringfy(char seperator = ' ') {
		return to_string(id) + seperator + username + seperator + password + seperator + name + seperator + email + seperator + to_string(AQ) + "\n";
	}
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
	const map<string, UserInfo> *users;

public:
	Login() {
		users = new map<string, UserInfo>();
	}
	Login(const map<string, UserInfo> *users) : users(users) {}

	int login(const string &username, const string &password) {
		if (!users->count(username))
			return -1;
	
		if (users->at(username).password != password)
			return -1;

		return users->at(username).id;
	}
};

class Signup {
private:
	const map<string, UserInfo> *users;

	bool userAlreadyExist(const string &username) {
		if (users->count(username))
			return true;
		return false;
	}

	bool containsSpaces(const string &str) {
		for (auto &ch: str)
			if (ch == ' ')
				return true;
		
		return false;
	}

public:
	Signup() {
		users = new map<string, UserInfo>();
	}
	Signup(const map<string, UserInfo> *users) : users(users) {}

	int signup(const string &username, const string &password, const string &name, const string &email, int AQ) {
		if (userAlreadyExist(username))
			return -1;

		if (containsSpaces(username) || containsSpaces(password) || containsSpaces(name) || containsSpaces(email))
			return -1;

		if (!(0 <= AQ && AQ <= 1))
			return -1;

		return users->size();
	}
};

class Register {
private:
	const string file = "users.txt";
	FileDataBase fileDataBase;
	Login login;
	Signup signup;
	map<string, UserInfo> users;

	void fetchUsersRecords() {
		vector<string> records = fileDataBase.read();

		for (auto &record: records) {
			istringstream iss(record);
			UserInfo user;
			iss >> user.id >> user.username >> user.password >> user.name >> user.email >> user.AQ;
			users[user.username] = user;
		}
	}

public:
	Register() {
		fileDataBase = FileDataBase(file);

		fetchUsersRecords();
		this->login = Login(&users);
		this->signup = Signup(&users);
	}

	int signUp(const string &username, const string &password, const string &name, const string &email, int AQ) {
		int userId = signup.signup(username, password, name, email, AQ);
		if (userId == -1)
			return userId;

		UserInfo newUser = {userId, username, password, name, email, AQ};
		fileDataBase.write(newUser.stringfy());
		users[username] = newUser;
		return userId;
	}

	int logIn(const string &username, const string &password) {
		return login.login(username, password);
	}

	string getUserName(int userId) {
		for (auto &user: users)
			if (user.second.id == userId)
				return user.first;

		return "";
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