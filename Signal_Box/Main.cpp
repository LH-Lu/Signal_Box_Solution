#include"AllTheFunctions.h"

// struct class is placed in the function header file and not here --> prevent duplication and errors.
/* struct SigPara{
		bool state;
		std::string TrueStateReq; // req to switch to true state
		std::string FalseStateReq; // req to switch to false state
		int LocationPx; // max location val 70px. Only at intervals of 10px. Positive values for UP line, Negative values for DOWN line
	};

	enum SigDirection { UP = 0, DOWN };
*/

int main() {

	// Initialise signals. [EDIT THIS SECTION]
	std::unordered_map<std::string, SigPara> signal;

	// Different sets of conditions separated by '||' e.g cond 1.1, 1.2, 1.3 || 2.1, 2.2, 2.3 ... 1.1 to 1.3 are one set of conditions and 2.1 to 2.3 are another set of mutaully exclusive conditions
	// As long as one set of condition is satisfied, the signal state will change
	// Note, Switch Point conditions to necessitate aspect signal change must be placed in the front of the condition set as shown below
	// 
	// UP aspect signals (T11 and T22 omitted as they'll be interlocked with their respective pairs T01 and T02)
	signal["Y01"] = { false, "T01F, T02F, R01T || T01T, T02F, R01T",	   "T01F, T02F, 0000 || T01T, T02F, 0000", 10 };
	signal["R01"] = { false, "T01F, T02F, R02T || T01T, T02F, R04F, Y03F", "T01F, T02F, Y01F || T01T, T02F, Y01F", 20 };
	signal["R02"] = { false, "T01F, T02F, 0000 || T01F, T02T, R05F",	   "T01F, T02F, R01F || T01F, T02T, 0000", 70 };
	// DOWN aspect signals
	signal["Y03"] = { false, "T11F, T22F, R04T || T11T, T22F, R04T",	   "T11F, T22F, 0000 || T11T, T22F, 0000", -70 };
	signal["R04"] = { false, "T11F, T22F, R05T || T11T, T22F, R01F, Y01F", "T11F, T22F, Y03F || T11T, T22F, Y03F", -60 };
	signal["R05"] = { false, "T11F, T22F, 0000 || T11F, T22T, R02F",	   "T11F, T22F, R04F || T11F, T22T, 0000", -10 };

	// SWITCH POINT pair 1 (everything has to be in default state in order to change switch points) Note, switch pairs interlocked tgt --> if one is in true state, the other will also be in true state
	signal["T01"] = { false, "T02F, T22F, Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", 30 };
	signal["T11"] = { false, "T02F, T22F, Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", -50 };
	// SWITCH POINT pair 2
	signal["T02"] = { false, "T01F, T11F, Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", 50 };
	signal["T22"] = { false, "T01F, T11F, Y01F, R01F, R02F, Y03F, R04F, R05F", "Y01F, R01F, R02F, Y03F, R04F, R05F", -30 };

	
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