#pragma once

#include "SandboxEngine/Core/Input.h"

namespace snd::input
{
    // Class that detects rapid tapping of the given button.
    template <typename TInputBit>
    class ButtonTapDetector
    {
    public:
        ButtonTapDetector(TInputBit button, float maxDt)
            : m_Button(button), m_MaxDt(maxDt), m_LastTime(CurrentTime() - maxDt)
        {
        }

        // Query whether the gesture is currently being performed.
        inline bool GestureValid() const
        {
            const float dt = CurrentTime() - m_LastTime;
            return (dt < m_MaxDt);
        }

        // Call this once per frame.
        inline void Update()
        {
            if (input::ButtonJustWentDown(m_Button))
            {
                m_LastTime = CurrentTime();
            }
        }

    private:
        TInputBit m_Button = 0;     // button to observe
        float m_MaxDt = 0.0f;       // max allowed time between presses
        float m_LastTime = 0.0f;    // last button-down event, in seconds
    };

    // Class that detects the given button sequence.
    // When the sequence is successfully detected, the given event is broadcast
    // so that the rest of the game can respond in an appropriate way.
    // Alternatively, you can check for gesture validity at any point of execution.
    template <typename TInputBit>
    class ButtonSequenceDetector
    {
    public:
        ButtonSequenceDetector(TInputBit* buttons, uint32_t buttonCount, float maxDt /*EventId eventIdToSend*/)
            : m_Buttons(buttons), m_ButtonCount(buttonCount), m_MaxDt(maxDt)
            //m_eventId(eventIdToSend)
        {
        }

        // Call this once per frame.
        inline void Update()
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
                
                const float dt = CurrentTime() - m_StartTime;
                if (dt < m_MaxDt)
                {
                    // Sequence is still valid.
                    m_NextButtonIndex++;
                    if (m_NextButtonIndex == m_ButtonCount)
                    {
                        //BroadcastEvent(m_eventId);
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

    private:
        TInputBit* m_Buttons = nullptr; // sequence of buttons to watch for
        uint32_t m_ButtonCount = 0;     // number of buttons in sequence
        uint32_t m_NextButtonIndex = 0; // next button to watch for in seq.
        float m_MaxDt = 0.0f;           // max time for entire sequence
        float m_StartTime = 0.0f;       // start time of sequence, in seconds
    };
}
