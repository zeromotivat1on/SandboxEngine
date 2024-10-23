#pragma once

#define SND_LOG(msg, ...)      snd::MsgLog(msg, __VA_ARGS__)
#define SND_WARNING(msg, ...)  snd::MsgWarn(msg, __VA_ARGS__)
#define SND_ERROR(msg, ...)    snd::MsgErr(msg, __VA_ARGS__)

namespace snd
{
    enum class MsgCategory : u8
    {
        Log     = 0,
        Warning = 1,
        Error   = 2,

        Count
    };

    inline const char* gMsgCategoryNames[(u8)MsgCategory::Count] = { "Log", "Warning", "Error" };

    inline void* gLogStdout;

    void* GetStdout();

    void Msg(void* outstream, MsgCategory category, const char* msg, ...);
    void MsgVa(void* outstream, MsgCategory category, const char* msg, va_list args);

    void MsgLog(const char* msg, ...);
    void MsgWarn(const char* msg, ...);
    void MsgErr(const char* msg, ...);
}
