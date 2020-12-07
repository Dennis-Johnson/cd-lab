#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	FILE *fin, *fout;
	int ca, cb;
	
	if(argc != 2){
		fprintf(stderr, "Usage: arg1: input file src\n");
		exit(EXIT_FAILURE);
	}

	fin = fopen(argv[1], "r");
	if(fin == NULL){
		fprintf(stderr, "Cannot open src file\n");
		exit(EXIT_FAILURE);
	}	

	fout = fopen("removedComments.c", "w");
	if(fout == NULL){
		fprintf(stderr, "Error opening output file\n");
		exit(EXIT_FAILURE);
	}

	while((ca = fgetc(fin)) != EOF){
		if(ca == ' '){
			fputc(ca, fout);
			while(ca == ' ')
				ca = fgetc(fin);
		}

		if(ca == '/'){
			cb = fgetc(fin);
			
			if(cb == '/'){
				while(ca != '\n')
					ca = fgetc(fin);
			}
			
			else if(cb == '*'){
				do{
					while(ca != '*')
						ca = fgetc(fin);
					ca = fgetc(fin);
				}while(ca != '/');
			}
			
			else{
					fputc(ca, fout);
					fputc(cb, fout);
			}
		}
		
		else fputc(ca, fout);
	}
	
	fclose(fin);
	fclose(fout);
	return 0;
}
