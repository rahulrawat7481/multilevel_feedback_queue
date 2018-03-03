#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct process_data {
	int pid;
	int arrivalT;
	int burstT;
	int priority;
	int finishT;
	int responseT;
	int waitingT;
} process_struct;

int nProcess;
process_struct *inpProcesses;

// fixed priority queue

// round robin queue

void test() {
	printf("\n\n");
	for(int i=0; i<nProcess; i++) {
		printf("\n");
		printf("\nProcess %d PID : %d", i, inpProcesses[i].pid);
		printf("\nProcess %d Arrival Time : %d", i, inpProcesses[i].arrivalT);
		printf("\nProcess %d Burst Time : %d", i, inpProcesses[i].burstT);
		printf("\nProcess %d Priority : %d", i, inpProcesses[i].priority);
	}
}

int main(int argc, char** argv) {
	printf("\nEnter number of processes : ");
	scanf("%d", &nProcess);

	inpProcesses = (process_struct *)malloc(nProcess * sizeof(process_struct));
	for(int i=0; i<nProcess; i++) {
		printf("\n");
		printf("\nProcess %d PID : ", i);
		scanf("%d", &inpProcesses[i].pid);
		printf("\nProcess %d Arrival Time : ", i);
		scanf("%d", &inpProcesses[i].arrivalT);
		printf("\nProcess %d Burst Time : ", i);
		scanf("%d", &inpProcesses[i].burstT);
		printf("\nProcess %d Priority : ", i);
		scanf("%d", &inpProcesses[i].priority);
	}
	test();
}
