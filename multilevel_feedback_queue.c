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
	int remainingT;
	int responseT;
	int waitingT;
} process_struct;

int nProcess = 0;
int totalExecTime = 0;
int CPU_STATE = IDLE;
process_struct *inpProcesses;
enum process_from {NONE, PQ, RRQ} PROCESS_QUEUE;

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

void q_push(NODE** root, process_struct* processPointer) {
	NODE* temp = (NODE *)malloc(sizeof(NODE));
	temp->process = processPointer;
	temp->next = NULL;

	// if root is null
	if(*root == NULL) {
		*root = temp;
	} else {
		// find the last place
		NODE* head = *root;
		while(head->next != NULL)
			head = head->next;
		head->next = temp;
	}
}

bool q_isEmpty(NODE** root) {
	return ((*root) == NULL);
}

process_struct* q_top(NODE** root) {
	if((*root) == NULL) {
		printf("\ntop on empty queue!\n");
		exit(-1);
	}
	return (*root)->process;
}

process_struct* q_pop(NODE** root) {
	if((*root) == NULL) {
		printf("\npop on empty queue!\n");
		exit(-1);
	}
	NODE* temp = *root;
	*root = (*root)->next;
	process_struct* retProcess = temp->process;
	free(temp);
	return retProcess;
}


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
		inpProcesses[i].remainingT = inpProcesses[i].burstT;
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
	NODE* roundRobinQueue = NULL;
	PROCESS_QUEUE = NONE;

	for(; cpuTime < totalExecTime; cpuTime++) {
		printf("\n-------------------------\n");
		if(current == NULL)
			printf("\n CPU TIME -> %3d --> No Process Running", cpuTime);
		else
			printf("\n CPU TIME -> %3d --> Process Running with PID = %d", cpuTime, current->pid);


		// add processes with arrivalT == cpuTime to fixed priority queue
		for(int i=0; i<nProcess; i++) {
			if(inpProcesses[i].arrivalT == cpuTime) {
				printf("\nPushing Process %d", inpProcesses[i].pid);
				pq_push(&fixedPriorityQueue, &inpProcesses[i]);
			}
		}

		if(CPU_STATE == IDLE) { // if cpu is idle
			printf("\n CPU TIME -> %3d --> CPU is IDLE", cpuTime);
			// if there is a new process in priority queue
			if(!pq_isEmpty(&fixedPriorityQueue)) {
				current = pq_pop(&fixedPriorityQueue);
				printf("\n CPU TIME -> %3d --> fixed priority queue not empty, process popped %d", cpuTime, current->pid);
				CPU_STATE = BUSY;
				PROCESS_QUEUE = PQ;
			}
			// else if there is a process remaining in round robin queue
			else if(!q_isEmpty(&roundRobinQueue)) {
				current = q_pop(&roundRobinQueue);
				printf("\n CPU TIME -> %3d --> round robin queue not empty, process popped %d", cpuTime, current->pid);
				CPU_STATE = BUSY;
				PROCESS_QUEUE = RRQ;
			}
		} else { // if cpu is busy
			printf("\n CPU TIME -> %3d --> CPU is BUSY", cpuTime);
			// check if process is from priority queue 
			// and new process comes in priority queue with high priority
			if(PROCESS_QUEUE == PQ && (!pq_isEmpty(&fixedPriorityQueue))) {
				printf("\n CPU TIME -> %3d --> fixed priority queue running & new process in fixed priority queue with pid %d", cpuTime, pq_top(&fixedPriorityQueue)->pid);
				if(pq_top(&fixedPriorityQueue)->priority < current->priority) {
					printf("\n CPU TIME -> %3d --> process %d has higher priority", cpuTime, pq_top(&fixedPriorityQueue)->pid);
					q_push(&roundRobinQueue, current);
					current = pq_pop(&fixedPriorityQueue);
				}
			}
			// check if process if from round robin queue
			// and new process comes in priority queue
			else if(PROCESS_QUEUE == RRQ && (!pq_isEmpty(&fixedPriorityQueue))) {
				printf("\n CPU TIME -> %3d --> round robin queue running & new process in fixed priority queue with pid %d", cpuTime, pq_top(&fixedPriorityQueue)->pid);
				q_push(&roundRobinQueue, current);
				current = pq_pop(&fixedPriorityQueue);
				PROCESS_QUEUE = PQ;
			}
		}


		if(current != NULL) { // if there is some process running
			current->remainingT -= 1;

			// if process finished
			if(current->remainingT == 0) {
				printf("\n CPU TIME -> %3d --> process with pid %d finished", cpuTime, current->pid);
				CPU_STATE = IDLE;
				current = NULL;
				PROCESS_QUEUE = NONE;
			}
		}

	}

}
