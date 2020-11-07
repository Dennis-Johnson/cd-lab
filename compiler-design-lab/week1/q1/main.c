#include <stdio.h>
#include <stdlib.h>

/*
	Provide a file src as input, replace blank spaces and tabs with a single space.
*/
int main(int argc, char *argv[]){
	
	if(argc != 3){
		fprintf(stderr, "Usage: arg1: input file src, arg2: output file src\n");
		exit(1);
	}

	FILE *inp, *out;
	int ch, wspace_count = 0;

  inp = fopen(argv[1], "r");
	out = fopen(argv[2], "w");	
	if(inp == NULL || out == NULL){
		fprintf(stderr, "Error opening file\n");
		exit(1);
	} 
 	
	while((ch = fgetc(inp))!= EOF){
		if(ch == '\t' || ch == ' '){
			wspace_count++;
		} 
		else {	
			if(wspace_count > 0)
				putc(' ', out);
			
			fputc(ch, out);
			wspace_count = 0;
		}
	}

	fclose(inp);
	fclose(out);

	return 0;
}
