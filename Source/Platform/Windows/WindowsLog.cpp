#include "sndpch.h"
#include "SandboxEngine/Core/Log.h"

void* snd::GetStdout()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

void snd::Msg(void* outstream, MsgCategory category, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    MsgVa(outstream, category, msg, args);
    va_end(args);
}

void snd::MsgVa(void* outstream, MsgCategory category, const char* msg, va_list args)
{
    static char buffer[2][2048];
    SND_ASSERT(outstream);

    char* vamsg = buffer[0];
    char* fmtmsg = buffer[1];

    vsnprintf(vamsg, sizeof(buffer[0]), msg, args);
    sprintf(fmtmsg, "[%s]: %s\n", gMsgCategoryNames[(u8)category], vamsg);

    WriteFile(outstream, fmtmsg, strlen(fmtmsg), nullptr, nullptr);
}

void snd::MsgLog(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    MsgVa(gLogStdout, MsgCategory::Log, msg, args);
    va_end(args);
}

void snd::MsgWarn(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    MsgVa(gLogStdout, MsgCategory::Warning, msg, args);
    va_end(args);
}

void snd::MsgErr(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    MsgVa(gLogStdout, MsgCategory::Error, msg, args);
    va_end(args);
}
