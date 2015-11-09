#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* the below is a C console interpreter for 3var.
   It includes several commands:
   .h - help: lists these commands.
   .d - debug: shows the values of a, b, & r.
   .R - reset: resets all variables to their default values.
   .q - quit: quit the console.
   
   It is maybe a bit messy right now.
   
   > nikoraito@gmail.com
   
   */

//using namespace std;

int main(int argc, char *argv[]) {

  long int a = 0;
  long int b = 0;
  long int r = 0;

  int cond = 0;
  int loop = 0;
  int lev = 0;
  int i = 0;
  int j = 0;

  char c;

  char in[256];

  int debug = 0;
  int running = 1;

  printf("This is a C++ 3var console based off of the original\n");
  printf("3var interpreter by LuaGeek2412. Enter > .h for help.\n");
  printf("To run a 3var file, please use:\n\n\t3var <filename>.");
  printf("\n\nType your commands:\n");
  
  while(running){

    printf ("\n> ");
    
    fgets (in, 256, stdin );
    
    for(j = 0; j < strlen(in); j++ ){

        c = in[j];
        
        switch (c) {
          case '.':
            c = in[++j];
            switch (c){
              case 'h':
                printf(".q - QUIT: quit 3Var console\n");
                printf(".d - TOGGLE DEBUG: print the values of A, B,\n and R after every line entered.\n");
                printf(".R - RESET VARS: resets all variables to their default values.\n");
                printf(".h - HELP: displays this list.\n");
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

              case 'q':
                running = 0;
                break;

              case 'R':
                printf("Reset all values? (y/n)\n>> ");
                
                if(getchar() == 'y'){
                  printf("Reseting goddamn EVERYTHING.");
                  a = 0;
                  b = 0;
                  r = 0;
                  i = 0;
                  j = 0;
                  lev = 0;
                  loop = 0;
                  cond = 0;
                }
                
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

          case 'w': // Prints R
            printf("%ld\n", r);
            break;

          case 'u': // Marks a conditional which executes only if a>b
            cond = a > b;
            break;

          case 'U': // Marks a conditional which executes only if a<b
            cond = a < b;
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
            r = getchar();
            break;

          case '"': // Takes a number input from the user and stores it in R
            printf("< ");
            scanf("%ld", &r);
            break;


          case 'r': // Resets all variables to 0
            a = 0;
            b = 0;
            r = 0;
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
      
      if(debug){
        printf("a = %ld %c\n", a, (char)a);
        printf("b = %ld %c\n", b, (char)b);
        printf("r = %ld %c\n", r, (char)r);
      }

  }
  
  printf("\n");
  return 0;

}
