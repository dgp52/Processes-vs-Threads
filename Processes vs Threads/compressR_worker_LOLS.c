//Name: Deepkumar Patel, NetID: dgp52
//Name: Viraj Patel, NetID: vjp60

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
//Used for timetest purposes
//#include <time.h>

int main (int argc, char *argv[]){
  //Used for timetest purposes
  //clock_t start = clock(), diff;

	//fName is the name of uncompressed file 
	//proTotalchar is the number of characters this worker process will compress
	//StartingIndex is the number/index at which this worker process will start compressing
	//ProNum is the count during the child process creation, this will help append the output file name
	//parts is the number of child processes
   char *fName = argv[1];
   int proTotalChar =  atoi(argv[2]);
   int startingIndex = atoi(argv[3]);
   int proNum = atoi(argv[4]);
   int parts = atoi(argv[5]);
  //Create the file name as required in the project
  //Create a buffer to get its length
  char str[200];
  //If theres only 1 process then we dont need to append the number
  if(parts != 1) {
    snprintf(str, 100, "%d" , proNum);
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
  if(parts != 1){
    strcat(outputFile, str);
  }  
  strcat(outputFile, ".txt") ;
  //Open the file using the fName
  FILE *file = fopen(fName ,"r");
  //Total number of characters this worker process will compress 
  int totalc =  proTotalChar;
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

  //Used for timetest purposes
  //diff = clock() - start;
  //int msec = diff*1000 / CLOCKS_PER_SEC;
  //printf("%d.%d+\n",msec/1000,msec%1000);
  return 0;
}
