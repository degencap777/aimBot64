//https://www.unknowncheats.me/forum/programming-for-beginners/267073-coding-hacking-introduction-guide-practical-external-game-hacking.html
//Hooking into another process
//Coded by Dennis Ying
//Started 24/02/2019

#include <Windows.h>
#include <stdio.h>

int main(void){
    HANDLE hProcess = OpenProcess(
        PROCESS_VM_READ,    //Permission level you are seeking
        FALSE,              //Do child processes inherit permissions?
        25836               //ProcessID you are hooking into
    );
    if(hProcess == NULL){
        printf("%s", GetLastError());
    }
    int read = 0;
    BOOL hook = ReadProcessMemory(
        hProcess,
        (LPCVOID)0x0060FEFC,  //Address we are trying to hook. Change to address of varInt
        &read,               //Buffer we are writing to
        sizeof(int),        //Size of things we are reading
        NULL
    );
    printf("%d",read);
    return 0;
}
