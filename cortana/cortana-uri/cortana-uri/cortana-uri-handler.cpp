#include <string>
#include <vector>
#include <sstream>

#include "urischeme.h"

using namespace std;

// arg1 : uri string
// arg2 : path to binary
int main(int argc, char** argv) {
	if (argc < 2) {
		printf("URI string required\n");
		return 0;
	}

	// Get URI
	const string uri(argv[1]);

	// Split protocol and URI body
	const string protocol(uri.substr(0, uri.find(':') + 1));
	const string body(uri.substr(protocol.length()));

	// Check that URI protocol is what is expected
	if (protocol.compare(URI_PROTOCOL) != 0) {
		printf("URI must begin with %s\n", URI_PROTOCOL);
		return 0;
	}

	// Get path to binary if it was passed
	const bool haspath = argc > 2;
	string binpath;
	if (haspath) binpath = argv[2];

	// Tokenize parameters from URI body
	stringstream ss(body);
	vector<string> tokens;
	string token;

	while (getline(ss, token, ARG_DELIM)) {
		tokens.push_back(token);
	}

	// Get command
	if (tokens.size() <= COMMAND_INDEX) {
		printf("No command in URI\n");
		return 0;
	}

	string* command = &(tokens.at(COMMAND_INDEX));

	// Open stream to build command string
	stringstream out;

	// Switch by command
	if (command->MATCHES(CMD_WOL)) {
		// Wake-on-LAN (WolCmd)
		if (tokens.size() < 3) {
			printf("Insufficent arguments to WolCmd\n");
			return 0;
		}

		// Set default bin
		if (!haspath) binpath = "WolCmd";

		// Get magic packet parameters
		string* macaddress(&(tokens.at(COMMAND_PARAMINDEX(1))));
		string* broadcast(&(tokens.at(COMMAND_PARAMINDEX(2))));
		string* subnetmask(&(tokens.at(COMMAND_PARAMINDEX(3))));

		// Build command string
		out << binpath << " ";
		out << *macaddress << " ";
		out << *broadcast << " ";
		out << *subnetmask;
	}
	else if (command->MATCHES(CMD_SHUTDOWN) || command->MATCHES(CMD_SLEEP) || command->MATCHES(CMD_RESTART)) {
		// Shutdown/suspend/restart (PsShutdown)
		if (tokens.size() < 2) {
			printf("Specify target computer\n");
			return 0;
		}

		// Set default bin
		if (!haspath) binpath = "PsShutdown";
		
		// Get computer name
		string* computer(&(tokens.at(COMMAND_PARAMINDEX(1))));

		// Get timeout value if present
		string* timeout = tokens.size() > 2 ? &(tokens.at(COMMAND_PARAMINDEX(2))) : new string("0");

		// Build command string
		out << binpath << " ";
		out << "\\\\" << *computer << " -t " << *timeout;
		if (command->MATCHES(CMD_SLEEP)) {
			out << " -d";
		}
		else if (command->MATCHES(CMD_SLEEP)) {
			out << " -r";
		}
	}
	else {
		printf("Command not recognized\n");
		return 0;
	}
#ifdef _DEBUG
	printf(out.str().c_str());
	system("pause");
#endif // _DEBUG

#ifndef _DEBUG
	system(out.str().c_str());
#endif // !_DEBUG

	return 0;
}