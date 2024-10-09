#include "SandboxEngine/sndpch.h"
#include "SandboxEngine/Memory/Arena.h"
#include <gtest/gtest.h>

constexpr u64 g_ArenaSize = 1000;
snd::Arena g_Arena = snd::Arena(g_ArenaSize);
u64 g_ArenaLastPushCount = 0;

TEST(ArenaTest, Construct)
{
    EXPECT_EQ(g_Arena.Pos(), 0);
    EXPECT_EQ(g_Arena.Size(), g_ArenaSize);
}

TEST(ArenaTest, Push)
{
    void* data = nullptr;
    g_ArenaLastPushCount = 100;

    data = g_Arena.Push(g_ArenaLastPushCount);

    EXPECT_NE(data, nullptr);
    EXPECT_NE(g_Arena.Ptr(), data);
    EXPECT_EQ((u8*)g_Arena.Ptr() - g_ArenaLastPushCount, data);
    EXPECT_EQ(g_Arena.Pos(), g_ArenaLastPushCount);
    EXPECT_EQ(g_Arena.Size(), g_ArenaSize);

    g_ArenaLastPushCount = 200;
    data = g_Arena.Push(g_ArenaLastPushCount);

    EXPECT_NE(data, nullptr);
    EXPECT_EQ(g_Arena.Pos(), 300);
    EXPECT_EQ(g_Arena.Size(), g_ArenaSize);
}

TEST(ArenaTest, Pop)
{
    g_Arena.Pop(10);
    EXPECT_EQ(g_Arena.Pos(), 300 - 10);
    EXPECT_EQ(g_Arena.Size(), g_ArenaSize);
}

TEST(ArenaTest, Fit)
{
    const u32 posWithoutLastPushCount = g_Arena.Pos() - g_ArenaLastPushCount;
    g_Arena.Fit(posWithoutLastPushCount);

    EXPECT_EQ(g_Arena.Pos(), posWithoutLastPushCount);
    EXPECT_EQ(g_Arena.Size(), g_ArenaSize);
}

TEST(ArenaTest, Clear)
{
    g_Arena.Clear();

    EXPECT_EQ(g_Arena.Pos(), 0);
    EXPECT_EQ(g_Arena.Size(), g_ArenaSize);
}

TEST(ArenaTest, Free)
{
    g_Arena.Free();

    EXPECT_EQ(g_Arena.Pos(), 0);
    EXPECT_EQ(g_Arena.Size(), 0);
}
