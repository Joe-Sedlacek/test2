[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10135216&assignment_repo_type=AssignmentRepo)
# Lab 3 - Word Count Problem Description

Joe Sedlacek

Be sure to run the following command after creating your program file **lab3.c** to have Git track your source code file and upload your program to your repository for grading. **Do not** commit and upload and binary files to your assignment's repository.

```
git add lab3.c
```

Understanding the frequency of words used in a body of text can be useful for various purposes. For example given a text, word frequency analysis is often used to determine authorship or the nature of the text. For this lab you will write a program to determine the word frequency of a text file.

## Word Count

We are interested in determining the word frequency (a count of the number of times a word is used) for a text file. The program will be executed with two file names as command line arguments. If an insufficient number of arguments are provided, Then give an appropriate error message and exit the program. The first file name (command line argument) will be the text file to process (let's call it the input file), while the second file store the results (let's call it the output file). If the input file does not exist or cannot be opened, print an appropriate error message and exit the program. There is no need to test the existence of the second file (it will be acceptable to create the output file if necessary). In the example below, a.out is the executable, roll.txt is the input file, and roll.frq is the output file.

```
> ./a.out roll.txt roll.frq 
roll.txt has 1269 unique words
```

If a valid input file is provided, the program will read the file and determine the frequency (number of appearances) every word found. Once the word frequencies have been determined, the program will print the number of words found to the screen and write the word frequencies to the output file (given as a command line argument). The output file should contain one line per unique word found, where each line has the word, a space, then the count. The word frequencies must be sorted in descending order based on the frequencies. For example, assume the input file is roll.txt then the first ten lines of the output file would contain the following.

```
the 215
a 98
up 93
to 85
of 80
april 78
retrieved 77
jump 66
astley 59
on 55     
```

## Program Design

Although word frequency is a simple task to describe, dealing with C-strings (as you found-out in CSC 112, maybe?) can be problematic. Your program must adhere to the following program design requirements.

## Reading and Processing Words

Reading the file and processing words must be done in a separate function. Once the input file has been successfully opened, read each character one at a time from the file and build C-strings.

It's easiest to consider the characters in the file as a long array that your program will process one character at a time. Once a word delimiter is encountered, the preceding characters form a word and should be processed. Once the word has been processed (frequency determined), then the process starts over and the next word that is built from the characters that follow.

## What is a word?

Considering a file can be thought of as list of characters, assume a word is delineated (something that indicates the end) by any non-letter character; therefore, a word is **any** collection of contiguous letters. In addition, the case of the letter does **not** matter, so convert all letters to **lower case**. For example, consider the following file contents

```
thiS :)  is, a 
   gr8 lab. OK?
```

The words in this file would be "this", "is", "a", "gr", "lab", and "ok"

## Data Structures

Your program will build a list of word frequencies based on the words encountered in the input file. In this situation, it might be a good idea to create a struct that stores a C-string (for a word) and count (for the frequency of that word). Note the C-string for the word must be dynamically allocated; therefore the struct would be as follows.

```
struct WordFreq {
   char* word;  /* the word */
   int count;   /* the word frequency */
}; 
```

You cannot assume a maximum number of possible words, therefore the frequency list **must be dynamically allocated**. For example, in the main you would declare your word frequency list as follows

```
int main(int argc, char* argv[]) {
   WordFreq* wordList = NULL;
   int numWords = 0;

   /* blah, blah, blah, ... */
}
```

As a result of dynamically allocated arrays, your program must reallocate the word_list array every-time a new word is encountered, as well as deleting the old list. Remember **no memory leaks** are allowed in your program. Use valgrind (described below) to test for memory leaks.

You are allowed to use **one static array** as a buffer to build C-strings (storing characters from the file as they are read). The maximum length is 20; therefore, we'll assume the maximum C-string has 19 characters (plus one for the null terminator). If a word has more than 19 characters, simply divide into multiple words (none greater than 19 characters).

## Special Cases

Your program is expected to handle all types of ASCII text files appropriately regardless of size. Make certain you test with files with various ASCII contents.

## Program Requirements

Your program must be written in ANSI C, compile cleanly (no errors or warnings with compiler flags -ansi -pedantic -Wall), pass the formatting check, and pass the tidy check using the makefile supplied. Note to run the format check enter command (at the terminal) 'make format', to run the tidy check enter (at the terminal) 'make tidy' and to compile just enter (at the terminal) 'make' The makefile is part of the git repo you cloned for this lab (as well as roll.txt used in the example above).

Your program **must be called** lab3.c (containing main and other functions) must adhere to the following program design requirements.

- Must meet the C Programming Style guideline for this course
- Must compile cleanly using -ansi -pedantic -Wall -g flags with gcc (note the new flag -g) (-5 per issue)
- Must pass clang-tidy check specified in the makefile (no user-code warnings or errors allowed, -5 per issue)
- Must pass valgrind check specified in this assignment
- No global variables
- All lists must be dynamically allocated except for one fixed size buffer for processing characters
- main function should be limited to variable declarations and function calls
- Divide you program into an appropriate and useful number of functions
- Limit the number of exit points in your program, write code that causes exits only in the the main function
