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

int mem_read(HANDLE hProcess, LPCVOID address){
    printf("function:mem_read called.\n");
    int result;
    BOOL hook = ReadProcessMemory(
        hProcess,
        address,
        &result,
        sizeof(int),
        NULL
    );
    if(hook==0){
        printf("%s\n", GetLastError());
    } else {
        printf("%p was found in address %p.", result, address);
    }
    return result;
}

void mem_write_int(HANDLE hProcess, LPVOID address, int data){
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
        printf("%p successfully overwritten.", address);
    }
}

void mem_write_float(HANDLE hProcess, LPVOID address, float data){
    printf("function:mem_write called.\n");
    BOOL hook = WriteProcessMemory(
        hProcess,
        address,
        &data,
        sizeof(float),
        NULL
    );
    if(hook==0){
        printf("%s\n", GetLastError());
    } else {
        printf("%p successfully overwritten.", address);
    }
}

struct ac_player{

};

int main(void){
    int procID;
    procID = findPID("ac_client.exe");
    HANDLE process;
    process = hook(procID, PROCESS_ALL_ACCESS);
    //00509B74 is player base
    int gameAddress = 0x00400000;
    int playerAddress = gameAddress+0x00109b74;

    int newInt = 20;
    int baseAddressMem = mem_read(process,playerAddress);
    float test = 0;
    while(1){
        mem_write_int(process, (LPVOID)baseAddressMem+0x140, newInt); //TMP-M&A Carbine Ammo
        mem_write_int(process, (LPVOID)baseAddressMem+0x144, newInt); //V-19 Combat Shotgun Ammo
        mem_write_int(process, (LPVOID)baseAddressMem+0x148, newInt); //A-ARD/10 Submachine Gun Ammo
        mem_write_int(process, (LPVOID)baseAddressMem+0x14C, newInt); //Precision Tech AD-81 Sniper Rifle Ammo
        mem_write_int(process, (LPVOID)baseAddressMem+0x150, newInt); //MTP-57 Assault Rifle Ammo
        mem_write_int(process, (LPVOID)baseAddressMem+0xf8, 999);  //health
        mem_write_int(process, (LPVOID)baseAddressMem+0xfc, 999);   //armour
        //mem_write_float(process, (LPVOID)baseAddressMem+0x40, test); //left + right
        //mem_write_float(process, (LPVOID)baseAddressMem+0x44, test); //up + down camera
    }
    CloseHandle(process);
}
