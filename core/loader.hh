#pragma once
#include <strsafe.h>

//=======================================================================
// 
//	Description:	Handles loading asi files.
// 
//=======================================================================

namespace core
{
	bool ShouldDisplayError(DWORD error_id) {return error_id != ERROR_DLL_INIT_FAILED; }

	void LoadModules(const char* path, const char* directory)
	{
		char find_path[MAX_PATH] = { 0 };
		size_t folder_path_len;
		
		StringCchCatA(find_path, sizeof(find_path), path);
		if (directory)
		{
			StringCchCatA(find_path, sizeof(find_path), "\\");
			StringCchCatA(find_path, sizeof(find_path), directory);
		}

		StringLengthWorkerA(find_path, sizeof(find_path), &folder_path_len);
		StringCchCatA(find_path, sizeof(find_path), "\\*.asi");
		
		WIN32_FIND_DATAA find_data = { 0 };
		HANDLE find = FindFirstFileA(find_path, &find_data);
		if (find == INVALID_HANDLE_VALUE) {
			return;
		}

		do
		{
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				continue;
			}

			char mod_path[MAX_PATH] = { '\0' };
			StringCchCopyNA(mod_path, sizeof(mod_path), find_path, folder_path_len + 1);
			StringCchCatA(mod_path, sizeof(mod_path), find_data.cFileName);

			HMODULE mod = LoadLibraryA(mod_path);
			if (mod) {
				continue;
			}

			DWORD error_id = GetLastError();
			if (!ShouldDisplayError(error_id)) {
				continue;
			}
			
			if (auto msg = GetFormattedMessageID(error_id))
			{
				if (auto str = GetFormattedMessage("Failed to load module: %1\n\n%2", find_data.cFileName, msg))
				{
					MessageBoxA(0, str, PROJECT_NAME, MB_OK | MB_ICONERROR);
					LocalFree(str);
				}

				LocalFree(msg);
			}
		} 
		while (FindNextFileA(find, &find_data));

		FindClose(find);
	}

	void StartLoader()
	{
		char path[MAX_PATH] = { 0 };
		GetCurrentDirectoryA(sizeof(path), path);

		LoadModules(path, 0);
		LoadModules(path, "plugins");
		LoadModules(path, "scripts");
	}
}