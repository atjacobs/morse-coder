#include <stdint.h>
#include <string.h>
#include "morse.h"


typedef struct {
	unsigned short sequence; // 0 for dot, 1 for dash.  In order of ascending bits.
	unsigned short length;
} morse_code;

morse_code morse_codes[36] = {
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
	{11,4}, // Q
	{2, 3}, // R
	{0, 3}, // S
	{1, 1}, // T
	{4, 3}, // U
	{8, 4}, // V
	{6, 3}, // W
	{9, 4}, // X
	{13,4}, // Y
	{3, 4}, // Z
	{31,5}, // 0
	{30,5}, // 1
	{28,5}, // 2
	{24,5}, // 3
	{16,5}, // 4
	{0, 5}, // 5
	{1, 5}, // 6
	{3, 5}, // 7
	{7, 5}, // 8
	{15,5}, // 9
};

static char term = '\0';
static char *messageIdx = &term;
static morse_state_t state;

static short morse_index(char c)
{
	if((c >= 'a') && (c <= 'z')){
		return c - 'a';
	}else if((c >= 'A') && (c <= 'Z')){
		return c - 'A';
	}else if((c >= '0') && (c <= '9')){
		return c - '0' + 26;
	}
	return -1;
}

void morse_init(char *message){
	state = MORSE_IDLE;
	messageIdx = message;
}

// works the state machine
morse_state_t morse_state(void){
	static short bit_position;
	static char current_code;
	static unsigned int current_code_length;
	uint8_t code_idx;
	switch (state){
		case MORSE_IDLE:
			bit_position = 0;
nextcharfromidle:
			if( *messageIdx == ' '){
				state = MORSE_SPACE_WORD;
				messageIdx++;
				break;
			}
			if (* messageIdx == 0){
				break;
			}
			code_idx = morse_index(*messageIdx);
			if( code_idx < 0 ){
				messageIdx++;
				goto nextcharfromidle;
			}
			current_code = morse_codes[code_idx].sequence;
			current_code_length = morse_codes[code_idx].length;
			state = current_code & 1 ? MORSE_DASH : MORSE_DOT;
			break;
		case MORSE_SPACE_WORD:
		if (* messageIdx == 0){
			state = MORSE_IDLE;
			break;
		}
nextcharfromwordspace:
			messageIdx++;
			if ((* messageIdx == 0) || ( *messageIdx == ' ')){
				state = MORSE_SPACE_WORD;
				break;
			}
			if ((code_idx = morse_index(*messageIdx)) < 0){
				goto nextcharfromwordspace;
			}
			bit_position = -1;
			current_code = morse_codes[code_idx].sequence;
			current_code_length = morse_codes[code_idx].length;
			// fall through to dot or dash.
		case MORSE_DASH:
		case MORSE_DOT:
			bit_position++;
			if( bit_position >= current_code_length ){
				bit_position = 0;
nextcharfromdotdash:
				messageIdx++;
				if( *messageIdx == 0 ){
					state = MORSE_SPACE_WORD; // TODO: make this MORSE_SPACE_WORD
				}else if( *messageIdx == ' ' ){
					state = MORSE_SPACE_WORD;
				}else if( (code_idx = morse_index(*messageIdx) ) < 0 ) {
					goto nextcharfromdotdash;
				}else{
					state = MORSE_SPACE_DASH;
				}
				break;
			}
			state = MORSE_SPACE_DOT;
			break;
		case MORSE_SPACE_DASH:
			code_idx = morse_index(*messageIdx);
			current_code = morse_codes[code_idx].sequence;
			current_code_length = morse_codes[code_idx].length;
			state = current_code & 1 ? MORSE_DASH : MORSE_DOT;
			break;
		case MORSE_SPACE_DOT:
			if((current_code >> bit_position) & 1){
				state = MORSE_DASH;
			}else{
				state = MORSE_DOT;
			}
			break;
	}
	return state;
}