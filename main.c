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
	int job_ids[100];
	int arrival_times[100];
	int durations[100];
	
	int j = 0;
	int i = 0;
	int t = 0;
	int d = 0;
	int arraytype = 0;

	f = fopen(file_name, "r");

	if(f == NULL)
	{
		perror("Cannot open file. \n");
	}
	
	while((echo = fgetc(f)) != EOF){
		if(echo == ' ' )
		{
			if(arraytype == 0)
			{
				job_ids[i] = atoi(buffer);
				i++;
				bzero(buffer,32);
				j=0;
				arraytype++;
			}
			else
			{
				arrival_times[t] = atoi(buffer);
				t++;
				bzero(buffer,32);
				j=0;
				arraytype++;

			}
		}
		else if(echo == '\n')
		{
			durations[d] = atoi(buffer);
			d++;
			bzero(buffer,32);
			j=0;
			arraytype=0;

		}
		else
		{
			buffer[j] = echo;
			j++;
		}
	}
	i = 0;
	while(i<3)
	{
		printf("job ids  %d \n", job_ids[i]);
		i++;
	}
	i=0;
	while(i<3)
	{
		printf("arrival times %d \n" , arrival_times[i]);
		i++;
	}
	i=0;
	while(i<3)
	{
		printf("durations %d \n", durations[i]);
		i++;
	}

	fclose(f);
	return 0;

}
