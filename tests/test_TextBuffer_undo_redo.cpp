#include "doctest/doctest.h"
#include "TextBuffer.h"

TEST_SUITE("TextBuffer - Undo")
{
    TEST_CASE("Undo insert once")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.undo();
        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Undo delete once")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.deleteChar();
        buf.undo();
        CHECK(buf.getContent() == "A");
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Undo move once")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.moveCursorLeft();
        buf.undo();
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("Multiple undo steps")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.insert('C');
        buf.undo();
        buf.undo();
        CHECK(buf.getContent() == "A");
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Undo then insert clears redo history")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.undo();
        buf.insert('X');
        buf.redo();
        CHECK(buf.getContent() == "AX");
    }

    TEST_CASE("Undo all available actions")
    {
        TextBuffer buf;
        for (char c : std::string("abc"))
            buf.insert(c);
        for (int i = 0; i < 3; ++i)
            buf.undo();
        CHECK(buf.getContent() == "");
    }

    TEST_CASE("Undo move and insert combined")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.moveCursorLeft();
        buf.insert('X');
        buf.undo();
        CHECK(buf.getContent() == "AB");
    }

    TEST_CASE("Undo delete in middle")
    {
        TextBuffer buf;
        for (char c : std::string("abc"))
            buf.insert(c);
        buf.moveCursorTo(2);
        buf.deleteChar();
        buf.undo();
        CHECK(buf.getContent() == "abc");
    }

    TEST_CASE("Undo does nothing when history empty")
    {
        TextBuffer buf;
        buf.undo();
        CHECK(buf.getContent() == "");
    }

    TEST_CASE("Undo all then redo all - undo part")
    {
        TextBuffer buf;
        for (char c : std::string("xyz"))
            buf.insert(c);
        buf.undo();
        buf.undo();
        buf.undo();
        CHECK(buf.getContent() == "");
    }
}

TEST_SUITE("TextBuffer - Redo")
{
    TEST_CASE("Redo insert once")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.undo();
        buf.redo();
        CHECK(buf.getContent() == "A");
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Redo delete once")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.deleteChar();
        buf.undo();
        buf.redo();
        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Redo move once")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.moveCursorLeft();
        buf.undo();
        buf.redo();
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Multiple redo steps")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.insert('C');
        buf.undo();
        buf.undo();
        buf.redo();
        buf.redo();
        CHECK(buf.getContent() == "ABC");
        CHECK(buf.getCursorPos() == 3);
    }

    TEST_CASE("Redo all available actions")
    {
        TextBuffer buf;
        for (char c : std::string("abc"))
            buf.insert(c);
        for (int i = 0; i < 3; ++i)
            buf.undo();
        for (int i = 0; i < 3; ++i)
            buf.redo();
        CHECK(buf.getContent() == "abc");
    }

    TEST_CASE("Redo after undo move and insert")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.moveCursorLeft();
        buf.insert('X');
        buf.undo();
        buf.redo();
        CHECK(buf.getContent() == "AXB");
    }

    TEST_CASE("Redo delete in middle")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('y');
        buf.insert('z');
        buf.moveCursorTo(2);
        buf.deleteChar();

        CHECK_EQ(buf.getContent(), "xz");
        CHECK_EQ(buf.getCursorPos(), 1);

        buf.undo();
        CHECK_EQ(buf.getContent(), "xyz");
        CHECK_EQ(buf.getCursorPos(), 2);

        buf.redo();
        CHECK_EQ(buf.getContent(), "xz");
        CHECK_EQ(buf.getCursorPos(), 1);
    }

    TEST_CASE("Redo does nothing when history empty")
    {
        TextBuffer buf;
        buf.redo();
        CHECK(buf.getContent() == "");
    }

    TEST_CASE("Undo insert then redo restores content")
    {
        TextBuffer buf;
        buf.insert('Z');
        buf.undo();
        buf.redo();
        CHECK(buf.getContent() == "Z");
    }

    TEST_CASE("Undo all then redo all - redo part")
    {
        TextBuffer buf;
        for (char c : std::string("xyz"))
            buf.insert(c);
        buf.undo();
        buf.undo();
        buf.undo();
        buf.redo();
        buf.redo();
        buf.redo();
        CHECK(buf.getContent() == "xyz");
    }
}
