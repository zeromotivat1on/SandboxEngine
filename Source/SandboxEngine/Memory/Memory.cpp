#include "sndpch.h"
#include "SandboxEngine/Memory/Memory.h"

namespace snd::memory
{
    void Init()
    {
        SND_ASSERT(!g_EngineBlock.Data);
        g_EngineBlock.Size = ENGINE_BLOCK_SIZE;
        g_EngineBlock.Data = (u8*)malloc(ENGINE_BLOCK_SIZE);

        g_CoreStack.Data = g_EngineBlock.Data;
        g_CoreStack.Size = CORE_STACK_SIZE;
        g_CoreStack.Position = 0;

        g_FrameStack.Data = g_CoreStack.Data + g_CoreStack.Size;
        g_FrameStack.Size = FRAME_STACK_SIZE;
        g_FrameStack.Position = 0;
    }

    void Shutdown()
    {
        SND_ASSERT(g_EngineBlock.Data);
        free(g_EngineBlock.Data);

        memset(&g_EngineBlock, 0, sizeof(g_EngineBlock));
        memset(&g_CoreStack, 0, sizeof(g_CoreStack));
        memset(&g_FrameStack, 0, sizeof(g_FrameStack));
    }

    void* Stack::Push(u64 bytes)
    {
        SND_ASSERT(Position + bytes <= Size);
        void* data = Data + Position;
        Position += bytes;
        return data;
    }

    void* Stack::PushZero(u64 bytes)
    {
        void* data = Push(bytes);
        memset(data, 0, bytes);
        return data;
    }

    void Stack::Pop(u64 bytes)
    {
        SND_ASSERT(Position >= bytes);
        Position -= bytes;
    }

    void Stack::Clear()
    {
        Position = 0;
    }

    StackScopeBlock::StackScopeBlock(Stack* stack, u64 size)
        : Owner(stack), Size(size)
    {
        SND_ASSERT(stack);
        Data = stack->Push(size);
    }

    StackScopeBlock::~StackScopeBlock()
    {
        Owner->Pop(Size);
    }
}
