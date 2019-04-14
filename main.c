#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (){
	char echo;
	char file_name [32] = "jobs.dat";
	FILE *f;

	//needed to parse input file 
	char buffer[32];
	int integers[300];
	
	int j = 0;
	int i = 0;

	f = fopen(file_name, "r");

	if(f == NULL)
	{
		perror("Cannot open file. \n");
	}
	
	while((echo = fgetc(f)) != EOF){
		if(echo == ' ' || echo == '\n' )
		{
			integers[i] = atoi(buffer);
			i++;
			bzero(buffer,32);
			j=0;
		}
		else
		{
			buffer[j] = echo;
			j++;
		}
	}
	i = 0;
	while(i<9)
	{
		printf("integers %d \n", integers[i]);
		i++;
	}

	fclose(f);
	return 0;

}
