#include "VT100Console.h"
#include <circle/string.h>

void VT100Console::Clear() {
    for (unsigned r = 0; r < 24; ++r)
        for (unsigned c = 0; c < 40; ++c)
            m_Buffer[r][c] = ' ';
    m_pFB->Clear();
    m_CursorRow = m_CursorCol = 0;
}

void VT100Console::DrawChar(u8 row, u8 col, char c, u32 fg, u32 bg) {
    if (c < 32 || c > 127) c = '?';
    const u8* glyph = Apple1Font[c - 32];
    u32 x = col * 16;
    u32 y = row * 16;
    for (u8 r = 0; r < 16; ++r) {
        u8 bits = glyph[r];
        for (u8 bit = 0; bit < 8; ++bit) {
            u32 color = (bits & (1 << (7 - bit))) ? fg : bg;
            for (u8 dx = 0; dx < 2; ++dx)
                for (u8 dy = 0; dy < 2; ++dy)
                    m_pFB->DrawPixel(x + bit*2 + dx, y + r*2 + dy, color);
        }
    }
}

void VT100Console::WriteChar(char c) {
    if (m_CursorVisible) {
        DrawChar(m_CursorRow, m_CursorCol, m_CursorSavedChar);
        m_CursorVisible = false;
    }
    if (c == '
') {
        ++m_CursorRow;
        m_CursorCol = 0;
        if (m_CursorRow >= 24) {
            Scroll();
            m_CursorRow = 23;
        }
    } else if (c == '') {
        m_CursorCol = 0;
    } else if (c == '') {
        if (m_CursorCol > 0) --m_CursorCol;
        DrawChar(m_CursorRow, m_CursorCol, ' ');
        m_Buffer[m_CursorRow][m_CursorCol] = ' ';
    } else {
        DrawChar(m_CursorRow, m_CursorCol, c);
        m_Buffer[m_CursorRow][m_CursorCol] = c;
        if (++m_CursorCol >= 40) {
            m_CursorCol = 0;
            if (++m_CursorRow >= 24) {
                Scroll();
                m_CursorRow = 23;
            }
        }
    }
}

void VT100Console::Scroll() {
    for (unsigned r = 1; r < 24; ++r)
        memcpy(m_Buffer[r - 1], m_Buffer[r], 40);
    memset(m_Buffer[23], ' ', 40);
    m_pFB->Clear();
    for (unsigned r = 0; r < 24; ++r)
        for (unsigned c = 0; c < 40; ++c)
            DrawChar(r, c, m_Buffer[r][c]);
}

void VT100Console::Tick() {
    if (++m_BlinkCounter >= 10) {
        m_BlinkCounter = 0;
        if (m_CursorVisible)
            DrawChar(m_CursorRow, m_CursorCol, m_CursorSavedChar);
        else {
            m_CursorSavedChar = m_Buffer[m_CursorRow][m_CursorCol];
            DrawChar(m_CursorRow, m_CursorCol, '@', 0xFF80FF80);
        }
        m_CursorVisible = !m_CursorVisible;
    }
}
