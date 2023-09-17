#include <bits/stdc++.h>
using namespace std;

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
	}

	const vector<string>& read() {
		this->records.clear();
		loadFile();
		return this->records;
	}

	void write(const string &record) {
		auto status = ios::in | ios::out | ios::app;
		ofstream fout(filename, status);
		fout << record;
		fout.close();
	}

	void clear() {
		ofstream fout(filename);
		fout << "";
		fout.close();
	}
};

// Registration Classes //
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

	void decode(const string &userRecord) {
		istringstream iss(userRecord);
		iss >> id >> username >> password >> name >> email >> AQ;
	}
};

class Register {
private:
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

		bool validUser(const string &username, const string &password, const string &name, const string &email, int AQ) {
			if (userAlreadyExist(username))
				return false;
			
			if (containsSpaces(username) || containsSpaces(password) || containsSpaces(name) || containsSpaces(email))
				return false;
			
			if (!(0 <= AQ && AQ <= 1))
				return false;

			return true;
		}

	public:
		Signup() {
			users = new map<string, UserInfo>();
		}
		Signup(const map<string, UserInfo> *users) : users(users) {}

		int signup(const string &username, const string &password, const string &name, const string &email, int AQ) {
			if (!validUser(username, password, name, email, AQ))
				return -1;
			return users->size();
		}
	};

private:
	const string file = "users.txt";
	FileDataBase fileDataBase;
	Login login;
	Signup signup;
	map<string, UserInfo> users;

	void fetchUsersRecords() {
		vector<string> records = fileDataBase.read();

		for (auto &record: records) {
			UserInfo user;
			user.decode(record);
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

class LoginMenuUI {
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
	LoginMenuUI() {}

	int run() {
		int loggedId = -1;
		while (loggedId == -1) {
			int menuOption = menu();

			if (menuOption == 1)
				loggedId = loginMenu();
			else if (menuOption == 2)
				loggedId = signupMenu();

			cout << endl;
		}

		cout << "Welcome " << reg.getUserName(loggedId) << endl;
		return loggedId;
	}
};
// Registration Classes //

// Question Operations Classes //
struct Question {
	int id;
	int fromUserId;
	int toUserId;
	int anonymous;
	int parentQuestionId;
	string question;
	string answer;
	vector<Question*> threads = {};

	string stringfy(char seperator = ',') const {
		return 
			to_string(id) + seperator + 
			to_string(fromUserId) + seperator +
			to_string(toUserId) + seperator + 
			to_string(anonymous) + seperator +
			to_string(parentQuestionId) + seperator + 
			question + "\n" +
			answer + "\n";
	}

	void decode(pair<string, string> questionRecord, char seperator = ',') {
		istringstream iss(questionRecord.first);

		iss >> id >> seperator;
		iss >> fromUserId >> seperator;
		iss >> toUserId >> seperator;
		iss >> anonymous >> seperator;
		iss >> parentQuestionId >> seperator;
		getline(iss, question);
		
		this->answer = questionRecord.second;
	}

	bool answered() {
		return answer != "";
	}
};

class QuestionLoaderSaver {
private:
	class QuestionLoader {
	private:
		const string file = "questions.txt";
		FileDataBase fileDataBase;

	public:
		QuestionLoader() {
			fileDataBase = FileDataBase(file);
		}

		void load(vector<Question*> &questions) {
			vector<string> records = fileDataBase.read();
			
			unordered_map<int, Question*> parentQuestions;
			vector<Question*> threadQuestions;
			for (int i = 0; i < records.size(); i += 2) {
				string questionInfo = records[i];
				string answer = records[i+1];

				Question* q = new Question();
				q->decode({questionInfo, answer});
				if (q->parentQuestionId == -1)
					parentQuestions[q->id] = q;
				else
					threadQuestions.push_back(q);
			}

			for (auto &thread: threadQuestions)
				parentQuestions[thread->parentQuestionId]->threads.push_back(thread);

			for (auto &question: parentQuestions)
				questions.push_back(question.second);
		}
	};

	class QuestionSaver {
	private:
		const string file = "questions.txt";
		FileDataBase fileDataBase;

	public:
		QuestionSaver() {
			fileDataBase = FileDataBase(file);
		}

		void save(vector<Question*> &questions) {
			fileDataBase.clear();

			for (auto &question: questions) {
				fileDataBase.write(question->stringfy());
				for (auto &thread: question->threads)
					fileDataBase.write(thread->stringfy());
			}
		}
	};

private:
	vector<Question*> questions;
	unordered_map<int, Question*> questionsMap;
	QuestionLoader qLoader = QuestionLoader();
	QuestionSaver qSaver = QuestionSaver();

	int generateQuestionId() {
		int id = -1;
		for (auto &question: questions) {
			id = max(id, question->id);
			for (auto &thread: question->threads)
				id = max(id, thread->id);
		}

		return id+1;
	}

public:
	QuestionLoaderSaver() {}

	void load(int userId, vector<Question*> &toUser, vector<Question*> &fromUser) {
		questions.clear();
		questionsMap.clear();
		qLoader.load(questions);

		for (auto &question: questions) {
			if (question->toUserId == userId)
				toUser.push_back(question);

			if (question->fromUserId == userId)
				fromUser.push_back(question);

			questionsMap[question->id] = question;
		}
	}

	void save() {
		qSaver.save(questions);
	}

	void addQuestion(Question* question) {
		question->id = generateQuestionId();
		questions.push_back(question);
		save();
	}

	void addThreadQuestion(Question* thread) {
		int parentQID = thread->parentQuestionId;
		if (!parentQuestionExist(parentQID))
			return;

		thread->id = generateQuestionId();
		questionsMap[parentQID]->threads.push_back(thread);
		save();
	}

	bool parentQuestionExist(int qid) {
		return questionsMap.count(qid);
	}

	void deleteQuestion(int qid) {
		for (int i = 0; i < questions.size(); i++) {
			auto &question = questions[i];
			if (question->id == qid) {
				swap(questions[i], questions[questions.size()-1]);
				questions.pop_back();
				save();
				break;
			}

			bool thread_found = false;
			int threadsSize = question->threads.size();
			for (int j = 0; j < threadsSize; j++) {
				auto &thread = question->threads[j];
				if (thread->id == qid) {
					swap(question->threads[j], question->threads[threadsSize-1]);
					question->threads.pop_back();
					thread_found = true;
					break;
				}
			}

			if (thread_found)
				break;
		}
	}
};

class UserOperations {
private:
	class UserListing {
	private:
		const string file = "users.txt";
		FileDataBase fileDataBase;
		vector<UserInfo> users;

		void loadUsers() {
			users.clear();

			vector<string> records = fileDataBase.read();
			for (auto &record: records) {
				UserInfo user;
				user.decode(record);
				users.push_back(user);
			}
		}

	public:
		UserListing() {
			fileDataBase = FileDataBase(file);
		}

		UserInfo getUser(int userId) {
			loadUsers();

			for (auto &user: users)
				if (user.id == userId)
					return user;
		
			UserInfo notFoundUser = {-1};	
			return notFoundUser;
		}

		vector<UserInfo> getAllUsers() {
			loadUsers();
			return users;
		}
	};

private:
	int userId;
	QuestionLoaderSaver qLoaderSaver;
	UserListing userListing;
	
	vector<Question*> toMeArr;
	vector<Question*> fromMeArr;
	unordered_map<int, Question*> toMe;
	unordered_map<int, Question*> fromMe;

	void update() {
		toMe.clear();
		fromMe.clear();
		toMeArr.clear();
		fromMeArr.clear();

		qLoaderSaver.load(userId, toMeArr, fromMeArr);
		for (auto &question: toMeArr) {
			toMe[question->id] = question;
			for (auto &thread: question->threads)
				toMe[thread->id] = thread;
		}

		for (auto &question: fromMeArr) {
			fromMe[question->id] = question;
			for (auto &thread: question->threads)
				fromMe[thread->id] = thread;
		}
	}

	void saveUpdates() {
		qLoaderSaver.save();
	}

public:
	UserOperations() {}
	UserOperations(int userId) {
		this->userId = userId;
	}

	vector<Question*> getQuestionsToMe() {
		update();
		return toMeArr;
	}

	vector<Question*> getQuestionsFromMe() {
		update();
		return fromMeArr;
	}

	bool questionToMeExist(int qid) {
		update();
		return toMe.count(qid);
	}

	bool questionFromMeExist(int qid) {
		update();
		return fromMe.count(qid);
	}

	Question* getQuestionToMeWithId(int qid) {
		update();
		if (!questionToMeExist(qid))
			return nullptr;

		return toMe[qid];
	}

	void answerQuestion(int qid, string answer) {
		update();
		if (!questionToMeExist(qid))
			return;

		toMe[qid]->answer = answer;
		saveUpdates();
	}

	UserInfo getUser(int userId) {
		return userListing.getUser(userId);
	}

	vector<UserInfo> getAllSystemUsers() {
		return userListing.getAllUsers();
	}

	void addQuestion(int to, bool AQ, string &question) {
		if (userId == to)
			return;

		Question *q = new Question();
		q->fromUserId = userId;
		q->toUserId = to;
		q->anonymous = AQ; 
		q->question = question;
		q->answer = "";
		q->parentQuestionId = -1;
		qLoaderSaver.addQuestion(q);
	}

	bool validThreadQuestion(int parentQID, int to) {
		if (userId == to) { // User sending to himself 
			cout << "Error 1\n";
			return false; 
		}

		if (!parentQuestionExist(parentQID)) { // Parent Question Id Doesn't exist
			cout << "Error 2\n";
			return false;
		} 
		
		if (questionToMeExist(parentQID)) { // Asking a thread question to a parent question sent to myself
			cout << "Error 3\n";
			return false;
		} 

		return true;
	}

	void addThreadQuestion(int parentQID, int to, bool AQ, string &question) {
		update();
		if (!validThreadQuestion(parentQID, to))
			return;

		Question *thread = new Question();
		thread->fromUserId = userId;
		thread->toUserId = to;
		thread->anonymous = AQ;
		thread->question = question;
		thread->answer = "";
		thread->parentQuestionId = parentQID;

		qLoaderSaver.addThreadQuestion(thread);
	}

	bool parentQuestionExist(int qid) {
		update();
		return qLoaderSaver.parentQuestionExist(qid);
	}

	void deleteQuestion(int qid) {
		update();
		if (!(questionToMeExist(qid) || questionFromMeExist(qid)))
			return;

		qLoaderSaver.deleteQuestion(qid);
	}
};

class QuestionsMenuUI {
private:
	class QuestoinPrinterUI {
	private:
		void listFeed(vector<Question*> &questions) {
			for (auto &question: questions) {
				if (question->answered())
					formatQuestionsToMe(question);

				for (auto &thread: question->threads)
					if (thread->answered())
						formatQuestionsToMe(thread, "Thread Parent ", false);
			}
		}

	public:
		QuestoinPrinterUI() {}

		void formatQuestionsToMe(Question* question, string start = "", bool displayStartForAnswer = true) {
			cout << start << "Question Id (" << question->id << ") ";
			if (!question->anonymous)
				cout << "from user id(" << question->fromUserId << ")\t"; 
			
			cout << "Question: " << question->question << "\n";
			
			if (question->answer != "") {
				if (displayStartForAnswer)
					cout << start;
				cout << "\tAnswer: " << question->answer << "\n";
			}
		
			cout << "\n";
		}

		void formatQuestionsFromMe(Question* question) {
			cout << "Question Id (" << question->id << ")";
			if (!question->anonymous)
				cout << " !AQ";

			cout << " to user id(" << question->toUserId << ")\t\t";
			if (question->parentQuestionId != -1)
				cout << "Thread ";
			cout << "Question: " << question->question << "\t\t";

			if (question->answer == "")
				cout << "NOT Answered YET\n";
			else
				cout << "Answer: " << question->answer << "\n"; 		
		}

		void printQuestionsToMe(UserOperations &userOperations) {
			cout << "\n";
			vector<Question*> toMe = userOperations.getQuestionsToMe();
			for (auto &question: toMe) {
				formatQuestionsToMe(question);
				for (auto &thread: question->threads)
					formatQuestionsToMe(thread, "\tThread: ");
			}
		}

		void printQuestionsFromMe(UserOperations &userOperations) {
			cout << "\n";
			vector<Question*> fromMe = userOperations.getQuestionsFromMe();
			for (auto &question: fromMe) {
				formatQuestionsFromMe(question);
				for (auto &thread: question->threads)
					formatQuestionsFromMe(thread);
			}

			cout << "\n";
		}

		void feed(UserOperations &userOperations) {
			vector<Question*> toMe = userOperations.getQuestionsToMe();
			vector<Question*> fromMe = userOperations.getQuestionsFromMe();
			listFeed(toMe);
			listFeed(fromMe);
		}
	};

private:
	int userId;
	UserOperations userOperations;
	QuestoinPrinterUI qPrinter;

	int menu() {
		cout << "Menu:\n";
		cout << "\t\t1: Print Questions To Me\n";
		cout << "\t\t2: Print Questions From Me\n";
		cout << "\t\t3: Answer Questions\n";
		cout << "\t\t4: Delete Questions\n";
		cout << "\t\t5: Ask Question\n";
		cout << "\t\t6: List System Users\n";
		cout << "\t\t7: Feed\n";
		cout << "\t\t8: Logout\n";

		cout << "\nEnter number in range 1 - 8: ";
		int choice;
		cin >> choice;

		return choice;
	}

	void run() {
		int choice = -1;
		while (true) {
			choice = menu();
			if (choice == 8)
				break; // logout

			if (choice == 1)
				qPrinter.printQuestionsToMe(userOperations);
			else if (choice == 2)
				qPrinter.printQuestionsFromMe(userOperations);
			else if (choice == 3)
				answerQuestion();
			else if (choice == 4)
				deleteQuestion();
			else if (choice == 5)
				askQuestion();
			else if (choice == 6)
				listSystemUsers();
			else if (choice == 7)
				qPrinter.feed(userOperations);
			else
				cout << "ERROR: invalid number...Try again\n";
		}
	}

	void answerQuestion() {
		cout << "Enter Question id or -1 to cancel: ";
		int qid;
		cin >> qid;

		if (qid == -1)
			return;

		if (!userOperations.questionToMeExist(qid)) {
			cout << "Invalid Question ID Try Again\n";
			return;
		}

		Question* question = userOperations.getQuestionToMeWithId(qid);
		qPrinter.formatQuestionsToMe(question);
		cout << "\n";

		if (question->answer != "")
			cout << "Warning: Already answerd. Answer will be updated\n";

		cout << "Enter answer: ";
		string answer;
		cin.ignore();
		getline(cin, answer);

		userOperations.answerQuestion(qid, answer);
		cout << "\n";
	}

	void deleteQuestion() {
		cout << "Enter Question id or -1 to cancel: ";
		int qid;
		cin >> qid;

		if (!(userOperations.questionToMeExist(qid) || userOperations.questionFromMeExist(qid))) {
			cout << "Invalid Question ID Try Again\n\n";
			return;
		}

		userOperations.deleteQuestion(qid);
	}

	void askQuestion() {
		cout << "Enter User id or -1 to cancel: ";
		int toUserId;
		cin >> toUserId;

		if (toUserId == -1)
			return;

		UserInfo user = userOperations.getUser(toUserId);
		if (user.id == this->userId) {
			cout << "Cannot Ask a Question to yourself Try Again \n\n";
			return;
		}

		if (user.id == -1) {
			cout << "Invalid User ID Try Again\n\n";
			return;
		}

		bool anonymous = false;
		if (!user.AQ)
			cout << "Note: Anonymous questions are not allowed for this user\n\n";
		else {
			cout << "Would you like to send this question anonymously (0/1): ";
			cin >> anonymous;
		}

		cout << "For thread questions: Enter Questino id or -1 for new question: ";
		int qid;
		cin >> qid;
		if (qid != -1) {
			if (userOperations.questionToMeExist(qid)) {
				cout << "Cannot ask a Thread Question to yourself\n\n";
				return;
			}

			if (!userOperations.parentQuestionExist(qid)) {
				cout << "Question Id Not Found Try Again\n\n";
				return;
			}
		} 

		cout << "Enter question text: ";
		string question;
		cin.ignore();
		getline(cin, question);		

		if (qid == -1)
			userOperations.addQuestion(toUserId, anonymous, question);
		else
			userOperations.addThreadQuestion(qid, toUserId, anonymous, question);

		cout << "\n";
	}

	void listSystemUsers() {
		vector<UserInfo> users = userOperations.getAllSystemUsers();
		for (auto &user: users)
			cout << "ID: " << user.id << "\t\t" << "Name: " << user.username << "\n";

		cout << "\n";
	}

public:
	QuestionsMenuUI(int userId): userId(userId), userOperations(userId) {
		run();
	}
};
// Question Operations Classes //

class Ask {
public:
	Ask() {	
		LoginMenuUI loginMenu;
		int userId = loginMenu.run();
		QuestionsMenuUI questionsMenuUI(userId);
	}
};

int main() {
	cout << boolalpha;
	Ask ask = Ask();

	// Question* q = new Question();
	// q->id = 1;
	// q->fromUserId = 2;
	// q->toUserId = 3;
	// q->parentQuestionId = 5;
	// q->anonymous = 0;
	// q->question = "abc";
	// q->answer = "def";

	// vector<Question*> questions = {q};
	// const vector<const Question*> questions2 = {q};
	// // cout << questions.size() << " " << questions2.size() << endl;

	// // questions[0]->id = 5;
	// cout << questions2[0]->id << endl;

	return 0;
}