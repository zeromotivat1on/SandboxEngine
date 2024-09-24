#include "sndpch.h"
#include "SandboxEngine/Memory/Buffer.h"
#include "SandboxEngine/Memory/SparseBuffer.h"
#include <gtest/gtest.h>

TEST(Buffer, Buffer)
{
    snd::Buffer buffer = snd::Buffer();
    EXPECT_FALSE(buffer.Valid());
}
