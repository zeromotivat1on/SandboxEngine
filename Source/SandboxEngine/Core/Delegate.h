#pragma once

#include "Vendor/fastdelegate/FastDelegate.h"

// Declare delegate of type - void().
#define DECLARE_DELEGATE(Name) ::snd::Delegate<void> Name

// Declare delegate of type - ReturnType().
#define DECLARE_DELEGATE_Return(Name, ReturnType) ::snd::Delegate<ReturnType> Name

// Declare delegate of type - void(params...)
#define DECLARE_DELEGATE_Params(Name, ...) ::snd::Delegate<void, __VA_ARGS__> Name

// Declare delegate of type - ReturnType(params...).
// Note: max amount of params is 8.
#define DECLARE_DELEGATE_Return_Params(Name, ReturnType, ...) ::snd::Delegate<ReturnType, __VA_ARGS__> Name

// Declare multicast delegate of type - void().
#define DECLARE_MULTICAST_DELEGATE(Name) ::snd::DelegateMulticast<> Name

// Declare multicast delegate of type - void(params...).
#define DECLARE_MULTICAST_DELEGATE_Params(Name, ...) ::snd::DelegateMulticast<__VA_ARGS__> Name

namespace snd
{
    // Object that allows to hold any function and call it when needed.
    // Subsequent bind calls will overwrite existing bound function if exists.
    template<typename TReturn, typename... Params>
    class Delegate
    {
    public:
        inline void BindRaw(TReturn (*bindFunction)(Params... params))
        {
            m_Handle.bind(bindFunction);
        }

        template <class TMethodOwner, class TObj>
        inline void BindMethod(TObj* obj, TReturn (TMethodOwner::*bindFunction)(Params... params))
        {
            m_Handle.bind(obj, bindFunction);
        }

        template <class TMethodOwner, class TObj>
        inline void BindMethod(const TObj* obj, TReturn (TMethodOwner::*bindFunction)(Params... params) const)
        {
            m_Handle.bind(obj, bindFunction);
        }
        
        template <class TFunctor>
        inline void BindLambda(TFunctor&& functor)
        {
            m_Handle.bind(std::forward<TFunctor>(functor));
        }

        inline bool Bound() const
        {
            return !m_Handle.empty();
        }

        // Execute delegate function without checking if its bound.
        // Returns bound function return type.
        inline TReturn Execute(Params... params) const
        {
            return m_Handle(params...);
        }

        // Execute delegate function only if its bound.
        // Returns the result of function invoke, true on success.
        inline bool ExecuteSafe(Params... params) const
        {
            if (Bound())
            {
                m_Handle(params...);
                return true;
            }

            return false;
        }
        
        inline void Clear()
        {
            m_Handle.clear();
        }
        
    private:
        fastdelegate::FastDelegate<TReturn(Params...)> m_Handle;
    };

    // Multicast delegate that can hold several functions to execute.
    // Bound functions cannot return value, but can have parameters.
    template<typename... Params>
    class DelegateMulticast
    {
    public:
        using DelegateType = Delegate<void, Params...>;
        
    public:
        static constexpr uint8_t MaxBindings = 16;

    public:
        inline void Add(const DelegateType& delegate)
        {
            SND_ASSERT(m_BindingsNum < MaxBindings);
            m_Bindings[m_BindingsNum++] = delegate;
        }
        
        inline void AddRaw(void (*bindFunction)(Params... params))
        {
            SND_ASSERT(m_BindingsNum < MaxBindings);
            m_Bindings[m_BindingsNum++].BindRaw(bindFunction);
        }

        template <class TMethodOwner, class TObj>
        inline void AddMethod(TObj* obj, void (TMethodOwner::*bindFunction)(Params... params))
        {
            SND_ASSERT(m_BindingsNum < MaxBindings);
            m_Bindings[m_BindingsNum++].BindMethod(obj, bindFunction);
        }

        template <class TMethodOwner, class TObj>
        inline void AddMethod(const TObj* obj, void (TMethodOwner::*bindFunction)(Params... params) const)
        {
            SND_ASSERT(m_BindingsNum < MaxBindings);
            m_Bindings[m_BindingsNum++].BindMethod(obj, bindFunction);
        }
        
        template <class TFunctor>
        inline void AddLambda(TFunctor&& functor)
        {
            SND_ASSERT(m_BindingsNum < MaxBindings);
            m_Bindings[m_BindingsNum++].BindLambda(std::forward<TFunctor>(functor));
        }

        inline bool Bound() const
        {
            return m_BindingsNum > 0;
        }
        
        inline void Broadcast(Params... params) const
        {
            for (uint8_t i = 0; i < m_BindingsNum; ++i)
            {
                m_Bindings[i].Execute(params...);
            }
        }
        
        inline void Clear()
        {
            for (auto& binding : m_Bindings)
            {
                binding.Clear();
            }
        }
        
    private:
        DelegateType m_Bindings[MaxBindings];
        uint8_t m_BindingsNum = 0;
    };
}
