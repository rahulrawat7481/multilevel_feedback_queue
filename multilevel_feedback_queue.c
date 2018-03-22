#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUSY 1
#define IDLE 0

typedef struct process_data {
	int pid;
	int arrivalT;
	int burstT;
	int priority;
	int finishT;
	int responseT;
	int waitingT;
} process_struct;

int nProcess = 0;
int totalExecTime = 0;
int CPU_STATE = IDLE;
process_struct *inpProcesses;


// fixed priority queue
// functions - push, top, pop, isEmpty
typedef struct node {
	process_struct* process;
	struct node* next;
} NODE;

void pq_push(NODE** root, process_struct* processPointer) {
	NODE* temp = (NODE *)malloc(sizeof(NODE));
	temp->process = processPointer;
	temp->next = NULL;

	// if root is null
	if(*root == NULL) {
		*root = temp;

	} else if((*root)->process->priority > processPointer->priority) { 
		// if the new node has high priority
		temp->next = *root;
		*root = temp;
	} else {
		// find a place to fit the node
		NODE* head = *root;
		while(head->next != NULL && head->next->process->priority < processPointer->priority)
			head = head->next;

		temp->next = head->next;
		head->next = temp;
	}
}

bool pq_isEmpty(NODE** root) {
	return ((*root) == NULL);
}

process_struct* pq_top(NODE** root) {
	if((*root) == NULL) {
		printf("\ntop on empty priority queue!\n");
		exit(-1);
	}
	return (*root)->process;
}

process_struct* pq_pop(NODE** root) {
	if((*root) == NULL) {
		printf("\npop on empty priority queue!\n");
		exit(-1);
	}
	NODE* temp = *root;
	*root = (*root)->next;
	process_struct* retProcess = temp->process;
	free(temp);
	return retProcess;
}

// round robin queue
int timeQuantum = 2;


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

// get total execution time
void calcTotalExecTime() {
	totalExecTime = inpProcesses[0].arrivalT;
	for(int i=0; i<nProcess; i++) {
		if(totalExecTime >= inpProcesses[i].arrivalT)
			totalExecTime += inpProcesses[i].burstT;
		else
			totalExecTime += (inpProcesses[i].arrivalT - totalExecTime) + inpProcesses[i].burstT;
	}
}

void printProcess(process_struct* p) {
		printf("\n");
		printf("\nProcess PID : %d", p->pid);
		printf("\nProcess Arrival Time : %d", p->arrivalT);
		printf("\nProcess Burst Time : %d", p->burstT);
		printf("\nProcess Priority : %d", p->priority);
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

	printf("\n\nTotal Exec Time : %d", totalExecTime);
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
	// total execution time calculation
	calcTotalExecTime();


	test();


	process_struct* current;
	int cpuTime = 0;
	NODE* fixedPriorityQueue = NULL;


	for(; cpuTime < totalExecTime; cpuTime++) {
		// add processes with arrivalT == cpuTime to fixed priority queue
		for(int i=0; i<nProcess; i++) {
			if(inpProcesses[i].arrivalT == cpuTime) {
				printf("\nPushing Process %d", inpProcesses[i].pid);
				pq_push(&fixedPriorityQueue, &inpProcesses[i]);
			}
		}

		if(CPU_STATE == IDLE) { // if cpu is idle
			// if there is a new process in priority queue
			if(!pq_isEmpty(&fixedPriorityQueue)) {
				current = pq_pop(&fixedPriorityQueue);
				CPU_STATE = BUSY;
			}
			// else if there is a process remaining in round robin queue
		} else {
			// if cpu is busy
		}

	}

}
