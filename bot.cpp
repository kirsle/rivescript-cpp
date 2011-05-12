#include <iostream>
#include <string>

#include "RiveScript.h"

using std::string;
using std::cin;
using std::cout;
using std::getline;
using std::endl;

// RiveScript bot script

int main () {
	RiveScript rs (true, 50);
	rs.loadDirectory("./demo");

	while (true) {
		string input;
		cout << "You> ";
		getline(cin, input);

		// Debug commands.
		if (input == "dump globals") {
			rs._dumpDefinitions();
			continue;
		}
		else if (input == "dump topics") {
			rs._dumpTopics();
			continue;
		}
		else if (input == "exit") {
			return 0;
		}

		//string reply = rs.reply("localuser", input);
		string reply = "To Do";

		cout << "Bot> " << reply << endl;
	}

	return 0;
}
