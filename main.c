//https://www.unknowncheats.me/forum/programming-for-beginners/267073-coding-hacking-introduction-guide-practical-external-game-hacking.html
//Dummy Program
//Coded by Dennis Ying
//Started 24/02/2019

#include <stdio.h>
#include <Windows.h>

int main(void){

    int varInt = 123456;
    char arrChar[128] = "DefaultString";
    int *ptr2int;
    ptr2int = &varInt;
    int **ptr2ptr;
    ptr2ptr = &ptr2int;
    int ***ptr2ptr2;
    ptr2ptr2 = &ptr2ptr;

    while(1==1){
        printf("Process ID: %d\n\n", GetCurrentProcessId());

        printf("varInt\t\t(0x%p) = %d\n", &varInt, varInt);
        printf("varChar\t\t(0x%p) = %s\n", &arrChar, arrChar);
        printf("ptr2int\t\t(0x%p) = 0x%p\n", &ptr2int, ptr2int);
        printf("ptr2ptr\t\t(0x%p) = 0x%p\n", &ptr2ptr, ptr2ptr);
        printf("ptr2ptr2\t(0x%p) = 0x%p\n\n", &ptr2ptr2, ptr2ptr2);

        printf("Press ENTER to print again.\n");
        getchar();
        printf("-------------------------------------\n");
        Sleep(1000);
    }
    return 0;
}
