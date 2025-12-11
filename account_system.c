
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