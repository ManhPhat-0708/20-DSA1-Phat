#ifndef __TEST_HELPER_H__
#define __TEST_HELPER_H__

#include "src/TextBuffer.h"

class TestHelper
{
public:
    // --- TextBuffer ---
    static void setCursorPos(TextBuffer &tb, int pos)
    {
        tb.cursorPos = pos;
    }

    static int getCursorPos(const TextBuffer &tb)
    {
        return tb.cursorPos;
    }

    static TextBuffer::HistoryManager *getHistory(TextBuffer &tb)
    {
        return tb.history;
    }

    static void setHistory(TextBuffer &tb, TextBuffer::HistoryManager *hm)
    {
        tb.history = hm;
    }

    // --- HistoryManager ---
    static int getCurrentIndex(const TextBuffer::HistoryManager &hm)
    {
        return hm.currentIndex;
    }

    static void setCurrentIndex(TextBuffer::HistoryManager &hm, int idx)
    {
        hm.currentIndex = idx;
    }
};

#endif // __TEST_HELPER_H__
