
#include "doctest/doctest.h"
#include "TextBuffer.h" // chứa DoublyLinkedList<T>

TEST_SUITE("DoublyLinkedList - All Functions")
{
    std::string pointToString(Point & p)
    {
        std::ostringstream oss;
        oss << "(" << p.getX() << "," << p.getY() << ")";
        return oss.str();
    }

    // ----------- 1. Khởi tạo và size ----------------
    TEST_CASE("Empty list has size 0")
    {
        DoublyLinkedList<int> list;
        CHECK(list.size() == 0);
        CHECK(list.toString() == "[]");
    }

    // ----------- 2-4. insertAtHead và insertAtTail ----------------
    TEST_CASE("insertAtHead + insertAtTail mix")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(2);
        list.insertAtHead(1);
        list.insertAtTail(3);
        CHECK(list.toString() == "[1, 2, 3]");
    }

    TEST_CASE("Insert multiple Points at head")
    {
        DoublyLinkedList<Point> list;
        list.insertAtHead(Point(1, 2));
        list.insertAtHead(Point(3, 4));
        list.insertAtHead(Point(5, 6));
        CHECK(list.toString(pointToString) == "[(5,6), (3,4), (1,2)]");
    }

    TEST_CASE("Insert at tail with one element")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(99);
        CHECK(list.get(0) == 99);
    }

    // ----------- 5-7. insertAt ----------------
    TEST_CASE("Insert at middle")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(3);
        list.insertAt(1, 2); // [1,2,3]
        CHECK(list.toString() == "[1, 2, 3]");
    }

    TEST_CASE("Insert at 0 in empty list")
    {
        DoublyLinkedList<int> list;
        list.insertAt(0, 100);
        CHECK(list.toString() == "[100]");
    }

    TEST_CASE("Insert at invalid index throws")
    {
        DoublyLinkedList<int> list;
        CHECK_THROWS_WITH(list.insertAt(-1, 5), "Index is invalid!");
        CHECK_THROWS_WITH(list.insertAt(1, 5), "Index is invalid!");
    }

    // ----------- 8-10. deleteAt ----------------
    TEST_CASE("Delete middle element")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);
        list.deleteAt(1); // remove 2
        CHECK(list.toString() == "[1, 3]");
    }

    TEST_CASE("Delete first element")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(10);
        list.insertAtTail(20);
        list.deleteAt(0);
        CHECK(list.toString() == "[20]");
    }

    TEST_CASE("Delete at invalid index throws")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        CHECK_THROWS_WITH(list.deleteAt(-1), "Index is invalid!");
        CHECK_THROWS_WITH(list.deleteAt(2), "Index is invalid!");
    }

    // ----------- 11-13. get ----------------
    TEST_CASE("Get value at index")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("a");
        list.insertAtTail("b");
        CHECK(list.get(0) == "a");
        CHECK(list.get(1) == "b");
    }

    TEST_CASE("Get with Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));
        CHECK(list.get(0) == Point(1, 2));
    }

    TEST_CASE("Get at invalid index throws")
    {
        DoublyLinkedList<int> list;
        CHECK_THROWS_WITH(list.get(0), "Index is invalid!");
        list.insertAtTail(1);
        CHECK_THROWS_WITH(list.get(2), "Index is invalid!");
    }

    // ----------- 14-15. indexOf & contains ----------------
    TEST_CASE("IndexOf existing and non-existing")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(10);
        list.insertAtTail(20);
        list.insertAtTail(30);
        CHECK(list.indexOf(20) == 1);
        CHECK(list.indexOf(99) == -1);
    }

    TEST_CASE("Contains test with Points")
    {
        DoublyLinkedList<Point> list;
        Point a(1, 2);
        Point b(3, 4);
        list.insertAtTail(a);
        CHECK(list.contains(a) == true);
        CHECK(list.contains(b) == false);
    }

    // ----------- 16-18. reverse ----------------
    TEST_CASE("Reverse integer list")
    {
        DoublyLinkedList<int> list;
        for (int i = 1; i <= 4; ++i)
            list.insertAtTail(i); // [1,2,3,4]
        list.reverse();
        CHECK(list.toString() == "[4, 3, 2, 1]");
    }

    TEST_CASE("Reverse empty list")
    {
        DoublyLinkedList<int> list;
        list.reverse(); // should not crash
        CHECK(list.toString() == "[]");
    }

    TEST_CASE("Reverse list with one element")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("hello");
        list.reverse();
        CHECK(list.toString() == "[hello]");
    }

    // ----------- 19-20. Destructor & Memory (basic verification) ----------------
    TEST_CASE("Destructor clears memory")
    {
        DoublyLinkedList<int> *list = new DoublyLinkedList<int>();
        list->insertAtTail(1);
        list->insertAtTail(2);
        delete list; // no leak expected
        CHECK(true); // Placeholder to compile
    }

    TEST_CASE("Insert, reverse and delete combined")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);
        list.reverse();   // [3,2,1]
        list.deleteAt(1); // remove 2
        CHECK(list.toString() == "[3, 1]");
        CHECK(list.size() == 2);
    }
}

TEST_SUITE("DoublyLinkedList - Advanced")
{
    TEST_CASE("Insert and delete alternately should maintain integrity")
    {
        DoublyLinkedList<int> list;
        for (int i = 0; i < 100; ++i)
            list.insertAtTail(i);

        for (int i = 0; i < 50; ++i)
            list.deleteAt(0);

        for (int i = 0; i < 50; ++i)
            CHECK_EQ(list.get(i), i + 50);
    }

    TEST_CASE("Reverse twice should return original order")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("one");
        list.insertAtTail("two");
        list.insertAtTail("three");

        list.reverse();
        list.reverse();

        CHECK_EQ(list.toString(), "[one, two, three]");
    }

    TEST_CASE("Get on all valid and invalid indices (stress get)")
    {
        DoublyLinkedList<int> list;
        for (int i = 0; i < 10; ++i)
            list.insertAtTail(i * 10);

        for (int i = 0; i < 10; ++i)
            CHECK_EQ(list.get(i), i * 10);

        CHECK_THROWS_WITH_AS(list.get(-1), "Index is invalid!", std::out_of_range);
        CHECK_THROWS_WITH_AS(list.get(10), "Index is invalid!", std::out_of_range);
    }

    TEST_CASE("IndexOf should detect duplicate correctly")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);
        list.insertAtTail(2);

        CHECK_EQ(list.indexOf(2), 1);
        CHECK_EQ(list.indexOf(3), 2);
        CHECK_EQ(list.indexOf(4), -1);
    }

    TEST_CASE("Contains on reversed complex list")
    {
        DoublyLinkedList<int> list;
        for (int i = 100; i >= 1; --i)
            list.insertAtTail(i);

        list.reverse();

        for (int i = 1; i <= 100; ++i)
            CHECK(list.contains(i));

        CHECK(!list.contains(101));
    }

    TEST_CASE("Reverse empty and single-node list")
    {
        DoublyLinkedList<char> list;
        list.reverse();
        CHECK_EQ(list.toString(), "[]");

        list.insertAtTail('Z');
        list.reverse();
        CHECK_EQ(list.toString(), "[Z]");
    }

    TEST_CASE("Stress delete until empty")
    {
        DoublyLinkedList<int> list;
        for (int i = 0; i < 100; ++i)
            list.insertAtTail(i);

        for (int i = 99; i >= 0; --i)
            list.deleteAt(i);

        CHECK_EQ(list.size(), 0);
        CHECK_EQ(list.toString(), "[]");
    }

    TEST_CASE("toString without convert2str")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("A");
        list.insertAtTail("B");
        list.insertAtTail("C");

        CHECK_EQ(list.toString(), "[A, B, C]");
    }

    TEST_CASE("Insert at head and tail forms expected order")
    {
        DoublyLinkedList<int> list;
        list.insertAtHead(2); // [2]
        list.insertAtTail(3); // [2, 3]
        list.insertAtHead(1); // [1, 2, 3]
        list.insertAtTail(4); // [1, 2, 3, 4]

        CHECK_EQ(list.toString(), "[1, 2, 3, 4]");
    }

    TEST_CASE("Custom toString formatter")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(10);
        list.insertAtTail(20);
        list.insertAtTail(30);

        auto formatter = [](int &val)
        {
            return "Num(" + std::to_string(val) + ")";
        };

        CHECK_EQ(list.toString(formatter), "[Num(10), Num(20), Num(30)]");
    }
}