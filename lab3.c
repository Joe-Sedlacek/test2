/**
 * This program reads a text file and [file this in] The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Your Name Here {@literal }
 * @date Jan. 30, 2022
 * @assignment Lab 3
 * @course CSC 250
 **/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 20

/* a useful struct, if you choose to use it  */  
struct WordFreq {
    char* word;
    int count;
};

/* function declarations go here */



int process_characters(char filename[]);


int main(int argc, char* argv[]){
    struct WordFreq *list = NULL;
    int num = 0;
    int fileOK = 1;

    if(argc < 3){ //if less than two files are given 
        printf("Usage: %s filename frequency_file \n", argv[0]);
        return 1;
    }

    fileOK = process_characters(argv[1]);

    if(!fileOK){
        printf("%s could not open file %s \n", argv[0], argv[1]);
        return 2;
    }

    return 0;
}


/* This function allocates and builds a string buffer from file input.
    How or if you use this function is up to you. It is only meant to demonstrate
    this process. If you use this function, you will have to modify according
    to your needs. E.g., the argument list may need to be modified, or you may
    need a different type of return value.
    You may decide to open the fie in main and pass the file pointer as an argument.
*/
int process_characters(char filename[]){
    char ch;
    FILE *filePtr;
    char buffer[MAX_STRING_SIZE]; 
    int n = 0;
    buffer[0] = '\0';
    int counter; //keep track of number of words
    
    filePtr = fopen(filename, "rie");
    if(filePtr == 0) {
        printf("could not open %s \n", filename);
        return 0;
    }

    ch = fgetc(filePtr); //get first character from file
    while(ch != EOF) { //make sure length doesn't go above 20? add in counter?
        if(n>= 19){ 
            printf("cString %s \n", buffer); //print word
            /* store the word (C-string in buffer) in dynamic list */
            buffer[0] = '\0'; //reset null terminator
            n = 0; //reset n
            counter++;


        }
        if(isalpha(ch)) { //if first character is in alphabet
            ch = tolower(ch);
            buffer[n] = ch; //add character to first spot in array buffer
            n++;
            buffer[n] = '\0'; //increment escape character and repeat
        }
        else if(n > 0){ //if not a character
            printf("cString %s \n", buffer); //print word
            /* store the word (C-string in buffer) in dynamic list */
            //CHECK When we malloc space for new string, Add null character!
            buffer[0] = '\0'; //reset null terminator
            n = 0; //reset n
            counter++;
        }
        ch = fgetc(filePtr); //get first character again
    }

    fclose(filePtr);
    return 1;
}



int isFound(char buf[], int size, struct WordFreq **wfpp){ //returns position of struct where word is found, or -1 if not found

    int index; 
    index=0;

    while(index < size){

        if(strcmp(buf, wfpp[index]->word == 0)){

            return index;
        }
        else{
            
            index++;
        }
    }



}
