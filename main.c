#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Psapi.h>

WCHAR str[MAX_PATH];
DWORD pid;
HANDLE phandle;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if (GetClassNameW(hwnd, str, MAX_PATH))
		if (!lstrcmpW(str, L"#32770"))
			if (GetWindowTextW(hwnd, str, MAX_PATH))
				if (IsWindowVisible(hwnd) && !lstrcmpW(str, L"Спонсируемый сеанс")) {
					GetWindowThreadProcessId(hwnd, &pid);
					phandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
					GetProcessImageFileNameW(phandle, str, MAX_PATH);
					CloseHandle(phandle);
					if (wcsstr(str, L"\\TeamViewer.exe") + 15 == str + wcslen(str)) {
						hwnd = FindWindowExW(hwnd, NULL, L"Button", L"OK");
						SendMessageA(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
						SendMessageA(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
						return FALSE;
					}
				}
	return TRUE;
}

DWORD WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	while (TRUE)
	{
		Sleep(15000);
		EnumWindows(&EnumWindowsProc, 0);
	}
  return FALSE;
}
