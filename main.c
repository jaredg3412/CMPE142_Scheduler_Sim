#include <stdio.h>
#include <stdlib.h>

int main (){
	char echo;
	char file_name [32] = "jobs.dat";
	FILE *f;

	f = fopen(file_name, "r");

	if(f == NULL)
	{
		perror("Cannot open file. \n");
	}
	
	while((echo = fgetc(f)) != EOF)
		printf("%c", echo);

	fclose(f);
	return 0;

}
