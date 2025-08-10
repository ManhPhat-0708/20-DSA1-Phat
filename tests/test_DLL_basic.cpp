
#include "doctest/doctest.h"
#include "TextBuffer.h" // chứa DoublyLinkedList<T>

TEST_SUITE("insertAtHead")
{

    TEST_CASE("Insert into empty list")
    {
        DoublyLinkedList<int> list;
        list.insertAtHead(10);

        CHECK(list.size() == 1);
        CHECK(list.get(0) == 10);
    }

    TEST_CASE("Insert multiple elements")
    {
        DoublyLinkedList<int> list;
        list.insertAtHead(30);
        list.insertAtHead(20);
        list.insertAtHead(10);

        CHECK(list.size() == 3);
        CHECK(list.get(0) == 10);
        CHECK(list.get(1) == 20);
        CHECK(list.get(2) == 30);
    }

    TEST_CASE("Insert negative numbers")
    {
        DoublyLinkedList<int> list;
        list.insertAtHead(-1);
        list.insertAtHead(-2);
        list.insertAtHead(-3);

        CHECK(list.get(0) == -3);
        CHECK(list.get(1) == -2);
        CHECK(list.get(2) == -1);
    }

    TEST_CASE("Insert duplicate values")
    {
        DoublyLinkedList<int> list;
        list.insertAtHead(5);
        list.insertAtHead(5);
        list.insertAtHead(5);

        CHECK(list.size() == 3);
        for (int i = 0; i < 3; ++i)
        {
            CHECK(list.get(i) == 5);
        }
    }

    TEST_CASE("Check string output")
    {
        DoublyLinkedList<int> list;
        list.insertAtHead(3);
        list.insertAtHead(2);
        list.insertAtHead(1);

        string output = list.toString();
        CHECK(output == "[1, 2, 3]");
    }

    TEST_CASE("Insert into large list")
    {
        DoublyLinkedList<int> list;
        const int N = 1000;
        for (int i = 0; i < N; ++i)
        {
            list.insertAtHead(i);
        }

        CHECK(list.size() == N);
        CHECK(list.get(0) == N - 1);
        CHECK(list.get(N - 1) == 0);
    }

    TEST_CASE("Insert with Point")
    {
        DoublyLinkedList<Point> list;

        Point p1(1, 2, 3);
        Point p2(4, 5, 6);
        Point p3(7, 8, 9);

        list.insertAtHead(p1);
        CHECK(list.get(0) == Point(1, 2, 3));

        list.insertAtHead(p2);
        CHECK(list.get(0) == Point(4, 5, 6));

        list.insertAtHead(p3);
        CHECK(list.get(0) == Point(7, 8, 9));

        CHECK(list.toString() == "[(7,8,9), (4,5,6), (1,2,3)]");
    }
}

TEST_SUITE("insertAtTail")
{
    TEST_CASE("insert single integer")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(10);
        CHECK(list.size() == 1);
        CHECK(list.get(0) == 10);
    }

    TEST_CASE("insert multiple integers")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);
        CHECK(list.size() == 3);
        CHECK(list.get(0) == 1);
        CHECK(list.get(1) == 2);
        CHECK(list.get(2) == 3);
    }

    TEST_CASE("insert single Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));
        CHECK(list.size() == 1);
        CHECK(list.get(0) == Point(1, 2));
    }

    TEST_CASE("insert multiple Points")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 1));
        list.insertAtTail(Point(2, 2));
        list.insertAtTail(Point(3, 3));
        CHECK(list.size() == 3);
        CHECK(list.get(0) == Point(1, 1));
        CHECK(list.get(1) == Point(2, 2));
        CHECK(list.get(2) == Point(3, 3));
    }

    TEST_CASE("insert after clear")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(42);
        CHECK(list.size() == 1);
    }
}

TEST_SUITE("insertAt")
{
    TEST_CASE("insert at head")
    {
        DoublyLinkedList<int> list;
        list.insertAt(0, 10);
        CHECK(list.size() == 1);
        CHECK(list.get(0) == 10);
    }

    TEST_CASE("insert at tail")
    {
        DoublyLinkedList<int> list;
        list.insertAt(0, 1);
        list.insertAt(1, 2);
        list.insertAt(2, 3);
        list.insertAt(3, 4);
        CHECK(list.get(3) == 4);
    }

    TEST_CASE("insert in middle")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(3);
        list.insertAt(1, 2); // [1, 2, 3]
        CHECK(list.get(1) == 2);
    }

    TEST_CASE("invalid index")
    {
        DoublyLinkedList<int> list;
        CHECK_THROWS_WITH_AS(list.insertAt(-1, 5), "Index is invalid!", std::out_of_range);
        CHECK_THROWS_WITH(list.insertAt(1, 5), "Index is invalid!");
    }

    TEST_CASE("insertHead-Tail-Middle")
    {
        DoublyLinkedList<int> list;

        list.insertAtHead(2); // [2]
        list.insertAtTail(4); // [2, 4]
        list.insertAt(1, 3);  // [2, 3, 4]

        CHECK(list.toString() == "[2, 3, 4]");
    }

    TEST_CASE("insertTail-Head-MiddleMultiple")
    {
        DoublyLinkedList<std::string> list;

        list.insertAtTail("b"); // [b]
        list.insertAtTail("d"); // [b, d]
        list.insertAtHead("a"); // [a, b, d]
        list.insertAt(2, "c");  // [a, b, c, d]
        list.insertAt(4, "e");  // [a, b, c, d, e]

        CHECK(list.toString() == "[a, b, c, d, e]");
    }

    TEST_CASE("insert Point at head, tail and middle")
    {
        DoublyLinkedList<Point> list;

        list.insertAtHead(Point(2, 2)); // [(2,2)]
        list.insertAtTail(Point(4, 4)); // [(2,2), (4,4)]
        list.insertAt(1, Point(3, 3));  // [(2,2), (3,3), (4,4)]
        list.insertAtHead(Point(1, 1)); // [(1,1), (2,2), (3,3), (4,4)]

        CHECK(list.toString() == "[(1,1,0), (2,2,0), (3,3,0), (4,4,0)]");
    }
}

TEST_SUITE("deleteAt")
{
    TEST_CASE("delete from middle")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);
        list.deleteAt(1); // delete 2
        CHECK(list.toString() == "[1, 3]");
    }

    TEST_CASE("delete from head")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(10);
        list.insertAtTail(20);
        list.insertAtTail(30);
        list.deleteAt(0); // delete 10
        CHECK(list.toString() == "[20, 30]");
    }

    TEST_CASE("delete from tail")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(5);
        list.insertAtTail(6);
        list.insertAtTail(7);
        list.deleteAt(2); // delete 7
        CHECK(list.toString() == "[5, 6]");
    }

    TEST_CASE("delete only element")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(42);
        list.deleteAt(0);
        CHECK(list.size() == 0);
    }

    TEST_CASE("delete invalid index - negative")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        CHECK_THROWS_WITH_AS(list.deleteAt(-1), "Index is invalid!", std::out_of_range);
    }

    TEST_CASE("delete invalid index - too large")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        CHECK_THROWS_WITH_AS(list.deleteAt(1), "Index is invalid!", std::out_of_range);
    }

    TEST_CASE("delete multiple times")
    {
        DoublyLinkedList<int> list;
        for (int i = 1; i <= 5; ++i)
        {
            list.insertAtTail(i); // [1, 2, 3, 4, 5]
        }
        list.deleteAt(0); // [2, 3, 4, 5]
        list.deleteAt(1); // [2, 4, 5]
        list.deleteAt(2); // [2, 4]
        CHECK(list.toString() == "[2, 4]");
    }

    TEST_CASE("delete middle Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 1));
        list.insertAtTail(Point(2, 2));
        list.insertAtTail(Point(3, 3));

        list.deleteAt(1); // remove Point(2,2)

        CHECK(list.toString() == "[(1,1,0), (3,3,0)]");
    }

    TEST_CASE("delete head Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(9, 9));
        list.insertAtTail(Point(8, 8));
        list.deleteAt(0);

        CHECK(list.toString() == "[(8,8,0)]");
    }

    TEST_CASE("delete tail Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(5, 5));
        list.insertAtTail(Point(6, 6));
        list.deleteAt(1);

        CHECK(list.toString() == "[(5,5,0)]");
    }

    TEST_CASE("delete only Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(0, 0));
        list.deleteAt(0);

        CHECK(list.toString() == "[]");
    }

    TEST_CASE("delete invalid index with Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));
        CHECK_THROWS_WITH_AS(list.deleteAt(5), "Index is invalid!", std::out_of_range);
        CHECK_THROWS_WITH_AS(list.deleteAt(-1), "Index is invalid!", std::out_of_range);
    }
}

TEST_SUITE("get")
{
    TEST_CASE("get valid index")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("alpha");
        list.insertAtTail("beta");
        list.insertAtTail("gamma");

        CHECK(list.get(0) == "alpha");
        CHECK(list.get(1) == "beta");
        CHECK(list.get(2) == "gamma");

        // Thay đổi giá trị thông qua tham chiếu
        list.get(1) = "BETA";
        CHECK(list.get(1) == "BETA");
    }

    TEST_CASE("get valid index with Point")
    {
        DoublyLinkedList<Point> list;

        list.insertAtTail(Point(1, 2));
        list.insertAtTail(Point(3, 4));

        CHECK(list.get(0) == Point(1, 2));
        CHECK(list.get(1) == Point(3, 4));
    }

    TEST_CASE("get invalid index")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("a");

        CHECK_THROWS_WITH_AS(list.get(-1), "Index is invalid!", std::out_of_range);
        CHECK_THROWS_WITH_AS(list.get(1), "Index is invalid!", std::out_of_range);
    }

    TEST_CASE("get invalid index with Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));

        CHECK_THROWS_WITH_AS(list.get(-1), "Index is invalid!", std::out_of_range);
        CHECK_THROWS_WITH_AS(list.get(2), "Index is invalid!", std::out_of_range);
    }
}

TEST_SUITE("indexOf")
{
    TEST_CASE("indexOf in empty list")
    {
        DoublyLinkedList<std::string> list;
        CHECK(list.indexOf("hello") == -1);
    }

    TEST_CASE("indexOf existing string item")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("apple");
        list.insertAtTail("banana");
        list.insertAtTail("cherry");

        CHECK(list.indexOf("apple") == 0);
        CHECK(list.indexOf("banana") == 1);
        CHECK(list.indexOf("cherry") == 2);
        CHECK(list.indexOf("durian") == -1);
    }

    TEST_CASE("indexOf duplicate values")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(5);
        list.insertAtTail(10);
        list.insertAtTail(5);
        list.insertAtTail(20);

        CHECK(list.indexOf(5) == 0);
        CHECK(list.indexOf(10) == 1);
        CHECK(list.indexOf(20) == 3);
        CHECK(list.indexOf(15) == -1);
    }

    TEST_CASE("indexOf with custom class Point")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));
        list.insertAtTail(Point(3, 4));
        list.insertAtTail(Point(1, 2)); // duplicate
        CHECK(list.indexOf(Point(1, 2)) == 0);
        CHECK(list.indexOf(Point(3, 4)) == 1);
        CHECK(list.indexOf(Point(5, 6)) == -1);
    }
}

TEST_SUITE("contains")
{
    TEST_CASE("contains in empty list")
    {
        DoublyLinkedList<int> list;
        CHECK(list.contains(10) == false);
    }

    TEST_CASE("contains in list with integers")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);

        CHECK(list.contains(1) == true);
        CHECK(list.contains(2) == true);
        CHECK(list.contains(3) == true);
        CHECK(list.contains(4) == false);
    }

    TEST_CASE("contains with duplicate values")
    {
        DoublyLinkedList<char> list;
        list.insertAtTail('a');
        list.insertAtTail('b');
        list.insertAtTail('a');

        CHECK(list.contains('a') == true);
        CHECK(list.contains('b') == true);
        CHECK(list.contains('c') == false);
    }

    TEST_CASE("contains with custom Point class")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));
        list.insertAtTail(Point(3, 4));

        CHECK(list.contains(Point(1, 2)) == true);
        CHECK(list.contains(Point(3, 4)) == true);
        CHECK(list.contains(Point(0, 0)) == false);
    }
}

TEST_SUITE("reverse")
{
    TEST_CASE("reverse empty list")
    {
        DoublyLinkedList<int> list;
        list.reverse();
        CHECK(list.toString() == "[]");
    }

    TEST_CASE("reverse single element")
    {
        DoublyLinkedList<std::string> list;
        list.insertAtTail("A");
        list.reverse();
        CHECK(list.toString() == "[A]");
    }

    TEST_CASE("reverse multiple elements")
    {
        DoublyLinkedList<int> list;
        list.insertAtTail(1);
        list.insertAtTail(2);
        list.insertAtTail(3);
        list.insertAtTail(4);
        list.reverse();
        CHECK(list.toString() == "[4, 3, 2, 1]");
    }

    TEST_CASE("reverse with duplicate values")
    {
        DoublyLinkedList<char> list;
        list.insertAtTail('x');
        list.insertAtTail('y');
        list.insertAtTail('x');
        list.reverse();
        CHECK(list.toString() == "[x, y, x]");
    }

    TEST_CASE("reverse with Point elements")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 1));
        list.insertAtTail(Point(2, 2));
        list.insertAtTail(Point(3, 3));
        list.reverse();
        CHECK(list.toString() == "[(3,3,0), (2,2,0), (1,1,0)]");
    }
}

TEST_SUITE("size and toString")
{
    std::string pointToString(Point & p)
    {
        std::ostringstream oss;
        oss << "(" << p.getX() << "," << p.getY() << ")";
        return oss.str();
    }

    TEST_CASE("size should return correct number of elements")
    {
        DoublyLinkedList<Point> list;
        CHECK(list.size() == 0);

        list.insertAtTail(Point(1, 2));
        list.insertAtTail(Point(3, 4));
        list.insertAtTail(Point(5, 6));

        CHECK(list.size() == 3);

        list.deleteAt(1); // remove (3,4)
        CHECK(list.size() == 2);
    }

    TEST_CASE("toString should return formatted string with custom convert2str function")
    {
        DoublyLinkedList<Point> list;
        list.insertAtTail(Point(1, 2));
        list.insertAtTail(Point(3, 4));
        list.insertAtTail(Point(5, 6));

        std::string expected = "[(1,2), (3,4), (5,6)]";
        CHECK(list.toString(pointToString) == expected);
    }

    TEST_CASE("toString with empty list should return empty brackets")
    {
        DoublyLinkedList<Point> list;
        CHECK(list.toString(pointToString) == "[]");
    }
}
//
