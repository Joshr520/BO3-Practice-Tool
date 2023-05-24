#include "GlobalData.h"

#include <Windows.h>
#include <mutex>
#include <fstream>

namespace BO3PT
{
    std::mutex logMutex;
    void LogFile(const std::string& text, bool initialBoot)
    {
        std::lock_guard<std::mutex> lock(logMutex);

        std::ofstream logFile;
        if (initialBoot)
            logFile.open(selfDirectory + "/log.txt");
        else
            logFile.open(selfDirectory + "/log.txt", std::ios::app);
        logFile << text << "\n";
        logFile.flush();
        logFile.close();
    }

    std::mutex nlogMutex;
    void NLog(const std::string& text)
    {
        std::lock_guard<std::mutex> lock(nlogMutex);

        HWND notepad, edit;
        notepad = FindWindowA(NULL, "Untitled - Notepad");
        if (!notepad)
        {
            notepad = FindWindowA(NULL, "*Untitled - Notepad");
        }
        edit = FindWindowExA(notepad, NULL, "EDIT", NULL);
        SendMessageA(edit, EM_REPLACESEL, TRUE, (LPARAM)text.c_str());
        SendMessageA(edit, EM_REPLACESEL, TRUE, (LPARAM)"\n");
    }

    std::string ParseTimeFromMilli(int milliseconds)
    {
        std::string finalTime = "";
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        if (milliseconds >= 3600000)
        {
            hours = milliseconds / 3600000;
            if (hours < 10)
                finalTime += "0";
            finalTime += std::to_string(hours) + ":";
            milliseconds -= hours * 3600000;
        }
        if (milliseconds >= 60000)
        {
            minutes = milliseconds / 60000;
            if (minutes < 10)
                finalTime += "0";
            finalTime += std::to_string(minutes) + ":";
            milliseconds -= minutes * 60000;
        }
        else if (hours > 0)
        {
            finalTime += "00:";
        }
        if (milliseconds >= 1000)
        {
            seconds = milliseconds / 1000;
            if (seconds < 10 && (hours || minutes))
                finalTime += "0";
            finalTime += std::to_string(seconds) + ".";
            milliseconds -= seconds * 1000;
        }
        else
            if (hours || minutes)
                finalTime += "00.";
            else
                finalTime += "0.";
        if (milliseconds >= 50)
        {
            if (milliseconds < 100)
                finalTime += "0";
            finalTime += std::to_string(milliseconds);
        }
        else
            finalTime += "000";
        return finalTime;
    }
}