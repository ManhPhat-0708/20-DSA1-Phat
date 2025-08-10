#include "doctest/doctest.h"
#include "TextBuffer.h"

TEST_SUITE("TextBuffer - insert")
{

    TEST_CASE("Insert single character at beginning")
    {
        TextBuffer buf;
        buf.insert('A');

        CHECK(buf.getContent() == "A");
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Insert multiple characters sequentially")
    {
        TextBuffer buf;
        buf.insert('H');
        buf.insert('e');
        buf.insert('l');
        buf.insert('l');
        buf.insert('o');

        CHECK(buf.getContent() == "Hello");
        CHECK(buf.getCursorPos() == 5);
    }

    TEST_CASE("Insert in middle of existing content")
    {
        TextBuffer buf;
        buf.insert('H');
        buf.insert('l');
        buf.insert('o');
        buf.moveCursorLeft(); // pos = 2
        buf.moveCursorLeft(); // pos = 1
        buf.insert('e');

        CHECK(buf.getContent() == "Helo");
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("Insert at end after moving cursor")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.insert('B');
        buf.insert('C');
        buf.moveCursorLeft();  // pos = 2
        buf.moveCursorLeft();  // pos = 1
        buf.moveCursorRight(); // pos = 2
        buf.insert('D');

        CHECK(buf.getContent() == "ABDC");
        CHECK(buf.getCursorPos() == 3);
    }

    TEST_CASE("Insert into empty buffer multiple times")
    {
        TextBuffer buf;
        for (char ch : std::string("abc"))
        {
            buf.insert(ch);
        }

        CHECK(buf.getContent() == "abc");
        CHECK(buf.getCursorPos() == 3);
    }
}

TEST_SUITE("TextBuffer - deleteChar")
{

    TEST_CASE("Delete on empty buffer does nothing")
    {
        TextBuffer buf;
        CHECK_THROWS_WITH(buf.deleteChar(), "Index is invalid!");

        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Delete after inserting one character")
    {
        TextBuffer buf;
        buf.insert('A');
        CHECK(buf.getContent() == "A");

        buf.deleteChar();
        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Delete in the middle of the buffer")
    {
        TextBuffer buf;
        buf.insert('H');
        buf.insert('e');
        buf.insert('l');
        buf.insert('l');
        buf.insert('o');

        buf.moveCursorLeft();
        buf.moveCursorLeft();

        buf.deleteChar();

        CHECK(buf.getContent() == "Helo");
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("Delete all characters one by one")
    {
        TextBuffer buf;
        for (char ch : std::string("abc"))
        {
            buf.insert(ch);
        }
        buf.deleteChar();
        buf.deleteChar();
        buf.deleteChar();

        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Delete at beginning does nothing")
    {
        TextBuffer buf;
        buf.insert('A');
        buf.moveCursorLeft();
        CHECK_THROWS_WITH(buf.deleteChar(), "Index is invalid!");

        CHECK(buf.getContent() == "A");
        CHECK(buf.getCursorPos() == 0);
    }
    TEST_CASE("Insert multiple characters then delete in reverse order")
    {
        TextBuffer buf;

        buf.insert('a');
        buf.insert('b');
        buf.insert('c');
        buf.insert('d');
        CHECK(buf.getContent() == "abcd");
        CHECK(buf.getCursorPos() == 4);

        buf.deleteChar();
        CHECK(buf.getContent() == "abc");
        CHECK(buf.getCursorPos() == 3);

        buf.deleteChar();
        CHECK(buf.getContent() == "ab");
        CHECK(buf.getCursorPos() == 2);

        buf.deleteChar();
        CHECK(buf.getContent() == "a");
        CHECK(buf.getCursorPos() == 1);

        buf.deleteChar();
        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Insert and delete alternately to test cursor and buffer integrity")
    {
        TextBuffer buf;

        buf.insert('x');
        CHECK(buf.getContent() == "x");
        CHECK(buf.getCursorPos() == 1);

        buf.deleteChar();
        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);

        buf.insert('y');
        buf.insert('z');
        CHECK(buf.getContent() == "yz");
        CHECK(buf.getCursorPos() == 2);

        buf.deleteChar();
        CHECK(buf.getContent() == "y");
        CHECK(buf.getCursorPos() == 1);

        buf.insert('w');
        CHECK(buf.getContent() == "yw");
        CHECK(buf.getCursorPos() == 2);

        buf.moveCursorLeft();
        buf.deleteChar();
        CHECK(buf.getContent() == "w");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Complex insert-delete pattern with loop and checks")
    {
        TextBuffer buf;

        // Insert letters a-j
        for (char c = 'a'; c <= 'j'; ++c)
        {
            buf.insert(c);
        }
        CHECK(buf.getContent() == "abcdefghij");
        CHECK(buf.getCursorPos() == 10);

        // Move cursor left 5 times
        for (int i = 0; i < 5; ++i)
        {
            buf.moveCursorLeft();
        }
        CHECK(buf.getCursorPos() == 5);

        // Delete 3 characters from middle
        for (int i = 0; i < 3; ++i)
        {
            buf.deleteChar();
        }
        CHECK(buf.getContent() == "abfghij");
        CHECK(buf.getCursorPos() == 2);

        // Insert letters x, y, z
        buf.insert('x');
        buf.insert('y');
        buf.insert('z');
        CHECK(buf.getContent() == "abxyzfghij");
        CHECK(buf.getCursorPos() == 5);

        buf.deleteChar();
        buf.deleteChar();
        CHECK(buf.getContent() == "abxfghij");
        CHECK(buf.getCursorPos() == 3);
    }
}

TEST_SUITE("TextBuffer - Cursor Movements")
{

    TEST_CASE("Cursor position after insertions")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('b');
        buf.insert('c');
        CHECK(buf.getCursorPos() == 3);
        CHECK(buf.getContent() == "abc");
    }

    TEST_CASE("Move cursor left step-by-step")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('y');
        buf.insert('z');
        buf.moveCursorLeft();
        CHECK(buf.getCursorPos() == 2);
        buf.moveCursorLeft();
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Move cursor left throws at beginning")
    {
        TextBuffer buf;
        CHECK_THROWS_AS(buf.moveCursorLeft(), cursor_error);
        buf.insert('a');
        buf.moveCursorLeft();
        CHECK_THROWS_AS(buf.moveCursorLeft(), cursor_error);
    }

    TEST_CASE("Move cursor right step-by-step")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('b');
        buf.insert('c');
        buf.moveCursorLeft();
        buf.moveCursorLeft();
        CHECK(buf.getCursorPos() == 1);
        buf.moveCursorRight();
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("Move cursor right throws at end")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('y');
        buf.insert('z');
        CHECK_THROWS_AS(buf.moveCursorRight(), cursor_error);
    }

    TEST_CASE("Move cursor to valid position")
    {
        TextBuffer buf;
        for (char c = 'a'; c <= 'e'; ++c)
            buf.insert(c);
        buf.moveCursorTo(2);
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("Move cursor to invalid position throws")
    {
        TextBuffer buf;
        buf.insert('x');
        CHECK_THROWS_AS(buf.moveCursorTo(-1), std::out_of_range);
        CHECK_THROWS_AS(buf.moveCursorTo(2), std::out_of_range);
    }

    TEST_CASE("Get content remains unchanged after cursor move")
    {
        TextBuffer buf;
        std::string s = "hello";
        for (char c : s)
            buf.insert(c);
        buf.moveCursorTo(2);
        CHECK(buf.getContent() == "hello");
    }

    TEST_CASE("Move cursor to beginning and insert")
    {
        TextBuffer buf;
        buf.insert('b');
        buf.insert('c');
        buf.moveCursorTo(0);
        buf.insert('a');
        CHECK(buf.getContent() == "abc");
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("Full cursor movement and integrity check")
    {
        TextBuffer buf;
        std::string s = "abcde";
        for (char c : s)
            buf.insert(c);
        for (int i = 0; i < 5; ++i)
            buf.moveCursorLeft();
        CHECK(buf.getCursorPos() == 0);
        for (int i = 0; i < 5; ++i)
            buf.moveCursorRight();
        CHECK(buf.getCursorPos() == 5);
        CHECK(buf.getContent() == "abcde");
    }

    TEST_CASE("moveCursorLeft throws at beginning")
    {
        TextBuffer buf;
        buf.insert('A');
        CHECK(buf.getCursorPos() == 1);
        buf.moveCursorLeft();
        CHECK(buf.getCursorPos() == 0);
        CHECK_THROWS_WITH(buf.moveCursorLeft(), "Cursor error!");
    }

    TEST_CASE("moveCursorRight throws at end")
    {
        TextBuffer buf;
        buf.insert('B');
        CHECK(buf.getCursorPos() == 1);
        CHECK_THROWS_WITH(buf.moveCursorRight(), "Cursor error!");
    }

    TEST_CASE("moveCursorTo negative index")
    {
        TextBuffer buf;
        buf.insert('C');
        CHECK_THROWS_WITH(buf.moveCursorTo(-1), "Index is invalid!");
    }

    TEST_CASE("moveCursorTo index > size")
    {
        TextBuffer buf;
        buf.insert('D');
        CHECK(buf.getCursorPos() == 1);
        CHECK_THROWS_WITH(buf.moveCursorTo(2), "Index is invalid!");
    }

    TEST_CASE("moveCursorTo valid at 0 and 1")
    {
        TextBuffer buf;
        buf.insert('E');
        buf.moveCursorTo(0);
        CHECK(buf.getCursorPos() == 0);
        buf.moveCursorTo(1);
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("mixing moveCursor and insert")
    {
        TextBuffer buf;
        buf.insert('H');
        buf.insert('I');
        buf.insert('J');
        CHECK(buf.getContent() == "HIJ");
        buf.moveCursorTo(1);
        buf.insert('X');
        CHECK(buf.getContent() == "HXIJ");
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("multiple moveCursorLeft then insert")
    {
        TextBuffer buf;
        for (char c : std::string("abc"))
            buf.insert(c);    // cursor at 3
        buf.moveCursorLeft(); // 2
        buf.moveCursorLeft(); // 1
        buf.insert('Z');
        CHECK(buf.getContent() == "aZbc");
        CHECK(buf.getCursorPos() == 2);
    }

    TEST_CASE("getContent after many cursor moves")
    {
        TextBuffer buf;
        for (char c : std::string("xyz"))
            buf.insert(c);
        buf.moveCursorTo(1);
        buf.insert('Q');
        CHECK(buf.getContent() == "xQyz");
    }

    TEST_CASE("getCursorPos correctness")
    {
        TextBuffer buf;
        buf.insert('M');
        CHECK(buf.getCursorPos() == 1);
        buf.moveCursorTo(0);
        CHECK(buf.getCursorPos() == 0);
        buf.moveCursorRight();
        CHECK(buf.getCursorPos() == 1);
    }

    TEST_CASE("moveCursorTo exact size")
    {
        TextBuffer buf;
        for (char c : std::string("klm"))
            buf.insert(c);                  // size = 3
        CHECK_NOTHROW(buf.moveCursorTo(3)); // end is valid
        CHECK(buf.getCursorPos() == 3);
    }

    TEST_CASE("Insert history is recorded")
    {
        TextBuffer buf;
        buf.insert('a');
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        buf.printHistory();
        std::cout.rdbuf(oldCout);
        CHECK(oss.str() == "[(insert, 0, 1, a)]");
    }

    TEST_CASE("DeleteChar history is recorded")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.deleteChar();
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        buf.printHistory();
        std::cout.rdbuf(oldCout);
        CHECK(oss.str() == "[(insert, 0, 1, x), (delete, 1, 0, x)]");
    }

    TEST_CASE("MoveCursorLeft and Right history")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('b');
        buf.moveCursorLeft();
        buf.moveCursorRight();
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        buf.printHistory();
        std::cout.rdbuf(oldCout);
        CHECK(oss.str() == "[(insert, 0, 1, a), (insert, 1, 2, b), (move, 2, 1, L), (move, 1, 2, R)]");
    }

    TEST_CASE("MoveCursorTo records index")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('y');
        buf.moveCursorTo(0);
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        buf.printHistory();
        std::cout.rdbuf(oldCout);
        CHECK(oss.str() == "[(insert, 0, 1, x), (insert, 1, 2, y), (move, 2, 0, J)]");
    }

    TEST_CASE("Multiple actions are recorded in order")
    {
        TextBuffer buf;
        buf.insert('1');
        buf.insert('2');
        buf.deleteChar();
        buf.insert('3');
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        buf.printHistory();
        std::cout.rdbuf(oldCout);
        CHECK(oss.str() == "[(insert, 0, 1, 1), (insert, 1, 2, 2), (delete, 2, 1, 2), (insert, 1, 2, 3)]");
    }
}
