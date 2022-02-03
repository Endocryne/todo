#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

bool to_bool(string const &s) {

	return s != "0";

}

map<string, bool> loadData(const string &fileName) {

	map<string, bool> result;
	string line, word;
	bool checked;
	ifstream input(fileName);

	while (getline(input, line)){

		istringstream iss(line);
		getline(iss, word);
		string s(1, word.back());
		checked = to_bool(s);
		cout << "Checked:" << checked << endl;
		word = word.substr(0, word.length()-2);
		result[word] = checked;
	}
	return result;

}


void printList(map<string, bool> &todo) {

	for (auto &entry : todo) {
		if (entry.second == 1){
		cout << "\033[32m✔ \033[0m" << "\033[9m" << entry.first << "\033[0m"<< endl;
		}
		else { 
			cout << "\033[31m✘ \033[0m"<< entry.first << endl;
		}
	}

}

void saveChanges(map<std::string, bool> &todo, string fileName) {

	std::ofstream out;
	out.open(fileName);
	for (auto &entry : todo) {

		out << entry.first << " " << entry.second << endl;

	}

	out.close();

}


void main_loop(map<string, bool> &todo){

	string userInput = "";
	cout << "Enter ':wq' to save and quit." << endl;
	getline(std::cin, userInput);

	while (userInput != ":wq") {

		int pid = fork();
		if (pid == 0) {

			execlp("clear","clear", 0);

		}
		else {

			waitpid(pid, nullptr, 0);
		}

		printList(todo);
		cout << "\033[47m   \033[0m\033[37m \033[0m";
		getline(cin, userInput);

		if (userInput == ":wq") {
			break;
		}
		else {

			istringstream iss(userInput);
			string command, word;
			iss >> command;
			getline(iss, word);
			word = word.substr(1);
			if (command == "add") {todo[word] = false;}
			else if (command == "finish" || command == "done") {todo[word] = true;}
			else if (command == "remove" || command == "del") {
				if (todo.find(word) != end(todo)){ todo.erase(word); } }
			else { cout << "Unvalid command." << endl; }
		}
	}
}


int main() {

	ifstream input_file;
	string fileName;
	cout << "Enter todo-filename: " << endl;
	cin >> fileName;
	fileName = fileName + ".txt";
	input_file.open(fileName);
	if (input_file)
	{
		map<string, bool> list = loadData(fileName);
		main_loop(list);
		saveChanges(list, fileName);
	}
	else
	{
		ofstream output(fileName);
		output.close();
		map<string, bool> list;
		main_loop(list);
		saveChanges(list, fileName);
	}
	return 0;
}
