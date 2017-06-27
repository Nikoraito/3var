#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* the below is a C console interpreter for 3varx.
   It includes several commands:
   .h - help: lists these commands.
   .d - debug: shows the values of a, b, & r.
   .R - reset: resets all variables to their default values.
   .q - quit: quit the console.
   
   It is maybe a bit messy right now.
   
   3varx is a dialect of 3var with some extensions.
   Any 3var code will run equally in this interpreter,
   but several new instructions are added:
   
   z - generates a pseudorandom integer and stores it in r.
   
   > nikoraito@gmail.com
   
   PLEASE NOTE:
   I only modified much of the code here to work as a console.
   Credit for the original interpreter goes to Olls.
   All of the extended commands, however, are my
   original work.
   
   */

//using namespace std;

char * LLToString(long long int n){
  char * output;
  int i;
  for (i = 0; i < 8; i++){
      *(output + i) = (char)( (n & (0x7F<<(8*i))) >> 8);
  }
  return output;
  
}

int main(int argc, char *argv[]) {
    FILE * fp;

  FILE * destout = stdout;
  FILE * sourcein = stdin;
  char pmode = 0;
    
  char filename[8];

  int safe = 0;


  long long int a = 0;
  long long int b = 0;
  long long int r = 0;

  int cond = 0;
  int loop = 0;
  int lev = 0;
  int i = 0;
  int j = 0;

  int ch = 0;

  char c;

  srand(time(NULL));

  char in[256];

  long int stack[8] = {0,0,0,0,0,0,0,0}; // stack of 8 long numbers
  int spointer = 0;                      // pointer to the stack's top

  int debug = 0;
  int running = 1;

  printf("This is a C++ 3var console based off of the original\n");
  printf("3var interpreter by Olls. Enter > .h for help.      \n");
  printf("To run a 3var file, please use:\n\n\t3var <filename>.\n");
  printf("\nType your commands:\n");
  
  while(running){
    



    printf ("\n> "); //Print a nice little shell symbol
    
    fgets (in, 256, sourcein); //Get the next 256 characters the user enters
    
    for(j = 0; j < strlen(in); j++ ){
        c = in[j];
        switch (c) {
          case '.':
            c = in[++j];
            switch (c){
              case 'h':
                printf(".q - QUIT: quit 3Var console\n");
                printf(".d - TOGGLE DEBUG: print the values of A, B,\n and R after every line entered.\n");
                printf(".R - RESET VARS: reset all variables including the track to their default values.\n");
                printf(".h - HELP: display this list.\n");
                printf(".s - TOGGLE SAFETY: on by default. Stop command ! if it creates more than 8 files.\n");
                printf(".c - COMMANDS: display a list of the 3var instructions.\n");
                printf(".x - EXTENSIONS: display a list of the additional, 3var-x instructions.\n");
                break;

              case 'd':
                if(debug){
                  printf(">> Debugging disabled.\n");
                  debug = 0;
                } else {
                  printf(">> Debugging enabled.\n");
                  debug = 1;
                }
                break;
                
              case'c':
                printf("Standard 3var instructions                    \n");
                printf("----------------A & B COMMANDS----------------\n");
                printf(" i : a++            a : b++                   \n");
	              printf(" d : a--            k : b--                   \n");
	              printf(" s : a*=a           m : b*=b                  \n");
	              printf(" p : print a        o : print b               \n");
	              printf(" P : print (char)a  O : print (char) b        \n");
	              printf(" x : a=abs(a)       0 : b=abs(b)              \n");
	              printf(" > : a = r          < : b = r                 \n");
	              printf(" @ : a = 0          # : b = 0                 \n");
	              printf("                                              \n");
                printf("------------------R COMMANDS------------------\n");
                printf(" + : r = a+b        ^ : r = pow(a, b)         \n");
                printf(" - : r = a-b        ' : r = getchar()         \n");
                printf(" * : r = a*b        \" : r = scanf()           \n");
                printf(" / : r = a/b        e : r = 0                  \n");
                printf("                                              \n");
                printf("---------------Control Commands---------------\n");
                printf("  CONDITIONALS:                               \n");
                printf(" =( ) do if a=b     \\[ ] : do while a=b       \n");
                printf(" u( ) do if a>b     |[ ] : do while a>b       \n");
                printf(" U( ) do if a<b     _[ ] : do while a<b       \n");
                printf("                                              \n");
                printf("  OTHER:                                      \n");
                printf(" { } infinite loop                            \n");
                printf(" f[ ] : run a times  F[ ] : run b times       \n");
                printf("                                              \n");
                printf("----------------------------------------------\n");
                break;
                
              case 'x':
                printf("3var-x Extended instructions                  \n");
                printf("----------------I / O COMMANDS----------------\n");
                printf(" ! : Creates/opens a file with name r         \n");
	              printf(" ? : Opens a file with name r                 \n");
	              printf("                                              \n");
                printf("----------------TRACK COMMANDS----------------\n");
                //printf(" q : r = track[current] by time               \n");
                //printf(" Q : track[current] = r by time               \n");
                printf("                                              \n");
                printf("                                              \n");
                printf(" j : r = track[current] by pointer            \n");
                printf(" J : track[current] = r by pointer            \n");
                printf("----------------OTHER COMMANDS----------------\n");
                printf(" z : r = rand()     t : a = clock()           \n");
                printf("                    T : b = clock()           \n");
                printf("                                              \n");
                printf("----------------------------------------------\n");
                break;
              case 'q':
                running = 0;
                break;

              case 'R':
                printf("Reset all values? (y/n)\n>> ");
                
                char inc = getchar();
                
                if(inc == 'y' || inc == 'Y'){
                  a = 0;
                  b = 0;
                  r = 0;
                  i = 0;
                  j = 0;
                  lev = 0;
                  loop = 0;
                  cond = 0;
                  ch = 0;
                  spointer = 0;

                  for(i = 0; i < sizeof stack; i++){
                        stack[i] = 0;
                  }
                  
                  printf("\nAll variables reset.\n");

                }
                else if (inc == 'n' || inc == 'N'){
                  printf("Variables not reset.\n");
                }
                getchar();
                break;
                
            }
            break;
            
          case 'i': // Increments A
            a++;
            break;

          case 'd': // Decrements A
            a--;
            break;

          case 's': // Squares A
            a *= a;
            break;

          case 'p': // Prints A
            printf("%ld\n", a);
            break;

          case 'P': // Prints the ASCII character associated with A
            printf("%c", (char)a);
            ch = 1;
            break;

          case 'x': // Sets A to the absolute value of A
            a = abs(a);
            break;

          case '>': // Sets A to R
            a = r;
            break;

          case 'a': // Increments B
            b++;
            break;

          case 'k': // Decrements B
            b--;
            break;

          case 'm': // Squares B
            b *= b;
            break;

          case 'o': // Prints B
            printf("%ld\n", b);
            break;

          case 'O': // Prints the ASCII character of B
            printf("%c", (char)b);
            ch = 1;
            break;

          case '0': // Sets B to the absolute value of B
            b = abs(b);
            break;

          case '<': // Sets B to R
            b = r;
            break;

          case '+': // Adds A and B, stores in R
            r = a + b;
            break;

          case '-': // Subtracts B from A, stores in R
            r = a - b;
            break;

          case '*': // Multiplies A and B, stores in R
            r = a * b;
            break;

          case '/': // Divides A by B, stores in R
            r = a / b;
            break;

          case '^': // Exponentiates A to the Bth power, stores in R
            r = pow(a, b);
            break;
            
          case 'T': // XXX 3var-x extention - sets r to the time in seconds
            r = clock();
            break;
            
          case 't': // XXX 3var-x extention - sets A to the time in ticks
            r = time(NULL);
            break;
            
          case 'z': // XXX 3varx extension - sets r to a pseudorandom number with seed (a+b)
            r = rand();
            break;
            
          case 'w': // Prints R
            printf("%ld\n", r);
            break;
            
            ////////////////////////// TRACK = 8 values that act as a stack that willfully overwrites its own values rather than overflow.
            ////////////////////////// TIMED BASED ON TIME IN SECONDS MOD 8? OH MY GOD.

          //case 'q': // pops a number off the track into r based on time in seconds % 8
          //  //stack[time(NULL)%8] = r;
          //  r = stack[clock()%8];
          //  break;
          
          //case 'Q': // pushes a number from r onto the track based on the time in seconds % 8
          //  //r = stack[time(NULL)%8];
          //  stack[clock()%8] = r;
          //  break;

          case 'j': // pops a number off the stack into r
            stack[spointer] = 0;
            r = stack[spointer--];
            if(spointer < 0) spointer = 7; // make this one integer operation instead of a decision?
            break;
            
          case 'J': // pushes the value of r onto the stack
            stack[spointer++] = r;
            spointer %= 8;
            break;
          
          case '!': // XXX 3var-x FILE I/O: Creates/opens a file named r
            *filename = *(LLToString(r));
            destout = fopen(strcat(filename, ".txt"), "a+");
            fprintf(destout, "Testing...\n");
            break;
            
          case '?': // XXX 3var-x FILE I/O: Toggles between reading and writing to and from files and the console.
               *filename = *(LLToString(r));
              if(destout != stdout){
                destout = stdout;
                printf("Switching to Console output");
              } // Prints a "(File)" above output when files are written/read from a file
              else {
                destout = fopen(strcat(filename, ".txt"), "a+");
                printf("Switching to File output");
              }
            break;
          case 'u': // Marks a conditional which executes only if a>b
            cond = a > b;
            break;

          case 'U': // Marks a conditional which executes only if a<b
            cond = a < b;
            break;
            
          case 'L':
               a = a << b;
               break;
          case 'R': 
               a = a >> b;
               break;
          case '&':
               r = a & b;
               break;
          case 'X':
               r = a ^ b;
               break;
          case 'I':
               r = a | b;
               break;
          case 'n': 
               a = ~a;
               break;
          case 'N':
               b = ~b;
               break;

          case '=': // Marks a conditional which executes only if a==b
            cond = a == b;
            break;

          case '(': // Starts an if statement body
            if (!cond) {
              lev = 1;
              while (lev && ++j < strlen(in)){ //As long there's another value in program...

                c = in[j];

                switch (c) {
                  case '(':
                    lev++;
                    break;
                  case ')':
                    lev--;
                    break;
                    
                }
              }
              if (lev) {
                printf("Error: conditionals not nested properly.\n");
                lev =0;
              }
            }
            break;

          case ')': // Ends an if statement body
            break;

          case '|': // Marks a loop which executes while a>b
            loop = '|';
            break;

          case '_': // Marks a loop which executes while a<b
            loop = '_';
            break;

          case '\\': // Marks a loop which executes while a==b
            loop = '\\';
            break;

          case 'f': // Marks a loop which executes a times
            loop = 'f';
            break;

          case 'F': // Marks a loop which executes B times
            loop = 'F';
            break;

          case '{': // Starts a loop which repeats forever
            break;

          case '}': // Ends a loop which repeats forever
            
            for (lev = 1; lev && (--j >= 0);){
              c = in[j];
              switch (c) {
                case '}':
                  lev++;
                  break;
                case '{':
                  lev--;
                  break;
              }
            }
            if (lev) {
              printf("Error: loops not nested properly.\n");
              printf("Please make sure loops are written on one line.\n");
              lev = 0;
            }
            break;

          case '[': // Starts a marked loop
            i = 0;
            break;

          case ']': // Ends a marked loop
            switch (loop) {
              case '|':
                cond = a > b;
                break;
              case '_':
                cond = a < b;
                break;
              case '\\':
                cond = a == b;
                break;
              case 'f':
                cond = i++ < a;
                break;
              case 'F':
                cond = i++ < b;
                break;
            }
            if (cond) {
              for (lev = 1; lev && (--j >= 0);) { //while j is within array
                c = in[j];
                switch (c) {
                  case ']':
                    lev++;
                    break;
                  case '[':
                    lev--;
                    break;
                }
              }
              if (lev) {
                printf("Error: loops not nested properly.\n");
                printf("Please make sure loops are written on one line.");
                lev = 0;
              }
            }
            break;

          case '\'': // Takes a 1 character input from the user and stores the ASCII value of the character in R

            printf("< ");
            r = fgetc(sourcein);
            break;

          case '"': // Takes a number input from the user and stores it in R
            printf("< ");
            scanf("%ld", &r);
            break;


          case 'r': // Resets all variables to 0
            a = 0;
            b = 0;
            r = 0;
            //stack = {0,0,0,0,0,0,0,0};
            break;

          case '@': // Resets A to 0
            a = 0;
            break;

          case '#': // Rests B to 0
            b = 0;
            break;

          case 'e': // Resets R to 0
            r = 0;
            break;
        }

      }
      
      if(ch){
        printf("\n");  //If the output contains any characters, give them extra space
        ch = 0;
      }
      
      if(debug){
        printf("a = %10ld %c\t%10ld %10ld \n", a, (char)a, stack[0], stack[4]);
        printf("b = %10ld %c\t%10ld %10ld \n", b, (char)b, stack[1], stack[5]);
        printf("r = %10ld %c\t%10ld %10ld \n", r, (char)r, stack[2], stack[6]);
        printf("                \t%10ld %10ld \n",         stack[3], stack[7]);
      }

  }
  
  printf("\n");
  return 0;

}
