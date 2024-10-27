#pragma once

#include "Vendor/fastdelegate/FastDelegate.h"

// Max amount of delegate binding params is 8.

// Declare delegate of type - void().
#define DECLARE_DELEGATE(Name)                                  using Name = snd::Delegate<void>

// Declare delegate of type - ReturnType().
#define DECLARE_DELEGATE_Return(Name, ReturnType)               using Name = snd::Delegate<ReturnType>

// Declare delegate of type - void(params...)
#define DECLARE_DELEGATE_Params(Name, ...)                      using Name = snd::Delegate<void, __VA_ARGS__>

// Declare delegate of type - ReturnType(params...).
#define DECLARE_DELEGATE_Return_Params(Name, ReturnType, ...)   using Name = snd::Delegate<ReturnType, __VA_ARGS__>

// Declare multicast delegate of type - void().
#define DECLARE_DELEGATE_MULTICAST(Name)                        using Name = snd::DelegateMulticast<>

// Declare multicast delegate of type - void(params...).
#define DECLARE_DELEGATE_MULTICAST_Params(Name, ...)            using Name = snd::DelegateMulticast<__VA_ARGS__>

namespace snd
{
    // Object that allows to hold any function and call it when needed.
    // Subsequent bind calls will overwrite existing bound function if exists.
    template<typename TReturn, typename... Params>
    class Delegate
    {
    public:
        using DelegateImpl = fastdelegate::FastDelegate<TReturn(Params...)>;

    public:
        void            BindRaw(TReturn (*bindFunction)(Params... params));

        template <typename T, typename TType>
        void            BindMethod(T* owner, TReturn (TType::*bindFunction)(Params... params));

        template <typename T, typename TType>
        void            BindMethod(const T* owner, TReturn (TType::*bindFunction)(Params... params) const);

        template <typename TFunctor>
        void            BindLambda(TFunctor&& functor);

        bool            Bound() const;

        TReturn         Execute(Params... params) const;
        bool            ExecuteSafe(Params... params) const;

        void            Clear();

    private:
        DelegateImpl    mHandle;
    };

    // Multicast delegate that can hold several functions to execute.
    // Bound functions cannot return value, but can have parameters.
    template<typename... Params>
    class DelegateMulticast
    {
    public:
        using DelegateType = Delegate<void, Params...>;

    public:
        static constexpr u8 s_MaxBindings = 16;

    public:
        void            Add(const DelegateType& delegate);
        void            AddRaw(void (*bindFunction)(Params... params));

        template <typename T, typename TType>
        void            AddMethod(T* owner, void (TType::*bindFunction)(Params... params));

        template <typename T, typename TType>
        void            AddMethod(const T* owner, void (TType::*bindFunction)(Params... params) const);

        template <typename TFunctor>
        void            AddLambda(TFunctor&& functor);

        bool            Bound() const;

        void            Broadcast(Params... params) const;
        void            Clear();

    private:
        DelegateType    mBindings[s_MaxBindings];
        u8              mBindingsNum = 0;
    };

    using SimpleDelegate            = Delegate<void>;
    using SimpleDelegateMulticast   = DelegateMulticast<void>;

    // Delegate

    template <typename TReturn, typename ... Params>
    void Delegate<TReturn, Params...>::BindRaw(TReturn(* bindFunction)(Params... params))
    {
        mHandle.bind(bindFunction);
    }

    template <typename TReturn, typename ... Params>
    template <typename T, typename TType>
    void Delegate<TReturn, Params...>::BindMethod(T* owner, TReturn(TType::* bindFunction)(Params... params))
    {
        mHandle.bind(owner, bindFunction);
    }

    template <typename TReturn, typename ... Params>
    template <typename T, typename TType>
    void Delegate<TReturn, Params...>::BindMethod(const T* owner, TReturn(TType::* bindFunction)(Params... params) const)
    {
        mHandle.bind(owner, bindFunction);
    }

    template <typename TReturn, typename ... Params>
    template <typename TFunctor>
    void Delegate<TReturn, Params...>::BindLambda(TFunctor&& functor)
    {
        mHandle.bind(std::forward<TFunctor>(functor));
    }

    template <typename TReturn, typename ... Params>
    bool Delegate<TReturn, Params...>::Bound() const
    {
        return !mHandle.empty();
    }

    template <typename TReturn, typename ... Params>
    TReturn Delegate<TReturn, Params...>::Execute(Params... params) const
    {
        return mHandle(params...);
    }

    template <typename TReturn, typename ... Params>
    bool Delegate<TReturn, Params...>::ExecuteSafe(Params... params) const
    {
        if (Bound())
        {
            mHandle(params...);
            return true;
        }

        return false;
    }

    template <typename TReturn, typename ... Params>
    void Delegate<TReturn, Params...>::Clear()
    {
        mHandle.clear();
    }

    // DelegateMulticast

    template <typename ... Params>
    void DelegateMulticast<Params...>::Add(const DelegateType& delegate)
    {
        SND_ASSERT(mBindingsNum < s_MaxBindings);
        mBindings[mBindingsNum++] = delegate;
    }

    template <typename ... Params>
    void DelegateMulticast<Params...>::AddRaw(void(* bindFunction)(Params... params))
    {
        SND_ASSERT(mBindingsNum < s_MaxBindings);
        mBindings[mBindingsNum++].BindRaw(bindFunction);
    }

    template <typename ... Params>
    template <typename T, typename TType>
    void DelegateMulticast<Params...>::AddMethod(T* owner, void(TType::* bindFunction)(Params... params))
    {
        SND_ASSERT(mBindingsNum < s_MaxBindings);
        mBindings[mBindingsNum++].BindMethod(owner, bindFunction);
    }

    template <typename ... Params>
    template <typename T, typename TType>
    void DelegateMulticast<Params...>::AddMethod(const T* owner, void(TType::* bindFunction)(Params... params) const)
    {
        SND_ASSERT(mBindingsNum < s_MaxBindings);
        mBindings[mBindingsNum++].BindMethod(owner, bindFunction);
    }

    template <typename ... Params>
    template <typename TFunctor>
    void DelegateMulticast<Params...>::AddLambda(TFunctor&& functor)
    {
        SND_ASSERT(mBindingsNum < s_MaxBindings);
        mBindings[mBindingsNum++].BindLambda(std::forward<TFunctor>(functor));
    }

    template <typename ... Params>
    bool DelegateMulticast<Params...>::Bound() const
    {
        return mBindingsNum > 0;
    }

    template <typename ... Params>
    void DelegateMulticast<Params...>::Broadcast(Params... params) const
    {
        for (u8 i = 0; i < mBindingsNum; ++i)
        {
            mBindings[i].Execute(params...);
        }
    }

    template <typename ... Params>
    void DelegateMulticast<Params...>::Clear()
    {
        for (auto& binding : mBindings)
        {
            binding.Clear();
        }
    }
}
