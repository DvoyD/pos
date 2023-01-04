#include<stdio.h>

//Section to initialize functions
void clearConsole();

void main() {
  // ...
}

void clearConsole()
{
printf("\e[1;1H\e[2J");
//This pushes everything to the top of the console out of view
//However one can still revisit what they did by scrolling up in the terminal
}
