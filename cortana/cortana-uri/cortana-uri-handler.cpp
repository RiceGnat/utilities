#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <fstream>

#include "cortana-uri.h"

using namespace std;

// arg1 : uri string
int main(int argc, char** argv) {
	if (argc < 2) {
		printf("URI string required\n");
		return 0;
	}

	// Get URI
	const string uri(argv[1]);

	// Call handler
	return cortana::handleUri(uri);
}

namespace cortana {
	int handleUri(const char* uri) {
		return handleUri(string(uri));
	}

	int handleUri(const string uri) {

		// Split protocol and URI body
		const string protocol(uri.substr(0, uri.find(':') + 1));
		const string body(uri.substr(protocol.length()));

		// Check that URI protocol is what is expected
		if (protocol.compare(URI_PROTOCOL) != 0) {
			printf("URI must begin with %s\n", URI_PROTOCOL);
			return EXIT_FAILURE;
		}

		// Get path to binary if it was passed
		string binpath;

		// Remove this functionality for now for the sake of simplicity
		// Assume that the directories for the binaries have been added to PATH
		//const bool haspath = argc > 2;
		//if (haspath) binpath = argv[2];

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
			return EXIT_FAILURE;
		}

		string* command = &(tokens.at(COMMAND_INDEX));

		// Open stream to build command string
		stringstream out;

		// Switch by command
		if (command->MATCHES(CMD_WOL)) {
			// Wake-on-LAN (WolCmd)
			if (tokens.size() < 3) {
				printf("Insufficent arguments to WolCmd\n");
				return EXIT_FAILURE;
			}

			// Set default bin
			binpath = "WolCmd";

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
				printf("Target computer not specified\n");
				return EXIT_FAILURE;
			}

			// Set default bin
			binpath = "PsShutdown";

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
			else if (command->MATCHES(CMD_RESTART)) {
				out << " -r";
			}
		}
		else {
			printf("Command not recognized\n");
			return EXIT_FAILURE;
		}

#ifdef _DEBUG
		printf("%s\n", out.str().c_str());
		system("pause");
#endif // _DEBUG

#ifndef _DEBUG
		system(out.str().c_str());
#endif // !_DEBUG

		return 1233;
	}
}