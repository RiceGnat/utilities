#define URI_SLEEP "sleep:"
#define URI_SHUTDOWN "shutdown:"
#define URI_RESTART "restart:"
#define ARG_DELIM '-'

#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("No arguments received.\n");
		return 0;
	}
	if (argc < 3) {
		printf("No URI received.\n");
		return 0;
	}

	const string binpath(argv[1]);
	const string uri(argv[2]);
	const string scheme(uri.substr(0, uri.find(':') + 1));
	const string body(uri.substr(scheme.length()));

	stringstream ss(body);
	vector<string> tokens;
	string token;

	while (getline(ss, token, ARG_DELIM)) {
		tokens.push_back(token);
	}
	
	string computer(tokens.at(0));

	stringstream out;

	out << binpath << " ";
	out << computer << " ";
	//"Invoke-WmiMethod -Class Win32_Process -Name Create -ArgumentList ""rundll32.exe" "user32.dll,LockWorkStation"""
	if (scheme.compare(URI_SLEEP)) {
	}
	else {
		printf("Unrecognized URI scheme\n");
		return 0;
	}

	return 0;
}