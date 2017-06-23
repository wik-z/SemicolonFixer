#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <ShObjIdl.h>

using namespace std;

HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;

bool pressed = false;

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN) {

			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			bool SHIFT_key = GetAsyncKeyState(VK_SHIFT);
			if (kbdStruct.vkCode == VK_OEM_5) {
				if (SHIFT_key)
					keybd_event(VK_SHIFT, 0x2A, 0, 0);

				keybd_event(VkKeyScan(';'), 0xA7, 0, 0);
				keybd_event(VkKeyScan(';'), 0xA7, KEYEVENTF_KEYUP, 0);

				return 1;
			}
		}
	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

int main() {
	ITaskbarList *pTaskList = NULL;
	HRESULT initRet = CoInitialize(NULL);
	HRESULT createRet = CoCreateInstance(CLSID_TaskbarList,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskbarList,
		(LPVOID*)&pTaskList);

	if (createRet == S_OK)
	{

		pTaskList->DeleteTab(GetConsoleWindow());

		pTaskList->Release();
	}

	CoUninitialize();

	_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
	cout << "Listening...";
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
	}

	return 0;
}