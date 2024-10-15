#include "sndpch.h"
#include "SandboxEngine/Core/Log.h"

namespace snd::log
{
    HANDLE g_Stdout;

    void Init()
    {
        g_Stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SND_ASSERT(g_Stdout != INVALID_HANDLE_VALUE);
    }

    void Shutdown()
    {
        g_Stdout = INVALID_HANDLE_VALUE;
    }

    void Console(Category category, const char* msg, ...)
    {
        static char s_Buffer[2][2048];

        const char* logFormat = "[%s]: %s\n";
        char* formattedMsg = s_Buffer[0];
        char* logMsg = s_Buffer[1];

        va_list args;
        va_start(args, msg);
        vsnprintf(formattedMsg, sizeof(s_Buffer[0]), msg, args);
        va_end(args);

        sprintf(logMsg, logFormat, ToString(category), formattedMsg);
        WriteFile(g_Stdout, logMsg, strlen(logMsg), nullptr, nullptr);
    }

    const char* ToString(Category category)
    {
        switch(category)
        {
            case Category::Log:     return "Log";
            case Category::Warning: return "Warning";
            case Category::Error:   return "Error";
            default:                return "None";
        }
    }
}
