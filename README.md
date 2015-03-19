# RiveScript-C++

This is an abandoned project that was imported from
`code.google.com/p/rivescript-cpp`. It was intended to be a port of RiveScript
to the C++ programming language.

I have no plans at this time to resurrect this project.

# State of the Code

This source code is currently only capable of reading and parsing RiveScript
documents into memory, via functions `loadDirectory()`, `loadFile()` and
`parse()`. It does not have reply sorting implemented, or reply fetching, or
anything else that's important to have in a RiveScript bot.

# See Also

There are much more complete ports of RiveScript in other programming
languages. If you want RiveScript in C++, consider embedding a Python
interpreter in your program, or use Cython, or something along those
lines. If all else fails, some of the RiveScript implementations have a
command line interface in which they can communicate with a third party
program via JSON over standard I/O.

* Perl - <https://github.com/aichaos/rivescript-perl>
* Python - <https://github.com/aichaos/rivescript-python>
* JavaScript - <https://github.com/aichaos/rivescript-js>
* Java - <https://github.com/aichaos/rivescript-java>

Official homepage of RiveScript - <http://www.rivescript.com/>
