#ifndef MORSE_H
#define MORSE_H

typedef enum {
	MORSE_SPACE,
	MORSE_SPACE_DASH,
	MORSE_SPACE_CHAR,
	MORSE_DASH,
	MORSE_DOT,
	MORSE_IDLE
} morse_state_t;

// Call morse_init to initialize the state machine
void morse_init(char *message);

// call this to actuate the state machine. 
morse_state_t morse_state(void);

#endif