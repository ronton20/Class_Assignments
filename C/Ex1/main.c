//Author: Ron Amar
//ID: 318479458

#include <stdio.h>
#define true 1
#define false 0

int nextPrime(int);
int isPrime(int);
int isPalindrom(int);
int containsNonPrimesDigitsOnly(int);
int isSpecial(int);
void printAllSpecial(int, int);



//this method gives the next smallest prime number following the inputed number. will return 0 if no number was found.
int nextPrime(int num) {
    if(num <= 1) return 2;
    
    while(num > 1) {
        num++;
        if(isPrime(num) == true) return num;
    }
    return false;
}

//this method checks if a number is a prime number. (prime numbers are considered positive numbers)
int isPrime(int num) {
    if(num >= -1 && num <= 1) return false;
    for(int i = 2; i < (num - 1); i++)
        if(num % i == 0) return false;
    return true;    
}

//this method checks if a number is a palindrom
int isPalindrom(int num) {
    if(num / 10 == 0) return true;
    
    int original = num;
    int res = 0;
    
    while(num != 0) {
        res *= 10;
        res += num % 10;
        num /= 10;
    }
    return res == original ? true : false;
}

//this method checks if a number contains only non prime digits
int containsNonPrimesDigitsOnly(int num) {
    if(num == 0) return true;
    
    while(num != 0) {
        int temp = num % 10;
        if(isPrime(temp) == true) return false;
        num /= 10;
    }
    return true;
}

//this method checks if a number is a prime number, a palindrom and contains only non prime digits
int isSpecial(int num) {
    if((isPrime(num) == true) && (isPalindrom(num) == true) && (containsNonPrimesDigitsOnly(num) == true)) return true;
    return false;
}

//this method prints all the special numbers between 2 given numbers
void printAllSpecial(int from, int to) {
    for(int i = from; i <= to; i++) {
        if(isSpecial(i) == true) printf("%d \n", i);
    }
}



