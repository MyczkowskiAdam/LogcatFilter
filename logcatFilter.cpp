//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#define VERSION "1.1 Windows"
char const *blacklist = "'";
#else
#define VERSION "1.1   Linux"
char const *blacklist = "'";
#endif

int iPriority, counter;
std::string fileName, priority[] = {
	" V ",
	" D ",
	" I ",
	" W ",
	" E ",
	" F ",
	" S "
};

int main() {
	std::cout << "___________________________________________" << std::endl;
	std::cout << "__________Welcome to LogcatFilter__________" << std::endl;
	std::cout << "_________By Adam Myczkowski (mycax)________" << std::endl;
	std::cout << "_____________Version: " << VERSION << "__________" << std::endl;
	std::cout << "___________________________________________" << std::endl;
	std::cout << "Enter file path here > " << std::flush;
	std::cin >> fileName;

	fileName.erase(std::remove(fileName.begin(), fileName.end(), blacklist[0]), fileName.end());

	std::ifstream iLog(fileName.c_str());
	if (iLog.is_open()) {
		std::ofstream oLog("filtered-logcat.txt");

		std::cout << std::endl;
		std::cout << "V: Verbose (1)" << std::endl;
		std::cout << "D : Debug (2)" << std::endl;
		std::cout << "I : Info (3)" << std::endl;
		std::cout << "W : Warning (4)" << std::endl;
		std::cout << "E : Error (5)" << std::endl;
		std::cout << "F : Fatal (6)" << std::endl;
		std::cout << "S : Silent (7)" << std::endl;
		std::cout << "Search for a phrase (8)" << std::endl;
		std::cout << std::endl;
		std::cout << "What do you want to filter? > " << std::flush;
		std::cin >> iPriority;

		while (iPriority > 8 || iPriority < 1 || std::cin.fail()) {
			std::cout << "Incorrect input, what do you want to filter? > " << std::flush;
			std::cin.clear();
			std::cin.ignore();
			std::cin >> iPriority;
		}

		std::string iLine;
		if (iPriority == 8) {
			std::string strToSearch;
			std::cout << "Enter a phrase to search > " << std::flush;
			std::cin >> strToSearch;

			std::cout << std::endl;
			std::cout << "Searching for: " << strToSearch << std::endl;
			oLog << "Searching for: " << strToSearch << std::endl;
			oLog << std::endl;

			while (std::getline(iLog, iLine)) {
				if (iLine.find(strToSearch) != std::string::npos) {
					oLog << iLine << std::endl;
					counter++;
				}
			}
			std::cout << "Found  " << counter << " matching lines." << std::endl;
			oLog << std::endl;
			oLog << "Found  " << counter << " matching lines." << std::endl;
		}
		else {

			std::cout << std::endl;
			std::cout << "Filtering: " << priority[iPriority - 1] << std::endl;
			oLog << "Filtering for: " << priority[iPriority - 1] << std::endl;
			oLog << std::endl;

			while (std::getline(iLog, iLine)) {
				if (iLine.find(priority[iPriority - 1]) != std::string::npos) {
					oLog << iLine << std::endl;
					counter++;
				}
			}
			std::cout << "Found  " << counter << " matching lines." << std::endl;
			oLog << std::endl;
			oLog << "Found  " << counter << " matching lines." << std::endl;
		}

		oLog.close();
		iLog.close();
	}
	else {
		std::cout << "Unable to open the file, it may not exist or be corrupted!" << std::endl;
	}

#ifdef _WIN32
	std::cout << "Press any key to exit . . ." << std::endl;
	while (true) {
		if (_kbhit()) {
			return EXIT_SUCCESS;
		}
	}
#else
	return 0;
#endif

}