#pragma once
#include <circle/bcmframebuffer.h>
#include "Apple1Font.h"

class VT100Console {
public:
    VT100Console(CBcmFramebuffer* fb) : m_pFB(fb) {}
    void Initialize() { Clear(); }
    void WriteChar(char c);
    void Tick();
    void Clear();

private:
    CBcmFramebuffer* m_pFB;
    char m_Buffer[24][40] = {};
    unsigned m_CursorRow = 0, m_CursorCol = 0;
    bool m_CursorVisible = false;
    char m_CursorSavedChar = ' ';
    unsigned m_BlinkCounter = 0;
    void DrawChar(u8 row, u8 col, char c, u32 fg = 0xFF00FF00, u32 bg = 0xFF000000);
    void Scroll();
};
