#include<stdio.h>
float calcsales (int units,float price);
void AddTostock(char *itemName,int quantity, float price);
void genreports();
void createreceipt(char *itemName,int quantity,float price);
char ch;
int totalquantity,quantity,units;
float totalvalue,price;
char itemName;
void main()
 {
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
               printf("Enter the units sold\n");
               scanf("%d",&units);
               printf("Enter the price for each item\n");
               scanf("%f",&price);
               printf("The Total sales is : %.2f\n" ,calcsales(units,price));
              break;
              case 2:
                     AddTostock(&itemName,quantity,price);
              break;
              case 3:
              genreports();
              break;
              case 4:
                     createreceipt(&itemName,quantity,price);
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
float calcsales (int units,float price)
{
  int sales=0;
  sales=units*price;
  return sales;
}
void AddTostock(char *itemName,int quantity,float price)
{
    printf("Enter name of the item\n");
    scanf("%s",itemName);
    printf("Enter the quantity available\n");
    scanf("%d",&quantity);
    printf("Enter the price\n");
    scanf("%f",&price);
}
void genreports()
{
  int product1st0ck=200;
  int product2st0ck=100;
  int product3st0ck=150;
  int itemssold=400;
  int profit=5000;
  printf("The following is a list of products available and their quantities\n");
  printf("Product 1 :%d\n",product1st0ck);
  printf("Product 2 :%d\n",product2st0ck);
  printf("Product 3 :%d\n",product3st0ck);
  printf("Total items sold this week is :%d\n",itemssold);
  printf("The profit this week is :%d\n",profit);
}
void createreceipt(char *itemName,int quantity,float price)
{
    char *itemname="hp";
  float totalCoast;
  float tax;
  float finalPrice;
  
   printf("Enter the quantity sold\n");
   scanf("%d",&quantity);
   printf("Enter the price for each item\n");
   scanf("%f",&price);
   totalCoast=quantity*price;
   tax=totalCoast*0.16;
   finalPrice=totalCoast+tax;
   printf("Receipt\n");
   printf("---------------------------\n");
   printf("Item:%s\n",itemname);
   printf("Quantity:%d\n",quantity);
   printf("Price:%.2f\n",price);
   printf("Total cost:%.2f\n",totalCoast);
   printf("Tax:%.2f\n",tax);
   printf("Final Price:%.2f\n",finalPrice);
}

