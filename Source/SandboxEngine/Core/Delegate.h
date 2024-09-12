#pragma once

#include "SandboxEngine/Core/Error.h"
#include "Vendor/fastdelegate/FastDelegate.h"

// Declare delegate of type - void().
#define DECLARE_DELEGATE(Name)                                  ::snd::Delegate<void> Name

// Declare delegate of type - ReturnType().
#define DECLARE_DELEGATE_Return(Name, ReturnType)               ::snd::Delegate<ReturnType> Name

// Declare delegate of type - void(params...)
#define DECLARE_DELEGATE_Params(Name, ...)                      ::snd::Delegate<void, __VA_ARGS__> Name

// Declare delegate of type - ReturnType(params...).
// Note: max amount of params is 8.
#define DECLARE_DELEGATE_Return_Params(Name, ReturnType, ...)   ::snd::Delegate<ReturnType, __VA_ARGS__> Name

// Declare multicast delegate of type - void().
#define DECLARE_DELEGATE_MULTICAST(Name)                        ::snd::DelegateMulticast<> Name

// Declare multicast delegate of type - void(params...).
#define DECLARE_DELEGATE_MULTICAST_Params(Name, ...)            ::snd::DelegateMulticast<__VA_ARGS__> Name

namespace snd
{
    // Object that allows to hold any function and call it when needed.
    // Subsequent bind calls will overwrite existing bound function if exists.
    template<typename TReturn, typename... Params>
    class Delegate
    {
    public:
        using DelegateImpl      = fastdelegate::FastDelegate<TReturn(Params...)>;
        
    public:
        void                    BindRaw(TReturn (*bindFunction)(Params... params));

        template <class TMethodOwner, class TObj>
        void                    BindMethod(TObj* obj, TReturn (TMethodOwner::*bindFunction)(Params... params));

        template <class TMethodOwner, class TObj>
        void                    BindMethod(const TObj* obj, TReturn (TMethodOwner::*bindFunction)(Params... params) const);

        template <class TFunctor>
        void                    BindLambda(TFunctor&& functor);

        bool                    Bound() const;

        TReturn                 Execute(Params... params) const;
        bool                    ExecuteSafe(Params... params) const;

        void                    Clear();

    private:
        DelegateImpl            m_Handle;
    };

    // Multicast delegate that can hold several functions to execute.
    // Bound functions cannot return value, but can have parameters.
    template<typename... Params>
    class DelegateMulticast
    {
    public:
        using DelegateType =        Delegate<void, Params...>;

    public:
        static constexpr uint8_t    MaxBindings = 16;

    public:
        void                        Add(const DelegateType& delegate);
        void                        AddRaw(void (*bindFunction)(Params... params));

        template <class TMethodOwner, class TObj>
        void                        AddMethod(TObj* obj, void (TMethodOwner::*bindFunction)(Params... params));

        template <class TMethodOwner, class TObj>
        void                        AddMethod(const TObj* obj, void (TMethodOwner::*bindFunction)(Params... params) const);

        template <class TFunctor>
        void                        AddLambda(TFunctor&& functor);

        bool                        Bound() const;

        void                        Broadcast(Params... params) const;
        void                        Clear();

    private:
        DelegateType                m_Bindings[MaxBindings];
        uint8_t                     m_BindingsNum = 0;
    };

    using SimpleDelegate            = Delegate<void>;
    using SimpleDelegateMulitcast   = DelegateMulticast<void>;

    // Delegate

    template <typename TReturn, typename ... Params>
    void Delegate<TReturn, Params...>::BindRaw(TReturn(* bindFunction)(Params... params))
    {
        m_Handle.bind(bindFunction);
    }

    template <typename TReturn, typename ... Params>
    template <class TMethodOwner, class TObj>
    void Delegate<TReturn, Params...>::BindMethod(TObj* obj, TReturn(TMethodOwner::* bindFunction)(Params... params))
    {
        m_Handle.bind(obj, bindFunction);
    }

    template <typename TReturn, typename ... Params>
    template <class TMethodOwner, class TObj>
    void Delegate<TReturn, Params...>::BindMethod(const TObj* obj, TReturn(TMethodOwner::* bindFunction)(Params... params) const)
    {
        m_Handle.bind(obj, bindFunction);
    }

    template <typename TReturn, typename ... Params>
    template <class TFunctor>
    void Delegate<TReturn, Params...>::BindLambda(TFunctor&& functor)
    {
        m_Handle.bind(std::forward<TFunctor>(functor));
    }

    template <typename TReturn, typename ... Params>
    bool Delegate<TReturn, Params...>::Bound() const
    {
        return !m_Handle.empty();
    }

    template <typename TReturn, typename ... Params>
    TReturn Delegate<TReturn, Params...>::Execute(Params... params) const
    {
        return m_Handle(params...);
    }

    template <typename TReturn, typename ... Params>
    bool Delegate<TReturn, Params...>::ExecuteSafe(Params... params) const
    {
        if (Bound())
        {
            m_Handle(params...);
            return true;
        }

        return false;
    }

    template <typename TReturn, typename ... Params>
    void Delegate<TReturn, Params...>::Clear()
    {
        m_Handle.clear();
    }

    // DelegateMulticast
    
    template <typename ... Params>
    void DelegateMulticast<Params...>::Add(const DelegateType& delegate)
    {
        SND_ASSERT(m_BindingsNum < MaxBindings);
        m_Bindings[m_BindingsNum++] = delegate;
    }

    template <typename ... Params>
    void DelegateMulticast<Params...>::AddRaw(void(* bindFunction)(Params... params))
    {
        SND_ASSERT(m_BindingsNum < MaxBindings);
        m_Bindings[m_BindingsNum++].BindRaw(bindFunction);
    }

    template <typename ... Params>
    template <class TMethodOwner, class TObj>
    void DelegateMulticast<Params...>::AddMethod(TObj* obj, void(TMethodOwner::* bindFunction)(Params... params))
    {
        SND_ASSERT(m_BindingsNum < MaxBindings);
        m_Bindings[m_BindingsNum++].BindMethod(obj, bindFunction);
    }

    template <typename ... Params>
    template <class TMethodOwner, class TObj>
    void DelegateMulticast<Params...>::AddMethod(const TObj* obj, void(TMethodOwner::* bindFunction)(Params... params) const)
    {
        SND_ASSERT(m_BindingsNum < MaxBindings);
        m_Bindings[m_BindingsNum++].BindMethod(obj, bindFunction);
    }

    template <typename ... Params>
    template <class TFunctor>
    void DelegateMulticast<Params...>::AddLambda(TFunctor&& functor)
    {
        SND_ASSERT(m_BindingsNum < MaxBindings);
        m_Bindings[m_BindingsNum++].BindLambda(std::forward<TFunctor>(functor));
    }

    template <typename ... Params>
    bool DelegateMulticast<Params...>::Bound() const
    {
        return m_BindingsNum > 0;
    }

    template <typename ... Params>
    void DelegateMulticast<Params...>::Broadcast(Params... params) const
    {
        for (uint8_t i = 0; i < m_BindingsNum; ++i)
        {
            m_Bindings[i].Execute(params...);
        }
    }

    template <typename ... Params>
    void DelegateMulticast<Params...>::Clear()
    {
        for (auto& binding : m_Bindings)
        {
            binding.Clear();
        }
    }
}
