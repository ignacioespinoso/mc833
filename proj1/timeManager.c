// Time manager Codes
/*
	This file is responsible for the code to
	get the connection time information
	print it - 
	and also save the information on a .txt file
*/
/*  Giovani Nascimento Pereira - 168609
  Ignacio Espinoso Ribeiro - 

  MC833 - 2S2018
  Unicamp
*/

// ---- Time manangement
typedef struct connectionTime{
	int operation;
	struct timeval sendTime;
	struct timeval recieveTime;
} connectionTime;

void writeClientTimeResults(char* filename, connectionTime op);
void writeServerTimeResults(char* filename, connectionTime op);

/// Print the connection time from a connectionTime struct variable
void printConnectionTimeClient(connectionTime op){
	printf(">>>>>Send Time: %d μs\n", op.sendTime.tv_usec);
	printf("<<<<<Receive Time: %d μs\n", op.recieveTime.tv_usec);
	printf("Total Interval Time: %d μs\n", (op.recieveTime.tv_usec - op.sendTime.tv_usec));

	//Write to file - easier to find later
	writeClientTimeResults("clientTime.txt", op);
}

/// Print the connection time from a connectionTime struct variable
void printExecutionTimeServer(connectionTime op){
	printf(">>>>>Receive Time: %d μs\n", op.recieveTime.tv_usec);
	printf("<<<<<Send Time: %d μs\n", op.sendTime.tv_usec);
	printf("Total Processing Time: %d μs\n", (op.sendTime.tv_usec - op.recieveTime.tv_usec));

	//Write to file - easier to find later
	writeServerTimeResults("serverTime.txt", op);
}

/// Write on .txt file the time results
void writeClientTimeResults(char* filename, connectionTime op){
	FILE *f = fopen(filename, "a");
	if (f == NULL){
		perror("The timeFile could not be opened\n");
		return;
	}

	//Writing the test result
	fprintf(f, "Operation: %d\n", op.operation);
	fprintf(f, ">>>>>Send Time: %d μs\n", op.sendTime.tv_usec);
	fprintf(f, "<<<<<Receive Time: %d μs\n", op.recieveTime.tv_usec);
	fprintf(f, "Total Interval Time: %d μs\n\n", (op.recieveTime.tv_usec - op.sendTime.tv_usec));

	fclose(f);
}

/// Write on .txt file the time results
void writeServerTimeResults(char* filename, connectionTime op){
	FILE *f = fopen(filename, "a");
	if (f == NULL){
		perror("The timeFile could not be opened\n");
		return;
	}

	//Writing the test result
	fprintf(f, "Operation: %d\n", op.operation);
	fprintf(f, ">>>>>Receive Time: %d μs\n", op.recieveTime.tv_usec);
	fprintf(f, "<<<<<Send Time: %d μs\n", op.sendTime.tv_usec);
	fprintf(f, "Total Processing Time: %d μs\n\n", (op.sendTime.tv_usec - op.recieveTime.tv_usec));

	fclose(f);
}
