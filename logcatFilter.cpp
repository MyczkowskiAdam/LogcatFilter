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
#include <errno.h>

#ifdef _WIN32
#include <conio.h>
#include <direct.h>
#define VERSION "1.5 Windows"
#define GetCWD _getcwd
char const *blacklist = "\"";
#else
#include <unistd.h>
#define VERSION "1.5   Linux"
#define GetCWD getcwd
char const *blacklist = "'";
#endif

int iPriority, counter;
std::string fileName, oFileName, ioFileName;
const std::string defaultFileName = "filtered-logcat", prefix = "input-", priority[] = {
	" V ",
	" D ",
	" I ",
	" W ",
	" E ",
	" F ",
	" S ",
	"denied"
};

int main() {
	std::cout << "___________________________________________" << std::endl;
	std::cout << "__________Welcome to LogcatFilter__________" << std::endl;
	std::cout << "_________By Adam Myczkowski (mycax)________" << std::endl;
	std::cout << "_____________Version: " << VERSION << "__________" << std::endl;
	std::cout << "___________________________________________" << std::endl;
	std::cout << "Enter file path here > " << std::flush;
	std::getline(std::cin, fileName);

	/*
	* When file is dragged and dropped into the terminal is being sorrounded with
	* the quotation marks. It's set as a value of fileName what will cause it fail
	* as it's non-existant. We need to strip fileName from those quotation marks.
	*/
	fileName.erase(std::remove(fileName.begin(), fileName.end(), blacklist[0]), fileName.end());

	/*
	* If uses wants to specify the output file name - use it, else use default file name
	* then append it .txt suffix.
	*/
	std::cout << "If you'd like to specificy output file name enter it here otherwise press enter > " << std::flush;
	std::getline(std::cin, ioFileName);
	oFileName = defaultFileName;
	if (ioFileName.length()>0) {
		oFileName = ioFileName;
	}
	oFileName.append(".txt");

	/*
	* If input file name has the same name and is under the same directory as the output file
	* rename input file adding 'input-' prefix. This is done checking if output file name
	* can be found inside of the input file name, then checking if it's under index position
	* 0 what means file name has been input in form of 'input.txt' (local directory) or
	* if current directory can be found in the input file name (when user inputs the name
	* in the form of '/home/user/foler/to/file.txt'). If one of those conditions is met
	* input file is being renamed with 'input-' prefix and fileName is being updated
	* as it has to read from a now different file.
	*/
	if (fileName.find(oFileName) != std::string::npos) {
		char cwd[FILENAME_MAX];
		if (GetCWD(cwd, sizeof(cwd))) {
			std::string scwd(cwd);
			if (fileName.find(oFileName) == 0 || fileName.find(scwd) != std::string::npos) {
				std::rename(const_cast<char*>(oFileName.c_str()), const_cast<char*>((fileName.insert(fileName.find(oFileName), prefix)).c_str()));
				fprintf(stderr,"WARNING: Input and output files have the same name and lay under the same directory, renaming input file from '%s' to 'input-%s'", 
					const_cast<char*>(oFileName.c_str()), const_cast<char*>(oFileName.c_str()));
			}
		}
		else {
			perror("getcwd() error");
		}
	}

	std::ifstream iLog(fileName.c_str());
	if (iLog.is_open()) {
		std::ofstream oLog(oFileName.c_str());

		std::cout << std::endl;
		std::cout << "V : Verbose         (1)" << std::endl;
		std::cout << "D : Debug           (2)" << std::endl;
		std::cout << "I : Info            (3)" << std::endl;
		std::cout << "W : Warning         (4)" << std::endl;
		std::cout << "E : Error           (5)" << std::endl;
		std::cout << "F : Fatal           (6)" << std::endl;
		std::cout << "S : Silent          (7)" << std::endl;
		std::cout << "Selinux Denials     (8)" << std::endl;
		std::cout << "Search for a phrase (9)" << std::endl;
		std::cout << std::endl;
		std::cout << "What do you want to filter? > " << std::flush;
		std::cin >> iPriority;

		/* Only execute if we input is a int and between 1-9 */
		while (iPriority > 9 || iPriority < 1 || std::cin.fail()) {
			std::cout << "Incorrect input, what do you want to filter? > " << std::flush;
			std::cin.clear();
			std::cin.ignore();
			std::cin >> iPriority;
		}

		/*
		* Search for the filter in the input file. Loop through each file of the
		* input file. If the line is matching it is being stored in iLine and 
		* written to the output textfile.
		*/
		std::string iLine;
		if (iPriority == 9) {
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
		}
		std::cout << "Found  " << counter << " matching lines." << std::endl;
		oLog << std::endl;
		oLog << "Found  " << counter << " matching lines." << std::endl;

		/* Always remember to close the files! */
		oLog.close();
		iLog.close();
	}
	else {
		fprintf(stderr, "Unable to open '%s': ", const_cast<char*>(fileName.c_str()));
		perror("");
	}

	/*
	* If we are on Windows use _kbhit() to wait for user interaction
	* in order to end the program as CMD closes straight after execution.
	* In Linux terminal stays open after execution so just end the program.
	*/
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
