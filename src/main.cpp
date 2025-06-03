#include "VT100Console.h"
#include <circle/startup.h>
#include <circle/bcmframebuffer.h>
#include <circle/usb/usbkeyboard.h>
#include <circle/usb/usbhcidevice.h>
#include <circle/usb/usbkeyboarddevice.h>
#include <circle/devices/serial.h>
#include <circle/input/keyboard.h>

class CTerminal : public CKernel
{
public:
    CTerminal() :
        m_Timer(&m_Interrupt),
        m_Serial(&m_Interrupt),
        m_USB(&m_Interrupt),
        m_KeyboardHandler(&m_Console),
        m_Console(&m_FB)
    {}

    boolean Initialize()
    {
        m_FB.SetScreenSize(640, 384);
        m_FB.Initialize();
        m_Console.Initialize();

        m_Serial.Initialize(115200);
        m_USB.Initialize();
        m_USB.RegisterDriver(&m_Keyboard);

        m_Keyboard.Initialize(&m_USB);
        m_Keyboard.SetEventHandler(&m_KeyboardHandler);

        return TRUE;
    }

    void Run()
    {
        char c;
        while (1)
        {
            if (m_KeyboardHandler.HasKey())
            {
                c = m_KeyboardHandler.GetChar();
                m_Console.WriteChar(c);
                m_Serial.Write(c);
            }

            if (m_Serial.IsDataReady())
            {
                c = m_Serial.Read();
                m_Console.WriteChar(c);
            }

            m_Console.Tick();
        }
    }

private:
    CTimer m_Timer;
    CSerialDevice m_Serial;
    CUSBHCIDevice m_USB;
    CUSBKeyboardDevice m_Keyboard;
    CUSBKeyboardHandler m_KeyboardHandler;
    CBcmFramebuffer m_FB;
    VT100Console m_Console;
};

CTerminal Terminal;
extern "C" void main(void) { Terminal.Run(); }
