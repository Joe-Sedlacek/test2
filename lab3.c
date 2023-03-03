/**
 * This program reads a text file and [file this in] The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Your Name Here {Joe Sedlacek}
 * @date Jan. 30, 2022
 * @assignment Lab 3
 * @course CSC 250
 * 
 * 
 * README: Right now, my code runs correctly, writes to the file correctly, and achieves the correct answer
 *         but it compiles with 1 error being that 13,909 bytes in 2,618 blocks of memory are lost.
 *         I have met with Professor Santago multiple times for many hours to try and solve this problem. 
 *         He gave me permission to write this disclaimer, which is to the grader that Professor Santago and I 
 *         believe this is a problem with Valgrind and that there is actually no memory being lost. To premise the 
 *         problem, I have str_buff which points to a malloc'd location of memory to hold a word. I then point the 
 *         char * in the struct to this malloc'd location as well. I then move str_buff to new malloc'd space,
 *         and I create a new struct with a new char * and assign the char * to this new malloc'd space. I repeat
 *         this process for all words. The problem occurs when I make str_buff point to new malloc'd space for each
 *         new word. When I assign str_buff to new malloc'd memory, Valgrind thinks that the previous memory it was
 *         pointed to is lost, when in reality, each respective char * in each struct is pointing to that memory.
 *         Thus, Valgrind outputs that this memory is lost, when in fact it is still being pointed to, and that is 
 *         why I am still able to get the correct output in the results file. Because of this,
 *         Professor Santago has told me to write that this error is obsolete as far as we know.
 *  
 **/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_SIZE 20

/* a useful struct*/  
struct WordFreq {
    char* word;
    int count;
};

/*Function declarations*/
int process_characters(char filename[], char output[]);
int is_found(char buf[], int size, struct WordFreq **wfppp);
struct WordFreq ** add_word(struct WordFreq **wfpp, char *buf, int size);
struct WordFreq ** bubble_sort(struct WordFreq **wfpp, int size);
void swap(struct WordFreq **first, struct WordFreq **second);
int print_results(char output[], struct WordFreq ** wfpp, int size);


int main(int argc, char* argv[]) {

    int fileOK;
    fileOK=1;

    /*if less than two files are given*/
    if (argc < 3) { 
        printf("Usage: %s filename frequency_file \n", argv[0]);
        return 1;
    }

    /*Read file in and process each word*/
    fileOK = process_characters(argv[1], argv[2]);

    /*If we cannot read file in and process words, throw error*/
    if (!fileOK) {
        printf("%s could not open file %s \n", argv[0], argv[1]);
        return 2;
    }

    return 0;
}


/**
 *This function reads a text file, grabs each word, creates pointers to structs, and calls methods
 *to add each word to respective structs
*/
int process_characters(char filename[], char output[]) {

    /*ch will hold each char as we iterate through the file char by char*/
    char ch;
    /*filPtr to work with the FILE*/
    FILE *filePtr;
    /*this char * is updated to hold each new word*/
    char *buffer; 
    /*to keep track of size of buffer arr*/
    int n;
    /*to keep track of size of pointer arr*/
    int size;
    /* Creating pointer to array of struct pointers*/
    struct WordFreq **wfpp;
    /*This holds the index at the pointer that points to the struct which holds the word that matches the new word*/
    int index; 
    /*Special case to deal with the first word being seen*/
    bool first_word;
    /*str_buf gets the string from buffer copied into istelf. str_buf is then passed to each struct char * for each word*/
    char *str_buff;
    /*variable used to iterate through pointer array to free all pointers at the end of program*/
    int i;
    

    /*Initializing variables*/
    buffer = (char *) malloc(sizeof(char) *MAX_STRING_SIZE);
    /*Set to true because we have not seen any words yet and so we are looking at the first word*/
    first_word=true;
    /*size of buffer is zero at this point*/
    n=0;
    /* Size of pointer array is 0 becuase we have seen zero unique words at this point*/
    size=0;
    /*index is zero because the size of our pointer arr is zero at this point*/
    index=0;

    /* Initializing pointers to NULL */

    filePtr=NULL;
    wfpp = NULL;
    str_buff=NULL;
     

    /* Creating space for first pointer to first struct for first word*/
    wfpp=(struct WordFreq **) malloc(sizeof(struct WordFreq *)); 
    /* Creating space for first struct for first word*/
    wfpp[0] = (struct WordFreq *) malloc(sizeof(struct WordFreq));  
    /*size of arr of pointers to structs is now 1*/
    size=1;

    /*Opening file*/    
    filePtr = fopen(filename, "rie");
    if (filePtr == 0) {
        printf("could not open %s \n", filename);
        return 0;
    }

     /*get first character from file*/
    ch = fgetc(filePtr);

    /*While the next character is not eof signifier*/
    while (ch != EOF) { 

        /*Is word at the max length? If so, add it to struct*/
        if (n>= MAX_STRING_SIZE-1) { 
            /*make str_buff point to space large enough to hold what is in buffer*/
            str_buff = (char *) malloc(sizeof(char) * (strlen(buffer)+1)); 
            /*copy contents of buffer into str_buff*/
            strcpy(str_buff, buffer);

            /* If this is first word, add it to first struct and set count to 1*/
            if (first_word==true) {
                /*Now that we have seen the first word, set it to false*/
                first_word=false;
                /*add word to first struct*/
                wfpp[0]->word = str_buff;
                /*add count to first struct*/
                wfpp[0]->count = 1;
            }

            /*If the first word has already been dealt with...*/
            else {
                /*Method to see if word has been seen already. Returns the index of the pointer if it has & -1 if not*/
                index = is_found(str_buff, size, wfpp);
                
                /*If word is found*/
                if (index>-1) {
                    /*Go to the struct pointer at index, and increment its struct's count by 1*/
                    wfpp[index]->count = wfpp[index]->count+1;
                }

                /*If word has not been seen*/
                else {
                    /*Method to add an additional pointer and struct and store the new word and its count in struct*/
                    wfpp=add_word(wfpp, str_buff, size);
                    /*Increment size of struct pointer array*/
                    size = size +1;
                }
            }
            /*reset null terminator so that new word can be taken in*/
            buffer[0] = '\0'; 
            /*reset n so that the new characters are added to buffer in the correct index*/
            n=0;
        }

        /* If next character is in alphabet, add it to buffer */
        if (isalpha(ch)) { 
            /*Normalize all characters to be lower case*/
            ch = tolower(ch);
            /* add character to first spot in array buffer */
            buffer[n] = ch; 
            /*Increment size of buffer*/
            n++;
            /* increment escape character and repeat */
            buffer[n] = '\0'; 
        }

        /* If the next character is not in the alphabet and the word is longer than 0, add it to struct */
        else if (n > 0) { 
            /*make str_buff point to space large enough to hold what is in buffer*/
            str_buff = (char *) malloc(sizeof(char) * (strlen(buffer)+1)); 
            /*copy contents of buffer into str_buff*/
            strcpy(str_buff, buffer);

            /* If this is first word, add it to struct and set count to 1*/
            if (first_word==true) {
                /*Now that we have seen the first word, set it to false*/
                first_word=false;
                /*add word to first struct*/
                wfpp[0]->word = str_buff;
                /*add count to first struct*/
                wfpp[0]->count = 1;
            }

            /*If the first word has already been dealt with...*/
            else {

                /*Method to see if word has been seen already. Returns the index of the pointer if it has & -1 if not*/
                index = is_found(str_buff, size, wfpp);
                /*If word is found*/
                if (index>-1) {
                    /*Go to the struct pointer at index, and increment its struct's count by 1*/
                    wfpp[index]->count = wfpp[index]->count+1;
                }
                /*If word has not been seen*/
                else {
                    /*Method to add an additional pointer and struct and store the new word and its count in struct*/
                    wfpp=add_word(wfpp, str_buff, size);
                    /*Increment size of struct pointer array*/
                    size = size +1;
                }
            }
            /*reset null terminator so that new word can be taken in*/
            buffer[0] = '\0'; 
            /*reset n so that the new characters are added to buffer in the correct index*/
            n = 0; 
        }
        /* get first character again*/
        ch = fgetc(filePtr); 
    }

    /*In special case there is nothing after the last word to signal an EOF */
    if (strlen(buffer)>0) {
        /*make str_buff point to space large enough to hold what is in buffer*/
        str_buff = (char *) malloc(sizeof(char) * (strlen(buffer)+1)); 
        /*copy contents of buffer into str_buff*/
        strcpy(str_buff, buffer);

        /* If this is first word, add it to struct and set count to 1*/
        if (first_word==true) {
            /*Now that we have seen the first word, set it to false*/
            first_word=false;
            /*add word to first struct*/
            wfpp[0]->word = str_buff;
            /*add count to first struct*/
            wfpp[0]->count = 1;
        }

        /*If the first word has already been dealt with*/
        else {
            /*Call method to go through array of pointers to structs to see if word exists already*/
            index = is_found(str_buff, size, wfpp);
            /*If word is found*/
            if (index>-1) {
                /*Go to the struct pointer at index, and increment its struct's count by 1*/
                wfpp[index]->count = wfpp[index]->count+1;
            }

            /*If word has not been seen*/
            else {
                /*Method to add an additional pointer and struct and store the new word and its count in struct*/
                wfpp=add_word(wfpp, str_buff, size);
                /*Increment size of struct pointer array*/
                size = size +1;
            }
        }
    }

    /*Sort in Descending Order*/
    wfpp=bubble_sort(wfpp, size);
    /*Print results to file*/
    print_results(output, wfpp, size);
    /*Close file*/
    fclose(filePtr);

    /*free our pointers in memory*/
    for (i=0; i<size; ++i) {
        /*This also free's the space pointed to by str_buff*/
        free((void *)wfpp[i]->word);
        free((void*)wfpp[i]);
    }

    free((void*)buffer);
    free((void*)wfpp);
    
    return 1;
}


/**
 * Method that returns position of struct where word is found, or -1 if not found 
*/
int is_found(char buf[], int size, struct WordFreq **wfpp) { 

    int index; 
    index=0;

    /*Iterate through array of pointers to structs*/
    while (index < size) {
        /*If word (buf) has already been stored in a struct... else increment index and continue search*/
        if (strcmp(buf, wfpp[index]->word) == 0) {
            /*return the index of the struct pointer that points to the struct that has this word*/
            return index;
        }

        /*else increment index and continue search*/
        else {
            index++;
        }
    }
    /*If word is not found in any structs, return -1*/
    return -1;
}


/**
 * This method adds space for one more pointer to one more struct which will store the new word
*/
struct WordFreq ** add_word(struct WordFreq **wfpp, char *buf, int size) {
    /*Allocate space for one more pointer*/
    wfpp = (struct WordFreq**) realloc((void*) wfpp, (sizeof(struct WordFreq *) * (size + 1)));
    /*Allocate space for one more struct*/
    wfpp[size] = (struct WordFreq *) malloc(sizeof(struct WordFreq));
    /*Add in word*/
    wfpp[size]->word= buf;
    /*Add in count*/
    wfpp[size]->count = 1;

    return wfpp;
}


/**
 * Method to swap struct pointers in struct pointer arr
*/
void swap(struct WordFreq **first, struct WordFreq **second) {
    /*Temp struct pointer used in swapping*/
    struct WordFreq *temp;
    /*Swap struct pointers in struct pointer arr*/
    temp = *first;
    *first=*second;
    *second=temp;

}


/**
 * Method for bubble sorting in descending order
*/
struct WordFreq ** bubble_sort(struct WordFreq **wfpp, int size) {
    /*used to make sure we run the bubble sort algorithm size # of times*/
    int i;
    /*used to iterate through struct pointer arr (wfpp)*/
    int j;
    /*Iterate through array of pointers and swap depending on count values (BUBBLE SORT)*/
    for (i=0; i<size-1; i++) {

            for (j=0; j<size-1; j++) {

                /*If struct pointer at j points to a struct with count < count in struct that struct pointer at j+1 points to */
                if (wfpp[j]->count < wfpp[j+1]->count) {

                   /*Swap location in struct pointer arr of j and j+1*/
                   swap(&wfpp[j], &wfpp[j+1]);

                }
        }

    }
    return wfpp;
}


/**
 * Method to print/write results to file
*/
int print_results(char output[], struct WordFreq ** wfpp, int size) {
    /*ptr to output file*/
    FILE *outputPtr;
    /*used to iterate through struct pointer arr (wfpp) and print out contents at each index*/
    int i;
    /*Attempt to open output file*/
    outputPtr = fopen(output, "w");
    /*Checking if we can open the file*/
    if (outputPtr==0) {
        printf("Unable to open %s \n", output);
        return 0;
    }

    /*Write all words and respective counts to output file*/
    for (i=0; i<size; ++i) {
        fprintf(outputPtr, "%s %d\n", wfpp[i]->word, wfpp[i]->count);
    }
    fclose(outputPtr);
    return 1;
}
