#ifndef __TEXT_BUFFER_H__
#define __TEXT_BUFFER_H__

#include "main.h"
#include "DoublyLinkedList.h"

class TextBuffer
{
public:
    class HistoryManager
    {
    public:
        struct Action
        {
            string actionName;
            int cursorBefore;
            int cursorAfter;
            char data;

            bool operator==(const Action &other) const
            {
                return actionName == other.actionName &&
                       cursorBefore == other.cursorBefore &&
                       cursorAfter == other.cursorAfter &&
                       data == other.data;
            }

            friend std::ostream &operator<<(std::ostream &os, const Action &a)
            {
                os << "("
                   << a.actionName << ", "
                   << a.cursorBefore << ", "
                   << a.cursorAfter << ", "
                   << a.data << ")";
                return os;
            }
        };

    private:
        DoublyLinkedList<Action> actions;
        int currentIndex;

    public:
        HistoryManager();
        ~HistoryManager();

        void addAction(const Action &a);
        bool canUndo() const;
        bool canRedo() const;
        void printHistory() const;

        Action getLastAct() const;
        Action getCurrentAction() const;
        void moveBack();
        void moveForward();

        friend class TestHelper;
        friend class TextBuffer;
    };

private:
    DoublyLinkedList<char> buffer;
    int cursorPos;

    HistoryManager *history;

    static bool compareAlphabet(const char &a, const char &b)
    {
        if (tolower(a) == tolower(b))
            return a < b;
        return tolower(a) < tolower(b);
    }

public:
    TextBuffer();
    ~TextBuffer();

    void insert(char c);
    void deleteChar();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorTo(int index);
    string getContent() const;
    int getCursorPos() const;
    int findFirstOccurrence(char c) const;
    int *findAllOccurrences(char c, int &count) const;
    void sortAscending();
    void deleteAllOccurrences(char c);
    void undo();
    void redo();
    void printHistory() const;

    friend class TestHelper;
};

#endif // __TEXT_BUFFER_H__
