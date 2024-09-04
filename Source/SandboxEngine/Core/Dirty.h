#pragma once

#include <optional>

namespace snd
{
    // Template type with an ability to detect its dirty state.
    template <typename T>
    class Dirty
    {
    public:
        Dirty() = default;

        explicit Dirty(const T& value)
            : m_Current(value) 
        {}

        explicit Dirty(T&& value)
            : m_Current(std::move(value)) 
        {}

        Dirty& operator=(const T& value)
        {
            if (m_Current != value)
            {
                m_Old = m_Current;
                m_Current = value;
                m_IsDirty = true;
            }

            return *this;
        }

        Dirty& operator=(T&& value)
        {
            if (m_Current != value)
            {
                m_Old = std::move(m_Current);
                m_Current = std::move(value);
                m_IsDirty = true;
            }

            return *this;
        }

        void Set(const T& value)
        {
            if (m_Current != value)
            {
                m_Old = m_Current;
                m_Current = value;
                m_IsDirty = true;
            }
        }

        void Set(T&& value)
        {
            if (m_Current != value)
            {
                m_Old = std::move(m_Current);
                m_Current = std::move(value);
                m_IsDirty = true;
            }
        }

        inline const T&                Current() const { return m_Current; }
        inline const std::optional<T>& Old()     const { return m_Old; }
        
        inline bool IsDirty() const { return m_IsDirty; }

        inline void MakeDirty()  { m_IsDirty = true; }
        inline void ClearDirty() { m_IsDirty = false; }

    private:
        T m_Current = {};
        std::optional<T> m_Old = std::nullopt;
        bool m_IsDirty = false;
    };
}
