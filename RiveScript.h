#ifndef _rivescript_h
#define _rivescript_h

#include <iostream>
#include <vector>
#include <map>

class RiveScript {
	private:
		// Private class variables
		bool   debug;      // Debug mode (defaults to false)
		int    depth;      // Recursion depth limit (defaults to 50)
		double rs_version; // Version of the RiveScript syntax we support (2.0)

		// Private hash std::maps.
		std::map<std::string, std::string> globals;         // ! global  global variables
		std::map<std::string, std::string> bot;             // ! var     bot variables
		std::map<std::string, std::vector<std::string> > arrays; // ! array   arrays
		std::map<std::string, std::string> subs;            // ! sub     substitutions
		std::map<std::string, std::string> person;          // ! person  person substitutions

		// Topic/Trigger/Reply structure
		struct rs_trigger {
			// A trigger is the parent of everything that comes after it.
			std::string redirect;          // @Redirection std::string
			std::vector<std::string> reply;     // List of -Replies
			std::vector<std::string> condition; // List of *Conditions
		};
		struct rs_topic {
			// A topic is the parent of many triggers.
			std::map<std::string,rs_trigger> trigger; // Hash std::map of triggers
			std::vector<std::string> includes;
			std::vector<std::string> inherits;
		};
		struct rs_that {
			std::map<std::string,rs_trigger> trigger;
		};
		struct rs_that_topic {
			// A %Previous topic contains an rs_that between it and the trigger(s).
			std::map<std::string,rs_that> that;
		};
		std::map<std::string, rs_topic> topics; // std::map of topic names
		std::map<std::string, rs_that_topic> thats;  // std::map of %Previous triggers

		// Notes: structure of the "topics" std::map is:
		// topics = std::map<std::string, std::map..>{
		//  "topic_name" => std::map<std::string, std::map..>{
		//   "trigger_text" => std::map{
		//    "replies" => std::vector<std::string>("reply1", "reply2", "etc"),
		//    "condition" => std::vector<std::string>("condition1", "cond2", "etc"),
		//    "redirect" => "redirect trigger (optional)",
		//   },
		//  },
		// };

	public:
		// Constructor method
		RiveScript ();
		RiveScript (bool debug);
		RiveScript (int depth);
		RiveScript (bool debug, int depth);
		void init (bool debug, int depth);

		// Debug methods
		void say (std::string line);
		void warn (std::string line);
		void warn (std::string line, std::string file, int lineno);

		// Reply loading methods
		bool loadDirectory (std::string folder);
		bool loadFile (std::string file);
		bool parse (std::string file,std::vector<std::string> code);

		// Debugging methods
		void _dumpDefinitions ();
		void _dumpDefinitions (std::string name, std::map<std::string, std::string> hash);
		void _dumpTopics ();

		// Util methods
		std::string trim (const std::string &t);
		std::string trim (const std::string &t, const std::string &ws);
		int indexOf (std::string s, std::string match);
		std::vector<std::string> split (std::string s, std::string delim, unsigned int pieces);
		std::vector<std::string> split (std::string s, std::string delim);
		std::string replace (std::string source, std::string search, std::string replace);
};

/*************** POD Documentation for RiveScript.cpp **************************

=head1 NAME

RiveScript - Rendering Intelligence Very Easily

=head1 SYNOPSIS

  RiveScript rs;

=head1 DESCRIPTION

This is a C++ implementation of a RiveScript interpreter.

=head1 METHODS

=head2 CONSTRUCTOR

=over 4

=item RiveScript ()

=item RiveScript (bool debug)

=item RiveScript (int depth)

=item RiveScript (bool debug, int depth)

Create a new instance of a RiveScript interpreter. Params:

  bool debug = false: Debug mode (prints verbose text to the terminal)
  int depth  = 50:    The depth limit for when the module does recursion, to
                      prevent it from getting out of control.

=back

=head2 LOADING AND PARSING

=over 4

=item bool loadDirectory (std::string path)

Load an entire directory full of RiveScript documents. Returns C<true> on
success and C<false> on failure.

  std::string path: Directory pathname where RS docs can be found.

=item bool loadFile (std::string path)

Load a single RiveScript document. Returns C<true> on success and C<false> on
failure.

  std::string path: Directory and file name of an RS document.

=item bool stream (std::string code)

Stream some RiveScript code directly in from your C++ code. Returns C<true> on
success and C<false> on failure.

  std::string code: RiveScript code to parse.

=item private bool parse (std::string[] code)

Parse the lines of RiveScript code and make some sense out of them.

  std::string[] code: Array of lines of code.

=back

=head2 PRIVATE METHODS

=over 4

=item void say (std::string line)

=item void warn (std::string line)

Debug methods. C<say()> prints a line of verbose debug text to the terminal when
debug mode is active; C<warn()> prints a line to the terminal when a warning
pops up whether or not debug mode is active.

=back

=cut

*******************************************************************************/

#endif
