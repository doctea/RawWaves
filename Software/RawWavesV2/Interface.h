#ifndef Interface_h
#define Interface_h

#include <Arduino.h>
#include <Bounce2.h>
#include "PlayState.h"
#include "Settings.h"
#include "AnalogInput.h"

using Button = Bounce2::Button;

#define CHAN_POT_PIN A0 	// pin for Station pot (aka Channel pot), was A9 in Teensy 3.2 version
#define CHAN_CV_PIN A1 		// pin for Station CV (aka Channel CV), was A6 in Teensy 3.2 version
#define TIME_POT_PIN A5 	// pin for Start pot (aka Time pot), was A7 in Teensy 3.2 version
#define TIME_CV_PIN A8 		// pin for Start CV (aka Time CV)
#define RESET_BUTTON 8 		// Reset button
#define RESET_CV 9 		    // Reset pulse input

#define TIME_POT_CHANGED 	1       //binary 000000000001
#define TIME_CV_CHANGED 	1 << 1  //binary 000000000010
#define CHANNEL_CHANGED 	1 << 2  //binary 000000000100
#define CHANGE_START_NOW 	1 << 3  //binary 000000001000
#define BUTTON_SHORT_PRESS 	1 << 4  //binary 000000010000
#define BUTTON_LONG_PRESS 	1 << 5  //binary 000000100000
#define BUTTON_LONG_RELEASE 1 << 6  //binary 000001000000
#define BUTTON_PULSE 		1 << 7  //binary 000010000000 Note: A BUTTON_PULSE is a long press that is held for longer than LONG_PRESS_DURATION
#define RESET_TRIGGERED 	1 << 8  //binary 000100000000
#define ROOT_CV_CHANGED		1 << 9  //binary 001000000000
#define ROOT_POT_CHANGED	1 << 10 //binary 010000000000
#define ROOT_NOTE_CHANGED	1 << 11 //binary 100000000000

#define SHORT_PRESS_DURATION 10
#define LONG_PRESS_DURATION 600
// after LONG_PRESS_DURATION every LONG_PRESS_PULSE_DELAY milliseconds the update
// function will set BUTTON_PULSE
#define LONG_PRESS_PULSE_DELAY 600

#define SAMPLEAVERAGE   16 	// How many values are read and averaged of pot/CV inputs each interface check.

class Interface {
public:
	boolean quantiseRootCV = true;
	boolean quantiseRootPot = true;

	float rootNoteCV = 36;
	float rootNotePot = 36;
	float rootNote = 36;

	//boolean buttonHeld = false;
	unsigned long start = 0;

	elapsedMillis buttonHoldTime;

	PlayState* playState;

	Interface() : channelCVInput(CHAN_CV_PIN),
			channelPotInput(CHAN_POT_PIN),
			startCVInput(TIME_CV_PIN),
			startPotInput(TIME_POT_PIN)
	{
		playState = NULL;
	}

	void init(int fileSize, int channels, const Settings& settings, PlayState* state);
	void setChannelCount(uint16_t count);

	uint16_t update();
	uint16_t updateButton();
private:
	AnalogInput channelCVInput;
	AnalogInput channelPotInput;
	AnalogInput startCVInput;
	AnalogInput startPotInput;

	Button resetButton;
	elapsedMillis buttonTimer = 0;

	uint16_t changes = 0;

	uint16_t startCVDivider = 1;
	boolean channelPotImmediate = true;
	boolean channelCVImmediate = true;
	boolean startPotImmediate = true;
	boolean startCVImmediate = true;

	uint16_t channelCount = 0;
	float rootNoteOld = -100;
	float rootNotePotOld = -100;
	float rootNoteCVOld = -100;

	uint16_t updateChannelControls();
	uint16_t updateStartControls();
	uint16_t updateRootControls();
	boolean pitchMode = false;
};

#endif
