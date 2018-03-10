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

// sort based on arrivalT -> priority -> pid
int processSort(const void* a, const void* b) {
	process_struct *p1 = (process_struct *)a;
	process_struct *p2 = (process_struct *)b;

	if(p1->arrivalT > p2->arrivalT)
		return true;
	else if(p1->arrivalT < p2->arrivalT)
		return false;

	// if arrival time is equal
	if(p1->priority > p2->priority)
		return true;
	else if(p1->priority < p2->priority)
		return false;

	// if both have same priority
	if(p1->pid > p2->pid)
		return true;
	else
		return false;
}

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

	// sorting
	qsort(inpProcesses, nProcess, sizeof(process_struct), processSort);

	test();
}
