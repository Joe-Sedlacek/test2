/**
 * This program reads a text file and [file this in] The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Your Name Here {Joe Sedlacek}
 * @date Jan. 30, 2022
 * @assignment Lab 3
 * @course CSC 250
 **/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_SIZE 20

/* a useful struct, if you choose to use it  */  
struct WordFreq {
    char* word;
    int count;
};

/* function declarations go here */

int process_characters(char filename[]);
int is_found(char buf[], int size, struct WordFreq **wfppp);
struct WordFreq ** add_word(struct WordFreq **wfpp, char buf[], int size);
void swap(struct WordFreq *first, struct WordFreq *second);
void bubble_sort(struct WordFreq **wfpp, int size);


int main(int argc, char* argv[]){
    int num;
    int fileOK;
    num=0;
    fileOK=1;
    

    /*if less than two files are given*/
    if(argc < 3){ 
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
    int n;
    int size;
    /* Creating pointer to array of struct pointers*/
    struct WordFreq **wfpp;
    int index; 
    bool first_word;
    char *str_buff;
    int k;

    /*Initializing variables*/
    buffer[0] = '\0';
    first_word=false;
    n=0;


    /* Initializing pointers to NULL */

    filePtr=NULL;
    wfpp = NULL;
     

    /*keep track of number of words*/
    
    filePtr = fopen(filename, "rie");
    if(filePtr == 0) {
        printf("could not open %s \n", filename);
        return 0;
    }


    /* Creating space for first pointer to first struct for first word*/
    wfpp=(struct WordFreq **) malloc(sizeof(struct WordFreq *)); 

    /* Creating space for first struct for first word*/
    wfpp[0] = (struct WordFreq *) malloc(sizeof(struct WordFreq));

    /* Size of pointer array is 1 becuase we have 1 unique word at this point*/
    size=1;

     /*get first character from file*/
    ch = fgetc(filePtr);

    /*make sure length doesn't go above 20? add in counter?*/
    while(ch != EOF) { 


    /* variable for keeping track of where we are in array of pointers */
        if(n>= 19){ 


            str_buff = (char *) malloc(sizeof(char) * MAX_STRING_SIZE); //same as above 
            strcpy(str_buff, buffer);


            /* If this is first word, add it to struct */
            if(first_word==false){

                first_word=true;
                wfpp[0]->word = str_buff;
                wfpp[0]->count = 1;


            }
            else{
            /*Call method to go through array of pointers to structs to see if word exists already*/
            index = is_found(str_buff, size, wfpp);
            
            /*If word is found, increment count by 1*/
            if(index>-1){
                wfpp[index]->count = wfpp[index]->count+1;

            }

            else{
                wfpp=add_word(wfpp, str_buff, size);
                size++;
            }
        }


            /*reset null terminator*/
            buffer[0] = '\0'; 
            
            

            /*reset n*/
            n=0;
            
            



        }

        /* if first character is in alphabet */
        if(isalpha(ch)) { 
            ch = tolower(ch);

            /* add character to first spot in array buffer */
            buffer[n] = ch; 
            n++;

            /* increment escape character and repeat */
            buffer[n] = '\0'; 
        }

        /* if not a character */
        else if(n > 0){ 


            str_buff = (char *) malloc(sizeof(char) * MAX_STRING_SIZE); //same as above 
            strcpy(str_buff, buffer);



            /* If this is first word, add it to struct*/
            if(first_word==false){
                first_word=true;

                wfpp[0]->word = str_buff;
                wfpp[0]->count = 1;


            }
            else{
            /*Call method to go through array of pointers to structs to see if word exists already*/
            index = is_found(str_buff, size, wfpp);

            /*If word is found, increment count by 1*/
            if(index>-1){
                wfpp[index]->count = wfpp[index]->count+1;

            }

            else{
                wfpp = add_word(wfpp, str_buff, size);
                size++;
            }
        }

            /* reset null terminator */
            buffer[0] = '\0'; 
            
            /* reset n */
            n = 0; 

            
        }

        /* get first character again */
        ch = fgetc(filePtr); 
    }

    printf("%d", size);

    bubble_sort(wfpp, size);

    for(k=0; k<size; k++){

        //printf("Word: %s, Count: %d \n", wfpp[k]->word, wfpp[k]->count);

    }

    fclose(filePtr);
    return 1;
}


/*returns position of struct where word is found, or -1 if not found */
int is_found(char buf[], int size, struct WordFreq **wfpp){ 

    int index; 
    index=0;

    /*Iterate through array of pointers to structs*/
    while(index < size){


        if(strcmp(buf, wfpp[index]->word) == 0){

            return index;
        }
        else{
            
            index++;
        }
    }

    return -1;

}


struct WordFreq ** add_word(struct WordFreq **wfpp, char buf[], int size){

    /*Allocate space for one more pointer*/
    wfpp = (struct WordFreq**) realloc((void*) wfpp, (sizeof(struct WordFreq *) * size) + 1);
    /*Allocate space for one more struct*/
    wfpp[size] = (struct WordFreq *) malloc(sizeof(struct WordFreq));

    /*Add in word and count*/
    wfpp[size]->word= buf;
    wfpp[size]->count = 1;

    return wfpp;
}


/*Swap pointers*/
void swap(struct WordFreq *first, struct WordFreq *second){

    struct WordFreq *temp;
    temp=NULL;

    temp = first;
    first=second;
    second=temp;

}

void bubble_sort(struct WordFreq **wfpp, int size){

    int i;
    int j;
    int g;
    
    /*Iterate through array of pointers and swap depending on count values (BUBBLE SORT)*/


    for(i=0; i<size-1; i++){


            for(j=0; j<size-1; j++){

                if(wfpp[j]->count < wfpp[j+1]->count){

                    swap(wfpp[j], wfpp[j+1]);

                }
        }

    }


}

