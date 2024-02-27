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

int main() {
	/*
	TO DO:
	1) Create condition sets based on switch point signals
	2) Modify interlocker function to accept condition sets (as long as one set of condition is true, change signal state)
	*/

	// test

	// Initialise signals. [EDIT THIS SECTION]
	std::unordered_map<std::string, SigPara> signal;
	// Different sets of conditions separated by & e.g cond 1.1, 1.2, 1.3& 2.1, 2.2, 2.3 ... 1.1 to 1.3 are one set of conditions and 2.1 to 2.3 are another set of conditions
	// As long as one set of condition is satisfied, the signal state will change
	// UP aspect signals (Different sets of conditions are sep)
	signal["Y01"] = { false, "R01T", "0000", 10 };
	signal["R01"] = { false, "R02T", "Y01F", 20 };
	signal["R02"] = { false, "0000", "R01F", 70 };
	// DOWN aspect signals
	signal["Y03"] = { false, "R04T", "0000", -70 };
	signal["R04"] = { false, "R05T", "Y03F", -60 };
	signal["R05"] = { false, "0000", "R04F", -10 };

	// SWITCH POINT pair 1 (everything has to be in default state in order to change switch points) 
	// if none of the switch points activated, i.e default state, standard signalling interlocking possible--> 
	// if switch points activated --> special set of interlocking
	signal["T01"] = { false, "Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", 30 };
	signal["T02"] = { false, "Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", -30 };
	// SWITCH POINT pair 2
	//signal["T03"] = { false, "Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", 50 };
	//signal["T04"] = { false, "Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", -50 };

	
	// Initialise user input and case matching
	std::string SignalUserInput = "000";
	bool SignalMatch = false;

	// Display initial state of line
	SignalStatusPrint(signal);

	while (SignalUserInput != "END") {

		// Enter signal key and check if user input match with signal identifier. Exits program if "END" entered
		std::cout << "Enter 3 character signal code ('END' to quit): ";
		std::getline(std::cin, SignalUserInput);
		std::cout << "Status:" << std::endl;
		SignalMatch = CheckInput(SignalUserInput, signal);
		if (SignalUserInput == "END") {
			break;
		}

		// Check if all pre-requisite match to change signal state then change signal state if pre-req match
		if (SignalMatch == true) {
			InterLocking(SignalUserInput, signal);
		}
		
		SignalMatch = false;

		// Display state of line
		SignalStatusPrint(signal);

		// Display section break
		PrintSectionBreak();
	}
	

	return 0;
}