#include "sndpch.h"
#include "SandboxEngine/Core/EntryPoint.h"
#include "SandboxEngine/Engine/EngineLoop.h"

u32 ThreadEntryPoint(void* data)
{
    snd::thread::WorkQueue* wq = (snd::thread::WorkQueue*)data;

    snd::thread::WorkQueue::OnEntryProcess callback;
    callback.BindLambda([](const snd::thread::WorkQueue* wq, void* data)
    {
        SND_CORE_INFO("____ Callback: Thread {}: data - {}", snd::thread::CurrentId(), (const char*)data);
    });

    u8 c = 0;

    while(true)
    {
        if (!wq->ProcessEntry())
        {
            wq->WaitForWork(UINT32_MAX);
        }

        if (c == 0)
        {
            //wq->AddEntry(callback, "String ____");
            c = (c + 1) % 2;
        }
    }

    return 0;
}

u32 ThreadEntryPointAWQE(void* data)
{
    snd::thread::WorkQueue* wq = (snd::thread::WorkQueue*)data;

    snd::thread::WorkQueue::OnEntryProcess callback;
    callback.BindLambda([](const snd::thread::WorkQueue* wq, void* data)
    {
        SND_CORE_INFO("AWQE Callback: Thread {}: data - {}", snd::thread::CurrentId(), (const char*)data);
    });

    // wq->AddEntry(callback, "String X1");
    // wq->AddEntry(callback, "String X2");
    // wq->AddEntry(callback, "String X3");
    // wq->AddEntry(callback, "String X4");
    // wq->AddEntry(callback, "String X5");
    // wq->AddEntry(callback, "String X6");
    // wq->AddEntry(callback, "String X7");
    // wq->AddEntry(callback, "String X8");
    // wq->AddEntry(callback, "String X9");

    u8 c = 0;

    while(true)
    {
        if (!wq->ProcessEntry())
        {
            wq->WaitForWork(UINT32_MAX);
        }

        if (c == 0)
        {
            //wq->AddEntry(callback, "String AWQE");
            c = (c + 1) % 2;
        }
    }

    return 0;
}

u32 ThreadEntryPointAWQE2(void* data)
{
    snd::thread::WorkQueue* wq = (snd::thread::WorkQueue*)data;

    snd::thread::WorkQueue::OnEntryProcess callback;
    callback.BindLambda([](const snd::thread::WorkQueue* wq, void* data)
    {
        SND_CORE_INFO("AWQE2 Callback: Thread {}: data - {}", snd::thread::CurrentId(), (const char*)data);
    });

    // wq->AddEntry(callback, "String Y1");
    // wq->AddEntry(callback, "String Y2");
    // wq->AddEntry(callback, "String Y3");
    // wq->AddEntry(callback, "String Y4");
    // wq->AddEntry(callback, "String Y5");
    // wq->AddEntry(callback, "String Y6");
    // wq->AddEntry(callback, "String Y7");
    // wq->AddEntry(callback, "String Y8");
    // wq->AddEntry(callback, "String Y9");

    u8 c = 0;

    while(true)
    {
        if (!wq->ProcessEntry())
        {
            wq->WaitForWork(UINT32_MAX);
        }

        if (c == 0)
        {
            //wq->AddEntry(callback, "String AWQE2");
            c = (c + 1) % 2;
        }
    }

    return 0;
}

int snd::EntryPoint(int argc, const char** argv)
{
    log::Init();

    thread::WorkQueue wq(thread::MakeSemaphore(0, 1));

    thread::Create(thread::CreateType::Immediate, ThreadEntryPointAWQE, &wq);
    thread::Create(thread::CreateType::Immediate, ThreadEntryPointAWQE2, &wq);

    for (u8 i = 0; i < 10; ++i)
    {
       thread::Create(thread::CreateType::Immediate, ThreadEntryPoint, &wq);
    }

    thread::WorkQueue::OnEntryProcess callback;
    callback.BindLambda([](const thread::WorkQueue* wq, void* data)
    {
        SND_CORE_INFO("Main Callback: Thread {}: data - {}", thread::CurrentId(), (const char*)data);
    });

    for (u32 i = 0; i < 15; ++i)
    {
        wq.AddEntry(callback, "String C1");
        wq.AddEntry(callback, "String C2");
        wq.AddEntry(callback, "String C3");
        wq.AddEntry(callback, "String C4");
        wq.AddEntry(callback, "String C5");
        wq.AddEntry(callback, "String C6");
        wq.AddEntry(callback, "String C7");
        wq.AddEntry(callback, "String C8");
        wq.AddEntry(callback, "String C9");

        wq.AddEntry(callback, "String B1");
        wq.AddEntry(callback, "String B2");
        wq.AddEntry(callback, "String B3");
        wq.AddEntry(callback, "String B4");
        wq.AddEntry(callback, "String B5");
        wq.AddEntry(callback, "String B6");
        wq.AddEntry(callback, "String B7");
        wq.AddEntry(callback, "String B8");
        wq.AddEntry(callback, "String B9");
    }

    SND_CORE_INFO("Work done!");
    while(wq.InProgress()) {}

    // EngineLoop engineLoop;
    // engineLoop.Init();
    // engineLoop.Run();
    // engineLoop.Shutdown();

	return EXIT_SUCCESS;
}
