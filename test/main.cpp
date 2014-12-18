#include "Util.hpp"
#include <string>

using namespace std;

string TestUtil::fixtureRootPath = ".";

#include "../command/AddEventCommand.cpp"
#include "../command/DeleteEventCommand.cpp"
#include "../command/AbstractCommand.cpp"
#include "../gui/PropertyValueProxy.cpp"
#include "../sequence/io/SAXAdapter.cpp"
#include "../sequence/io/XVSQFileReader.cpp"
