#include "SandboxEngine/sndpch.h"
#include "SandboxEngine/Memory/StackAllocator.h"
#include <gtest/gtest.h>

constexpr u32 g_AlcCapacity = 1000;
snd::StackAllocator g_Alc = snd::StackAllocator(g_AlcCapacity);
u32 g_LastAllocSize = 0;

TEST(StackAllocatorTest, Construct)
{
    EXPECT_NE(g_Alc.Data(), nullptr);
    EXPECT_EQ(g_Alc.Size(),     0);
    EXPECT_EQ(g_Alc.Capacity(), g_AlcCapacity);
}

TEST(StackAllocatorTest, Alloc)
{
    void* data = nullptr;
    g_LastAllocSize = 100;

    data = g_Alc.Alloc(g_LastAllocSize);

    EXPECT_NE(data, nullptr);
    EXPECT_EQ(g_Alc.Size(), g_LastAllocSize);
    EXPECT_EQ(g_Alc.Capacity(), g_AlcCapacity);

    g_LastAllocSize = 200;
    data = g_Alc.Alloc(g_LastAllocSize);

    EXPECT_NE(data, nullptr);
    EXPECT_EQ(g_Alc.Size(), 300);
    EXPECT_EQ(g_Alc.Capacity(), g_AlcCapacity);
}

TEST(StackAllocatorTest, Fit)
{
    const u32 sizeWithoutLastAllocSize = g_Alc.Size() - g_LastAllocSize;
    g_Alc.Fit(sizeWithoutLastAllocSize);

    EXPECT_EQ(g_Alc.Size(), sizeWithoutLastAllocSize);
    EXPECT_EQ(g_Alc.Capacity(), g_AlcCapacity);
}

TEST(StackAllocatorTest, Clear)
{
    g_Alc.Clear();

    EXPECT_NE(g_Alc.Data(), nullptr);
    EXPECT_EQ(g_Alc.Size(), 0);
    EXPECT_EQ(g_Alc.Capacity(), g_AlcCapacity);
}

TEST(StackAllocatorTest, Free)
{
    g_Alc.Free();

    EXPECT_EQ(g_Alc.Data(), nullptr);
    EXPECT_EQ(g_Alc.Size(), 0);
    EXPECT_EQ(g_Alc.Capacity(), 0);
}
