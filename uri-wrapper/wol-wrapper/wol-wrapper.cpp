#define URI_SCHEME "wol:"
#define ARG_DELIM '-'

#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("No arguments received\n");
		return 0;
	}
	if (argc < 3) {
		printf("No URI received\n");
		return 0;
	}

	const string binpath(argv[1]);
	const string uri(argv[2]);
	const string scheme(uri.substr(0, uri.find(':') + 1));
	const string body(uri.substr(scheme.length()));

	if (scheme.compare(URI_SCHEME) != 0) {
		printf("URI must begin with wol:");
		return 0;
	}

	stringstream ss(body);
	vector<string> tokens;
	string token;
	while (getline(ss, token, ARG_DELIM)) {
		tokens.push_back(token);
	}

	if (tokens.size() < 3) {
		printf("Insufficent arguments to WolCmd\n");
		return 0;
	}

	string macaddress(tokens.at(0));
	string broadcast(tokens.at(1));
	string subnetmask(tokens.at(2));

	stringstream out;

	out << binpath << " ";
	out << macaddress << " ";
	out << broadcast << " ";
	out << subnetmask;

	system(out.str().c_str());
	return 0;
}