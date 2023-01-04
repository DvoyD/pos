#include<stdio.h>

//Section to initialize functions
void clearConsole();

void main()
 {
     char ch;
     int choice;
      do {
             printf("Menu:\n");
             printf("1.Sales\n");
             printf("2.Stock Management\n");
             printf("3.Reports\n");
             printf("4.Receipts\n");
             printf("5.Settings\n");
             scanf("%d",&choice);

             switch(choice)
             {
              case 1:
              break;
              case 2:
              break;
              case 3:
              break;
              case 4:
              break;
              case 5:
              break;
              default:
              printf("Invalid choice.Please Try Again.\n");
              
             }
             printf("Want to continue Y/N: \n");
             scanf("%s",&ch);
      }while (ch=='y'|| ch=='Y');      

}
  
void clearConsole()
{
printf("\e[1;1H\e[2J");
//This pushes everything to the top of the console out of view
//However one can still revisit what they did by scrolling up in the terminal
}
