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

// Check if Signal Req.s met to change signal(interlocking function)
void InterLocking(std::string UserInput, std::unordered_map<std::string, SigPara>& signals) {

	bool InterLockCheck = false;

	// Check current state to determine which signal state user wants to switch
	switch (signals[UserInput].state) {
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
	int idx2;
	int StartingIdx;
	int ConditionSetNum;
	int FalseFlag = 0;
	int ErrorFlag = 0;
	bool ReturnReqCheck = false;
	bool IdvReqCheck = false;
	std::string IdvStateReq;

	const int SpacingToNextSignalCondition = 6;
	const int SignalConditionStrLength = 4;

	switch (UserInput[0]) {
	case 'T':
		// Switch point
		for (idx = 0; idx < StateReq.length(); idx += SpacingToNextSignalCondition) {

			IdvStateReq = StateReq.substr(idx, SignalConditionStrLength);
			IdvReqCheck = CheckSignal(signals, IdvStateReq, SignalConditionStrLength);

			if (IdvReqCheck == false) {
				FalseFlag++;
			}
		}
		break;

	case 'Y':
	case 'R':
		// Aspect signal
		ConditionSetNum = CheckSwitchCondition(signals, StateReq, SpacingToNextSignalCondition, SignalConditionStrLength); // Check switch point condition number as this will dictate what conditions need to be met

		if (ConditionSetNum == -1) {
			FalseFlag++;
			break;
		}

		StartingIdx = 0;

		for (idx = 0; idx < StateReq.length(); idx++) { // Get starting idx of req condition set
			if (ConditionSetNum == 0) {
				break;
			}
			if (StateReq[StartingIdx] == '|') {
				StartingIdx += 2;
				ConditionSetNum--;
			}
			StartingIdx++;
		}

		for (idx = StartingIdx; idx < StateReq.length(); idx += SpacingToNextSignalCondition) { // Check if other aspect signal conditions met for given condition set dictated by the state of the switch points

			IdvStateReq = StateReq.substr(idx, SignalConditionStrLength);

			// Note, IF-statement runs if the condition is TRUE. ! inverts bool val from TRUE to FALSE and vice versa. 
			// (IdvStateReq[0] == 'T' ... ) is a bool val that evaluates TRUE whenever 'T', 'Y', or 'R' are the first char
			if (!(IdvStateReq[0] == 'T' || IdvStateReq[0] == 'R' || IdvStateReq[0] == 'Y' || IdvStateReq[0] == '0')) {
				// Check if reached the end of the condition set. If it does not, most likely an errenous signal code entered --> fail safe
				for (idx2 = 0; idx2 < IdvStateReq.length(); idx2++) {
					if (IdvStateReq[idx2] == '|') {
						ErrorFlag = 0;
						break; // From inner for loop
					}
					ErrorFlag++;
				}
				if (ErrorFlag != 0) {
					std::cout << "> Errenous signal index / code detected in state requirements. No signals changed.\n";
				}

				break;
			}

			if (IdvStateReq[0] == 'T') {
				continue; // Proceed to next itr
			}

			if (IdvStateReq == "0000") {
				std::cout << "> For switch point condition, signal change has no requirements. \n";
				return true;
			}

			IdvReqCheck = CheckSignal(signals, IdvStateReq, SignalConditionStrLength);

			if (IdvReqCheck == false) {
				FalseFlag++;
			}
		}
		break;
	}

	if (FalseFlag == 0 && ErrorFlag == 0) {
		ReturnReqCheck = true;
	}

	return ReturnReqCheck;
}

// Check switch point condition and return Condition Set Number in C++ array idx style based on the state of the switch points (different state of the switch points --> different conditional sets)
// Based on the state of switch points --> know the conditional set to focus on --> determines what set of conditions must be fulfilled to change the state of the aspect signal
int CheckSwitchCondition(std::unordered_map<std::string, SigPara>& signals, std::string StateReq, const int SpacingToNextSignalCondition, const int SignalConditionStrLength) {

	// Declare variables
	int idx;
	int ConditionIdx = 0;
	bool IdvReqCheck = false;
	bool SwitchCond = true;
	std::string IdvStateReq;

	// Find which condition set has been fulfilled for each aspect signal
	// Note, Switch conditions are ALWAYS placed at the start of each condition set
	std::cout << "> Switch condition set #" << ConditionIdx + 1 << ":" << std::endl;
	for (idx = 0; idx < StateReq.length(); idx += SpacingToNextSignalCondition) {

		IdvStateReq = StateReq.substr(idx, SignalConditionStrLength);

		if (IdvStateReq[0] == 'T') {
			IdvReqCheck = CheckSignal(signals, IdvStateReq, SignalConditionStrLength);
			if (IdvReqCheck == false) {
				SwitchCond = false;
			}
		}
		else {
			if (SwitchCond == true) {
				std::cout << "> Switch condition set #" << ConditionIdx + 1 << " FULFILLED." << std::endl << std::endl;
				return ConditionIdx;
			}


			while (StateReq[idx] != '|') {
				idx++;
				if (idx > StateReq.length()) {
					std::cout << "> All switch condition sets UNFULFILLED." << std::endl << std::endl;
					return -1; // reached end of state req string --> no conditions fulfulled
				}
			}
			idx -= (SpacingToNextSignalCondition - 3);
			IdvReqCheck = false;
			SwitchCond = true;
			ConditionIdx++;
			std::cout << "> Switch condition set #" << ConditionIdx + 1 << ":" << std::endl;
		}

	}
	std::cout << "> All switch condition sets UNFULFILLED." << std::endl << std::endl;
	return -1; // reached end of state req string --> no conditions fulfulled
}

// Check if EACH signal requirement has been met
bool CheckSignal(std::unordered_map<std::string, SigPara>& signals, std::string IdvStateReq, const int SignalConditionStrLength) {
	// Get State Requirement type for req signal (i.e shld the req signal be TRUE or FALSE)
	bool RequiredSignalState = false;
	switch (IdvStateReq[SignalConditionStrLength - 1]) {
	case 'T':
		RequiredSignalState = true;
		break;
	case 'F':
		RequiredSignalState = false;
		break;
	default:
		std::cout << "> Unable to verify required signal state" << std::endl;
		return false;
	}

	// Get part of IdvStateReq string with signal identifier
	std::string ReqSignal = IdvStateReq.substr(0, SignalConditionStrLength - 1);

	bool CurrentSignalState = signals[ReqSignal].state;
	signals[ReqSignal].state = RequiredSignalState;
	std::cout << "> Signal change requirement SIGNAL: " << ReqSignal << ". REQ STATE: " << SignalOutput(signals, ReqSignal) << ". REQ SATISFIED: ";
	signals[ReqSignal].state = CurrentSignalState;


	// Signal fulfilled required state for requested signal to change state
	if (RequiredSignalState == CurrentSignalState) {
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
	std::cout << SignalOutput(signals, UserInput) << ".\n";

	// For switch point pairs, if one has been selected to change and changes successfully, the other will change too
	

	if (UserInput[0] == 'T') {
		const int SpacingToNextSignalCondition = 6;
		const int SignalConditionStrLength = 4;
		int NumOfDigits = SignalConditionStrLength - 2; // Number of digits in the signal idx
		int UserInputSwitchIdx = std::stoi(UserInput.substr(1, NumOfDigits));
		int SwitchPairIdx;
		int UserInputSwitchIdxTemp = UserInputSwitchIdx;
		int NumOfNonZeroDigits = 0;
		int idx;
		std::string SwitchPair;

		// ** Note, for now, this only works for 2 digit signal idx. Therefore, the max possible is 9 switch pairs **

		while (UserInputSwitchIdxTemp != 0) { 
			UserInputSwitchIdxTemp /= 10;
			NumOfNonZeroDigits++;
		}

		SwitchPairIdx = (UserInputSwitchIdx % 10) * pow(11, (NumOfNonZeroDigits - 2) * -1);

		SwitchPair = "T";
		for (idx = NumOfNonZeroDigits; idx > 1; idx--) {
			SwitchPair += "0";
		}
		SwitchPair += std::to_string(SwitchPairIdx);

		std::cout << "> SIGNAL " << SwitchPair << " changed. State changed " << SignalOutput(signals, SwitchPair) << " -> ";
		signals[SwitchPair].state = !signals[SwitchPair].state; //flip bool variable from true to false and vice versa
		std::cout << SignalOutput(signals, SwitchPair) << ".\n";
	}

	std::cout << "\n";
}