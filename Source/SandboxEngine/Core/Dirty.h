#pragma once

#include <optional>

namespace snd
{
    // Template type with an ability to detect its dirty state.
    template <typename T>
    class Dirty
    {
    public:
                            Dirty();
        explicit            Dirty(const T& value);
        explicit            Dirty(T&& value);

        Dirty&              operator=(const T& value);
        Dirty&              operator=(T&& value);

        void                Set(const T& value);
        void                Set(T&& value);

        const T&            Current() const;
        const auto&         Old() const;

        bool                IsDirty() const;
        void                MakeDirty();
        void                ClearDirty();

    private:
        T                   m_Current;
        std::optional<T>    m_Old;
        bool                m_IsDirty;
    };

    template <typename T>
    Dirty<T>::Dirty()
        : m_Current(), m_IsDirty(false)
    {
    }

    template <typename T>
    Dirty<T>::Dirty(const T& value)
        : m_Current(value), m_IsDirty(false)
    {
    }

    template <typename T>
    Dirty<T>::Dirty(T&& value)
        : m_Current(std::move(value)), m_IsDirty(false)
    {
    }

    template <typename T>
    Dirty<T>& Dirty<T>::operator=(const T& value)
    {
        Set(value);
        return *this;
    }

    template <typename T>
    Dirty<T>& Dirty<T>::operator=(T&& value)
    {
        Set(value);
        return *this;
    }

    template <typename T>
    void Dirty<T>::Set(const T& value)
    {
        if (m_Current != value)
        {
            m_Old = m_Current;
            m_Current = value;
            m_IsDirty = true;
        }
    }

    template <typename T>
    void Dirty<T>::Set(T&& value)
    {
        if (m_Current != value)
        {
            m_Old = std::move(m_Current);
            m_Current = std::move(value);
            m_IsDirty = true;
        }
    }

    template <typename T>
    const T& Dirty<T>::Current() const
    {
        return m_Current;
    }

    template <typename T>
    const auto& Dirty<T>::Old() const
    {
        return m_Old;
    }

    template <typename T>
    bool Dirty<T>::IsDirty() const
    {
        return m_IsDirty;
    }

    template <typename T>
    void Dirty<T>::MakeDirty()
    {
        m_IsDirty = true;
    }

    template <typename T>
    void Dirty<T>::ClearDirty()
    {
        m_IsDirty = false;
    }
}
