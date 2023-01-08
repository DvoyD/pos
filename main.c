#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

//The structure that will be used to store item information
typedef struct items{
//item code - item name - price -amount -buying price
int itemCode;
char itemName[100];
float price;
int amount;
float buyingPrice;
}items;

typedef struct receipts{
char receiptCode[100];
int itemCode;
int amount;
float price;
float total;
} receipts;


typedef struct sales{
int itemCode;
float price;
float value;
int unitsSold;
} sales;


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
void viewItem();//To allow the user to see just one specific item from the inventory
void openReceipts();//To open receipts menu
void createsales();//To make sales in the program
float getPrice(int code);//To get the price of a certain item from the data file
void printItem(int code);//To get the name of the product from the item code
void updateSales(int, int);//Function to reduce number of items in stock once they are sold
void viewReceipt();//To view items in the receipt
void viewReceipts();//To view all receipts
void reports();//To show statistics of sales in the Business
void getRevenue();//To show how much profit has been made
float getBuyingPrice(int itemCode);//To get the buying price of the item
void showSales();//Function that shows items sold so far
void bestSelling();// show item that has sold the most units
void restockAlerts();//To alert user to restock in case of low stock

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
             scanf("%d",&choice);

             switch(choice)
             {
              case 1:
                  clearConsole();
                  createsales();
              break;
              case 2:
                  clearConsole();
                  stockManagement();
              break;
              case 3:
                  clearConsole();
                  reports();
              break;
              case 4:
                  clearConsole();
                  openReceipts();
              break;
              default:
              printf("Invalid choice.Please Try Again.\n");
             }
             printf("Want to continue Y/N: \n");
             scanf("%s",&ch);
             clearConsole();
      }while (ch=='y'|| ch=='Y');

}


void clearConsole()
{
//this function will clear the console is platform specific
//The macros may make the code slow on first build but will be fine afterwards
  #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void stockManagement()
{
    fileManagement();//To create all needed files if they are not present
    printf("\t\t Stock Management  \n");
    printf("1:Update Stocks \n");
    printf("2:Remove Items \n");
    printf("3:Add new Item \n");
    printf("4:View Inventory \n");
    printf("5:view Item \n");
    printf("6:Back\n");
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
            viewItem();
        break;
        case 6:
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
 struct items *itemList=malloc(sizeof(struct items)*numItems);
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
    free(itemList);
    fclose(fp);
    printf("Update another item? y/n \n");
    getchar();
    char response;
    scanf("%c",&response);
    if(response=='Y' || response=='y')
    {
      updateStocks();
    }
    else{
        clearConsole();
        stockManagement();
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
 struct items *itemList=malloc(sizeof(struct items) * numItems);
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
    free(itemList);
    printf("Remove another item? y/n \n");
    getchar();
    char response;
    scanf("%c",&response);
    if(response=='Y' || response=='y')
    {
      removeItems();
    }
    else{
    clearConsole();
    stockManagement();
    }
 }
 else{
    if(numItems>=1){
    printf("The Item code does not exist. Try again? y/n \n");
    getchar();
    char response;
    scanf("%c",&response);
    if(response=='n' || response=='N')
    {
      viewInventory();
    }
    else{
    removeItems();
    }
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
   //
   FILE *salesFile=fopen("sales.dat","a+");
   struct sales initializeSale;
   initializeSale.itemCode=itemToBeAdded.itemCode;
   initializeSale.price=itemToBeAdded.price;
   initializeSale.unitsSold=0;
   initializeSale.value=0;
   fwrite(&initializeSale,sizeof(struct sales),1,salesFile);
   fclose(salesFile);
   //
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
    if(file1==NULL){ file1=fopen("stock.dat","w");}
    fclose(file1);
    FILE *file2=fopen("receipts.dat","r");
    if(file2==NULL){ file2=fopen("receipts.dat","w");}
    fclose(file2);
    FILE *file3=fopen("sales.dat","r");
    if(file3==NULL){ file3=fopen("sales.dat","w");}
    fclose(file3);
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
    if(getNumItems()<1){
        printf("Looks like you don't have anything in your inventory \n");
    }
    else{
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
    }
    fclose(fp);
    getchar();
    char pl;
    scanf("%c",&pl);
    clearConsole();
    stockManagement();
}

void viewItem()
{
    printf("Enter the item code: \n");
    getchar();
    int code;
    scanf("%d",&code);
    if(checkItem(code)==1)
    {
    struct items currentItem;
    FILE *fp=fopen("stock.dat","r");
    while(fread(&currentItem,sizeof(struct items),1,fp))
    {
        if(currentItem.itemCode==code)
        {
           printf("##################\n");
           printf("Item Code:%d\n",currentItem.itemCode);
            printf("Name:%s\n",currentItem.itemName);
            printf("Amount:%d\n",currentItem.amount);
            printf("Buying Price:%.2f\n",currentItem.buyingPrice);
            printf("Selling Price(per piece):%.2f\n",currentItem.price);
            printf("Estimated Value:%.2f\n",currentItem.price*currentItem.amount);
        }
    }
    fclose(fp);
    }
    else{
        printf("Retry? y/n");
        getchar();
        char c;
        scanf("%c",&c);
        if(c=='y' || c=='Y')
        {
            viewItem();
        }
    }
    getchar();
    char p;
    scanf("%c",&p);
    clearConsole();
    stockManagement();
}

void createsales()
{
          static float total=0.00;
          static int itemNum=0;
          srand(time(NULL));//Seed the random number generator
          //struct to store data for receipts
          time_t currentTime;
          struct tm *localTime;
          time(&currentTime);// Get the current time
          localTime = localtime(&currentTime);// Convert the current time to the local time
          int day   = localTime->tm_mday;
          int month = localTime->tm_mon + 1;  // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
          int year  = localTime->tm_year + 1900;
          //
          FILE *fp=fopen("receipts.dat","a+");
          //
          char nday[2];
          sprintf(nday,"%d",day);
          char nyear[4];
          sprintf(nyear,"%d",year);
          char nmonth[2];
          sprintf(nmonth,"%d",month);
        //
        struct items *receiptList=malloc(1*sizeof(struct items));
        //Function to make sales in the POS
        int itemcode;
        int quantity;
        float price;
        char status;
        char pick;
        while(status!='N' && status!='n'){
        printf("Enter the item code\n");
        scanf("%d",&itemcode);
        if(checkItem(itemcode)==1){
        itemNum++;
        receiptList=realloc(receiptList,itemNum* sizeof(struct items));
        printItem(itemcode);
        printf("\n");
        price=getPrice(itemcode);
        printf("Enter the quantity bought\n");
        scanf("%d",&quantity);
        float due=quantity*price;
        //To add the current item to the generated receipt
        receiptList[itemNum-1].itemCode=itemcode;
        receiptList[itemNum-1].amount=quantity;
        //
        total+=due;
        printf("Price:%.2f\t Piece(s):%d\t Amount:%.2f \n",price,quantity,due);
        printf("Add item? y/n \n");
        getchar();
        scanf("%c",&status);
        }
        else{
            printf("The item code does not exist! Try again? Y/N \n");
            getchar();
            scanf("%c",&pick);
            if(pick=='y' || pick=='Y')
            {
                //Nothing here so as to prevent re-initializing the variables
            }
            else{
                pick='n';
                status='n';
            }
        }
        }
        if(total>0 && itemNum>=1){
        //Code to generate receipt details
        //Receipt structure is d-m-y-random-totalAmount-numItems
        char namount[10];
        sprintf(namount,"%d",total);
        char nitems[10];
        sprintf(nitems,"%d",itemNum);
        char random[5];
        sprintf(random,"%d",rand());
        char *receiptCode=malloc(sizeof(nyear)+sizeof(nmonth)+sizeof(nday)+sizeof(namount)+sizeof(random)+sizeof(nitems)+1);
        sprintf(receiptCode,"%s%s%s%s%s%s",nday,nmonth,nyear,random,namount,nitems);
        clearConsole();
        printf("\t\t Receipt Code: %s \n",receiptCode);
        //To print each item bought
        for(int i=0;i<itemNum;i++)
        {
            printf("\t");
            printf("ItemCode: %d \t",receiptList[i].itemCode);
            printf("Item: ");printItem(receiptList[i].itemCode);
            printf("\t Piece(s):%d \t",receiptList[i].amount);
            printf("\t Total:%.2f \n",getPrice(receiptList[i].itemCode)*receiptList[i].amount);
        }
        printf("Net Total: %.2f \n",total);
        //To ask for cash tendered
        float cashReceived,balance;
        char settled='n';
        char cancelled='n';
        while(settled=='n'){
        float cashReceived;
        printf("Enter amount received:   ");
        scanf("%f",&cashReceived);
        if(cashReceived==0.00)//0 received is a magic value to cancel the transaction
        {
            printf("Transaction canceled!");
            settled='y';
            cancelled='y';//To show transaction was cancelled
            getchar();
        }
        else{
        balance=cashReceived-total;
        if(balance>=0)
        {
            printf("Balance: %.2f \n",balance);
            settled='y';
        }
        else{
            printf("Amount not sufficient! \n");
        }
        }
        }
        //Section to store receipt in a data file if transaction is sucessful
        //To write data to the files
        struct receipts receipt;
        if(settled=='y' && cancelled=='n'){
        for(int j=0;j<itemNum;j++)
        {
            //To reduce amount in inventory as the items are sold and also increase amount sold
            updateSales(receiptList[j].itemCode,receiptList[j].amount);
            //The part that writes it to a file
            strcpy(receipt.receiptCode,receiptCode);
            receipt.itemCode=receiptList[j].itemCode;
            receipt.amount=receiptList[j].amount;
            receipt.price=receiptList[j].price;
            receipt.total=receipt.amount*receipt.price;
            fwrite(&receipt,sizeof(struct receipts),1,fp);
        }
        //restore initial values for next sale
        total=0.00;
        itemNum=0;
        }
        }
        fclose(fp);
        free(receiptList);

        printf("Make another sale ? y/n");
        char choice;
        getchar();
        scanf("%c",&choice);
        if(choice=='y' || choice=='Y')
        {
            clearConsole();
            createsales();
        }
}

float getPrice(int code)
{
    float price;
    struct items currentItem;
    FILE *fp=fopen("stock.dat","r");
    while(fread(&currentItem,sizeof(struct items),1,fp))
    {
        if(currentItem.itemCode==code)
        {
            price=currentItem.price;
        }
    }
    fclose(fp);
    return price;
}

void printItem(int code)
{
    struct items currentItem;
    FILE *fp=fopen("stock.dat","r");
    while(fread(&currentItem,sizeof(struct items),1,fp))
    {
        if(currentItem.itemCode==code)
        {
           printf("%s",currentItem.itemName);
        }
    }
    fclose(fp);
}


void viewReceipt()
{
  printf("Enter receipt code:\n");
  char rcode[50];
  scanf("%s",rcode);
  int hits=0;
  FILE *receiptFile=fopen("receipts.dat","r");
  struct receipts currentReceipt;
  while(fread(&currentReceipt,sizeof(struct receipts),1,receiptFile))
    {
    if(strcmp(currentReceipt.receiptCode,rcode)==0)
    {
        hits++;
        //printf("ItemCode: %s \n",currentReceipt.receiptCode);
        printf("ItemCode: %d \t",currentReceipt.itemCode);
        printf("Amount:%d \t",currentReceipt.amount);
        printf("Item:");
        printItem(currentReceipt.itemCode);
        printf("\t");
        printf("Total: %.2f \n",getPrice(currentReceipt.itemCode)*currentReceipt.amount);
    }
    }
    fclose(receiptFile);
    if(hits<1)
    {
        printf("No such receipt found! Try again?y/n");
        getchar();
        char choice;
        scanf("%c",&choice);
        if(choice=='y' || choice=='Y')
        {
            viewReceipt();
        }
        else{
            clearConsole();
        }
    }
}

void openReceipts()
{
    char ch;
    int choice;
    while(choice!=3){
    printf("Receipts:\n");
    printf("1.Search receipt by code\n");
    printf("2.View all receipts\n");
    printf("3.Back\n");
    getchar();
    scanf("%d", &choice);
    switch(choice) {
        case 1:
            viewReceipt();
            break;
        case 2:
            viewReceipts();
            break;
        case 3:
            clearConsole();
            break;
        default:
            printf("Invalid choice! Please try again.");
            clearConsole();
            break;
    }
    }
    printf("\n");
}

void viewReceipts()
{
    fileManagement();//To make sure that there is a file to read
    clearConsole();
    struct receipts currentReceipt;
    FILE *receiptFile=fopen("receipts.dat","r");
    printf("\t\t RECIEPTS \n");
    if(getNumItems() < 1) {
        printf("No receipts generated so far. \n");
    } else {
        while(fread(&currentReceipt, sizeof(struct receipts), 1, receiptFile)) {
            printf("##################\n");
            printf("Receipt Code:%s\n",currentReceipt.receiptCode);
            printf("Item Code:%d\n",currentReceipt.itemCode);
            printf("Amount:%d\n",currentReceipt.amount);
            printf("Item:");
            printItem(currentReceipt.itemCode);
            printf("\t");
            printf("Total: %.2f \n",getPrice(currentReceipt.itemCode)*currentReceipt.amount);
        }
    }
    fclose(receiptFile);
    getchar();
    char choice;
    scanf("%c", &choice);
    clearConsole();
}

void updateSales(int itemCode,int quantity)
{
    //To reduce the number of items left
     int code=itemCode;
     int restock=-1*quantity;//To reduce number
     int numItems=getNumItems();
     int position=0;
     //To create an array of structures for the records
     FILE *fp=fopen("stock.dat","a+");
     struct items *itemList=malloc(sizeof(struct items)*numItems);
     while(fread(&itemList[position],sizeof(struct items),1,fp))
    {
        if(itemList[position].itemCode==code)
        {
        itemList[position].buyingPrice+=(itemList[position].buyingPrice/itemList[position].amount)*restock;
        if(itemList[position].amount<1)//In case there is a negative stock
        {
            printf("Impossible! Item is not in stock \n");
        }
        else{
        itemList[position].amount+=restock;
        }
        }
        position++;
    }
    fclose(fp);
    FILE *fp2=fopen("stock.dat","w");
    //To write the new info into the file
    for(int i=0;i<numItems;i++)
    {
        fwrite(&itemList[i],sizeof(struct items),1,fp2);
    }
    free(itemList);
    fclose(fp2);

    //########### Now to update sales file ##################33
    FILE *fp3=fopen("sales.dat","a+");
    position=0;
     struct sales *salesList=malloc(sizeof(struct sales)*numItems);
     while(fread(&salesList[position],sizeof(struct sales),1,fp3))
    {
        if(salesList[position].itemCode==itemCode)
        {
        salesList[position].unitsSold+=quantity;
        salesList[position].value+=salesList[position].price*quantity;
        }
        position++;
    }
    fclose(fp3);
    FILE *fp4=fopen("sales.dat","w");
    //To write the new info into the file
    for(int i=0;i<numItems;i++)
    {
        fwrite(&salesList[i],sizeof(struct sales),1,fp);
    }
    free(salesList);
    fclose(fp4);
}

void reports()
{
    int choice;
    while(choice!=5){
    printf("Reports:\n");
    printf("1.Get Revenue\n");
    printf("2.Show Sales\n");
    printf("3.Show best Selling item \n");
    printf("4:Restock Alerts \n");
    printf("5.Back\n");
    getchar();
    scanf("%d", &choice);
    switch(choice) {
        case 1:
            clearConsole();
            getRevenue();
            break;
        case 2:
            clearConsole();
            showSales();
            break;
        case 3:
            clearConsole();
            bestSelling();
            break;
        case 4:
            clearConsole();
            restockAlerts();
            break;
        case 5:
            clearConsole();
            break;
        default:
            printf("Invalid choice! Please try again.");
            clearConsole();
            reports();
        break;
    }
    }
}

void getRevenue()
{
 //To ask for tax %
 float taxRate;
 printf("What is the current tax rate?(Percentage): ");
 scanf("%f",&taxRate);
 printf("\t\t Sales Sofar \n");
 //To calculate Revenue plus taxes
  FILE *fp3=fopen("sales.dat","r");
  float totalRevenue=0.00,taxes,netProfit,investment=0.00;
    struct sales salesList;
    while(fread(&salesList,sizeof(struct sales),1,fp3))
    {
      printf("ItemCode:%d \t",salesList.itemCode);
      printf("Item name:");
      printItem(salesList.itemCode);
      printf("\tUnits Sold:%d \t",salesList.unitsSold);
      printf("Value:%.2f \n",salesList.value);
      totalRevenue+=salesList.value;
      investment+=(getBuyingPrice(salesList.itemCode)*salesList.unitsSold);
    }
    fclose(fp3);
    taxes=(taxRate/100)*totalRevenue;
    netProfit=totalRevenue-(taxes+investment);
    printf("Total Revenue: %.2f \n",totalRevenue);
    printf("Taxes(%.2f %):%.2f \n",taxRate,taxes);
    printf("Capital Used:%.2f \n",investment);
    printf("Net Profit:%.2f \n\n",netProfit);
}

float getBuyingPrice(int itemCode)
{
    float buyingPrice;
    struct items currentItem;
    FILE *fp=fopen("stock.dat","r");
    while(fread(&currentItem,sizeof(struct items),1,fp))
    {
        if(currentItem.itemCode==itemCode)
        {
            buyingPrice=currentItem.buyingPrice/currentItem.amount;
        }
    }
    fclose(fp);
    return buyingPrice;
}

void showSales()
{
    FILE *fp3=fopen("sales.dat","r");
    struct sales salesList;
    while(fread(&salesList,sizeof(struct sales),1,fp3))
    {
      printf("$$$$$$$$$$$$$$$$$$$$$$$\n");
      printf("ItemCode: %d \n",salesList.itemCode);
      printf("Item name: ");
      printItem(salesList.itemCode);
      printf("\nUnits Sold: %d \n",salesList.unitsSold);
      printf("Total Value of units Sold: %.2f \n",salesList.value);
    }
    fclose(fp3);
    printf("\n\n");
}

void bestSelling()
{
    int highestSale=0;//To test for the best selling item
    char filename[]="sales.dat";//The name of the file to open
    FILE *fp=fopen(filename,"r");//To open the file
    struct sales current;//A structure to store data in the file
    struct sales bestSelling;//To store data about the highest selling item
    while(fread(&current,sizeof(struct sales),1,fp))//To read the file
    {
        //
        if(current.unitsSold>highestSale)//If it is the highest selling name
        {
            highestSale=current.unitsSold;
            bestSelling.itemCode=current.itemCode;
            bestSelling.price=current.price;
            bestSelling.unitsSold=current.unitsSold;
            bestSelling.value=current.unitsSold*current.value;
        }
        //
    }
    fclose(fp);//To close the file and prevent memory leak
    //To print the items to the screen
    printf("Highest selling Item:\n");
    printf("Item Code: %d \n",bestSelling.itemCode);
    printf("Item name:");printItem(bestSelling.itemCode);printf("\n");
    printf("Items Sold: %d\n",bestSelling.unitsSold);
    printf("Value Sold: %.2f\n",bestSelling.value);
    printf("\n\n");
}

void restockAlerts()
{
 int hits=0;
 FILE *fp=fopen("stock.dat","r");
 struct items itemList;
    while(fread(&itemList,sizeof(struct items),1,fp))
    {
       if(itemList.amount<=2)
       {

           hits++;
           printf("ItemCode:%d \t",itemList.itemCode);
           printf("ItemName: %s \t",itemList.itemName);
           printf("Units left: %d \n\n",itemList.amount);
       }
    }
    fclose(fp);
    if(hits==0)
    {
        printf("Looks like the inventory is still healthy! \n\n");
    }
}
