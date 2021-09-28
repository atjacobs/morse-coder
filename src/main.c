#include <stdlib.h>
#include <stdio.h>
#include "morse.h"

int main(int argc, char *argv[]){
	morse_state_t state;
	for(int i = 1; i < argc; i++){
		morse_init(argv[i]);
		while((state = morse_state()) != MORSE_IDLE){
			switch (state){
				case MORSE_DOT:
					printf(".");
					break;
				case MORSE_DASH:
					printf("_");
					break;
				case MORSE_SPACE_CHAR:
					printf("\t");
					break;
				case MORSE_SPACE_DASH:
					printf(" ");
					break;
			}
		}
	}
	printf("\n");
}