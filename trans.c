#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[20];
    char firstName[20];
    double balance;
};

// function prototypes
void createTextFile(FILE *fPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
int login();
unsigned int menu();

int main()
{
    FILE *cfPtr;

    if (!login())
    {
        printf("Access Denied!\n");
        return 0;
    }

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blank = {0, "", "", 0.0};

        for (int i = 0; i < SIZE; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }

        rewind(cfPtr);
    }

    unsigned int choice;

    while ((choice = menu()) != 9)
    {
        switch (choice)
        {
        case 1:
            createTextFile(cfPtr);
            break;

        case 2:
            newRecord(cfPtr);
            break;

        case 3:
            updateRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            searchAccount(cfPtr);
            break;

        case 6:
            displayAll(cfPtr);
            break;

        case 7:
            depositMoney(cfPtr);
            break;

        case 8:
            withdrawMoney(cfPtr);
            break;

        default:
            printf("Invalid choice!\n");
        }
    }

    fclose(cfPtr);

    printf("\nThank you for using Bank Management System\n");

    return 0;
}

// LOGIN SYSTEM
int login()
{
    char password[20];

    printf("====================================\n");
    printf("      BANK MANAGEMENT SYSTEM\n");
    printf("====================================\n");

    printf("Enter Admin Password: ");
    scanf("%s", password);

    if (strcmp(password, "admin123") == 0)
    {
        printf("Login Successful!\n");
        return 1;
    }

    return 0;
}

// MENU
unsigned int menu()
{
    unsigned int choice;

    printf("\n====================================\n");
    printf("1. Create Text File\n");
    printf("2. Add New Account\n");
    printf("3. Update Account\n");
    printf("4. Delete Account\n");
    printf("5. Search Account\n");
    printf("6. Display All Accounts\n");
    printf("7. Deposit Money\n");
    printf("8. Withdraw Money\n");
    printf("9. Exit\n");
    printf("====================================\n");

    printf("Enter choice: ");
    scanf("%u", &choice);

    return choice;
}

// CREATE TEXT FILE
void createTextFile(FILE *fPtr)
{
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0};

    writePtr = fopen("accounts.txt", "w");

    rewind(fPtr);

    fprintf(writePtr, "%-10s%-20s%-20s%-10s\n",
            "Acc No", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-10u%-20s%-20s%-10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt created successfully!\n");
}

// ADD NEW ACCOUNT
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter Account Number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter Last Name: ");
    scanf("%s", client.lastName);

    printf("Enter First Name: ");
    scanf("%s", client.firstName);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    client.acctNum = accountNum;

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully!\n");
}

// UPDATE RECORD
void updateRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double transaction;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);

    printf("Enter transaction amount (+/-): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated Balance: %.2f\n", client.balance);
}

// DELETE RECORD
void deleteRecord(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};
    struct clientData client;
    unsigned int account;

    printf("Enter Account Number to delete: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist!\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully!\n");
}

// SEARCH ACCOUNT
void searchAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter Account Number to search: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
    }
    else
    {
        printf("\nAccount Found\n");
        printf("Account No : %u\n", client.acctNum);
        printf("Name       : %s %s\n",
               client.firstName,
               client.lastName);
        printf("Balance    : %.2f\n",
               client.balance);
    }
}

// DISPLAY ALL
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-10s%-20s%-20s%-10s\n",
           "Acc No", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u%-20s%-20s%-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// DEPOSIT
void depositMoney(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Deposit successful!\n");
}

// WITHDRAW
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient Balance!\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Withdrawal successful!\n");
}
