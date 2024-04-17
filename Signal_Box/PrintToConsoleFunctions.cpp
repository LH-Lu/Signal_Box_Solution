#include"AllTheFunctions.h"

// struct class is placed in the function header file and not here --> prevent duplication and errors.
/* struct SigPara{
		bool state;
		std::string TrueStateReq;
		std::string FalseStateReq;
		int LocationPx; // max location val 70px. Only at intervals of 10px. Positive values for UP line, Negative values for DOWN line
	};

	enum SigDirection { UP = 0, DOWN };
*/

// NOTE: With the exception of SignalStatusPrint, if std::unordered_map para is: FIRST argument --> function called privately; SECOND arguement --> function called directly from main function. 

// Prints section break
void PrintSectionBreak() {
	int idx;
	const int MAX = 31;
	for (idx = 0; idx < MAX; idx++) {
		std::cout << "=";
	}
	std::cout << " NEXT SECTION ";
	for (idx = 0; idx < MAX;idx++) {
		std::cout << "=";
	}
	std::cout << "\n\n";
}



// Prints signal location and state along the line
void SignalStatusPrint(std::unordered_map<std::string, SigPara>& signals) {

	const int MaxDist = 71;

	PrintSignalLocation(signals, UP, MaxDist);
	PrintSignalState(signals, UP, MaxDist);


	std::cout << " ->  ----*---------*---------*---------*---------*---------*---------*  -> UP" << std::endl; // main line. 70 char from start to last dash
	std::cout << "                              \\ ______________ /                           " << std::endl; // to print \ char, need to use \\ designation
	std::cout << "                              /                 \\                          " << std::endl;
	std::cout << " <-  ----*---------*---------*---------*---------*---------*---------*  <- DOWN" << std::endl;

	PrintSignalLocation(signals, DOWN, MaxDist);
	PrintSignalState(signals, DOWN, MaxDist);

	std::cout << std::endl;
}

// Prints signal information at the desired location
void PrintSignalLocation(std::unordered_map<std::string, SigPara>& signals, SigDirection Direction, const int MaxDist) {

	std::unordered_map<std::string, SigPara>::iterator itr;
	itr = signals.begin(); // note maps.end() points to the address just past the last element of the map
	int idx;
	bool check = false;

	for (idx = 1; idx < MaxDist; idx++) {
		if (idx % 10 != 0) {
			std::cout << " ";
		}
		else {
			for (itr = signals.begin(); itr != signals.end(); itr++) {
				check = false;
				if (Direction == UP && idx == itr->second.LocationPx) {
					std::cout << itr->first;
					idx += 2;
					check = true;
					break;
				}
				if (Direction == DOWN && idx == (-1 * itr->second.LocationPx)) {
					std::cout << itr->first;
					idx += 2;
					check = true;
					break;
				}
			}
			if (check == false) {
				std::cout << " ";
			}
		}
	}
	std::cout << std::endl;
}

// Prints signal state at the desired location
void PrintSignalState(std::unordered_map<std::string, SigPara>& signals, SigDirection Direction, const int MaxDist) {

	std::unordered_map<std::string, SigPara>::iterator itr;
	itr = signals.begin(); // note maps.end() points to the address just past the last element of the map
	int idx;
	bool check = false;

	for (idx = 1; idx < MaxDist; idx++) {
		if (idx % 10 != 0) {
			std::cout << " ";
		}
		else {
			for (itr = signals.begin(); itr != signals.end(); itr++) {
				check = false;
				if (Direction == UP && idx == itr->second.LocationPx) {
					std::cout << SignalOutput(signals, itr->first) << " >";
					idx += 2;
					check = true;
					break;
				}
				if (Direction == DOWN && idx == (-1 * itr->second.LocationPx)) {
					std::cout << SignalOutput(signals, itr->first) << " <";
					idx += 2;
					check = true;
					break;
				}
			}
			if (check == false) {
				std::cout << " ";
			}
		}
	}
	std::cout << std::endl;
}

// Check state of signal and prints out correct char
char SignalOutput(std::unordered_map<std::string, SigPara>& signals, std::string SignalMapKey) {

	switch (SignalMapKey[0]) {
	case 'Y':
		if (signals[SignalMapKey].state == true) {
			return 'G';
		}
		return 'Y';
		break;

	case 'R':
		if (signals[SignalMapKey].state == true) {
			return 'G';
		}
		return 'R';
		break;

	case 'T':
		if (signals[SignalMapKey].state == true) {
			return '/';
		}
		return '-';
		break;
	}
}