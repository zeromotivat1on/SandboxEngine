#pragma once

#define SND_CORE_LOG(categoryType, msg, ...) snd::log::Console(snd::log::Category::categoryType, msg, __VA_ARGS__)

namespace snd::log
{
    enum class Category : u8
    {
        Log     = 0,
        Warning = 1,
        Error   = 2,
    };

    void Init();
    void Shutdown();

    void Console(Category category, const char* msg, ...);
    void File(FILE* fileh, Category category, const char* msg, ...);

    const char* ToString(Category category);
}
