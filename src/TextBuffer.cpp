#include "TextBuffer.h"

// ==================== HistoryManager ====================
TextBuffer::HistoryManager::HistoryManager() : currentIndex(0) {}

TextBuffer::HistoryManager::~HistoryManager() {}

// TODO

// ================== TextBuffer ==================
TextBuffer::TextBuffer() : cursorPos(0)
{
    history = new HistoryManager();
}

// TODO

void TextBuffer::printHistory() const
{
    history->printHistory();
}