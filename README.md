A C++ application that helps visualise railway interlocking.
The application models a simple crossover point between a section of UP and DOWN track.
Users can change the interlocking conditions to see its effect or to test their logic thinking.

// OVERVIEW OF RAILWAY SIGNALLING //

In order to understand railway interlocking (and this application), you need to have a brief understand railway signalling.

This application is modelled after UK semaphore signals, not modern day multi-aspect signals. Each signal only has two states - True (proceed) or False (slow down / stop).

These signals are usually placed around a signal box whose signaller is in-charge of a section of track. This application is modelled after a very basic crossover section between one UP line and one DOWN line.
For the purpose of this overview, UP and DOWN just means opposing directions of travel.

For simplicity, only two common types of aspect signals are included - a Warning and a Stop signal.

A Warning aspect signal displays either a CAUTION (yellow) signal or a GO (green) signal. If the signal is at CAUTION, this means the preceeding signal may be another CAUTION or a STOP, so trains should slow down.

A Stop aspect signal displays either a STOP (red) signal or a GO (green) signal. If the signal is at STOP, the train has to...well...stop.

// OVERVIEW OF RAILWAY INTERLOCKING //

On the railways, interlocking is a method used to prevent conflicting movements or changes to the signals based on the state of the track points.

For example, if the UP line requires a crossover to the DOWN line, when the UP line signals are GO, the DOWN line signals cannot be changed to GO.
Otherwise, there would be a conflict as two trains would now be allowed to travel in opposite directions on the same track!

Another use of interlocking is to ensure the safe deployment of signals by forcing signals to be changed in a certain order. 
For example, a Warning aspect signal cannot be changed from CAUTION to GO unless the preceding Stop signal, which the former is dependent on, has been set to GO. 
This is logical as a CAUTION signal is meant to slow trains down in preparation for a stop. It will be very dangerous for a train to travel at line speed to suddenly hit a STOP (red) signal without any prior warning.

Interlocking is also used to ensure the correct pairs or sets of switch points are changed so as to prevent conflicting pathing of trains.

// USING THIS PROGRAM //

Using this program is very simple. A basic diagram of the section of track is displayed on the console, with infomration of the signals and switch points located above or below an *.

The signal or swtich point code / identifier are located on top (R01, T11, Y03 etc). Signals starting with Y are Warning aspect signals, R are Stop signals and T are switch points.

For Warning and Stop signals, they each have unqiue numbers. For switch points, a pair of signals is denoted by having the same digits (for example, T01 and T11 are switch point pairs as they share the common digit 1).

Below it are the state the signal / switch points are in. R means STOP (default false state), Y means WARNING (default false state), G means GO, - means the switch points point straight (default false state) and / means the swtich points are in turnout state.

Next to the state is an arrow head > or < denoting the direction of travel the signal is intended for.

To change the signal state, just quote the signal code / identifier when prompted and the program will change the signal / switch states if all requirements have been met.

To reset everything to their default positions (i.e default false state), just enter "RST". 

To quit the program, just enter "END".

// EDITING STATE REQUIREMENTS //

State requirements are the set of conditions that has to be fulfilled in order for the signal or switch point to change states.

The signals are kept in the memory using C++ unordered maps with the key being a struct data type. The struct data type stores (in the following order) the signal state [bool variable], the true state req [str variable], the false state req [str variable], signal location [int variable].

By default, the signal state is false. The default false states for Warning aspect signals, Stop aspect signals and switch points are WARNING, STOP and straight respectively. The true states for aspect signals and switch points are GO and turnout respectively.

True state req and false state req denote the state requirements to change a signal / switch state from false to true and true to false respectively.

Signal location denotes the location of the signal in the basic diagram. This must be in multiples of 10 and less than the value of 71 for the program to print out the signal. Positive values denote signals for the UP track. Negative values denote signals for the DOWN track.

For the state req, there is a fixed naming convention that must be strictly adhered to. This is repeated in the comments inside the source code.

Different sets of conditions separated by ' || ' (|| surrounaded by a whitespace on each side). As long as one set of condition is satisfied, the signal state will change. Note, Switch Point conditions to necessitate aspect signal change must be placed in the front of the condition set.

Naming convention for state req: 4 CHAR -> X00R

 X: Denotes type of signal. Y for warning / caution aspect. R for stop aspect. T for switch points

00: Two digit signal index. Unique numbers for aspect signals. Common digits for switch pairs (e.g T01 and T11 are switch pair). Pairs will change states together (e.g If T01 change state, T11 will also change)

 R: Required state of the signal in order for the requested signal to change state (e.g R01T means stop aspect signal R01 must be in true state in order for requested signal to change state).
 T denotes True state (GREEN for aspect signals, TURNOUT state for switch points). F denotes False state (YELLOW / RED for aspect signals, STRAIGHT state for switch points)
 
Note, 0000 for state req --> No pre-requisites needed to change requested signal state for that particular condition set

Each state req are separated by ', ' (comma followed by a whitespace)

To edit the signal state requirements, go to Main.cpp and search for the EDITABLE AREA. 

The variables are in the format: signal["3 char Signal Code"] = { false, "True state req condition sets", "False state req condition sets", Signal Location };

For example: signal["Y01"] = { false, "T01F, T02F, R01T || T01T, T02F, R01T", "T01F, T02F, 0000 || T01T, T02F, 0000", 10 };

T01F, T02F, R01T are one condition set while T01T, T02F, R01T are another condition set in order to change signal Y01 from false to true state.
