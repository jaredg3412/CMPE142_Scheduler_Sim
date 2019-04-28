#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void FIFO(int job_ids[], int arrival_times[], int durations[], int n);

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

	FIFO(job_ids,arrival_times,durations,i);

	fclose(f);
	return 0;

}

//assumes arrival times are in ascending order
void FIFO(int job_ids[], int arrival_times[], int durations[], int n){

	int start_times[n];
	int finish_times[n];
	int total_times[n];
	int response_times[n];

	for(int i=0; i<n; i++)
	{
		if(i==0)
		{
			start_times[i] = 0;
			finish_times[i] = durations[i];
			total_times[i] = finish_times[i];
			response_times[i] = arrival_times[i] - start_times[i];
		}
		else{
			start_times[i] = finish_times[i-1];
			finish_times[i] = start_times[i] + durations[i];
			total_times[i] = finish_times[i] - start_times[i];
			response_times[i] = start_times[i]- arrival_times[i];
		}
	

	printf("JOB ID: %d \n",job_ids[i]);
	printf("Start time: %d \n",start_times[i]);
	printf("Finish time: %d \n",finish_times[i]);
	printf("Time elapsed: %d \n",total_times[i]);
	printf("Response time: %d \n",response_times[i]);
	}

} 


