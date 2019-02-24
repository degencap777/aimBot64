//https://www.unknowncheats.me/forum/programming-for-beginners/267073-coding-hacking-introduction-guide-practical-external-game-hacking.html
//Hooking into another process
//Coded by Dennis Ying
//Started 24/02/2019

#include <Windows.h>
#include <stdio.h>
//Need following library to take a snapshot of running processes
#include <tlhelp32.h>

void hook(int procID)
{
    HANDLE hProcess = OpenProcess(
        PROCESS_VM_READ,    //Permission level you are seeking
        FALSE,              //Do child processes inherit permissions?
        procID              //ProcessID you are hooking into
    );
    if(hProcess == NULL){
        printf("%s", GetLastError());
    }
    int read = 0;
    BOOL hook = ReadProcessMemory(
        hProcess,
        (LPCVOID)0x0060FEFC,  //Address we are trying to hook
        &read,               //Buffer we are writing to
        sizeof(int),        //Size of things we are reading
        NULL
    );
    printf("%d",read);
}

int main(void){
    int pid = 0;
    char target[] = "main.exe";

    //Create a snapshot of currently running processes
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(snap==INVALID_HANDLE_VALUE){
        printf("%s",GetLastError());
        return EXIT_FAILURE;
    }

    PROCESSENTRY32 pe32;
    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process and exit if unsuccessful
    if(!Process32First(snap, &pe32) )
    {
        printf("%s",GetLastError());
        CloseHandle(snap);          // clean the snapshot object
        return EXIT_FAILURE;
    }

    //Cycle through Process List
    do {
        //printf("%s\t\t\t%d\n",pe32.szExeFile,pe32.th32ProcessID);
        if(strcmp(pe32.szExeFile, target)==0) {
            pid = pe32.th32ProcessID;
        }
    } while (Process32Next(snap, &pe32));

    if(pid != 0){
        hook(pid);
    } else {
        printf("Process '%s' not found. Exiting...", target);
        return EXIT_FAILURE;
    }
}
