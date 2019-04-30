#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void printInfo(int*, int*, int*, int*, int*, int);
//QUEUE RETURNS ELEMENTS LEFT IN QUEUE AND MODIFIES THE QUEUE ARRAY
int queue(int * queue, int job_ids[], int timeleft[], int ,int);
//DE-QUEUE RETURNS ELEMENTS LEFT IN QUEUE AND MODIFIES QUEUE ARRAY
int dequeue(int * queue, int job_ids[], int timeleft[], int, int);
void FIFO(int job_ids[], int arrival_times[], int durations[], int n);
void sjf(int job_ids[], int arrival_times[], int durations[], int n);
void stcf(int job_ids[], int arrival_times[], int durations[], int n);

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

//    Sample data for testing
//    int job_ids1[6] = {0,1,2,3,4,5};
//    int arrival_times1[6] = {1,1,10,25,45,10};
//    int durations1[6] = {100,75,50,30,20,50};
	
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

	sjf(job_ids,arrival_times,durations,i);

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
	}
	printf("FIFO Results:\n");
	printInfo(job_ids,start_times,finish_times,total_times,response_times,n);
}

void sjf(int job_ids[], int arrival_times[], int durations[], int n){
    int time = 0;
    int start_times[n];
    int finish_times[n];
    int total_times[n];
    int response_times[n];
    int queue[n+1];
    int queueStart = 0, queueEnd = 0, jobCompletedCount = 0;
    while(jobCompletedCount < n - 1){
        for(int i = 0; i < n; i++){
            if(arrival_times[i] == time){
                int insert = queueEnd;
                for(int j = queueStart; j < queueEnd; j++){
                    if(durations[i] < durations[queue[j]]){
                        for(int k = queueEnd; k > j; k--) {
                            queue[k] = queue[k - 1];
                        }
                        insert = j;
                        break;
                    }
                }
                queue[insert] = job_ids[i];
                queueEnd++;
            }
        }
        if(queueStart == 0 && queueEnd != 0){
            start_times[queue[queueStart]] = time;
            finish_times[queue[queueStart]] = durations[queue[queueStart]] + time;
            total_times[queue[queueStart]] = finish_times[queue[queueStart]] - time;
            response_times[queue[queueStart]] = arrival_times[queue[queueStart]] - start_times[queue[queueStart]];
            queueStart++;
        }
        else if(time >= finish_times[queue[queueStart-1]] && queueStart > 0){
            jobCompletedCount++;
            //queueStart++;
            start_times[queue[queueStart]] = finish_times[queue[queueStart-1]];
            finish_times[queue[queueStart]] = start_times[queue[queueStart]] + durations[queue[queueStart]];
            total_times[queue[queueStart]] = finish_times[queue[queueStart]] - start_times[queue[queueStart]];
            response_times[queue[queueStart]] = start_times[queue[queueStart]]- arrival_times[queue[queueStart]];
            queueStart++;
        }
        time++;
    }
    printf("SJF Results:\n");
    printInfo(job_ids,start_times,finish_times,total_times,response_times,n);
}
/*
void stcf(int job_ids[], int arrival_times[], int durations[], int n){
    int time = 0;
    int start_times[n], finish_times[n], total_times[n], response_times[n];
    int timeLeft[n], queue[n];
    int queueAmnt = 0;
    int jobCompletedCount = 0, currentlyExecutingID = -1;
    memcpy(timeLeft,durations,n*sizeof(int));

    while(jobCompletedCount < n - 1){
        for(int i = 0; i < n; i++){
            if(arrival_times[i] == time) {
                if(currentlyExecutingID != -1){
                    if(durations[i] < timeLeft[currentlyExecutingID]){
                        //queue it
                        queueAmnt = queue(queue, job_ids, timeLeft, job_ids[i], queueAmnt);
                    }
                }
            }
        }
    }
}
*/
void printInfo(int job_ids[], int start_times[], int finish_times[], int total_times[], int response_times[],int n){
    printf("JOB ID\t|\tSTART TIME\t|\tFINISH TIME\t|\tTIME ELAPSED\t|\tRESPONSE TIME\n");
    printf("-----------------------------------------------------------------------------\n");
    for(int i=0; i<n; i++){
        printf("%d\t\t\t",job_ids[i]);
        printf("%d\t\t\t\t",start_times[i]);
        printf("%d\t\t\t\t",finish_times[i]);
        printf("%d\t\t\t\t\t",total_times[i]);
        printf("%d\t\t\n",response_times[i]);
    }
    printf("\n");
}
/*
int queue(int * queue, int job_ids[], int timeLeft[], int jobIdInsert ,int queueAmnt){
    int insert = queueAmnt;
    for(int j = 0; j < queueAmnt; j++){
        if(timeLeft[jobIdInsert] < timeLeft[queue[j]]){
            for(int k = queueAmnt; k > j; k--) {
                queue[k] = queue[k - 1];
            }
            insert = j;
            break;
        }
    }
    queue[insert] = jobIdInsert;
    return queueAmnt + 1;
}

int dequeue(int * queue, int job_ids[], int timeLeft[], int jobIdDelete ,int queueAmnt){
    for(int j = 0; j < queueAmnt; j++){
        if(queue[j] == jobIdDelete){
            for(int k = queueAmnt; k > j; k--) {
                queue[k] = queue[k - 1];
            }
            insert = j;
            break;
        }
    }
    queue[insert] = jobIdInsert;
    return queueAmnt + 1;
}
 */