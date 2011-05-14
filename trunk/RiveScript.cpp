// Standard libraries
#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
//#include <regex> // Requires TR-1 compatible compiler

// Non-standard libraries that may need to be installed
#include "boost/regex.hpp"

#include "RiveScript.h"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;

/******************************************************************************
 * Constructor Methods                                                        *
 ******************************************************************************/

RiveScript::RiveScript () {
	// Default values for private variables.
	init(false, 50);
}
RiveScript::RiveScript (bool debug_mode) {
	init(debug_mode, 50);
}
RiveScript::RiveScript (int depth) {
	init(false, depth);
}
RiveScript::RiveScript (bool debug_mode, int depth) {
	init (debug_mode, depth);
}

void RiveScript::init (bool debug, int depth) {
	this->debug      = debug;
	this->depth      = depth;
	this->rs_version = 2.0;
	cout << "RS object created with debug mode " << this->debug << " and depth " << this->depth << endl;
	cout << "We support RS version " << this->rs_version << endl;
}

// Debug methods
void RiveScript::say (string line) {
	if (this->debug == true) { // not working?
		cout << "RS: " << line << endl;
	}
}
void RiveScript::warn (string line) {
	if (this->debug == true) {
		cout << "RS-WARNING: " << line << endl;
	}
	else {
		cout << "RS: " << line << endl;
	}
}
void RiveScript::warn (string line, string file, int lineno) {
	if (this->debug == true) {
		cout << "RS-WARNING: " << line;
	}
	else {
		cout << "RS: " << line;
	}
	cout << " at " << file << " line " << lineno << ".\n";
}

bool RiveScript::loadDirectory (string folder) {
	say("Loading directory " + folder);

	// Vector to hold file names.
	vector<string> files = vector<string>();

	// Directory handles.
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(folder.c_str())) == NULL) {
		cout << "Error (" << errno << ") opening " << folder << endl;
		return false;
	}

	// Read files.
	while ((dirp = readdir(dp)) != NULL) {
		string file = string(dirp->d_name);
		if (indexOf(file, ".") == 0) {
			continue;
		}
		else if (file == "begin.rs") { // TODO be case insensitive
			// A begin.rs must always be first to be loaded
			vector<string>::iterator it;
			it = files.begin();
			it = files.insert(it, file);
		}
		else {
			files.push_back(file);
		}
	}

	// Close the folder.
	closedir(dp);

	// Debug the files.
	for (unsigned int i = 0; i < files.size(); i++) {
		//cout << files[i] << endl;
		if (!loadFile(folder + "/" + files[i])) {
			warn("Couldn't load file " + folder + "/" + files[i]);
			return false;
		}
	}

	return true;
}

bool RiveScript::loadFile (string file) {
	say("Loading RiveScript document " + file);

	// Open the file for reading.
	std::ifstream fh (file.c_str());
	if (fh.is_open()) {
		say("Opening of " + file + " was successful.");

		// Read lines from the file and add to a vector.
		vector<string> lines = vector<string>();
		string line;
		while (! fh.eof() ) {
			getline (fh, line);
			lines.push_back(line);
		}

		// Close it when we're done.
		fh.close();

		// Parse it.
		if (!parse(file, lines)) {
			warn("Failed to parse " + file);
			return false;
		}
	}
	else {
		warn("Unable to open file " + file + " for reading!");
		return false;
	}

	return true;
}

bool RiveScript::parse (string file, vector<string> code) {
	say("Called upon to parse " + file);

	// State variables.
	string topic   = "random"; // Default topic = random
	int    lineno  = 0;        // Keep track of line numbers for error reporting
	bool   comment = false;    // When we're in a multi-line comment
	bool   inobj   = false;    // When we're in an object block
	string objname = "";       // Name of the object we're in
	string objlang = "";       // Language of the object we're in
	string objbuf  = "";       // Code buffer of the object we're in
	string ontrig  = "";       // Current +Trigger text we're working with
	int    repcnt  = 0;        // Reply counter
	int    concnt  = 0;        // Condition counter
	string lastcmd = "";       // Last command symbol
	string isThat  = "";       // Is a %Previous trigger.

	// Start parsing lines.
	int lp;
	for (lp = 0; lp < code.size(); lp++) {
		string line = code[lp];
		lineno++;

		// Chomp this line down to size.
		line = trim(line);

		// Skip a blank line.
		if (line.length() == 0) {
			continue;
		}

		// Look for comments.
		if (line.substr(0,2) == "//") { // Single line comment.
			continue;
		}
		else if (line.substr(0,2) == "/*") { // Multi-line comment START
			comment = true;
			continue;
		}
		else if (indexOf(line, "*/") > -1) {
			comment = false;
			continue;
		}
		else if (comment) {
			continue;
		}

		// The left-most character is the command code. Get it and chop any
		// spaces off the front of what's left.
		string cmd = line.substr (0,1);
		line = line.substr(1);
		line = trim(line);

		// Strip off in-line comments if there's a space before and after a "//"
		int inlineComment = indexOf(line, " // ");
		if (inlineComment > -1) {
			say("This line has an inline comment!");
			line.erase(inlineComment); // Erase from there to the end
			line = trim(line);
		}

		say("Cmd [" + cmd + "] Line: " + line + " (Topic: " + topic + ")");

		// TODO: syntax check this line

		// Reset the %Previous state if this is a new +Trigger.
		if (cmd == "+") {
			isThat = "";
		}

		// Do a look-ahead for ^Continue and %Previous commands.
		for (int i = (lp + 1); i < code.size(); i++) {
			string lookahead = code[i];
			lookahead = trim(lookahead);
			if (lookahead.length() == 0) {
				continue;
			}

			// Get the look-ahead command and args.
			string lookCmd = lookahead.substr (0,1);
			lookahead = trim(lookahead.substr (1));

			// Only continue if there's any data.
			if (lookahead.length() > 0) {
				// The lookahead command has to be either a % or a ^.
				if (lookCmd != "^" && lookCmd != "%") {
					break;
				}

				// If the current command is a +, see if the following command
				// is a %Previous.
				if (cmd == "+") {
					if (lookCmd == "%") {
						say("This line has a %Previous (" + lookahead + ")");
						isThat = lookahead;
						break;
					}
					else {
						isThat = "";
					}
				}

				// If the current command is a ! and the next command(s) are
				// ^, we'll tack each extension on as a line break (which is
				// useful information for arrays; everything else is gonna ditch
				// this info).
				if (cmd == "!") {
					if (lookCmd == "^") {
						line += "<crlf>" + lookahead;
						say("^ Line: " + line);
					}
					continue;
				}

				// If the current line is not a ^ and the line after is
				// not a %, but the line after IS a ^, then tack it onto
				// the end of the current line (this is fine for every other
				// type of command that doesn't require special treatment).
				if (cmd != "^" && lookCmd != "%") {
					if (lookCmd == "^") {
						line += lookahead;
					}
					else {
						break;
					}
				}
			}
		}

		if (cmd == "!") {
			// ! DEFINE
			vector<string> halves = split(line, "=", 2);
			string what = trim(halves[0]);
			string is   = trim(halves[1]);
			say("! DEFINE: " + what + " => " + is);

			// Is this a !version declaration?
			if (what == "version") {
				say("Using RiveScript Version " + is);

				// Cast it to a double to validate the version.
				double version = strtod (is.c_str(), NULL);
				if (version > this->rs_version) {
					cout << version << " > " << this->rs_version << endl;
					//warn("Unsupported RiveScript version " + version + "; refusing to parse file!");
					return false;
				}
			}
			else if (indexOf(what," ") > -1) {
				// This is a type of definition.
				vector<string> halves = split(what, " ", 2);
				string type = trim(halves[0]);
				string name = trim(halves[1]);
				say("Definition type=" + type + " name=" + name);

				// If setting to <undef>, we delete the field.
				bool undef = false;
				if (is == "<undef>") {
					undef = true;
				}

				// Handle the types.
				if (type == "global") {
					// Setting a global variable.
					if (undef) {
						this->globals.erase(name);
					}
					else {
						this->globals[name] = is;
					}
					say("Set global " + name + " => " + is);
				}
				else if (type == "var") {
					// Setting a bot variable.
					if (undef) {
						this->bot.erase(name);
					}
					else {
						this->bot[name] = is;
					}
				}
				else if (type == "array") {
					// Setting an array.
					vector<string> parts = split("<crlf>", is);
					vector<string> fields;

					// An array can be defined over many lines via the ^CONTINUE,
					// and each line ("parts" here) can separate its array elements
					// by a pipe symbol "|" or by spaces. First we go over the "parts"
					// and then look at the elements defined on each part.
					for (unsigned int i = 0; i < parts.size(); i++) {
						string part = parts[i];

						// Is it pipe-separated or space separated?
						if (indexOf(part, "|") > -1) {
							vector<string> pieces = split(part,"|");
							for (unsigned int j = 0; j < pieces.size(); j++) {
								fields.push_back(pieces[j]);
							}
						}
						else {
							vector<string> pieces = split(part," ");
							for (unsigned int j = 0; j < pieces.size(); j++) {
								if (pieces[j].length() == 0) {
									// Skip blank pieces (in case of multiple spaces to separate)
									continue;
								}
								fields.push_back(pieces[j]);
							}
						}
					}

					// Convert escape code \s into a space
					for (unsigned int i = 0; i < fields.size(); i++) {
						fields[i] = replace(fields[i], "\\s", " ");
					}

					// Store the array.
					this->arrays[name] = fields;
				}
				else if (type == "sub") {
					// Setting a substitution variable.
					if (undef) {
						this->subs.erase(name);
					}
					else {
						this->subs[name] = is;
					}
				}
				else if (type == "person") {
					// Setting a substitution variable.
					if (undef) {
						this->person.erase(name);
					}
					else {
						this->person[name] = is;
					}
				}
				else {
					warn("Unknown definition type \"" + type + "\"", file, lineno);
				}
			}
		}
		else if (cmd == ">") {
			// > LABEL
			vector<string> parts = split(line, " ");
			string type  = parts[0];
			string name  = parts.size() >= 2 ? parts[1] : "";

			// Handle the label types.
			if (type == "begin") {
				say("Found the BEGIN Statement.");
				type = "topic";
				name = "__begin__";
			}
			if (type == "topic") {
				say("Set topic to " + name);
				ontrig = "";
				topic  = name;

				// TODO look for inherits and includes keywords
				if (parts.size() >= 3) {
					string mode = ""; // inherits or includes
					for (unsigned int i = 2; i < parts.size(); i++) {
						string text = parts[i];
						if (text == "inherits" || text == "includes") {
							mode = text;
						}
						else {
							if (mode == "inherits") {
								topics[topic].inherits.push_back(text);
							}
							else if (mode == "includes") {
								topics[topic].includes.push_back(text);
							}
						}
					}
				}
			}
			if (type == "object") {
				// If an extra field was provided, it should be the programming language.
				string lang = parts.size() >= 3 ? parts[2] : "";
				lang = trim(lang);
				say("Found an object definition named " + name + " of language " + lang);
				// TODO: handle this
			}
		}
		else if (cmd == "<") {
			// < LABEL
			string type = line;

			if (type == "begin" || type == "topic") {
				say("End " + type + " label.");
				topic = "random";
			}
			else if (type == "object") {
				say("End object label.");
				inobj = false;
			}
		}
		else if (cmd == "+") {
			// + TRIGGER
			say("Trigger pattern: " + line);
//				// Initialize the rs_trigger object. (DON'T NEED TO! YAY!)
//				rs_trigger trigger;
//				topics[topic].trigger[line] = trigger;
			ontrig = line;
		}
		else if (cmd == "-") {
			// - REPLY
			say("Reply: " + line);
			if (ontrig.length() == 0) {
				warn("Reply found before a trigger!");
				continue;
			}

			// Is this a %Previous?
			if (isThat.length() > 0) {
				thats[topic].that[isThat].trigger[ontrig].reply.push_back(line);
			}
			else {
				// Add the reply to this trigger.
				topics[topic].trigger[ontrig].reply.push_back(line);
			}
		}
		else if (cmd == "%") {
			// % PREVIOUS
			say("%Previous pattern: " + line);
			// This was handled above.
		}
		else if (cmd == "^") {
			// ^ CONTINUE
			// This was handled above.
		}
		else if (cmd == "@") {
			// @ REDIRECT
			say("Redirect: " + line);
			if (ontrig.length() == 0) {
				warn("Redirect found before a trigger!");
				continue;
			}

			// Set the redirect for this trigger.
			if (isThat.length() > 0) {
				thats[topic].that[isThat].trigger[ontrig].redirect = line;
			}
			else {
				topics[topic].trigger[ontrig].redirect = line;
			}
		}
		else if (cmd == "*") {
			// * CONDITION
			say("Condition: " + line);
			if (isThat.length() > 0) {
				thats[topic].that[isThat].trigger[ontrig].condition.push_back(line);
			}
			else {
				topics[topic].trigger[ontrig].condition.push_back(line);
			}
		}
		else {
			warn("Unrecognized command \"" + cmd + "\"", file, lineno);
		}
	}

	return true;
}

void RiveScript::_dumpDefinitions (string name, map<string, string> hash) {
	// Loop over the globals.
	say("<<< " + name + " >>>");
	map<string, string>::const_iterator iter;
	for (iter = hash.begin(); iter != hash.end(); ++iter) {
		say(iter->first + " => " + iter->second);
	}
	say("\n\n");
}

void RiveScript::_dumpDefinitions () {
	// Dump all the definitions.
	_dumpDefinitions("Global Variable Dump", this->globals);
	_dumpDefinitions("Bot Variable Dump", this->bot);
	_dumpDefinitions("Substitutions Dump", this->subs);
	_dumpDefinitions("Person Substitutions Dump", this->person);
}

void RiveScript::_dumpTopics () {
	// Dump all the topic/trigger/reply data.
	say("topics = {");

	// Loop through the topic keys.
	map<string, rs_topic>::const_iterator topic_iter;
	for (topic_iter = topics.begin(); topic_iter != topics.end(); ++topic_iter) {
		rs_topic topic = topics[topic_iter->first];
		say("\t'" + topic_iter->first + "' => {");

		// Loop through the topic's triggers.
		map<string, rs_trigger>::const_iterator trig_iter;
		for (trig_iter = topic.trigger.begin(); trig_iter != topic.trigger.end(); ++trig_iter) {
			string trig_text = trig_iter->first;
			rs_trigger trigger = topic.trigger[trig_iter->first];
			say("\t\t'" + trig_text + "' => {");

			// Dump the replies.
			if (trigger.reply.size() > 0) {
				say("\t\t\t'reply' => [");
				for (int i = 0; i < trigger.reply.size(); i++) {
					say("\t\t\t\t'" + trigger.reply[i] + "',");
				}
				say("\t\t\t],");
			}

			// Dump the conditions.
			if (trigger.condition.size() > 0) {
				say("\t\t\t'condition' => [");
				for (int i = 0; i < trigger.condition.size(); i++) {
					say("\t\t\t\t'" + trigger.condition[i] + "',");
				}
				say("\t\t\t],");
			}

			// Dump the redirect.
			if (trigger.redirect.size() > 0) {
				say("\t\t\t'redirect' => '" + trigger.redirect + "',");
			}

			say("\t\t},");
		}

		if (topic.includes.size() > 0) {
			say("\t\t\'includes\' => [");
			for (unsigned int i = 0; i < topic.includes.size(); i++) {
				say("\t\t\t\'" + topic.includes[i] + "\',");
			}
			say("\t\t],");
		}

		if (topic.inherits.size() > 0) {
			say("\t\t\'inherits\' => [");
			for (unsigned int i = 0; i < topic.inherits.size(); i++) {
				say("\t\t\t\'" + topic.inherits[i] + "\',");
			}
			say("\t\t],");
		}

		say("\t},");
	}

	say("};\n\n");
}

/*******************************************************************************
 * Utility Functions (trim, split, etc. not directly related to RiveScript     *
 ******************************************************************************/

string RiveScript::trim (const string &t) {
	string s = t;
	s = trim(s, " \t\x0A\x0D");
	return s;
}

string RiveScript::trim (const string &t, const string &ws) {
	string s = t;
	size_t found;

	// Trim off the end
	found = s.find_last_not_of(ws);
	if (found != string::npos) {
		s.erase(found+1);
	}
	else {
		s.clear();
	}

	// Trim off the front
	found = s.find_first_not_of(ws);
	if (found != string::npos) {
		s.erase(0, found);
	}
	else {
		s.clear();
	}

	return s;
}

int RiveScript::indexOf (string s, string match) {
	string::size_type loc = s.find(match, 0);
	if (loc != string::npos) {
		return loc;
	}
	else {
		return -1;
	}
}

vector<string> RiveScript::split (string s, string delim, unsigned int pieces) {
	vector<string> result = vector<string>();

	// See if the string even contains the delimiter in the first place.
	if (indexOf(s, delim) == -1) {
		// It doesn't. Just add the whole string to the first element.
		result.push_back(s);
	}

	// Do a loop of: look for the delimeter, substr the two sides of it, add them
	// to the result and continue until the result is as big as we wanted it.
	while (indexOf(s, delim) > -1 && result.size() < pieces) {
		int pos = indexOf(s, delim);
		if (pos == -1) {
			// No more matches!
			break;
		}
		else {
			// Substr that!
			string left = s.substr(0, pos);
			string right = s.substr(pos + delim.length());
			s = right;
			result.push_back(left);
		}
	}

	if (s.size() > 0) {
		result.push_back(s);
	}

	// Pad the extra entries with empty space if they're not defined.
	while (result.size() < pieces) {
		result.push_back("");
	}

	return result;
}

vector<string> RiveScript::split (string s, string delim) {
	vector<string> result;

	// Doesn't contain the delim?
	if (indexOf(s, delim) == -1) {
		result.push_back(s);
		return result;
	}

	// Keep splitting.
	while (indexOf(s, delim) > -1) {
		int pos = indexOf(s, delim);
		if (pos == -1) {
			break;
		}
		else {
			string left = s.substr(0, pos);
			string right = s.substr(pos + delim.length());
			s = right;
			result.push_back(left);
		}
	}

	if (s.size() > 0) {
		result.push_back(s);
	}

	return result;
}

string RiveScript::replace (string source, string search, string replace) {
	boost::regex pattern (search, boost::regex_constants::icase | boost::regex_constants::perl);
	string output;
	output = boost::regex_replace (source, pattern, replace);
	return output;

	// A replace w/o regexes
	string result = "";

	while (indexOf(source, search) > -1) {
		int pos = indexOf(source, search);
		if (pos == -1) {
			break;
		}

		string left = source.substr(0, pos);
		string right = source.substr(pos + search.length());

		result += left + replace + right;
	}

	return result;
}
