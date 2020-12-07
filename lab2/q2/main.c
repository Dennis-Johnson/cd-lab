#include <stdio.h>
#include <stdlib.h>

/*
	Removes preprocessor directives from the specified source c file, and writes the 
	modified text to removed.txt.
*/

int main(int argc, char *argv[]){
	
	if (argc != 3) {
		fprintf(stderr, "Usage: arg1: input C file src, arg2: output c file src\n");
		exit(0);
	}

	FILE *in, *out;
	int ch, temp;
	
	in  = fopen(argv[1], "r");
	if(in == NULL) {
		fprintf(stderr, "Error opening input file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	out = fopen(argv[2], "w");
	if(out == NULL){
		fprintf(stderr, "Error opening output file: %s\n", argv[2]); 
		exit(EXIT_FAILURE);	
	}	
	
	while ((ch = fgetc(in)) != EOF){
		if(ch == '#'){
			//Read and ignore chars till the end of the line. 
			while( (temp = fgetc(in)) != '\n');
		} else {
			fputc(ch, out);
		}
	}
	
	fclose(in);
	fclose(out);

	return 0;
}
