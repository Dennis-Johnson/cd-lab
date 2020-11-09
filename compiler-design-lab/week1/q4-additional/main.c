#include <stdio.h>
#include <stdlib.h>
#define BUFFER_LENGTH 120
/*
	Adds line numbers to a source c file given as input, writes modified code to
	a .txt file
*/

int main (int argc, char *argv[]){
	
	if(argc != 2){
		fprintf(stderr, "Usage: Source C file as input\n");
		exit(0);
	}

	FILE *fin, *fout;
	int line_no = 0;
	char line[BUFFER_LENGTH];
	
	fin = fopen(argv[1], "r");
	if (fin == NULL) {
		fprintf(stderr, "Couldn't open input file\n");
		exit(1);
	}
	
	fout = fopen("main_numbered.txt", "w");
	if (fout == NULL) {
		fprintf(stderr, "Couldn't open output file\n");
		exit(1);
	}
	
	while (fgets(line, BUFFER_LENGTH, fin) != NULL) {
		line_no++;
		fprintf(fout, "%4d %s", line_no, line);
	} 
	
	fclose(fin);
	fclose(fout);

	return 0;
}
