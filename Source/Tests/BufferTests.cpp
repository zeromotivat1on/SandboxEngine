#include "SandboxEngine/sndpch.h"
#include "SandboxEngine/Memory/Buffer.h"
#include <gtest/gtest.h>

snd::Buffer g_Buffer;

TEST(BufferTest, Construct)
{
    g_Buffer = snd::Buffer();
    EXPECT_FALSE(g_Buffer.Valid());

    g_Buffer = snd::Buffer(1, 1);
    EXPECT_TRUE(g_Buffer.Valid());
}

TEST(BufferTest, Reallocate)
{
    g_Buffer.Realloc(100, sizeof(u64));

    EXPECT_TRUE(g_Buffer.Valid());
    EXPECT_EQ(g_Buffer.Capacity(),      100);
    EXPECT_EQ(g_Buffer.ElementSize(),   sizeof(u64));
    EXPECT_NE(g_Buffer.Data(),          nullptr);
}

TEST(BufferTest, Get)
{
    void* elem5 = g_Buffer.Get(5);

    EXPECT_NE(elem5, nullptr);
    EXPECT_EQ(elem5, (u64*)g_Buffer.Data() + 5);

    new (elem5) u64(10);

    EXPECT_EQ(*(u64*)elem5, 10);
}

TEST(BufferTest, Free)
{
    g_Buffer.Free();

    EXPECT_FALSE(g_Buffer.Valid());
}
