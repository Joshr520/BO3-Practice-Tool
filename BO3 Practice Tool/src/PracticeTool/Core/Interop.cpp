#include "Interop.h"

#include <Windows.h>

namespace BO3PracticeTool
{
	void ParseMessage(std::string_view msg);

    void NotifyGame(std::string_view data)
    {
        HANDLE pipe = CreateFileA("\\\\.\\pipe\\t7compiler", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (!pipe) {
            return;
        }

        WriteFile(pipe, data.data(), static_cast<DWORD>(data.size()), nullptr, NULL);
        CloseHandle(pipe);
    }

    void ReadGame()
    {
		HANDLE pipe = CreateNamedPipeA("\\\\.\\pipe\\bo3pt", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT, NULL);
		char buffer[1024];
		DWORD dwRead;

		while (pipe != INVALID_HANDLE_VALUE) {
			if (ConnectNamedPipe(pipe, NULL) != FALSE) {
				while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
					buffer[dwRead] = '\0';

					ParseMessage(buffer);
				}
			}
			DisconnectNamedPipe(pipe);
		}
    }

	void ParseMessage(std::string_view msg)
	{

	}
}