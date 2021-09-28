#include <stdint.h>
#include <string.h>
#include "morse.h"


typedef struct {
	char sequence : 5; // 0 for dot, 1 for dash.  In order of ascending bits.
	unsigned int length: 3;
} morse_code;

morse_code morse_codes[26] = {
	{2, 2}, // A 
	{1, 4}, // B
	{3, 4}, // C
	{1, 3}, // D
	{0, 1}, // E
	{4, 4}, // F
	{3, 3}, // G
	{0, 4}, // H
	{0, 2}, // I
	{14,4}, // J
	{5, 3}, // K
	{2, 4}, // L
	{3, 2}, // M
	{1, 2}, // N
	{7, 3}, // O
	{6, 4}, // P
	{11,4}, // R
	{0, 3}, // S
	{1, 1}, // T
	{4, 3}, // U
	{8, 4}, // V
	{6, 3}, // W
	{9, 4}, // X
	{13,4}, // Y
	{3, 4}, // Z
};

static char term = '\0';
static char *messageIdx = &term;
static morse_state_t state;

void morse_init(char *message){
	state = MORSE_IDLE;
	messageIdx = message;
}

// works the state machine
morse_state_t morse_state(void){
	static unsigned short bit_position;
	static char current_code;
	static unsigned int current_code_length;
	uint8_t code_idx;
	switch (state){
		case MORSE_IDLE:
			// are there any letters in the queue?
			// Immediately assume next state.
			bit_position = 0;
			// TODO: handle lower case, spaces
			if( *messageIdx == ' '){
				state = MORSE_SPACE_CHAR;
				messageIdx++;
				break;
			}
			if (* messageIdx == 0){
				break;
			}
			code_idx = *messageIdx - 'A';
			current_code = morse_codes[code_idx].sequence;
			current_code_length = morse_codes[code_idx].length;
			state = current_code & 1 ? MORSE_DASH : MORSE_DOT;
			break;
		case MORSE_DASH:
		case MORSE_DOT:
			bit_position++;
			if( bit_position >= current_code_length ){
				bit_position = 0;
				messageIdx++;
				if( *messageIdx == 0 ){
					state = MORSE_IDLE;
				}else if( *messageIdx == ' ' ){
					state = MORSE_SPACE_CHAR;
				}else {
					state = MORSE_SPACE_DASH;
				}
				break;
			}
			if((current_code >> bit_position) & 1){
				state = MORSE_DASH;
			}else{
				state = MORSE_DOT;
			}
			break;
		case MORSE_SPACE_CHAR:
			messageIdx++;
			if( *messageIdx == ' '){
				state = MORSE_SPACE_CHAR;
				messageIdx++;
				break;
			}
			if (* messageIdx == 0){
				break;
			}
		case MORSE_SPACE_DASH:
			code_idx = *messageIdx - 'A';
			current_code = morse_codes[code_idx].sequence;
			current_code_length = morse_codes[code_idx].length;
			state = current_code & 1 ? MORSE_DASH : MORSE_DOT;
			break;
	}
	return state;
}