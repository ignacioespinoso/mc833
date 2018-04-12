// Time manager Codes
/*
	This file is responsible for the code to
	get the connection time information
	print it -
	and also save the information on a .txt file
*/
/*  Giovani Nascimento Pereira - 168609
  Ignacio Espinoso Ribeiro - 169767

  MC833 - 2S2018
  Unicamp
*/

// ---- Time manangement
typedef struct connectionTime{
	int operation;
	struct timeval sendTime;
	struct timeval receiveTime;
} connectionTime;

int recordCounter = 0;

void writeClientTimeResults(char* filename, connectionTime op);
void writeServerTimeResults(char* filename, connectionTime op);

/// Print the connection time from a connectionTime struct variable
void printConnectionTimeClient(connectionTime op){
	printf(">>>>>Send Time: %d μs\n", op.sendTime.tv_usec);
	printf("<<<<<Receive Time: %d μs\n", op.receiveTime.tv_usec);
	printf("Total Interval Time: %d μs\n", (op.receiveTime.tv_usec - op.sendTime.tv_usec));

	//Write to file - easier to find later
	writeClientTimeResults("clientTimeLog.txt", op);
}

/// Print the connection time from a connectionTime struct variable
void printExecutionTimeServer(connectionTime op){
	printf(">>>>>Receive Time: %d μs\n", op.receiveTime.tv_usec);
	printf("<<<<<Send Time: %d μs\n", op.sendTime.tv_usec);
	printf("Total Processing Time: %d μs\n", (op.sendTime.tv_usec - op.receiveTime.tv_usec));

	//Write to file - easier to find later
	writeServerTimeResults("serverTimeLog.txt", op);
}

/// Write on .txt file the time results
void writeClientTimeResults(char* filename, connectionTime op){
	FILE *f = fopen(filename, "a");
	if (f == NULL){
		perror("The timeFile could not be opened\n");
		return;
	}

	// Log a new session
	if (recordCounter == 0){
		time_t now = time(NULL);
		struct tm *time = localtime(&now);
		fprintf(f, "\n-------- SESSION  ");
		fprintf(f, "%d/%d/%d ", time->tm_mday, time->tm_mon, time->tm_year);
		fprintf(f, "%d:%d:%d ", time->tm_hour, time->tm_min, time->tm_sec);
		fprintf(f, " --------\n\n");
	}

	//Calculating Time - considering seconds and u seconds
	int seconds = op.sendTime.tv_sec;
	int useconds = op.sendTime.tv_usec;
	double sendTime = (double)seconds + (double)useconds * 0.00001;

	seconds = op.receiveTime.tv_sec;
	useconds = op.receiveTime.tv_usec;
	double receiveTime = (double)seconds + (double)useconds * 0.00001;

	//Writing the test result
	fprintf(f, "Operation: %d\n", op.operation);
	// fprintf(f, ">>>>>Send Time: %lf s\n", sendTime);
	// fprintf(f, "<<<<<Receive Time: %lf s\n", receiveTime);
	// fprintf(f, "Total Interval Time: %lf μs\n\n", (receiveTime - sendTime));

	fprintf(f, "%lf\n\n", (receiveTime - sendTime));

	fclose(f);
	recordCounter++;
}

/// Write on .txt file the time results
void writeServerTimeResults(char* filename, connectionTime op){
	FILE *f = fopen(filename, "a");
	if (f == NULL){
		perror("The timeFile could not be opened\n");
		return;
	}

	// Log a new session
	if (recordCounter == 0){
		time_t now = time(NULL);
		struct tm *time = localtime(&now);
		fprintf(f, "\n-------- SESSION  ");
		fprintf(f, "%d/%d/%d ", time->tm_mday, time->tm_mon, time->tm_year);
		fprintf(f, "%d:%d:%d ", time->tm_hour, time->tm_min, time->tm_sec);
		fprintf(f, " --------\n\n");
	}

	//Calculating Time - considering seconds and u seconds
	int seconds = op.sendTime.tv_sec;
	int useconds = op.sendTime.tv_usec;
	double sendTime = (double)seconds + (double)useconds * 0.00001;

	seconds = op.receiveTime.tv_sec;
	useconds = op.receiveTime.tv_usec;
	double receiveTime = (double)seconds + (double)useconds * 0.00001;

	//Writing the test result
	fprintf(f, "Operation: %d\n", op.operation);
	// fprintf(f, ">>>>>Receive Time: %lf s\n", receiveTime);
	// fprintf(f, "<<<<<Send Time: %lf s\n", sendTime);
	// fprintf(f, "Total Processing Time: %lf s\n\n", (sendTime - receiveTime));


	fprintf(f, "%lf\n", (sendTime - receiveTime));


	fclose(f);
	recordCounter++;
}
