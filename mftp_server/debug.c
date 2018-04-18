#include <stdio.h>
#include <debug.h>

void debugprint(char* str) {
	if ( DEBUG ) {
		printf("%s\n", str);
	}
}
