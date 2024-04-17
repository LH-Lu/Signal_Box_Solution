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

// Check if user input matches map key (i.e signal name)
bool CheckInput(std::string UserInput, std::unordered_map<std::string, SigPara>& signals) {
	if (UserInput == "END") {
		std::cout << "> Ending program!\n\n";
		return false;
	}

	if (UserInput == "RST") {
		std::cout << "> Resetting all signals and switch points back to default state! \n\n";
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



// Resets all signals back to default state
void ResetState(std::unordered_map<std::string, SigPara>& signals) {
	
	std::unordered_map<std::string, SigPara>::iterator itr;
	itr = signals.begin();

	for (itr = signals.begin(); itr != signals.end(); itr++) {
		itr->second.state = false;
	}

}