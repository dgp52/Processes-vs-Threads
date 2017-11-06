//Name: Deepkumar Patel, NetID: dgp52
//Name: Viraj Patel, NetID: vjp60

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
//used for timetest purposes
//#include <time.h>

//Check if argv[2] is a number
int checkifNum(char num[]) {
    int i;
    //Check if the number is negative/negative sign
    if (num[0] == '-') {
        printf("Cannot have negative number of processes!\n");
        return 0;
    }
    for ( i = 0; num[i]!= 0; i++) {
        if (!isdigit(num[i])) {
            return 0;
        }
    }
    return 1;
}

//Main method
int main (int argc, char *argv[]) {
  //used for time test purposes
  //clock_t start = clock(), diff;
  if(argc!=3){
    //Number of arguments has to be 3 otherwise show error message
    printf("Invalid number of arguments!\n");
    return 1;
  } else {
      //Open the file and check if its not null.
       FILE *file = fopen(argv[1],"r");
       if(file==NULL){
          //If could not open the file show error
          printf("Could not open the file\n");
          return 1;
        }else {
          //Fseek will give us the number of characters in a given file.
          fseek(file, 0, SEEK_END );
          //fseek returns number of characters + 1, therefore we need to do minus 1 to get total characters in a file.
          long totalChar = ftell(file)-1;
          //Close the file
          fclose(file);
          if(checkifNum(argv[2]) && totalChar >= atoi(argv[2]) && atoi(argv[2])>0){
            //Get the number of processes passed by user
            int processP = atoi(argv[2]);
            //pCompress array keeps track of how many characters should each worker process compress.
            int *pCompress = (int *) malloc(sizeof(int)*processP);
            //Get the modulus 
            int modulus = totalChar % processP;
            //Get the remainder 
            int div = totalChar/processP;
            //Set the values of array to zero
            int n;
            for(n=0; n<processP;n++) {
              pCompress[n] = 0;
            }
            //They divide unevenly
            if(modulus != 0){
              int i;
              for(i=0; modulus>0; i++){
                pCompress[i]=1;
                modulus --;
              }
              for(i=0;i<processP;i++){
                pCompress[i] += div;
              }
            } else {
              //Everything divides evenly
              int i;
              for(i=0;i<processP;i++){
                pCompress[i] += div;
              }
            }
           //Array of ints that will tell the starting point of each worker process.
           int *pStartingIndex = (int *) malloc( sizeof(int) * processP ) ; 
           pStartingIndex[0] = 1 ;
           int j;
           for(j=1;j<processP;j++) {
              pStartingIndex[j] = pStartingIndex[j-1] + pCompress[j-1];
            }
            //Pids for processes
		 	pid_t pids[processP];
		  	int p;
	    	int w = processP;

		  	//Following 4 variables will convert int to string, so that we can pass values as arguments to execl
		  	char proTotalChar[50] = {0};
		  	char startingIndex[50] = {0};
		  	char proNum[50] = {0};
		  	char parts[50] = {0};
		  	/* Start children. */
		  	for (p = 0; p < processP; ++p) {
		    	if ((pids[p] = fork()) < 0) {
		    		//If the worker process wasn't created
		      		printf("Worker process[%d] couldn't be created.\n", p);
		    	} else if (pids[p]==0) {
		    		//We are in the worker process, pass in necessary arguments
		    		sprintf(proTotalChar, "%d", pCompress[p]);
		    		sprintf(startingIndex, "%d", pStartingIndex[p]);
		    		sprintf(proNum, "%d", p);
		    		sprintf(parts, "%d", processP);
		      		execl("./compressR_worker_LOLS", "./compressR_worker_LOLS", argv[1], proTotalChar, startingIndex, proNum, parts, (char*)0);
		      		exit(0);
		    	}
		  	}
        free(pCompress);
        free(pStartingIndex);
        //Parent process will wait for child processes
  			int wChild;
  			while (w > 0) {
    			wait(&wChild);
    			--w;
  			}
          } else {
            //Error if the number of parts or processes is not valid
            printf("Invalid number of processes!\n");
            return 1;
          }
        }
  }
  //Used for timetest purposes
  //diff = clock() - start;
  //int msec = diff*1000 / CLOCKS_PER_SEC;
  //printf("Time taken %d seconds %d milliseconds\n",msec/1000,msec%1000);
  return 0;
}