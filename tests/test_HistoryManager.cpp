#include "doctest/doctest.h"
#include "src/TextBuffer.h"
#include "helpers/TestHelper.h"
#include <sstream>

TEST_SUITE("TextBuffer::HistoryManager")
{
    using HM = TextBuffer::HistoryManager;

    TEST_CASE("Undo/Redo availability by changing currentIndex")
    {
        HM hm;
        hm.addAction({"insert", 0, 1, 'A'});
        hm.addAction({"move", 1, 2, 'R'});

        CHECK(hm.canUndo() == true);
        CHECK(hm.canRedo() == false);

        TestHelper::setCurrentIndex(hm, 1);
        CHECK(hm.canUndo() == true);
        CHECK(hm.canRedo() == true);

        TestHelper::setCurrentIndex(hm, 0);
        CHECK(hm.canUndo() == false);
        CHECK(hm.canRedo() == true);
    }

    TEST_CASE("Add action clears redo history")
    {
        HM hm;
        hm.addAction({"insert", 0, 1, 'A'});
        hm.addAction({"insert", 1, 2, 'B'});
        hm.addAction({"insert", 2, 3, 'C'});

        TestHelper::setCurrentIndex(hm, 2);
        CHECK(hm.canRedo() == true);

        hm.addAction({"insert", 2, 3, 'X'});
        CHECK(hm.canRedo() == false);
        std::ostringstream oss;
        auto *oldBuf = std::cout.rdbuf(oss.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(oldBuf);

        CHECK(oss.str() == "[(insert, 0, 1, A), (insert, 1, 2, B), (insert, 2, 3, X)]");
    }

    TEST_CASE("Move redo history")
    {
        HM hm;
        hm.addAction({"insert", 0, 1, 'A'});
        hm.addAction({"insert", 1, 2, 'B'});
        hm.addAction({"insert", 2, 3, 'C'});

        TestHelper::setCurrentIndex(hm, 2);
        CHECK(hm.canRedo() == true);

        hm.addAction({"move", 1, 2, 'R'});
        CHECK(hm.canRedo() == true);
        std::ostringstream oss;
        auto *oldBuf = std::cout.rdbuf(oss.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(oldBuf);

        CHECK(oss.str() == "[(insert, 0, 1, A), (insert, 1, 2, B), (move, 1, 2, R)]");
    }

    TEST_CASE("Print history format")
    {
        HM hm;
        hm.addAction({"insert", 0, 1, 'A'});
        hm.addAction({"move", 1, 2, 'R'});
        hm.addAction({"remove", 2, 1, 'B'});

        std::ostringstream oss;
        auto *oldBuf = std::cout.rdbuf(oss.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(oldBuf);

        CHECK(oss.str() == "[(insert, 0, 1, A), (move, 1, 2, R), (remove, 2, 1, B)]");
    }

    TEST_CASE("Empty history behavior")
    {
        HM hm;
        CHECK(hm.canUndo() == false);
        CHECK(hm.canRedo() == false);

        std::ostringstream oss;
        auto *oldBuf = std::cout.rdbuf(oss.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(oldBuf);

        CHECK(oss.str() == "[]");
    }

    TEST_CASE("Undo/Redo edge movement")
    {
        HM hm;
        hm.addAction({"insert", 0, 1, 'A'});
        hm.addAction({"insert", 1, 2, 'B'});

        // Move to end
        TestHelper::setCurrentIndex(hm, 2);
        CHECK(hm.canRedo() == false);
        CHECK(hm.canUndo() == true);

        // Move to start
        TestHelper::setCurrentIndex(hm, 0);
        CHECK(hm.canRedo() == true);
        CHECK(hm.canUndo() == false);
    }

    TEST_CASE("Adding after undo clears redo actions")
    {
        HM hm;
        hm.addAction({"insert", 0, 1, 'A'});
        hm.addAction({"insert", 1, 2, 'B'});
        hm.addAction({"insert", 2, 3, 'C'});

        // Undo 2 steps
        TestHelper::setCurrentIndex(hm, 1);
        CHECK(hm.canRedo() == true);

        // Add new action should clear redo
        hm.addAction({"remove", 1, 0, 'B'});
        CHECK(hm.canRedo() == false);

        std::ostringstream oss;
        auto *oldBuf = std::cout.rdbuf(oss.rdbuf());
        hm.printHistory();
        std::cout.rdbuf(oldBuf);

        CHECK(oss.str() == "[(insert, 0, 1, A), (remove, 1, 0, B)]");
    }
}
