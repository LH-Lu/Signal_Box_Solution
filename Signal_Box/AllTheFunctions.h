#pragma once

struct SigPara {
	bool state;
	std::string TrueStateReq;
	std::string FalseStateReq;
	int LocationPx;
};

enum SigDirection { UP = 0, DOWN };


// Prints section break
void PrintSectionBreak();

// Prints signal state along the line
void SignalStatusPrint(std::unordered_map<std::string, SigPara>& signals);

// Prints signal information at the desired location
void PrintSignalLocation(std::unordered_map<std::string, SigPara>& signals, SigDirection Direction, const int MaxDist);

// Prints signal state at the desired location
void PrintSignalState(std::unordered_map<std::string, SigPara>& signals, SigDirection Direction, const int MaxDist);

// Check state of signal and prints out correct char
char SignalOutput(std::unordered_map<std::string, SigPara>& signals, std::string SignalMapKey);

// Check if user input matches map key (i.e signal name)
bool CheckInput(std::string UserInput, std::unordered_map<std::string, SigPara>& Signals);

// Check if Signal Req.s met to change signal(interlocking function)
void InterLocking(std::string UserInput, std::unordered_map<std::string, SigPara>& signals);

// Check if requirements for signal to change state are met
bool CheckSignalReq(std::unordered_map<std::string, SigPara>& signals, std::string UserInput, std::string StateReq);

// Check switch point conditionand return Condition Set Number in C++ array idx style based on the state of the switch points(different state of the switch points-- > different conditional sets)
// Based on the state of switch points --> know the conditional set to focus on --> determines what set of conditions must be fulfilled to change the state of the aspect signal
int CheckSwitchCondition(std::unordered_map<std::string, SigPara>& signals, std::string StateReq);

// Check if EACH signal requirement has been met
bool CheckSignal(std::unordered_map<std::string, SigPara>& signals, std::string IdvStateReq);

// Change signal state if the check is true and display what state the signal changed to
void ChangeSignal(std::unordered_map<std::string, SigPara>& signals, bool check, std::string UserInput);