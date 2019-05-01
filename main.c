#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void printInfo(int*, int*, int*, int*, int*, int);
//QUEUE RETURNS ELEMENTS LEFT IN QUEUE AND MODIFIES THE QUEUE ARRAY
int requeue(int * queue, int job_ids[], int timeleft[], int ,int);
//DE-QUEUE RETURNS ELEMENTS LEFT IN QUEUE AND MODIFIES QUEUE ARRAY
int dequeue(int * queue, int job_ids[], int timeleft[], int, int);
void FIFO(int job_ids[], int arrival_times[], int durations[], int n);
void sjf(int job_ids[], int arrival_times[], int durations[], int n);
void bjf(int job_ids[], int arrival_times[], int durations[], int n);
void stcf(int job_ids[], int arrival_times[], int durations[], int n);
void rr(int job_ids[], int arrival_times[], int durations[], int n);

int main (){
	char echo;
	char file_name [32] = "jobs.dat";
	FILE *f;
	char line[100];
	char *token;
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
	
	while(fgets(line,100,f)){
		token = strtok(line," \t\n");

		while(token != NULL){
			
			if(arraytype == 0){
				job_ids[i] = atoi(token);
				i++;
				arraytype++;
			}
			else if(arraytype == 1){
				arrival_times[j] = atoi(token);
				j++;
				arraytype++;
			}
			else{
				durations[t] = atoi(token);
				t++;
				arraytype = 0;

			}
			token = strtok(NULL, " \t\n");
		}
	}
	FIFO(job_ids,arrival_times,durations,i);
	sjf(job_ids,arrival_times,durations,i);
	bjf(job_ids,arrival_times,durations,i);
	stcf(job_ids,arrival_times,durations,i);
    rr(job_ids,arrival_times,durations,i);

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

void bjf(int job_ids[], int arrival_times[], int durations[], int n){
    int time = 0;
    int start_times[n];
    int finish_times[n];
    int total_times[n];
    int response_times[n];

    //Make sorting the jobs easier with a job struct
    typedef struct{
        int id;
        int arrivalTime;
        int duration;
        bool finished;
    } job;

    //now we can make an array of jobs
    job arr[n];
    for(int i = 0; i != n; ++i){
        arr[i].id = job_ids[i];
        arr[i].arrivalTime = arrival_times[i];
        arr[i].duration = durations[i];
        arr[i].finished = false;
    }

    //Sort by duration in descending order
    //Since n<100, might as well just use insertion sort
    int j;
    job tmp;
    for(int i = 1; i != n; ++i){
        j = i;
        while(j < 0 && arr[j-1].duration < arr[j].duration){
            tmp = arr[j];
            arr[j] = arr[j-1];
            arr[j-1] = tmp;
        }
    }
    int jobsLeft = n;

    while(jobsLeft){
        int i = 0;
        int j = 0;
        while((arr[i].arrivalTime > time) || (arr[i].finished)){
            i++;
        }
        if(i == n){ //case where no unfinished jobs have an arrival time before the current time
            time++;
        }
        else{
            //find the original, pre-sorting index for our process
            //job_ids is passed by reference into this function and then out of this function, so we
            //should not alter it, rather making our output indexes match it.
            while(arr[i].id != job_ids[j]){
                j++;
            }
            start_times[j] = time;
            time = time + arr[i].duration;
            finish_times[j] = time;
            total_times[j] = finish_times[j] - arrival_times[j];
            response_times[j] = start_times[j] - arrival_times[j];
            arr[i].finished = true;
            jobsLeft--;
        }
    }

    printf("BJF Results:\n");
    printInfo(job_ids,start_times,finish_times,total_times,response_times,n);
}

void rr(int job_ids[], int arrival_times[], int durations[], int n){
    int time = 0;
    int start_times[n];
    int finish_times[n];
    int total_times[n];
    int response_times[n];

    //struct to organize job info in a linked list easily
    typedef struct{
        int id;
        int arrivalTime;
        int startTime;
        int duration;
        int remaining;
        bool listed;
    } job;

    job arr[n];
    for(int i = 0; i != n; ++i){
        arr[i].id = job_ids[i];
        arr[i].arrivalTime = arrival_times[i];
        arr[i].duration = durations[i];
        arr[i].remaining = arr[i].duration;
        arr[i].listed = false;
        arr[i].startTime = -1;
    }

    typedef struct node {
        job data;
        struct node * next;
    } node_j;

    node_j * head = NULL;
    head = malloc(sizeof(node_j));
    if (head == NULL) {
        printf("head malloc failed in rr");
        exit(1);
    }
    bool headAssigned = false;
    int jobsLeft = n;

    while(jobsLeft){

        //adds all jobs that have arrived so far to our linked-list FIFO queue
        for(int i = 0; i != n; ++i){
            if(arr[i].arrivalTime <= time && !arr[i].listed){
                arr[i].listed = true;
                if(headAssigned){
                    node_j * current = head;
                    while(current->next != NULL){
                        current = current->next;
                    }
                    current->next = malloc(sizeof(node_j));
                    current->next->data = arr[i];
                    current->next->next = NULL;
                }
                else{
                    node_j * head = NULL;
                    head = malloc(sizeof(node_j));
                    if (head == NULL) {
                        printf("head malloc failed in rr");
                        exit(1);
                    }
                    head->data = arr[i];
                    head->next = NULL;
                    headAssigned = true;
                }
            }
        }

        //Take the head off the list, work on it for a 1 second timeslice, put it back on the end of the list
        //If that 1 second finishes the process, add it's data to the output arrays and tag it as finished
        if(head->next == NULL){ // only one item in list
            if(head->data.startTime == -1){
                head->data.startTime = time;
            }
            head->data.remaining--;
            time++;
            if(head->data.remaining == 0){ // process is finished
                int i;
                while(head->data.id != job_ids[i]){
                    i++;
                }
                start_times[i] = head->data.startTime;
                finish_times[i] = time;
                total_times[i] = finish_times[i] - arrival_times[i];
                response_times[i] = start_times[i] - arrival_times[i];
                headAssigned = false;
                free(head);
                jobsLeft--;
            }
        }
        else{// multiple items in list, take the first item off
            job tmp = head->data;
            node_j * next_node = NULL;
            next_node = head->next;
            free(head);
            head = next_node;
            if(tmp.startTime == -1){
                tmp.startTime == time;
            }

            tmp.remaining--;
            time++;
            if(tmp.remaining == 0){ //process is finished
                int i;
                while(tmp.id != job_ids[i]){
                    i++;
                }
                start_times[i] = tmp.startTime;
                finish_times[i] = time;
                total_times[i] = finish_times[i] - arrival_times[i];
                response_times[i] = start_times[i] - arrival_times[i];
                jobsLeft--;
            }
            else{ //process is not finished, put it at the end of the FIFO
                node_j * current = head;
                while(current->next != NULL){
                    current = current->next;
                }
                current->next = malloc(sizeof(node_j));
                current->next->data = tmp;
                current->next->next = NULL;
            }

        }

    }

    printf("RR Results:\n");
    printInfo(job_ids,start_times,finish_times,total_times,response_times,n);
}

void stcf(int job_ids[], int arrival_times[], int durations[], int n){
    int time = 0;
    int start_times[n], finish_times[n], total_times[n], response_times[n];
    int timeLeft[n];
    int queue[n];
    int queueAmnt = 0;
    int jobCompletedCount = 0, currentlyExecutingID = -1;
    memcpy(timeLeft,durations,n*sizeof(int));

    while(jobCompletedCount < n){
        for(int i = 0; i < n; i++){
            if(arrival_times[i] == time) {
                if(currentlyExecutingID != -1){
                    if(durations[i] < timeLeft[currentlyExecutingID]){
                        //queue it
                        queueAmnt = requeue(queue, job_ids, timeLeft, currentlyExecutingID, queueAmnt);
                        currentlyExecutingID =  job_ids[i];
                    }
                    else{
                        queueAmnt = requeue(queue, job_ids, timeLeft, job_ids[i], queueAmnt);
                    }
                }
                else{
                    queueAmnt = requeue(queue, job_ids, timeLeft, job_ids[i], queueAmnt);
                }
            }
        }
        if((currentlyExecutingID == -1) && queueAmnt > 0){
            //initialize everything
            currentlyExecutingID = queue[0];
            start_times[currentlyExecutingID] = time;
            //finish_times[currentlyExecutingID] = durations[currentlyExecutingID] + time;
            //total_times[currentlyExecutingID] = finish_times[currentlyExecutingID] - time;
            response_times[currentlyExecutingID] = start_times[currentlyExecutingID] - arrival_times[currentlyExecutingID];
            queueAmnt = dequeue(queue,job_ids,timeLeft,currentlyExecutingID,queueAmnt);
            timeLeft[currentlyExecutingID] = timeLeft[currentlyExecutingID] - 1;
        }
        else if (currentlyExecutingID != -1){
            if(timeLeft[currentlyExecutingID] == 0){
                finish_times[currentlyExecutingID] = time;
                total_times[currentlyExecutingID] = finish_times[currentlyExecutingID] - start_times[currentlyExecutingID];
                //response_times[currentlyExecutingID] = arrival_times[currentlyExecutingID] - start_times[currentlyExecutingID];
                if(queueAmnt > 0) {
                    currentlyExecutingID = queue[0];
                    queueAmnt = dequeue(queue, job_ids, timeLeft, currentlyExecutingID, queueAmnt);
                    if (timeLeft[currentlyExecutingID] == durations[currentlyExecutingID]) {
                        start_times[currentlyExecutingID] = time;
                        response_times[currentlyExecutingID] =
                                start_times[currentlyExecutingID] - arrival_times[currentlyExecutingID];
                    }
                    timeLeft[currentlyExecutingID] = timeLeft[currentlyExecutingID] - 1;
                }
                else{
                    currentlyExecutingID = -1;
                }
                jobCompletedCount++;
            }
            else if (timeLeft[currentlyExecutingID] == durations[currentlyExecutingID]){
                start_times[currentlyExecutingID] = time;
                response_times[currentlyExecutingID] = start_times[currentlyExecutingID] - arrival_times[currentlyExecutingID];
                timeLeft[currentlyExecutingID] = timeLeft[currentlyExecutingID] - 1;
            }
            else{
                timeLeft[currentlyExecutingID] = timeLeft[currentlyExecutingID] - 1;
            }
        }
        time++;
    }
    printf("STCF Results:\n");
    printInfo(job_ids,start_times,finish_times,total_times,response_times,n);
}

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

int requeue(int * queue, int job_ids[], int timeLeft[], int jobIdInsert ,int queueAmnt){
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
            for(int k = j; k < queueAmnt; k++) {
                queue[k] = queue[k+1];
            }
        }
    }
    return queueAmnt - 1;
}

