#include "sndpch.h"
#include "Engine/Core/Log.h"

void* snd::GetStdout()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
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
