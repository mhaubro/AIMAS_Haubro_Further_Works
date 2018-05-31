#include "Client.h"
#include <string>
#include <list>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <iterator>
#include "Command.h"
#include "MasterSearcher.h"
#include "Initializer.h"
#include "ManualNodePool.h"

using std::stringstream;
using std::istream;
using std::vector;
using std::string;
using std::getline;
using std::pair;
using std::list;

Client::Client()
{
   initialState = Initializer::setupEnvironment();
	/*
	 * This is a constructor that reads input from std::cin, from the server about the map
	 * This should not be necessary to edit, and if need be, most likely only in main method
	 * Where input parameters are read and might be added.
	 */

}

//As we never expect to have more than 1, this is not implemented.
Client::~Client()
{
}

int main(int argc, char * argv[]){
	/*
	argv[1] contains memory limit in MB. This is the FIRST input, as argv[0] is program name
	argv[2] contains how often the print should happen. This is optional, default is 1000
	*/
	pool.setup(atoi(argv[1]));

	int printfreq = 1000;
	char buffer[200];

	Client client = Client();

	//StrategyBFS strategy = StrategyBFS();

	std::vector<std::string> solution;

	MasterSearcher::getPlan(client.initialState);
	return 0;
}
