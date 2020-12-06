#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_INT 5

/*
	Removes preprocessor directives from the specified source c file, and writes the 
	modified text to removed.txt. Some of the preprocessor directives above are usused.
*/

int main(int argc, char *argv[]){
	
	if (argc != 2) {
		fprintf(stderr, "Usage: source c file as sole input\n");
		exit(0);
	}

	FILE *in, *out;
	int ch, temp;
	char *output_path = "removed.txt";
	
	in  = fopen(argv[1], "r");
	if(in == NULL) {
		fprintf(stderr, "Error opening input file\n");
		exit(1);
	}
	
	out = fopen(output_path, "w");
	
	if(out == NULL){
		fprintf(stderr, "Error opening output file %s\n", output_path); 
		exit(1);	
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
