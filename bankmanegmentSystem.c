#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
//--------------------- Structure of user ---------------------------------------//
typedef struct
{
    char Account_num[20];
    char first_name[20];
    char last_name[20];
    long int pass;
    long long int Aadhar_number;
} user;
user u1;

typedef struct 
{   
    long int balance;
    long int withdraw;
    long int deposit;
} transaction;
transaction t1;

long int amount = 0;

//-------------------------------- Function prototypes ------------------------// 
void authenticate();
void addnew_user();
void account_menu();
void delet_account();
void check_balance(transaction *t1);
void  statment(transaction *t1);
void deposit_money(transaction *t1, long int amount);
void withdraw_money(transaction *t1, long int amount);

//------------------------- Main Function --------------------------------------//
int main() {
    int x;
    char name[20];
    long int password;
    long long int acc;

    do {
        printf("\n");
        printf("----------------------------- Choose Any Option ---------------------------------------------- \n");
        printf("Enter 1 For Login\n");
        printf("Enter 2 For New User Register\n");
        printf("Enter 3 For Delete Account\n");
        printf("Enter 4 For Exit \n");
        scanf("%d", &x);

        switch (x) {
            case 1: {
                printf("\nEnter your Account number :- ");   
                scanf("%lld", &acc);     
                printf("Enter your name           :- ");
                scanf("%s", name);
                printf("Enter your password       :- ");
                scanf("%ld", &password);

                authenticate(name, password, acc); //-----------Authentication call ------------------------
                break;
            }
            case 2: {
                addnew_user();     //---calling add user function-------------------------//
                break;
            }
            case 3:{
                printf("\nEnter your Account number :- ");   
                scanf("%lld", &acc);     
              //  printf("Enter your name           :- ");
              //  scanf("%s", name);
              //  printf("Enter your password       :- ");
              //  scanf("%ld", &password);
                delet_account(acc);
            }
            default:
                break;
        }
    } while (x != 4);

    return 0;
}

//-------------------------------------------- Account Menu option -------------------------------------------//

void account_menu() {
    int option;
    do {
        printf("\n");
        printf("------------------ Account Menu options -------------------- \n");
        printf("Press 1 For Balance Inquiry\n");
        printf("Press 2 For withdraw Money\n");
        printf("Press 3 For Deposit Money\n");
        printf("Press 4 For Last Statement\n");
        printf("Press 5 For Exit\n");
        scanf("%d", &option);
        switch (option) {
            case 1: {
                check_balance(&t1);  // --------------- Calling check balance function ---------------//
                break;
            }
            case 2: {
                printf("\nEnter The Amount To Withdraw : ");
                scanf("%ld", &amount);
                withdraw_money(&t1, amount); //---------------Calling withdraw function------------//
                break;
            }
            case 3: {
                printf("\nEnter The Amount To Deposit : ");
                scanf("%ld", &amount);
                deposit_money(&t1, amount); //-------------------calling Deposit function ----------------//
                break;
            }
            case 4: {
                statment(&t1);
                break;
            }
        }
    } while (option != 5);
}
//-------------------------------------- Function declaration for adding new user ---------------------------// 

void addnew_user() {
    printf("-------------------- Create Your Account By Entering The Details: -------------------- \n");
    printf("\nEnter Your First Name    :- ");
    scanf("%s", u1.first_name);
    printf("\nEnter Your Last Name     :- ");
    scanf("%s", u1.last_name);
    printf("\nEnter Your Aadhar Number :- ");
    scanf("%lld", &u1.Aadhar_number);
    printf("\nEnter Your Password      :- ");
    scanf("%ld", &u1.pass);
    
    char account_start[] = "3514810000"; 
    char end_num[10];

    srand(time(0));
    int random_number = (rand() % 9000) + 1000;
    sprintf(end_num, "%d", random_number);

    strcpy(u1.Account_num, strcat(account_start, end_num));
    char file_name[50];
    strcpy(file_name, u1.Account_num);
    FILE *fptr;
    fptr = fopen(strcat(file_name,".dat"), "wb");
    if (fptr == NULL) {
        printf("Error creating file!\n");
        exit(1);
    } else {
        fwrite(&u1, sizeof(user), 1, fptr);
        // Initialize transaction data with zero balance
        t1.balance = 0;
        t1.deposit=0;
        t1.withdraw=0;
        fwrite(&t1, sizeof(transaction), 1, fptr);
    }
    fclose(fptr);

    printf("\nThe Account has been created with Account number: %s\n", u1.Account_num); 
}

//----------------------------- Function definition for user Authentication ------------------------------------//

void authenticate(char name[20], long int password, long long int acc) {
    char filename[20];
    sprintf(filename, "%lld", acc);
    
    FILE *fptr;
    fptr = fopen(strcat(filename, ".dat"), "rb");

    if (fptr == NULL) {
        printf("You may not be registered in our system.\n");
    } else {
        fread(&u1, sizeof(user), 1, fptr); // users structure
        fread(&t1, sizeof(transaction), 1, fptr); // Read transaction data
    }
    
    if (strcmp(name, u1.first_name) == 0 && password == u1.pass) {
        printf("\nAccess Granted\n");
        printf("-------------------- Your Account Details Are -------------------- \n\n");
        printf("Account no       :     %s\n", u1.Account_num);
        printf("First Name       :     %s\n", u1.first_name);
        printf("Last Name        :     %s\n", u1.last_name);
        printf("Aadhar number    :     %lld\n", u1.Aadhar_number);
        printf("Password         :     %ld\n", u1.pass);
        printf("------------------------------------------------------------------- \n");
        account_menu(); //------------------ account menu calling 
    } else {
        printf("\nYou Have Entered Wrong Details\n");
    }

    fclose(fptr);   
}



//------------------- To check balance --------------------------------------
void check_balance(transaction *t1) {
    printf("Your current balance is %ld Rs.\n", t1->balance);    
}

//--------------------------- To print Statement-------------------------
void statment(transaction *t1)
{
 printf("\n------------------ Your Last Statement is ----------------------\n\n");
 printf("Your Last withdrowl is  :   %ld Rs.\n", t1->withdraw);  
 printf("Your Last Deposit is    :   %ld Rs.\n",t1->deposit);
 printf("Your current balance is :   %ld Rs.\n", t1->balance);  
 printf("----------------------------------------------------------------\n");
}

// ------------------------ To deposit money --------------------------------
void deposit_money(transaction *t1, long int amount) {
    t1->balance += amount;
   

    // Update balance in the file
    char filename[50];
    strcpy(filename, u1.Account_num);
    strcat(filename, ".dat");

    FILE *fptr = fopen(filename, "rb+");

    if (fptr == NULL) {
        printf("Error opening file!\n");
        exit(1);
    } else {
        fseek(fptr, sizeof(user), SEEK_SET); // Move the file pointer to the transaction part
        t1->deposit=amount;
        fwrite(t1, sizeof(transaction), 1, fptr);
        printf("Your amount of Rs %ld is successfully deposited.\n", amount);
        fclose(fptr);
    }

    check_balance(t1);
}

//------------------------- To withdraw money ----------------------------------
void withdraw_money(transaction *t1, long int amount) {
    if (t1->balance >= amount) 
    {
        t1->balance -= amount;

        // Update balance in the file
        char filename[50];
        strcpy(filename, u1.Account_num);
        strcat(filename, ".dat");
        FILE *fptr = fopen(filename, "rb+");

        if (fptr == NULL) {
            printf("Error opening file!\n");
            exit(1);
        } else {
            fseek(fptr, sizeof(user), SEEK_SET); // Move the file pointer to the transaction part
            t1->withdraw=amount;
            fwrite(t1, sizeof(transaction), 1, fptr);
            printf("Your amount of Rs %ld is successfully withdrawn.\n", amount);
            fclose(fptr);
        }

        
} 
    else
 {
        printf("You have insufficient balance. Maintain at least Rs 2000 in your account.\n");
    }

    check_balance(t1);
}
/// -------------------------------------To delete Account-------------//
void delet_account( long long int acc) {
    char filename[20];
    sprintf(filename, "%lld", acc);
    strcat(filename, ".dat");
    if(remove(filename)==0)
        {
             printf("Account %lld successfully deleted.\n", acc);
        }
         else 
        {
              perror("Error deleting Account");
         } 
    }
