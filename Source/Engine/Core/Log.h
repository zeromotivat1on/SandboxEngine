#pragma once

#define SND_VERBOSE(msg, ...)  snd::MsgVerb(msg, __VA_ARGS__)
#define SND_DEBUG(msg, ...)    snd::MsgDbg(msg, __VA_ARGS__)
#define SND_LOG(msg, ...)      snd::MsgLog(msg, __VA_ARGS__)
#define SND_WARNING(msg, ...)  snd::MsgWarn(msg, __VA_ARGS__)
#define SND_ERROR(msg, ...)    snd::MsgErr(msg, __VA_ARGS__)
#define SND_CRITICAL(msg, ...) snd::MsgCrit(msg, __VA_ARGS__)

namespace snd
{
    enum class MsgCategory : u8
    {
        Verbose,
        Debug,
        Log,
        Warning,
        Error,
        Critical,

        Count
    };

    inline const char* gMsgCategoryNames[(u8)MsgCategory::Count] =
        { "Verbose", "Debug", "Log", "Warning", "Error", "Critical" };

    inline void* gMsgStdout;

    void* GetStdout();

    void MsgVa(void* outstream, MsgCategory category, const char* msg, va_list args);

    SND_INLINE void Msg(void* outstream, MsgCategory category, const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(outstream, category, msg, args);
        va_end(args);
    }

    SND_INLINE void Msg(MsgCategory category, const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, category, msg, args);
        va_end(args);
    }

    SND_INLINE void MsgVerb(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, MsgCategory::Verbose, msg, args);
        va_end(args);
    }

    SND_INLINE void MsgDbg(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, MsgCategory::Debug, msg, args);
        va_end(args);
    }

    SND_INLINE void MsgLog(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, MsgCategory::Log, msg, args);
        va_end(args);
    }

    SND_INLINE void MsgWarn(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, MsgCategory::Warning, msg, args);
        va_end(args);
    }

    SND_INLINE void MsgErr(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, MsgCategory::Error, msg, args);
        va_end(args);
    }

    SND_INLINE void MsgCrit(const char* msg, ...)
    {
        va_list args;
        va_start(args, msg);
        MsgVa(gMsgStdout, MsgCategory::Critical, msg, args);
        va_end(args);
    }
}
