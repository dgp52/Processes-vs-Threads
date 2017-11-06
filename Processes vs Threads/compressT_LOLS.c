//Name: Deepkumar Patel, NetID: dgp52
//Name: Viraj Patel, NetID: vjp60

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/file.h> 
#include <pthread.h> 
//Used for timetest purposes
//#include <time.h>

//Structure for threads
struct tInfo {
  //Name of the file to which compression is going to run
  char *fName;
  //Number of characters this thread will compress
  int thTotalChar;
  //Index at which this thread starts at
  int startingIndex;
  //Which number of thread in tArray: for example 0...n thread
  int threadNum;
  //Number of parts
  int parts;
};

//Takes parameter which points to struct, compression algorithm
void* compressionFun(void* tInfo) {
  //Create a dummy struct and pass in all the values necessary to compress the file.
  struct tInfo* ti = (struct tInfo*)tInfo;
  //Set below variables using struct that was passed in
  char *fName = ti->fName;
  int thTotalChar = ti->thTotalChar;
  int startingIndex = ti->startingIndex;
  int threadNum = ti->threadNum;
  int threadP = ti->parts;
  //Free ti
  free(ti); 
  //Create the file name as required in the project
  //Create a buffer to get its length
  char str[200];
  //If theres only 1 thread then we dont need to append the number
  if(threadP != 1) {
    snprintf(str, 100, "%d" , threadNum);
  }
  //Malloc the file name + 5 for _LOLS + 5 for .txt and '\0' + strlen(str)
  char *outputFile = (char*)malloc((strlen(fName)+5+(int)strlen(str)+5)*sizeof(char));
  const char *inFile = fName;
  char period = '.';
  const char *replaceTo = "_";
  int count = 0;
  const char *fp;
  for(fp=inFile; *fp; fp++) {
    count += (*fp == period);
  }
  size_t rtlength = strlen(replaceTo);
  char *result = malloc(strlen(inFile) + (rtlength-1)*count + 1);
  char *resptr = result;
  for(fp=inFile; *fp; fp++) {
    if(*fp == period) {
      memcpy(resptr, replaceTo, rtlength);
      resptr += rtlength;
    } else {
      *resptr++ = *fp;
    }
  }
  *resptr = 0;
  strcpy(outputFile, result) ;
  strcat(outputFile, "_LOLS") ;
  if(threadP != 1){
    strcat(outputFile, str);
  }  
  strcat(outputFile, ".txt") ;
  //Open the file using the fName
  FILE *file = fopen(fName ,"r");
  //Total number of characters this thread will compress 
  int totalc =  thTotalChar;
  //Create the output .txt file, using the output file name created above 
  FILE *outFile = fopen(outputFile , "w");
  int i;
  char c1 = NULL;
  int c1count = 0;
  //Go through for loop and start compressing
  for(i = 0; i < totalc; i++){
    //Increment the file pointer
    fseek(file,startingIndex+i-1,0);
    char temp = fgetc(file);
    //Check if the character is alphabet
    if(isalpha(temp)){
      if(c1 == '\0'){
	       c1 = temp;
	       c1count = 1;
         if(i == totalc-1) {
            //If the last character is an alphabet and c1 is null
            if(c1count >  2){
              fprintf(outFile, "%d%c",c1count,c1);
            }else if (c1count == 2){
              fprintf(outFile, "%c",c1);
              fprintf(outFile, "%c",c1);
            } else {
              fprintf(outFile, "%c",c1);
            } 
         }
      } else {
          	//If the current character matches the previous char
          	if(c1 == temp){
          	  if(i==totalc-1){
          	    c1count++;
          	    if(c1count >  2){
          	      fprintf(outFile, "%d%c",c1count,c1);
          	    }else if (c1count == 2){
          	      fprintf(outFile, "%c",c1);
          	      fprintf(outFile, "%c",c1);
          	    } else {
          	      fprintf(outFile, "%c",c1);
          	    }
                    } else {
                      c1count++;
                    }
          	} else {
          	  //The previous character doesnt match the current char
          	  if(c1count >  2){
          	    fprintf(outFile, "%d%c",c1count,c1);
          	  }else if (c1count == 2){
          	    fprintf(outFile, "%c",c1);
          	    fprintf(outFile, "%c",c1);
          	  } else {
          	    fprintf(outFile, "%c",c1);
          	  }
                    c1 = temp;
                    c1count = 1;
          	  if(i == totalc-1){
          	    if(c1count >  2){
          	      fprintf(outFile, "%d%c",c1count,c1);
          	    }else if (c1count == 2){
          	      fprintf(outFile, "%c",c1);
          	      fprintf(outFile, "%c",c1);
          	    } else {
          	      fprintf(outFile, "%c",c1);
          	    } 
          	   }
            }
      }
    } else if (i == totalc-1){
      //If theres a non-alphabetic charcter at the end
      if(c1count >  2){
	       fprintf(outFile, "%d%c",c1count,c1);
      }else if (c1count == 2){
	       fprintf(outFile, "%c",c1);
	       fprintf(outFile, "%c",c1);
      } else {
	       fprintf(outFile, "%c",c1);
      } 
    }
  }
  //Close the files
  fclose(file);
  fclose(outFile);
  free(outputFile);
  free(result);
  pthread_exit(0);
}

//Check if argv[2] is a number
int checkifNum(char num[]) {
    int i;
    //Check if the number is negative/negative sign
    if (num[0] == '-') {
        printf("Cannot have negative number of threads!\n");
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
  //Used for timetest purposes
  //clock_t start = clock(), diff;
  if(argc!=3){
    //Number of arguments has to be 3 otherwise show error message
    printf("Invalid number of arguments!\n");
    return 1;
  } else {
      //Check if file opens
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
            //Get the number of threads passed by user
            int threadP = atoi(argv[2]);
            //tCompress array keeps track of how many characters should each thread compress, it will be located on the
            //Heap because all thread needs an access to calculate starting index from where thread will start.
            int *tCompress = (int *) malloc(sizeof(int)*threadP);
            //Get the modulus 
            int modulus = totalChar % threadP;
            //Get the remainder 
            int div = totalChar/threadP;
            //Set the values of array to zero
            int n;
            for(n=0; n<threadP;n++) {
              tCompress[n] = 0;
            }
            //They divide unevenly
            if(modulus != 0){
              int i;
              for(i=0; modulus>0; i++){
                tCompress[i]=1;
                modulus --;
              }
              for(i=0;i<threadP;i++){
                tCompress[i] += div;
              }
            } else {
              //Everything divides evenly
              int i;
              for(i=0;i<threadP;i++){
                tCompress[i] += div;
              }
            }
           //Array of ints that will tell the starting point of each threads.
           int *tStartingIndex = (int *) malloc( sizeof(int) * threadP ) ; 
           tStartingIndex[0] = 1 ;
           int j;
           for(j=1;j<threadP;j++) {
              tStartingIndex[j] = tStartingIndex[j-1] + tCompress[j-1];
            }
           //Array of pthread_t
           pthread_t tArray[threadP]  ; 
           int t ; 
            for ( t = 0 ; t < threadP ; t++ ) {
              //void compressionFun(char *fName, int thTotalChar, int startingIndex, int threadNum)
              struct tInfo *ti = malloc(sizeof(struct tInfo));
              ti->fName = argv[1];
              ti->thTotalChar = tCompress[t];
              ti->startingIndex = tStartingIndex[t];
              ti->threadNum = t;
              ti->parts = threadP;
              //Throws error if the thread is not created
              if(pthread_create (&tArray[t],NULL,compressionFun,(void *)ti) !=0) {
                printf("Worker thread[%d] not created!\n", t);
                return -1;
              } 
            }
            free(tCompress);
            free(tStartingIndex);
            int w;
            for(w=0; w<threadP; w++){
              if(pthread_join(tArray[w], NULL)!=0){
                //Throws an error is the thread has not joined
                printf("Thread[thread#] has not joined!\n");
              }
            }
          } else {
            //Error if the number of parts or threads is not valid
            printf("Invalid number of threads!\n");
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
