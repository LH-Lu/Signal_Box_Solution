#include<iostream>
#include<string>
#include<unordered_map>
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
	for (idx = 0; idx < MAX; idx++){
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



// Check if user input matches map key (i.e signal name)
bool CheckInput(std::string UserInput, std::unordered_map<std::string, SigPara>& signals) {
	if (UserInput == "END") {
		std::cout << "> Ending program!\n\n";
		return false;
	}
	
	if (signals.find(UserInput) == signals.end()) {
		// UserInput not found
		std::cout << "> Input not found!\n\n";
		return false;
	}

	// UserInput found
	std::cout << "> Signal found.\n";
	return true;
}



// Check if Signal Req.s met to change signal(interlocking function)
void InterLocking(std::string UserInput, std::unordered_map<std::string, SigPara>& signals) {

	bool InterLockCheck = false;

	// Check current state to determine which signal state user wants to switch
	switch (signals[UserInput].state){
		case false: // User wants to switch signal false -> true
			InterLockCheck = CheckSignalReq(signals, UserInput, signals[UserInput].TrueStateReq);
			ChangeSignal(signals, InterLockCheck, UserInput);
			break;

		case true: // User wants to switch signal true -> false
			InterLockCheck = CheckSignalReq(signals, UserInput, signals[UserInput].FalseStateReq);
			ChangeSignal(signals, InterLockCheck, UserInput);
			break;
	}
}

// Check if requirements for signal to change state are met
bool CheckSignalReq(std::unordered_map<std::string, SigPara>& signals, std::string UserInput, std::string StateReq) {

	// Declare variables
	int idx;
	int ConditionSetNum;
	bool ReturnReqCheck = true;
	bool IdvReqCheck = false;
	std::string IdvStateReq;

	switch (UserInput[0]) {
	case 'T':
		// Switch point
		for (idx = 0; idx < StateReq.length(); idx += 6) {

			IdvStateReq = StateReq.substr(idx, idx + 4);
			IdvReqCheck = CheckSignal(signals, IdvStateReq);

			if (IdvReqCheck == false) {
				ReturnReqCheck = false;
			}
		}
		break;


	case 'Y':
	case 'R':
		// Aspect signal

		// Check switch point condition as this will dictate what conditions need to be met and whetehr to skip to next set of conditions
		

		// Null case, signal has no req to change state
		if (StateReq == "0000") {
			std::cout << "> Signal change no requirement. \n";
			return true;
		}


		IdvReqCheck = CheckSignal(signals, IdvStateReq);

		if (IdvReqCheck == false) {
			ReturnReqCheck = false;
		}
	}

	return ReturnReqCheck;
}




// Check switch point condition and return Condition Set Number in C++ array idx style based on the state of the switch points (different state of the switch points --> different conditional sets)
// Based on the state of switch points --> conditional set --> determines what set of conditions must be fulfilled to change the state of the aspect signal
int CheckSwitchCondition(std::unordered_map<std::string, SigPara>& signals, std::string StateReq) {

	// Declare variables
	int idx;
	int ConditionIdx = 0;
	bool IdvReqCheck = false;
	bool SwitchCond = true;
	std::string IdvStateReq;

	// Find which condition set has been fulfilled for each aspect signal
	// Note, Switch conditions are ALWAYS placed at the start of each condition set
	std::cout << "> Switch condition set #" << ConditionIdx + 1 << ":" << std::endl;
	for (idx = 0; idx < StateReq.length(); idx += 6) {
		
		IdvStateReq = StateReq.substr(idx, idx + 4);

		if (IdvStateReq[0] == 'T') {
			IdvReqCheck = CheckSignal(signals, IdvStateReq);
			if (IdvReqCheck == false) {
				SwitchCond = false;
			}
		}
		else { 
			if (SwitchCond == true) {
				std::cout << "> Switch condition set #" << ConditionIdx + 1 << " FULFILLED." << std::endl;
				return ConditionIdx;
			}

						
			while (StateReq[idx] != '|') {
				idx++;
				if (idx > StateReq.length()) {
					std::cout << "> All switch condition sets UNFULFILLED." << std::endl;
					return -1; // reached end of state req string --> no conditions fulfulled
				}
			}
			idx -= 3;
			SwitchCond = true;
			ConditionIdx++;
			std::cout << "> Switch condition set #" << ConditionIdx + 1 << ":" << std::endl;
		}

	}
	std::cout << "> All switch condition sets UNFULFILLED." << std::endl;
	return -1; // reached end of state req string --> no conditions fulfulled
}

// Check if EACH signal requirement has been met
bool CheckSignal(std::unordered_map<std::string, SigPara>& signals, std::string IdvStateReq) {
	// For signal with ONE req only
	// Get State Requirement type for req signal (i.e shld the req signal be TRUE or FALSE)
	bool StateReqType = false;
	switch (IdvStateReq[3]) {
	case 'T':
		StateReqType = true;
		break;
	case 'F':
		StateReqType = false;
		break;
	}

	// Get part of IdvStateReq string with signal identifier
	std::string ReqSignal = IdvStateReq.substr(0, 3);

	bool CurrentSignalState = signals[ReqSignal].state;
	signals[ReqSignal].state = StateReqType;
	std::cout << "> Signal change requirement SIGNAL: " << ReqSignal << ". REQ STATE: " << SignalOutput(signals, ReqSignal) << ". REQ SATISFIED: ";
	signals[ReqSignal].state = CurrentSignalState;
	
	
	// Signal fulfilled required state for requested signal to change state
	if (StateReqType == CurrentSignalState) {
		std::cout << "YES. \n";
		return true;
	}

	// Signal DID NOT fulfill required state for requested signal to change state
	std::cout << "NO. \n";
	return false;
}

// Change signal state if the check is true and display what state the signal changed to
void ChangeSignal(std::unordered_map<std::string, SigPara>& signals, bool InterLockCheck, std::string UserInput) {
	
	if (InterLockCheck == false) {
		std::cout << "> SIGNAL " << UserInput << " not changed. Current state: " << SignalOutput(signals, UserInput) << ".\n\n";
		return;
	}

	std::cout << "> SIGNAL " << UserInput << " changed. State changed " << SignalOutput(signals, UserInput) << " -> ";
	signals[UserInput].state = !signals[UserInput].state; //flip bool variable from true to false and vice versa
	std::cout << SignalOutput(signals, UserInput) << ".\n\n";

	// For switch point pairs, if one has been selected to change and changes successfully, the other will change too
	if (UserInput[0] == 'T') {
		int UserInputSwitch = std::stoi(UserInput.substr(1, 2));
		int SwitchPairOfUserInputSwitch;
		std::string SwitchPair;

		if (UserInputSwitch / 10 == 0) {
			SwitchPairOfUserInputSwitch = (UserInputSwitch % 10) * 11;
			SwitchPair = 'T' + std::to_string(SwitchPairOfUserInputSwitch);
		}
		else if (UserInputSwitch / 10 == UserInputSwitch % 10) {
			SwitchPairOfUserInputSwitch = (UserInputSwitch % 10);
			SwitchPair = "T0" + std::to_string(SwitchPairOfUserInputSwitch);
		}

		std::cout << "> SIGNAL " << SwitchPair << " changed. State changed " << SignalOutput(signals, SwitchPair) << " -> ";
		signals[SwitchPair].state = !signals[SwitchPair].state; //flip bool variable from true to false and vice versa
		std::cout << SignalOutput(signals, SwitchPair) << ".\n\n";
	}
}