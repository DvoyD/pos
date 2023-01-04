#include<stdio.h>
//The structure that will be used to store item information
typedef struct items{
//item code - item name - price -amount -buying price
int itemCode;
char itemName[100];
float price;
int amount;
float buyingPrice;
}items;

//Section to initialize functions
void clearConsole();//To clear the console and create a 'refresh' effect
void stockManagement();//The function that will enable the user to view and edit the inventory
void updateStocks();//To update the number of particular items in the stock
void viewInventory();//Exactly what the function is called -it is to view the inventory
void clearConsole();//To clear the console
void removeItems();//To remove unwanted items
void addItems();//To add more items to the stocks
void fileManagement();//To create files needed for the program
int checkItem(int code);//To check whether an item exists
int getNumItems();//To get the number of items in the inventory

//Main function where the program begins execution
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
                  clearConsole();
                  stockManagement();
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

void stockManagement()
{
    fileManagement();//To create all needed files if they are not present
    printf("\t\t Stock Management  \n");
    printf("1:Update Stocks \n");
    printf("2:Remove Items \n");
    printf("3:Add new Item \n");
    printf("4:View Inventory \n");
    printf("5:Back\n");
    int stockOption;
    scanf("%d",&stockOption);
    switch(stockOption)
    {
        case 1:
            updateStocks();
        break;
        case 2:
            removeItems();
        break;
        case 3:
            addItems();
        break;
        case 4:
            viewInventory();
        break;
        case 5:
            clearConsole();
            //It has nothing because the menu is still running in the loop
        break;
        default:
            printf("Choice does not exist \n\n");
            clearConsole();
            stockManagement();
        break;
    }
    printf("\n");
}

void updateStocks()
{
 //This is the section to update the number of items of a particular type
 FILE *fp=fopen("stock.dat","a+");
 //To create an array to store each record
 int numItems=getNumItems();
 int position=0;
 //To create an array of structures for the records
 struct items itemList[numItems];
 printf("Enter the Item code \n");
 int code;
 int restock;
 scanf("%d",&code);
 if(checkItem(code)==1)
 {
    printf("Enter the restock amount \n");
    scanf("%d",&restock);
    //To read all records into the array
    while(fread(&itemList[position],sizeof(struct items),1,fp))
    {
        if(itemList[position].itemCode==code)
        {
        printf("You have chosen to update %s which has %d peices \n",itemList[position].itemName,itemList[position].amount);
        itemList[position].buyingPrice+=(itemList[position].buyingPrice/itemList[position].amount)*restock;
        itemList[position].amount+=restock;
        printf("Amount has been updated to %d \n",itemList[position].amount);
        }
        position++;
    }
    fclose(fp);
    FILE *fp=fopen("stock.dat","w");
    //To write the new info into the file
    for(int i=0;i<numItems;i++)
    {
        fwrite(&itemList[i],sizeof(struct items),1,fp);
    }
    fclose(fp);
    printf("Update another item? y/n \n");
    getchar();
    char response;
    scanf("%c",&response);
    if(response=='Y' || response=='y')
    {
      updateStocks();
    }
 }
 else{
    if(numItems>=1){
    printf("The Item code does not exist. Please try again \n");
    updateStocks();
    }
    else{
        clearConsole();
        printf("It seems you have nothing in the inventory yet.Add Items first in order to begin managing them \n");
        stockManagement();
    }
 }
 fclose(fp);
}

void removeItems()
{
 //This is the section to remove the items of a particular type
 FILE *fp=fopen("stock.dat","a+");
 //To create an array to store each record
 int numItems=getNumItems();
 int position=0;
 //To create an array of structures for the records
 struct items itemList[numItems];
 printf("Enter the Item code \n");
 int code;
 scanf("%d",&code);
    if(checkItem(code)==1)
    {
    //To read all records into the array
    while(fread(&itemList[position],sizeof(struct items),1,fp))
    {
        position++;
    }
    fclose(fp);
    FILE *fp=fopen("stock.dat","w");
    //To write the new info into the file
    int target;
    for(int i=0;i<numItems;i++)
    {
        if(itemList[i].itemCode!=code){
        fwrite(&itemList[i],sizeof(struct items),1,fp);
        }
        else{
            target=i;
        }
    }
    fclose(fp);
    printf("%s was removed from inventory\n\n",itemList[target].itemName);
    printf("Remove another item? y/n \n");
    getchar();
    char response;
    scanf("%c",&response);
    if(response=='Y' || response=='y')
    {
      removeItems();
    }
    clearConsole();
    stockManagement();
 }
 else{
    if(numItems>=1){
    printf("The Item code does not exist. Please try again \n");
    removeItems();
    }
    else{
    clearConsole();
    printf("You do not have any items in your inventory \n");
    stockManagement();
    }
 }
 fclose(fp);
}

void addItems()
{
  char addchoice='y';
  FILE *stocks=fopen("stock.dat","a+");
  while(addchoice!='n'){
  struct items itemToBeAdded;
  printf("Please enter the following info \n");
  printf("Item code:\n");
  scanf("%d",&itemToBeAdded.itemCode);
  getchar();
  if(checkItem(itemToBeAdded.itemCode)==1)
  {
        printf("Item Exists!\n");
        getchar();
        addchoice='n';
        clearConsole();
  }
  else{
  printf("Item Name: \n");
  scanf("%[^\n]s",itemToBeAdded.itemName);
  printf("Buying Price :\n");
  scanf("%f",&itemToBeAdded.buyingPrice);
  printf("Number of piece(S) \n");
  scanf("%d",&itemToBeAdded.amount);
  printf("Please choose 1 option \n 1.Set Unit price \n 2.Set profit margin \n");

  int priceChoice;
  scanf("%d",&priceChoice);

  float profitMargin;
  float unitPrice;

  int choiceSet=0;
  while(choiceSet!=1)
  {
      switch(priceChoice)
      {
      case 1:
          printf("Enter Unit price \n");
          scanf("%f",&unitPrice);
          itemToBeAdded.price=unitPrice;
          choiceSet=1;
      break;
      case 2:
          printf("Enter profit margin (percentage) \n");
          scanf("%f",&profitMargin);
          itemToBeAdded.price=((profitMargin/100)+1)*((itemToBeAdded.buyingPrice)/(itemToBeAdded.amount));
          choiceSet=1;
      break;
      default:
      printf("Option not available \n");
      break;
      }
  }
   //To write data to the file
   fwrite(&itemToBeAdded,sizeof(struct items),1,stocks);
   printf("Item successfully added! \n");
   printf("Would you like to add another item? y/n \n");
   getchar();
   addchoice=getchar();
  }
  }
  //To close file
  fclose(stocks);
  clearConsole();
  stockManagement();
}

void fileManagement()
{
    FILE *file1=fopen("stock.dat","r");
    if(file1==NULL){ file1=fopen("stock.dat","r");}
    fclose(file1);
}

int getNumItems()
{
    char filename[]="stock.dat";
    FILE *fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File not found");
    }
    struct items myStock;
    int num=0;
    while(fread(&myStock,sizeof(struct items),1,fp))
    {
        num++;
    }
    fclose(fp);
    return num;
}

int checkItem(int code)
{
    char filename[]="stock.dat";
    FILE *fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File not found");
    }
    struct items myStock;
    int status=0;
    while(fread(&myStock,sizeof(struct items),1,fp))
    {
        if(myStock.itemCode==code)
        {
          status=1;
        }
    }
    fclose(fp);
    return status;
}

void viewInventory()
{
    fileManagement();//To make sure that there is a file to read
    clearConsole();
    struct items currentItem;
    FILE *fp=fopen("stock.dat","r");
    printf("\t\t INVENTORY \n");
    while(fread(&currentItem,sizeof(struct items),1,fp))
    {
    printf("##################\n");
    printf("Item Code:%d\n",currentItem.itemCode);
    printf("Name:%s\n",currentItem.itemName);
    printf("Amount:%d\n",currentItem.amount);
    printf("Buying Price:%.2f\n",currentItem.buyingPrice);
    printf("Selling Price(per piece):%.2f\n",currentItem.price);
    printf("Estimated Value:%.2f\n",currentItem.price*currentItem.amount);
    }
    fclose(fp);
    getchar();
    char pl;
    scanf("%c",&pl);
    clearConsole();
    stockManagement();
}

