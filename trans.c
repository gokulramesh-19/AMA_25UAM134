#include <stdio.h>
#include <stdlib.h>

#define TOTAL_RECORDS 100

struct clientData{
    unsigned int acctNum;
    char lastName[15];
    char firstName[15];
    char accountType[10];
    double balance;
};

void initializeFile(FILE *fPtr);
int menu();
void createAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);
void searchAccount(FILE *fPtr);
void displayAccounts(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void exportText(FILE *fPtr);

int main(){
    FILE *cfPtr=fopen("credit.dat","rb+");

    if(cfPtr==NULL){
        cfPtr=fopen("credit.dat","wb+");

        if(cfPtr==NULL){
            printf("File cannot be created.\n");
            return 1;
        }

        initializeFile(cfPtr);
    }

    int choice;

    while((choice=menu())!=9){
        switch(choice){

            case 1:
                createAccount(cfPtr);
                break;

            case 2:
                depositMoney(cfPtr);
                break;

            case 3:
                withdrawMoney(cfPtr);
                break;

            case 4:
                transferMoney(cfPtr);
                break;

            case 5:
                searchAccount(cfPtr);
                break;

            case 6:
                displayAccounts(cfPtr);
                break;

            case 7:
                deleteAccount(cfPtr);
                break;

            case 8:
                exportText(cfPtr);
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);
    printf("Program Closed.\n");

    return 0;
}

void initializeFile(FILE *fPtr){
    struct clientData blank={0,"","","",0.0};

    rewind(fPtr);

    for(int i=0;i<TOTAL_RECORDS;i++)
        fwrite(&blank,sizeof(struct clientData),1,fPtr);
}

int menu(){
    int choice;

    printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Transfer Money\n");
    printf("5. Search Account\n");
    printf("6. Display Accounts\n");
    printf("7. Delete Account\n");
    printf("8. Export Report\n");
    printf("9. Exit\n");

    printf("Enter choice: ");
    scanf("%d",&choice);

    return choice;
}

void createAccount(FILE *fPtr){
    struct clientData client={0,"","","",0.0};
    unsigned int account;

    printf("Enter account number (1-100): ");
    scanf("%u",&account);

    if(account<1 || account>TOTAL_RECORDS){
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum!=0){
        printf("Account already exists.\n");
        return;
    }

    client.acctNum=account;

    printf("Enter Last Name: ");
    scanf("%14s",client.lastName);

    printf("Enter First Name: ");
    scanf("%14s",client.firstName);

    printf("Enter Account Type: ");
    scanf("%9s",client.accountType);

    printf("Enter Initial Balance: ");
    scanf("%lf",&client.balance);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fwrite(&client,sizeof(struct clientData),1,fPtr);

    fflush(fPtr);

    printf("Account Created Successfully.\n");
}

void depositMoney(FILE *fPtr){
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u",&account);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum==0){
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n",client.balance);

    printf("Enter deposit amount: ");
    scanf("%lf",&amount);

    client.balance+=amount;

    fseek(fPtr,-(long)sizeof(struct clientData),SEEK_CUR);
    fwrite(&client,sizeof(struct clientData),1,fPtr);

    fflush(fPtr);

    printf("Deposit Successful.\n");
}

void withdrawMoney(FILE *fPtr){
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u",&account);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum==0){
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n",client.balance);

    printf("Enter withdraw amount: ");
    scanf("%lf",&amount);

    if(amount>client.balance){
        printf("Insufficient Balance.\n");
        return;
    }

    client.balance-=amount;

    fseek(fPtr,-(long)sizeof(struct clientData),SEEK_CUR);
    fwrite(&client,sizeof(struct clientData),1,fPtr);

    fflush(fPtr);

    printf("Withdrawal Successful.\n");
}

void transferMoney(FILE *fPtr){
    struct clientData sender,receiver;
    unsigned int sAcc,rAcc;
    double amount;

    printf("Enter sender account: ");
    scanf("%u",&sAcc);

    printf("Enter receiver account: ");
    scanf("%u",&rAcc);

    printf("Enter amount: ");
    scanf("%lf",&amount);

    fseek(fPtr,(sAcc-1)*sizeof(struct clientData),SEEK_SET);
    fread(&sender,sizeof(struct clientData),1,fPtr);

    fseek(fPtr,(rAcc-1)*sizeof(struct clientData),SEEK_SET);
    fread(&receiver,sizeof(struct clientData),1,fPtr);

    if(sender.acctNum==0 || receiver.acctNum==0){
        printf("Invalid account.\n");
        return;
    }

    if(amount>sender.balance){
        printf("Insufficient balance.\n");
        return;
    }

    sender.balance-=amount;
    receiver.balance+=amount;

    fseek(fPtr,(sAcc-1)*sizeof(struct clientData),SEEK_SET);
    fwrite(&sender,sizeof(struct clientData),1,fPtr);

    fseek(fPtr,(rAcc-1)*sizeof(struct clientData),SEEK_SET);
    fwrite(&receiver,sizeof(struct clientData),1,fPtr);

    fflush(fPtr);

    printf("Transfer Successful.\n");
}

void searchAccount(FILE *fPtr){
    struct clientData client;
    unsigned int account;

    printf("Enter account number: ");
    scanf("%u",&account);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum==0){
        printf("Account not found.\n");
        return;
    }

    printf("\nAccount Number : %u\n",client.acctNum);
    printf("Last Name      : %s\n",client.lastName);
    printf("First Name     : %s\n",client.firstName);
    printf("Account Type   : %s\n",client.accountType);
    printf("Balance        : %.2f\n",client.balance);
}

void displayAccounts(FILE *fPtr){
    struct clientData client;

    rewind(fPtr);

    printf("\n%-10s%-15s%-15s%-15s%-10s\n",
           "Account","Last Name","First Name","Type","Balance");

    while(fread(&client,sizeof(struct clientData),1,fPtr)==1){
        if(client.acctNum!=0){
            printf("%-10u%-15s%-15s%-15s%-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.accountType,
                   client.balance);
        }
    }
}

void deleteAccount(FILE *fPtr){
    struct clientData client;
    struct clientData blank={0,"","","",0.0};
    unsigned int account;

    printf("Enter account number to delete: ");
    scanf("%u",&account);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum==0){
        printf("Account not found.\n");
        return;
    }

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fwrite(&blank,sizeof(struct clientData),1,fPtr);

    fflush(fPtr);

    printf("Account Deleted Successfully.\n");
}

void exportText(FILE *fPtr){
    FILE *txtPtr=fopen("accounts.txt","w");
    struct clientData client;

    if(txtPtr==NULL){
        printf("Cannot create text file.\n");
        return;
    }

    rewind(fPtr);

    fprintf(txtPtr,"%-10s%-15s%-15s%-15s%-10s\n",
            "Account","Last Name","First Name","Type","Balance");

    while(fread(&client,sizeof(struct clientData),1,fPtr)==1){
        if(client.acctNum!=0){
            fprintf(txtPtr,"%-10u%-15s%-15s%-15s%-10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.accountType,
                    client.balance);
        }
    }

    fclose(txtPtr);

    printf("accounts.txt created successfully.\n");
}