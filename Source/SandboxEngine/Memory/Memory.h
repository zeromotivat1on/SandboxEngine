#pragma once

#define ENGINE_BLOCK_SIZE   MB(16)
#define CORE_STACK_SIZE     MB(2)
#define FRAME_STACK_SIZE    MB(2)

namespace snd::memory
{
    struct Block
    {
        u8* Data;
        u64 Size;
    };

    struct Stack
    {
        u8*     Data;
        u64     Size;
        u64     Position;

        void*   Push(u64 bytes);
        void    Pop(u64 bytes);
        void    Clear();
    };

    struct StackScopeBlock
    {
                StackScopeBlock(Stack* stack, u64 size);
                ~StackScopeBlock();

        Stack*  Owner;
        void*   Data;
        u64     Size;
    };

    inline Block g_EngineBlock;
    inline Stack g_CoreStack;
    inline Stack g_FrameStack;

    void Init();
    void Shutdown();
}
