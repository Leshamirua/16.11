// WindowsProject10.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "16.11.h"
#include <time.h>
#include "Commctrl.h"
#pragma comment(lib,"comctl32")

#define MAX_LOADSTRING 100
HWND hTimer, hDate, hProgress;
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI TimerThread(LPVOID lp);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{
    static time_t t;
    static TCHAR str[100];
    int RightAnswers = 0, MaxRightAnswers = 2;
    int DefaultPriority = 0;
    switch (message)
    {
    case WM_CLOSE:
        SetThreadPriority(TimerThread, DefaultPriority);
        EndDialog(hWnd, 0);
        return TRUE;

    case  WM_INITDIALOG: {
        SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
        hTimer = GetDlgItem(hWnd, IDC_EDIT1);
        hDate = GetDlgItem(hWnd, IDC_EDIT2);
        hProgress = GetDlgItem(hWnd, IDC_PROGRESS1);
        SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 2));
        SendMessage(hProgress, PBM_SETPOS, 0, 0);
        SendMessage(hProgress, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 0)));
        SendMessage(hProgress, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0)));
        HANDLE hThread = CreateThread(NULL, 0, TimerThread, hTimer, 0, NULL);
        DefaultPriority = GetThreadPriority(TimerThread);
        SetThreadPriority(TimerThread, THREAD_PRIORITY_ABOVE_NORMAL);
        CloseHandle(hThread);
        return TRUE;
    }
    case WM_COMMAND: {
        LRESULT result1 = SendDlgItemMessage(hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0);
        if (result1 == BST_CHECKED)
            RightAnswers++;
        result1 = SendDlgItemMessage(hWnd, IDC_RADIO3, BM_GETCHECK, 0, 0);
        if (result1 == BST_CHECKED)
            RightAnswers++;

        int Result = (RightAnswers * 100) / MaxRightAnswers;

        if (LOWORD(wp) == IDFINISH) {
            SendMessage(hProgress, PBM_SETPOS, RightAnswers, 0);
            _stprintf_s(str, TEXT("Правильных ответов: %d, из 100 "), Result);
            MessageBox(hWnd, str, TEXT("Результаты"), MB_OK | MB_ICONINFORMATION);
        }
    }

                   return TRUE;
    }
    return FALSE;
}

DWORD WINAPI TimerThread(LPVOID lp)
{
    static time_t t;
    static TCHAR str[100];
    while (true) {
        t = time(NULL);
        struct tm DateTime;
        (localtime_s(&DateTime, &t));
        _tcsftime(str, 100, TEXT("%H:%M:%S"), &DateTime);
        SetWindowText(hTimer, str);
        _tcsftime(str, 100, TEXT("%d.%m.%Y"), &DateTime);
        SetWindowText(hDate, str);
    }
    return 0;
}
