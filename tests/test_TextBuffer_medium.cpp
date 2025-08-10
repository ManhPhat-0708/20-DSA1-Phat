#include "doctest/doctest.h"
#include "TextBuffer.h"

TEST_SUITE("TextBuffer - findFirstOccurrence")
{
    TEST_CASE("Empty buffer returns -1")
    {
        TextBuffer buf;
        CHECK(buf.findFirstOccurrence('a') == -1);
    }

    TEST_CASE("Single matching character")
    {
        TextBuffer buf;
        buf.insert('x');
        CHECK(buf.findFirstOccurrence('x') == 0);
    }

    TEST_CASE("Single non-matching character")
    {
        TextBuffer buf;
        buf.insert('x');
        CHECK(buf.findFirstOccurrence('y') == -1);
    }

    TEST_CASE("Multiple characters, match at beginning")
    {
        TextBuffer buf;
        std::string s = "abcde";
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence('a') == 0);
    }

    TEST_CASE("Multiple characters, match in middle")
    {
        TextBuffer buf;
        std::string s = "abcde";
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence('c') == 2);
    }

    TEST_CASE("Multiple characters, match at end")
    {
        TextBuffer buf;
        std::string s = "abcde";
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence('e') == 4);
    }

    TEST_CASE("Character appears multiple times, return first occurrence")
    {
        TextBuffer buf;
        std::string s = "abacad";
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence('a') == 0);
    }

    TEST_CASE("Search for space character")
    {
        TextBuffer buf;
        std::string s = "hello world";
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence(' ') == 5);
    }

    TEST_CASE("Search for special character")
    {
        TextBuffer buf;
        std::string s = "!@#abc";
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence('@') == 1);
    }

    TEST_CASE("Character not present in large buffer")
    {
        TextBuffer buf;
        std::string s(100, 'x');
        for (char ch : s)
            buf.insert(ch);
        CHECK(buf.findFirstOccurrence('y') == -1);
    }
}

TEST_SUITE("TextBuffer - findAllOccurrences")
{

    TEST_CASE("Empty buffer returns count 0 and null array")
    {
        TextBuffer buf;
        int count = -1;
        int *result = buf.findAllOccurrences('x', count);
        CHECK_EQ(count, 0);
        CHECK(result == nullptr);
    }

    TEST_CASE("Character appears once in middle")
    {
        TextBuffer buf;
        for (char c : std::string("abc"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('b', count);
        REQUIRE(count == 1);
        CHECK(result[0] == 1);
        delete[] result;
    }

    TEST_CASE("Character appears multiple times at start")
    {
        TextBuffer buf;
        for (char c : std::string("aaabbb"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('a', count);
        REQUIRE(count == 3);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 2);
        delete[] result;
    }

    TEST_CASE("Character appears multiple times at end")
    {
        TextBuffer buf;
        for (char c : std::string("bbbaaa"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('a', count);
        REQUIRE(count == 3);
        CHECK(result[0] == 3);
        CHECK(result[1] == 4);
        CHECK(result[2] == 5);
        delete[] result;
    }

    TEST_CASE("Character appears in alternating pattern")
    {
        TextBuffer buf;
        for (char c : std::string("ababa"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('a', count);
        REQUIRE(count == 3);
        CHECK(result[0] == 0);
        CHECK(result[1] == 2);
        CHECK(result[2] == 4);
        delete[] result;
    }

    TEST_CASE("Character does not appear")
    {
        TextBuffer buf;
        for (char c : std::string("abcdefg"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('x', count);
        CHECK_EQ(count, 0);
        CHECK(result == nullptr);
    }

    TEST_CASE("Character appears once at beginning")
    {
        TextBuffer buf;
        for (char c : std::string("xabcdef"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('x', count);
        REQUIRE(count == 1);
        CHECK(result[0] == 0);
        delete[] result;
    }

    TEST_CASE("Character appears once at end")
    {
        TextBuffer buf;
        for (char c : std::string("abcdefx"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('x', count);
        REQUIRE(count == 1);
        CHECK(result[0] == 6);
        delete[] result;
    }

    TEST_CASE("All characters are the same")
    {
        TextBuffer buf;
        for (int i = 0; i < 5; ++i)
            buf.insert('z');
        int count;
        int *result = buf.findAllOccurrences('z', count);
        REQUIRE(count == 5);
        for (int i = 0; i < 5; ++i)
            CHECK(result[i] == i);
        delete[] result;
    }

    TEST_CASE("Character appears in complex pattern")
    {
        TextBuffer buf;
        for (char c : std::string("axbxcxdxex"))
            buf.insert(c);
        int count;
        int *result = buf.findAllOccurrences('x', count);
        REQUIRE(count == 5);
        CHECK(result[0] == 1);
        CHECK(result[1] == 3);
        CHECK(result[2] == 5);
        CHECK(result[3] == 7);
        CHECK(result[4] == 9);
        delete[] result;
    }

    TEST_CASE("All characters are the same")
    {
        TextBuffer buf;
        for (int i = 0; i < 100; ++i)
            buf.insert('x');
        int count = -1;
        int *result = buf.findAllOccurrences('x', count);
        CHECK(count == 100);
        for (int i = 0; i < count; ++i)
            CHECK(result[i] == i);
        delete[] result;
    }

    TEST_CASE("Alternate repeating pattern")
    {
        TextBuffer buf;
        std::string pattern = "abcabcabcabcabc";
        for (char ch : pattern)
            buf.insert(ch);
        int count = -1;
        int *result = buf.findAllOccurrences('b', count);
        CHECK(count == 5);
        CHECK(result[0] == 1);
        CHECK(result[1] == 4);
        CHECK(result[2] == 7);
        CHECK(result[3] == 10);
        CHECK(result[4] == 13);
        delete[] result;
    }

    TEST_CASE("Cursor moved between inserts")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('b');
        buf.moveCursorLeft(); // cursor at 1
        buf.insert('a');      // buffer: "aab"
        buf.moveCursorTo(1);
        buf.insert('a'); // buffer: "aaab"
        int count = -1;
        int *result = buf.findAllOccurrences('a', count);
        CHECK(count == 3);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 2);
        delete[] result;
    }

    TEST_CASE("Many scattered occurrences")
    {
        TextBuffer buf;
        for (int i = 0; i < 100; ++i)
        {
            if (i % 10 == 0)
                buf.insert('z');
            else
                buf.insert('x');
        }
        int count = -1;
        int *result = buf.findAllOccurrences('z', count);
        CHECK(count == 10);
        for (int i = 0; i < 10; ++i)
            CHECK(result[i] == i * 10);
        delete[] result;
    }
}

TEST_SUITE("TextBuffer - deleteAllOccurrences")
{

    TEST_CASE("Delete all occurrences of a character that appears multiple times")
    {
        TextBuffer buf;
        for (char c : std::string("bananan"))
            buf.insert(c);
        CHECK(buf.getCursorPos() == 7);
        buf.deleteAllOccurrences('a');
        CHECK(buf.getContent() == "bnnn");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("deleteAllOccurrences - delete before cursor")
    {
        TextBuffer buf;
        buf.insert('a'); // 0
        buf.insert('b'); // 1
        buf.insert('a'); // 2
        // Cursor at 3

        buf.moveCursorLeft();          // cursorPos = 2
        buf.deleteAllOccurrences('a'); // removes index 0, 2

        CHECK(buf.getContent() == "b");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("deleteAllOccurrences - delete at cursor")
    {
        TextBuffer buf;
        buf.insert('x'); // 0
        buf.insert('y'); // 1
        buf.insert('z'); // 2

        buf.moveCursorLeft();
        buf.deleteAllOccurrences('z');

        CHECK(buf.getContent() == "xy");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("deleteAllOccurrences - mixed positions")
    {
        TextBuffer buf;
        buf.insert('a'); // 0
        buf.insert('b'); // 1
        buf.insert('a'); // 2
        buf.insert('a'); // 3
        buf.insert('c'); // 4

        // Cursor at 5
        buf.moveCursorLeft(); // 4
        buf.moveCursorLeft(); // 3

        buf.deleteAllOccurrences('a'); // xóa 0, 2, 3

        CHECK(buf.getContent() == "bc");
        CHECK(buf.getCursorPos() == 0); // 2 ký tự bị xóa trước cursor
    }

    TEST_CASE("deleteAllOccurrences - delete all and reset cursor")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('x');
        buf.insert('x');

        buf.deleteAllOccurrences('x');

        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0); // con trỏ reset
    }

    TEST_CASE("DeleteAllOccurrences - cursorPos remains when no char deleted")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('b');
        buf.insert('c');
        CHECK(buf.getCursorPos() == 3);

        buf.deleteAllOccurrences('x'); // 'x' không có trong buffer

        CHECK(buf.getContent() == "abc");
        CHECK(buf.getCursorPos() == 3); // Không đổi
    }

    TEST_CASE("DeleteAllOccurrences - cursorPos stays when deleting before it")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('a');
        buf.insert('b');
        buf.insert('c');
        buf.moveCursorLeft();
        buf.moveCursorRight();

        buf.deleteAllOccurrences('a');

        CHECK(buf.getContent() == "bc");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("DeleteAllOccurrences - cursorPos unchanged when deleting after it")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('y');
        buf.insert('z');
        buf.moveCursorLeft();

        buf.deleteAllOccurrences('z');

        CHECK(buf.getContent() == "xy");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("DeleteAllOccurrences - cursorPos adjusts on middle deletes")
    {
        TextBuffer buf;
        buf.insert('a');
        buf.insert('b');
        buf.insert('c');
        buf.insert('b');
        buf.insert('d');
        buf.moveCursorLeft();

        buf.deleteAllOccurrences('b');

        CHECK(buf.getContent() == "acd");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("DeleteAllOccurrences - delete all chars, cursor should be 0")
    {
        TextBuffer buf;
        buf.insert('x');
        buf.insert('x');
        buf.insert('x');
        buf.insert('x');
        buf.insert('x');
        for (int i = 0; i < 5; ++i)
            buf.moveCursorLeft();

        buf.deleteAllOccurrences('x');

        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }
}

TEST_SUITE("TextBuffer - sortAscending")
{

    TEST_CASE("Empty buffer remains unchanged after sorting")
    {
        TextBuffer buf;
        buf.sortAscending();
        CHECK(buf.getContent() == "");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Single character buffer remains unchanged after sorting")
    {
        TextBuffer buf;
        buf.insert('b');
        buf.sortAscending();
        CHECK(buf.getContent() == "b");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort with mixed upper and lower case letters")
    {
        TextBuffer buf;
        for (char c : std::string("abcdfDA"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "AabcDdf");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort already sorted buffer")
    {
        TextBuffer buf;
        for (char c : std::string("ABCabc"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "AaBbCc");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort reverse sorted buffer")
    {
        TextBuffer buf;
        for (char c : std::string("zyxZYX"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "XxYyZz");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort buffer with all lowercase letters")
    {
        TextBuffer buf;
        for (char c : std::string("dcba"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "abcd");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort buffer with all uppercase letters")
    {
        TextBuffer buf;
        for (char c : std::string("DCBA"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "ABCD");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort buffer with interleaved cases")
    {
        TextBuffer buf;
        for (char c : std::string("aBcAdC"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "AaBCcd");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort buffer with duplicate characters")
    {
        TextBuffer buf;
        for (char c : std::string("aAaAaA"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "AAAaaa");
        CHECK(buf.getCursorPos() == 0);
    }

    TEST_CASE("Sort buffer with repeated patterns")
    {
        TextBuffer buf;
        for (char c : std::string("zZaAzZ"))
            buf.insert(c);
        buf.sortAscending();
        CHECK(buf.getContent() == "AaZZzz");
        CHECK(buf.getCursorPos() == 0);
    }
}
