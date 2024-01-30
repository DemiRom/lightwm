#include <Windows.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> 
#include "error.h"
#include "tiling.h"
#include "keyboard.h" 
#include "config.h"

#include "resource.h" 

#include "debug.h" 

HMODULE wmDll;
HHOOK hookShellProcHandle;
HANDLE windowEvent;

void cleanupObjects() {
	if (hookShellProcHandle) {
		UnhookWindowsHookEx(hookShellProcHandle);
	}
	
	if (wmDll) {
		FreeLibrary(wmDll);
	}

	if (windowEvent) {
		CloseHandle(windowEvent);
	}
	
	CleanupConfigReader();
	CleanupKeyboard(); 
}

void ctrlc(int sig) {
	cleanupObjects();
	
	puts("Exiting"); 

	exit(ERROR_SUCCESS);
}

LRESULT CALLBACK ShellProc(int code, WPARAM wparam, LPARAM lparam) {
	if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
		const HANDLE windowEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"LightWMWindowEvent");
		if (windowEvent) {
			SetEvent(windowEvent);
			CloseHandle(windowEvent);
		}
	}

	return CallNextHookEx(NULL, code, wparam, lparam);
}

LRESULT HotKeyProc(int code, WPARAM wparam, LPARAM lparam) {
	DEBUG_PRINT("HotkeyProc called - %i %i %i", code, wparam, lparam); 
	
	switch(wparam) 
	{ 
		//TODO Can either trigger an event like the ShellProc callback, or handle directly. 
		// one method to handle virtual desktops is using the IVirtualDesktopManager in ShObjIdl but 
		// that is only available for Window 10 1809 or later. 
		case WORKSPACE_1:
			puts("Switch to workspace 1");
			break;
		case WORKSPACE_2:
			puts("Switch to workspace 2"); 
			break;
		case WORKSPACE_3:
			puts("Switch to workspace 3"); 
			break;
		case WORKSPACE_4:
			puts("Switch to workspace 4"); 
			break;
		case WINDOW_UP: 
			puts("Highlight window above"); 
			break; 
		case WINDOW_DOWN: 
			puts("Highlight window below"); 
			break; 
		case WINDOW_LEFT: 
			puts("Highlight window left"); 
			break; 
		case WINDOW_RIGHT: 
			puts("Highlight window right"); 
			break; 
		default: 
			DEBUG_PRINT("Unhandled hotkey message! Hotkey ID: %i", wparam); 
			break; 
	}
	
	return CallNextHookEx(NULL, code, wparam, lparam);
}

int main() {
	windowEvent = CreateEventW(NULL, FALSE, FALSE, L"LightWMWindowEvent");

	if (windowEvent == NULL) {
		reportWin32Error(L"CreateEvent");
		goto cleanup;
	}

	hookShellProcHandle = SetWindowsHookExW(WH_SHELL, ShellProc, wmDll, GetCurrentThreadId(NULL));

	if (hookShellProcHandle == NULL) {
		reportWin32Error(L"SetWindowsHookExW failed for shell hook");
		goto cleanup;
	}
	
	signal(SIGINT, ctrlc);
 
	//Load the configuration
	if(LoadConfigFile(NULL) != ERROR_SUCCESS) 
	{ 
		reportWin32Error(L"Load config file");
		goto cleanup; 
	}
	
	if(!InitializeKeyboardConfig(GetConfigItems())) 
	{ 
		reportWin32Error(L"Setup keyboard config"); 
		goto cleanup; 
	}
 
	// Handle a message loop
	tileWindows();
	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		if(msg.message == WM_HOTKEY) { 
			//Because Win32 doesn't support hook callbacks with RegisterHotkey lets make our own callback. 
			LRESULT ret = HotKeyProc(0, msg.wParam, msg.lParam);
			if(ret != ERROR_SUCCESS) { 
				DEBUG_PRINT("HotKey was unhandled! Ret: %i", ret); 
			}
			
			TranslateMessage(&msg); 
			DispatchMessageW(&msg); 
			continue;
		} else if (WaitForSingleObject(windowEvent, INFINITE) == WAIT_FAILED) {
			reportWin32Error(L"WaitForSingleObject");
			goto cleanup;
		}

		Sleep(100);

		tileWindows();
	
		TranslateMessage(&msg); 
		DispatchMessageW(&msg); 
	}

cleanup:
	cleanupObjects();
	
	return EXIT_FAILURE;
}
