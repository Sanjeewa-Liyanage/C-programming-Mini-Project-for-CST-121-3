#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 100
#define MAX_NAME_LENGTH 50

bool run = true;
int itemcode;
int quantity;
float total = 0;

struct MenuItem {
    char itemName[MAX_NAME_LENGTH];
    float price;
};

struct MenuItem menu[MAX_ITEMS];

struct PurchasedItem {
    char itemName[MAX_NAME_LENGTH];
    int code;
    int quantity;
    float itemPrice;
};

struct PurchasedItem purchasedItems[100];
int purchasedItemcount = 0;
char* getcurrentdatetime();
void menulist();
void shopfunctions();
void calculatebill();
void displaytotal();
void rerunCalculateBill();
void exitproject();
void printmenu();
void changeitemprices();
void writedatatofile();
void readdatafromfile();
void addnewitem();
void rerunadditems();
void deleteItem();
void rerundeleteitem();
void refreshsystem();

void loadinganimation();

int main() {
    readdatafromfile();
    starting();
    while (run) {
        printmenu();
        shopfunctions();
        char Option;
        printf("Select Function do you want: ");
        scanf(" %c", &Option);
        switch (Option) {
            case 'E': case 'e':
                run = false;
                writedatatofile();
                exitproject();
                break;
            case 'A': case 'a':
                calculatebill();
                break;
            case 'B': case 'b':
                changeitemprices();
                break;
            case 'C': case 'c':
                addnewitem();
                break;
            case 'f': case 'F':
                deleteitem();
                break;

            case 'S': case 's':
                loadinganimation();
                readbills();

                while (getchar() != '\n');
                loadinganimation();
                break;

            default:
                printf("Invalid input. Please try again.\n");
                Sleep(400);
                system("cls");
                break;
        }
    }
    return 0;
}
void readdatafromfile() {
    FILE *file = fopen("struct_data.txt", "r");

    if (file == NULL) {
        printf("Error opening file for reading. Creating a new file...\n");
        return;
    }

    int menuIndex = 0;
    while (fscanf(file, "%49s %f", menu[menuIndex].itemName, &menu[menuIndex].price) == 2) {

           menuIndex++;


        if (menuIndex >= MAX_ITEMS) {
            break;
        }
    }

    fclose(file);
}

void rerundeleteitem(){
char decision[10];
    printf("Do you want to update more item prices? (Y/N)\n");
    scanf(" %c", &decision[0]);

    if (decision[0] == 'Y' || decision[0] == 'y') {
        changeitemprices();
    } else if (decision[0] == 'N' || decision[0] == 'n') {

       main();
    } else {
        printf("Invalid input. Please enter 'Y' or 'N'.\n");
        rerundeleteitem();
    }

}

void printmenu() {
    menulist();
    printf("\n");
    printf("  %-3s %-30s %-10s\n", " \t \tCode", "Item Name", "Price");
    printf("\t \t==============================================\n");
    for (int menuIndex = 0; menuIndex < MAX_ITEMS; menuIndex++) {
        if (menu[menuIndex].price > 0) {
            printf("\t \t [%-2d] %-30s %-10.2f\n", menuIndex + 1, menu[menuIndex].itemName, menu[menuIndex].price);

        }

    }
    printf("\t \t==============================================\n");
}

void menulist() {


    printf("\n  \t\t\t****Beverage Shop Main Menu****\n\n");
    printf("\t \t==============================================\n");
    char *currentDateTime = getcurrentdatetime();
    printf("\t\t%s\n", currentDateTime);
    printf("\t \t==============================================\n");
    free(currentDateTime);//deallocate memory cause time changed
    printf("\t \t==============================================\n");

}



void shopfunctions() {
    printf("\n");
    printf("\n");
    printf("\t \t==============================================\n");
    printf("\t\t\t   *****Shop Functions*****\t\n");
    printf("\t \t==============================================\n");
    printf("\t\t[A] Calculate Bill\n");
    printf("\t\t[B] Change Item Prices\n");
    printf("\t\t[C] Add Items\n");
    printf("\t\t[F] Delete Item\n");
    printf("\t\t[S] Show Bills\n");
    printf("\t\t[E] Exit\n");
     printf("\t \t==============================================\n");
}

void calculatebill() {
    system("cls");
    printmenu();
    total = 0;
    purchasedItemcount = 0;

    do {

        printf("\n\nEnter The Item code [Press 0 to finish]: ");
        scanf(" %d", &itemcode);

        if (itemcode != 0 && itemcode >= 1 && itemcode <= MAX_ITEMS && menu[itemcode - 1].price > 0) {
            printf("Enter the Quantity: ");
            scanf("%d", &quantity);
            int menuIndex = itemcode - 1;

            total += quantity * menu[menuIndex].price;

            strcpy(purchasedItems[purchasedItemcount].itemName, menu[menuIndex].itemName);
            purchasedItems[purchasedItemcount].itemPrice = quantity * menu[menuIndex].price;
            purchasedItems[purchasedItemcount].quantity = quantity;
            purchasedItemcount++;

        } else if (itemcode < 0 || itemcode !=0) {
            printf("Invalid item code. Try again!\n");
        }
    } while (itemcode !=0);

    displaytotal();
    sendbillstofile();
    rerunCalculateBill();
}
void rerunCalculateBill(){
    printf("Do You Want to Issue A New bill\n");
    char choice[2];
    printf("Press Y to coninue or N to Mainmenu ");

    scanf(" %c",&choice);

if (choice[0]=='Y'|| choice[0]=='y'){
    calculatebill();
}else if (choice[0]=='N'|| choice[0]=='n'){

    main();
}else{
printf("Invalid Input. Try again!");
}

}

void displaytotal() {
    system("cls");
printf("\t \t*********************** Bill ************************\n\n");
printf("\t \t=====================================================\n");
char *currentDateTime = getcurrentdatetime();
    printf("\t\t%s\n", currentDateTime);

printf("\t \t=====================================================\n");
    printf("\t \t %-30s %-10s %-10s\n", "Item", "Quantity", "Price");
printf("\t \t=====================================================\n");

    for (int purchaseIndex = 0; purchaseIndex < purchasedItemcount; purchaseIndex++) {
        printf("\t \t %-30s %-10d %-10.2f\n", purchasedItems[purchaseIndex].itemName, purchasedItems[purchaseIndex].quantity, purchasedItems[purchaseIndex].itemPrice);
    }
    printf("\t \t-----------------------------------------------------\n");
    printf("\n\t \t\t\t\t \t Total bill: %.2f\n", total);
    printf("\t \t=====================================================\n");
    printf("\t \t---------------------ThankYou!----------------------\n\n");
}
void sendbillstofile() {
    FILE *file = fopen("bills.txt", "a");

    if (file == NULL) {
        printf("Error opening file for writing...\n");
        return;
    }

    fprintf(file, "\n\n\t \t*********************** Bill ************************\n\n");
    fprintf(file, "\t \t=====================================================\n");
    char *currentDateTime = getcurrentdatetime();
    fprintf(file, "\t\t%s\n", currentDateTime);

    fprintf(file, "\t \t=====================================================\n");
    fprintf(file, "\t \t %-30s %-10s %-10s\n", "Item", "Quantity", "Price");
    fprintf(file, "\t \t=====================================================\n");

    for (int purchaseIndex = 0; purchaseIndex < purchasedItemcount; purchaseIndex++) {
        fprintf(file, "\t \t %-30s %-10d %-10.2f\n", purchasedItems[purchaseIndex].itemName, purchasedItems[purchaseIndex].quantity, purchasedItems[purchaseIndex].itemPrice);
    }
fprintf(file, "\t \t-----------------------------------------------------\n");
    fprintf(file, "\n\t \t\t\t\tTotal bill: %.2f\n", total);
fprintf(file, "\t \t=====================================================\n");
fprintf(file, "\t \t---------------------ThankYou!----------------------\n\n");
    fclose(file);
}
void readbills() {
    system("cls");
    FILE *file = fopen("bills.txt", "r");

    if (file == NULL) {
        printf("Error opening file for reading...\n");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
printf("\nPress Enter to return to the main menu...");
    while (getchar() != '\n');

    fclose(file);
}


void changeitemprices() {
    system("cls");
    printmenu();
    int itemcode;
    printf("Enter the item code to change the price: ");
    scanf("%d", &itemcode);

    if (itemcode >= 1 && itemcode <= MAX_ITEMS && menu[itemcode - 1].price > 0) {
        int menuIndex = itemcode - 1;

        printf("Enter the new price for %s: ", menu[menuIndex].itemName);
        scanf("%f", &menu[menuIndex].price);

        printf("Price for %s updated successfully!\n", menu[menuIndex].itemName);
        writedatatofile();
        rerundeleteitem();
    } else {
        printf("Invalid item code.\n");
    }
}




void writedatatofile() {
    FILE *file = fopen("struct_data.txt", "w");

    if (file == NULL) {
        printf("Err opening file for writing.\n");
        return;
    }

    for (int menuIndex = 0; menuIndex < MAX_ITEMS; menuIndex++) {
        if (menu[menuIndex].price >= 0) {
            fprintf(file, "%s %f\n", menu[menuIndex].itemName, menu[menuIndex].price);
        }
    }

    fclose(file);
}


void deleteitem(){
system("cls");
printmenu();
int itemcode;//local variable(itemcode) for only this function this item code will be change on user input
printf("Enter the item code to delete: ");
scanf("%d",&itemcode);



 if (itemcode >= 1 && itemcode <= MAX_ITEMS && menu[itemcode - 1].price > 0) {
        int menuIndex = itemcode - 1;

    printf("Are you sure you want to delete %s? (Press Y to Delete or press N to Cancel)\n", menu[menuIndex].itemName);
    char decision;

    scanf(" %c", &decision);

    if(decision == 'Y' || decision == 'y'){
        strcpy(menu[menuIndex].itemName,"");
        menu[menuIndex].price = 0;
        printf("%s deleted successfully!\n", menu[menuIndex].itemName);

        for(int i = menuIndex; i <MAX_ITEMS -1;i++){
            strcpy(menu[i].itemName, menu[i+1].itemName);
            menu[i].price = menu[i+1].price;
        }
        strcpy(menu[MAX_ITEMS - 1].itemName, "");
        menu[MAX_ITEMS - 1].price = 0;

        writedatatofile();
    }else if (decision == 'N' || decision == 'n') {
            printf("Deletion canceled.\n");
            loadinganimation();
            deleteitem();

        } else {
            printf("Invalid input. Please enter 'Y' or 'N'.\n");

            deleteitem();
        }
    } else {
        printf("Invalid item code.\n");
    }


    main();
}



void addnewitem() {

    system("cls");
    printmenu();
    int newItemIndex = -1;

    for (int menuIndex = 0; menuIndex < MAX_ITEMS; menuIndex++) {
        if (menu[menuIndex].price == 0) {
            newItemIndex = menuIndex;
            break;
        }
    }

    if (newItemIndex != -1) {
        printf("Enter the name of the new item: ");
        scanf("%s", menu[newItemIndex].itemName);

        printf("Enter the price of %s: ", menu[newItemIndex].itemName);
        scanf("%f", &menu[newItemIndex].price);

        printf("%s added successfully!\n", menu[newItemIndex].itemName);
        writedatatofile();
        rerunadditems();
    } else {
        printf("Cannot add more items. The menu is full.\n");
    }

    main();
}

void rerunadditems(){
char decision[10];
    printf("Do you want to add more items ? (Y/N)\n");
    scanf(" %c", &decision[0]);
    while(getchar() != '\n');

    if (decision[0] == 'Y' || decision[0] == 'y') {
        loadinganimation();
        addnewitem();
    } else if (decision[0] == 'N' || decision[0] == 'n') {

    main();
    } else {
        printf("Invalid input. Please enter 'Y' or 'N'.\n");
        rerunadditems();
    }

}



// Additional Functions for decoration

char* getcurrentdatetime() {

    time_t t;
    struct tm *current_time;
    t = time(NULL);
    current_time = localtime(&t);


    char *dateTimeString = (char*)malloc(25);
    if (dateTimeString == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strftime(dateTimeString, 25, "%Y-%m-%d \t\t\t  %H:%M:%S", current_time);

    return dateTimeString;
}




void exitproject() {
    system("cls");
    int i;
    char equal1[100] =   " \t \t==============================\n";
    char thankyou[100] = " \t \t========= THANK YOU! =========\n";
    char goodbye[100] =  " \t \t========= GOOD BYE!  =========\n";
    char equal2[100] =   " \t \t==============================\n";

    for (i =0; i<strlen(equal1); i++){
        printf("%c", equal1[i]);
        Sleep(40);
    }
    for (i = 0; i < strlen(thankyou); i++) {
        printf("%c", thankyou[i]);
        Sleep(40);
    }
    for (i = 0; i < strlen(goodbye); i++) {
        printf("%c", goodbye[i]);
        Sleep(40);
    }
    for (i =0; i<strlen(equal2); i++){
        printf("%c", equal2[i]);
        Sleep(40);
    }
    exit(0);
}

void loadinganimation(){

int e;
int numFrames =10;
int delay=60000;

for(e=0; e<numFrames;++e){
    printf("\r\t\t\t\t\tLOADING -");
    fflush(stdout);
    usleep(delay);

    printf("\r\t\t\t\t\tLOADING /");
    fflush(stdout);
    usleep(delay);

    printf("\r\t\t\t\t\tLOADING |");
    fflush(stdout);
    usleep(delay);

    printf("\r\t\t\t\t\tLOADING \\");
    fflush(stdout);
    usleep(delay);
}
system("cls");

}
// starting function work as swap animation
void starting(){
system("cls");
loadinganimation();
}



