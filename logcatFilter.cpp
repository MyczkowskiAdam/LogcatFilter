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

#define VERSION "1.0"

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

int iPriorty, counter;
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
	std::cout << "________________Version: " << VERSION << "_______________" << std::endl;
	std::cout << "___________________________________________" << std::endl;
	std::cout << "Enter file path here > " << std::flush;
	std::cin >> fileName;

	std::ifstream iLog(fileName);
	if (iLog.is_open()) {
		std::ofstream oLog("filtered-logcat.txt");
		while (iPriorty > 8 || iPriorty < 1) {

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
			std::cin >> iPriorty;

			std::string iLine;
			if (iPriorty == 8) {
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
				std::cout << "Filtering: " << priority[iPriorty - 1] << std::endl;
				oLog << "Filtering for: " << priority[iPriorty - 1] << std::endl;
				oLog << std::endl;

				while (std::getline(iLog, iLine)) {
					if (iLine.find(priority[iPriorty - 1]) != std::string::npos) {
						oLog << iLine << std::endl;
						counter++;
					}
				}
				std::cout << "Found  " << counter << " matching lines." << std::endl;
				oLog << std::endl;
				oLog << "Found  " << counter << " matching lines." << std::endl;
			}
		}
		oLog.close();
		iLog.close();
	}
	else {
		std::cout << "Unable to open the file, it may not exist or be corrupted!" << std::endl;
	}

	std::cout << "Press any key to exit . . ." << std::endl;
	while (true) {
		if (_kbhit()) {
			return 0;
		}
	}
}