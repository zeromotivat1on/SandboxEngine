#pragma once

#include "SandboxEngine/Core/Timer.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/Delegate.h"

namespace snd::input
{
    // Class that detects rapid tapping of the given button.
    template <typename TInputBit>
    class ButtonTapDetector
    {
    public:
                            ButtonTapDetector(TInputBit button, f32 maxDt);

        bool                GestureValid() const;
        void                Update();

    private:
        TInputBit           m_Button;
        f32                 m_MaxDt;
        f32                 m_LastTime;
    };

    // Class that detects the given button sequence.
    // When the sequence is successfully detected, the given delegate is executed.
    template <typename TInputBit>
    class ButtonSequenceDetector
    {
    public:
                            ButtonSequenceDetector(TInputBit* buttons, u32 buttonCount, f32 maxDt, SimpleDelegate& delegate);

        void                Update();

    private:
        SimpleDelegate*     m_Delegate;
        TInputBit*          m_Buttons;
        u32                 m_ButtonCount;
        u32                 m_NextButtonIndex;
        f32                 m_MaxDt;
        f32                 m_StartTime;
    };

    // ButtonTapDetector

    template <typename TInputBit>
    ButtonTapDetector<TInputBit>::ButtonTapDetector(TInputBit button, f32 maxDt)
        : m_Button(button), m_MaxDt(maxDt), m_LastTime(CurrentTime() - maxDt)
    {
    }

    template <typename TInputBit>
    bool ButtonTapDetector<TInputBit>::GestureValid() const
    {
        const f32 dt = CurrentTime() - m_LastTime;
        return (dt < m_MaxDt);
    }

    template <typename TInputBit>
    void ButtonTapDetector<TInputBit>::Update()
    {
        if (input::ButtonJustWentDown(m_Button))
        {
            m_LastTime = CurrentTime();
        }
    }

    // ButtonSequenceDetector

    template <typename TInputBit>
    ButtonSequenceDetector<TInputBit>::ButtonSequenceDetector(TInputBit* buttons, u32 buttonCount, f32 maxDt, SimpleDelegate& delegate)
        : m_Delegate(&delegate), m_Buttons(buttons), m_ButtonCount(buttonCount), m_NextButtonIndex(0), m_MaxDt(maxDt), m_StartTime(0)
    {
    }

    template <typename TInputBit>
    void ButtonSequenceDetector<TInputBit>::Update()
    {
        SND_ASSERT(m_NextButtonIndex < m_ButtonCount);

        const TInputBit nextButton = m_Buttons[m_NextButtonIndex];
            
        // If any button other than the expected button just went down, invalidate the sequence.
        if (input::AnyButtonJustWentDownExcept(nextButton))
        {
            m_NextButtonIndex = 0; // reset
            return;
        }
            
        // Otherwise, if the expected button just went down, check dt and update our state appropriately.
        if (input::ButtonJustWentDown(nextButton))
        {
            if (m_NextButtonIndex == 0)
            {
                // The first button in the sequence.
                m_StartTime = CurrentTime();
                m_NextButtonIndex++;
                return;
            }
                
            const f32 dt = CurrentTime() - m_StartTime;
            if (dt < m_MaxDt)
            {
                // Sequence is still valid.
                m_NextButtonIndex++;
                if (m_NextButtonIndex == m_ButtonCount)
                {
                    (void)m_Delegate->ExecuteSafe();
                    m_NextButtonIndex = 0;
                }
            }
            else
            {
                // Sorry, not fast enough.
                m_NextButtonIndex = 0;
            }
        }
    }
}
