#pragma once
#include <strsafe.h>

//=======================================================================
// 
//	Description:	Handles loading asi files.
// 
//=======================================================================

namespace Core
{
	void LoadModules(const char* p_szPath, const char* p_szDirectory)
	{
		char szFindPath[MAX_PATH] = { '\0' };
		size_t szFolderPathSize;
		{
			StringCchCatA(szFindPath, sizeof(szFindPath), p_szPath);
			if (p_szDirectory) 
			{
				StringCchCatA(szFindPath, sizeof(szFindPath), "\\");
				StringCchCatA(szFindPath, sizeof(szFindPath), p_szDirectory);
			}

			StringLengthWorkerA(szFindPath, sizeof(szFindPath), &szFolderPathSize);
			StringCchCatA(szFindPath, sizeof(szFindPath), "\\*.asi");
		}

		WIN32_FIND_DATAA wFindData;
		HANDLE hFind = FindFirstFileA(szFindPath, &wFindData);
		if (hFind == INVALID_HANDLE_VALUE) {
			return;
		}

		do
		{
			if (wFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				continue;
			}

			char szModulePath[MAX_PATH] = { '\0' };
			StringCchCopyNA(szModulePath, sizeof(szModulePath), szFindPath, szFolderPathSize + 1);
			StringCchCatA(szModulePath, sizeof(szModulePath), wFindData.cFileName);

			HMODULE hModule = LoadLibraryA(szModulePath);
			if (!hModule) 
			{
				auto pErrorMsg = Core::GetFormattedMessageID(GetLastError());
				auto pStr = Core::GetFormattedMessage("Failed to load module: %1\n\n%2", wFindData.cFileName, pErrorMsg);
				LocalFree(pErrorMsg);

				MessageBoxA(0, pStr, PROJECT_NAME, MB_OK | MB_ICONERROR);
				LocalFree(pStr);
			}
		} 
		while (FindNextFileA(hFind, &wFindData));

		FindClose(hFind);
	}

	void StartLoader()
	{
		char szPath[MAX_PATH];
		GetCurrentDirectoryA(sizeof(szPath), szPath);

		LoadModules(szPath, 0);
		LoadModules(szPath, "plugins");
		LoadModules(szPath, "scripts");
	}
}