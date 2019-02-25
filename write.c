#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>

int findPID(char target[]){
    int pid = 0;

    //Create a snapshot of currently running processes
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    //Some error handling in case we failed to get a snapshot of running processes
    if(snap==INVALID_HANDLE_VALUE){
        printf("%s\n",GetLastError());
        return 1;
    }

    //Declare a PROCESSENTRY32 variable
    PROCESSENTRY32 pe32;
    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process and exit if unsuccessful
    if(!Process32First(snap, &pe32) )
    {
        printf("%s\n",GetLastError());
        CloseHandle(snap);          // clean the snapshot object
    }

    //Cycle through Process List
    do {
        //Uncomment line below if you want your program to spit out every single list
        //printf("%s\t\t\t%d\n",pe32.szExeFile,pe32.th32ProcessID);
        //Comparing two strings containing process names for 'equality'
        if(strcmp(pe32.szExeFile, target)==0) {
            pid = pe32.th32ProcessID;
        }
    } while (Process32Next(snap, &pe32));
    //Clean the snapshot object to prevent resource leakage
    CloseHandle(snap);

    if(pid != 0){
        printf("The process ID of process %s is %d\n", target, pid);
        return pid;
    } else {
        printf("Process '%s' not found. Exiting...\n", target);
        exit(1);
    }
}

HANDLE hook(int procID, DWORD access)
{
    printf("function:hook called.\n");
    HANDLE hProcess = OpenProcess(
        access,             //Permission level you are seeking
        FALSE,              //Do child processes inherit permissions?
        procID              //ProcessID you are hooking into
    );
    if(hProcess == NULL){
        printf("%s\n", GetLastError());
    } else {
        printf("function:hook completed successfully with parameters - procID: %d, access: %lu\n", procID, access);
    }
    return hProcess;
}

void mem_write(HANDLE hProcess, LPVOID address, int data){
    printf("function:mem_write called.\n");
    BOOL hook = WriteProcessMemory(
        hProcess,
        address,
        &data,
        sizeof(int),
        NULL
    );
    if(hook==0){
        printf("%s\n", GetLastError());
    } else {
        printf("test");
    }
}

int main(void){
    int procID;
    procID = findPID("main.exe");
    HANDLE process;
    process = hook(procID, PROCESS_VM_OPERATION);
    int newInt = 0;
    while(1){
        newInt++;
        mem_write(process, (LPVOID)0x0060FEFC, newInt);
    }
    CloseHandle(process);
}
