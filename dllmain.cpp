// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>



bool bFly = false, bGod = false, bTime = false,bUp = false, bSpeed = false;

std::string isOpen(bool status)
{
    return status ? "ON" : "OFF";
}

void printOnOff()
{
    system("cls");
    std::cout << std::setw(28) << std::left << "[NUMPAD1] 小朋友會飛" << isOpen(bFly) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD2] 小朋友不扣血" << isOpen(bGod) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD3] 小朋友時間暫停" << isOpen(bTime) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD4] 小朋友爬樓梯" << isOpen(bUp) << std::endl;
    std::cout << std::setw(28) << std::left << "[NUMPAD5] 小朋友飆車" << isOpen(bSpeed) << std::endl;
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    printOnOff();

    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bFly = !bFly;
            printOnOff();
            if(bFly)
                mem::Patch((BYTE*)0x40645F, (BYTE*)"\x01\x84\x11\x5c\x11\x00\x00", 7);
            else
                mem::Patch((BYTE*)0x40645F, (BYTE*)"\x29\x84\x11\x5c\x11\x00\x00", 7);
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bGod = !bGod;
            printOnOff();
            if (bGod)
            {
                mem::Nop((BYTE*)0x406311, 8);
                mem::Nop((BYTE*)0x406991, 8);
                mem::Nop((BYTE*)0x40661E, 8);
            }
            else
            {
                mem::Patch((BYTE*)0x406311, (BYTE*)"\x83\xac\x08\x70\x11\x00\x00\x05", 8);
                mem::Patch((BYTE*)0x406991, (BYTE*)"\x83\xac\x08\x70\x11\x00\x00\x06", 8);
                mem::Patch((BYTE*)0x40661E, (BYTE*)"\x83\xac\x08\x70\x11\x00\x00\x05", 8);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bTime = !bTime;
            bUp = false;
            bSpeed = false;
            printOnOff();
            if (bTime)
                mem::Patch((BYTE*)0x406177, (BYTE*)"\xb9\xfe\xff\xff\xff\x90", 6);
            else
                mem::Patch((BYTE*)0x406177, (BYTE*)"\x8b\x89\x08\x13\x00\x00\x00", 6);
            
        }


        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            bUp = !bUp;
            bTime = false;
            bSpeed = false;
            printOnOff();
            if (bUp)
                mem::Patch((BYTE*)0x406177, (BYTE*)"\xb9\xf6\xff\xff\xff\x90", 6);
            else
                mem::Patch((BYTE*)0x406177, (BYTE*)"\x8b\x89\x08\x13\x00\x00\x00", 6);
            
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            bSpeed = !bSpeed;
            bTime = false;
            bUp = false;
            printOnOff();
            if (bSpeed)
                mem::Patch((BYTE*)0x406177, (BYTE*)"\xb9\x32\x00\x00\x00\x90", 6);
            else
                mem::Patch((BYTE*)0x406177, (BYTE*)"\x8b\x89\x08\x13\x00\x00\x00", 6);
            
        }

        Sleep(10);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HANDLE hThread;
        hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
        if (hThread)
            CloseHandle(hThread);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

