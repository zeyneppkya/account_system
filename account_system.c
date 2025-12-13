
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Sabitler */
#define MAX_ACCOUNTS 100
#define DATA_FILE "accounts.dat"
#define MAX_NAME_LENGTH 50

/* Veri Yapıları */

/* Banka hesabını temsil eden yapı */
typedef struct {
    int accountNumber;           /* Benzersiz hesap numarası */
    char name[MAX_NAME_LENGTH];  /* Hesap sahibinin adı */
    double balance;              /* Güncel hesap bakiyesi */
} Account;

/* Global değişkenler */
static Account accounts[MAX_ACCOUNTS];
static int accountCount = 0;

/* Fonksiyon bildirimleri */
void loadAccounts();
void saveAccounts();
void createAccount();
void listAccounts();
void depositMoney();
void withdrawMoney();
void deleteAccount();
int findAccount(int accountNumber);
int generateAccountNumber();
void clearInputBuffer();
void displayMenu();

/* Fonksiyon implementasyonları */

/* Girdi hatalarını önlemek için girdi tamponunu temizler */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Benzersiz hesap numarası üretir */
int generateAccountNumber() {
    if (accountCount == 0) {
        return 1001;
    }
    
    int maxAccountNumber = accounts[0].accountNumber;
    for (int i = 1; i < accountCount; i++) {
        if (accounts[i].accountNumber > maxAccountNumber) {
            maxAccountNumber = accounts[i].accountNumber;
        }
    }
    return maxAccountNumber + 1;
}

/* Hesap numarasına göre hesap arar */
int findAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

/* Tüm hesapları dosyadan belleğe yükler */
void loadAccounts() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        /* Dosya henüz mevcut değil, boş hesap listesiyle başla */
        accountCount = 0;
        return;
    }
    
    accountCount = fread(accounts, sizeof(Account), MAX_ACCOUNTS, file);
    fclose(file);
    
    printf("Loaded %d account(s) from file.\n", accountCount);
} 
/* Tüm hesapları bellekten dosyaya kaydeder */
void saveAccounts() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        printf("Error: Unable to save accounts to file!\n");
        return;
    }
    
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
    
    printf("Successfully saved %d account(s) to file.\n", accountCount);
}

/* Yeni bir banka hesabı oluşturur */
void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("\nError: Maximum account limit reached!\n");
        return;
    }
    
    Account newAccount;
    newAccount.accountNumber = generateAccountNumber();
    
    printf("\n=== Create New Account ===\n");
    printf("Account Number: %d (auto-generated)\n", newAccount.accountNumber);
    
    printf("Enter account holder name: ");
    clearInputBuffer();
    fgets(newAccount.name, MAX_NAME_LENGTH, stdin);
    /* Satır sonu karakterini kaldır */
    newAccount.name[strcspn(newAccount.name, "\n")] = '\0';
    
    printf("Enter initial deposit amount: $");
    while (scanf("%lf", &newAccount.balance) != 1 || newAccount.balance < 0) {
        printf("Invalid amount! Please enter a valid positive number: $");
        clearInputBuffer();
    }
    
    accounts[accountCount++] = newAccount;
    saveAccounts();
    
    printf("\nAccount created successfully!\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
    printf("Account Holder: %s\n", newAccount.name);
    printf("Initial Balance: $%.2f\n", newAccount.balance);
}

/* Mevcut tüm hesapları listeler */
void listAccounts() {
    if (accountCount == 0) {
        printf("\nNo accounts found in the system.\n");
        return;
    }
    
    printf("\n=== All Accounts ===\n");
    printf("%-15s %-30s %-15s\n", "Account Number", "Account Holder", "Balance");
    printf("------------------------------------------------------------------------\n");
    
    for (int i = 0; i < accountCount; i++) {
        printf("%-15d %-30s $%-14.2f\n", 
               accounts[i].accountNumber, 
               accounts[i].name, 
               accounts[i].balance);
    }
    
    printf("------------------------------------------------------------------------\n");
    printf("Total Accounts: %d\n", accountCount);
}

/* Hesaba para yatırır */
void depositMoney() {
    int accountNumber;
    double amount;
    
    printf("\n=== Deposit Money ===\n");
    printf("Enter account number: ");
    
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid account number!\n");
        clearInputBuffer();
        return;
    }
    
    int index = findAccount(accountNumber);
    if (index == -1) {
        printf("Error: Account not found!\n");
        return;
    }
    
    printf("Current balance: $%.2f\n", accounts[index].balance);
    printf("Enter deposit amount: $");
    
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Deposit must be positive.\n");
        clearInputBuffer();
        return;
    }
    
    accounts[index].balance += amount;
    saveAccounts();
    
    printf("\nDeposit successful!\n");
    printf("Amount deposited: $%.2f\n", amount);
    printf("New balance: $%.2f\n", accounts[index].balance);
}

/* Hesaptan para çeker */
void withdrawMoney() {
    int accountNumber;
    double amount;
    
    printf("\n=== Withdraw Money ===\n");
    printf("Enter account number: ");
    
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid account number!\n");
        clearInputBuffer();
        return;
    }
    
    int index = findAccount(accountNumber);
    if (index == -1) {
        printf("Error: Account not found!\n");
        return;
    }
    
    printf("Current balance: $%.2f\n", accounts[index].balance);
    printf("Enter withdrawal amount: $");
    
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Withdrawal must be positive.\n");
        clearInputBuffer();
        return;
    }
    
    if (amount > accounts[index].balance) {
        printf("Error: Insufficient funds!\n");
        printf("Available balance: $%.2f\n", accounts[index].balance);
        return;
    }
    
    accounts[index].balance -= amount;
    saveAccounts();
    
    printf("\nWithdrawal successful!\n");
    printf("Amount withdrawn: $%.2f\n", amount);
    printf("New balance: $%.2f\n", accounts[index].balance);
}

/* Mevcut bir hesabı siler */
void deleteAccount() {
    int accountNumber;
    char confirmation;
    
    printf("\n=== Delete Account ===\n");
    printf("Enter account number to delete: ");
    
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid account number!\n");
        clearInputBuffer();
        return;
    }
    
    int index = findAccount(accountNumber);
    if (index == -1) {
        printf("Error: Account not found!\n");
        return;
    }
    
    printf("\nAccount Details:\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Balance: $%.2f\n", accounts[index].balance);
    
    printf("\nAre you sure you want to delete this account? (y/n): ");
    clearInputBuffer();
    scanf("%c", &confirmation);
    
    if (confirmation == 'y' || confirmation == 'Y') {
        /* Silinen hesaptan sonraki tüm hesapları kaydır */
        for (int i = index; i < accountCount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        accountCount--;
        saveAccounts();
        
        printf("\nAccount deleted successfully!\n");
    } else {
        printf("\nAccount deletion cancelled.\n");
    }
}

/* Ana menüyü gösterir */
void displayMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║    ACCOUNT MANAGEMENT SYSTEM - MENU        ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║  1. Create New Account                     ║\n");
    printf("║  2. List All Accounts                      ║\n");
    printf("║  3. Deposit Money                          ║\n");
    printf("║  4. Withdraw Money                         ║\n");
    printf("║  5. Delete Account                         ║\n");
    printf("║  6. Exit                                   ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

/* Ana fonksiyon - Programın giriş noktası */
int main() {
    int choice;
    
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   WELCOME TO ACCOUNT MANAGEMENT SYSTEM     ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    /* Mevcut hesapları dosyadan yükle */
    loadAccounts();
    
    while (1) {
        displayMenu();
        
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number between 1 and 6.\n");
            while (getchar() != '\n'); /* Girdi tamponunu temizle */
            continue;
        }
        
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                listAccounts();
                break;
            case 3:
                depositMoney();
                break;
            case 4:
                withdrawMoney();
                break;
            case 5:
                deleteAccount();
                break;
            case 6:
                printf("\n╔════════════════════════════════════════════╗\n");
                printf("║  Thank you for using our banking system!   ║\n");
                printf("║  All changes have been saved.              ║\n");
                printf("╚════════════════════════════════════════════╝\n");
                return 0;
            default:
                printf("\nInvalid choice! Please select a number between 1 and 6.\n");
        }
        
        /* Kullanıcının çıktıyı görmesi için duraklat */
        printf("\nPress Enter to continue...");
        while (getchar() != '\n'); /* Girdi tamponunu temizle */
        getchar(); /* Enter tuşunu bekle */
    }
    
    return 0;
}
