#include "gtest.h"
#include "Unordered_tab.h"
#include <string>

TEST(UnorderedTabTest, CanCreateEmptyTable) {
    Unordered_tab<int, std::string> table;
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

TEST(UnorderedTabTest, CanInsertAndFindElement) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});
    EXPECT_FALSE(table.empty());
    EXPECT_EQ(table.size(), 1);

    auto it = table.find(1);
    EXPECT_NE(it, table.end());
    EXPECT_EQ(it->second, "one");
}

TEST(UnorderedTabTest, CannotFindNonExistentElement) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});

    auto it = table.find(2);
    EXPECT_EQ(it, table.end());
}

TEST(UnorderedTabTest, InsertDoesNotDuplicateKey) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});
    table.insert({1, "odin"});

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.find(1)->second, "one");
}

TEST(UnorderedTabTest, CanEraseElement) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});
    table.insert({2, "two"});

    EXPECT_EQ(table.size(), 2);

    size_t erased_count = table.erase(1);
    EXPECT_EQ(erased_count, 1);
    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.find(1), table.end());
    EXPECT_NE(table.find(2), table.end());
}

TEST(UnorderedTabTest, EraseNonExistentElementReturnsZero) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});

    size_t erased_count = table.erase(3);
    EXPECT_EQ(erased_count, 0);
    EXPECT_EQ(table.size(), 1);
}

TEST(UnorderedTabTest, OperatorBracketsInsertsNewKey) {
    Unordered_tab<int, std::string> table;
    table[1] = "one";

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.find(1)->second, "one");
}

TEST(UnorderedTabTest, OperatorBracketsUpdatesExistingKey) {
    Unordered_tab<int, std::string> table;
    table[1] = "one";
    table[1] = "odin";

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.find(1)->second, "odin");
}

TEST(UnorderedTabTest, AtMethodReturnsValue) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});

    EXPECT_EQ(table.at(1), "one");
}

TEST(UnorderedTabTest, AtMethodThrowsExceptionForMissingKey) {
    Unordered_tab<int, std::string> table;
    EXPECT_THROW(table.at(1), std::out_of_range);
}

TEST(UnorderedTabTest, CanClearTable) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});
    table.insert({2, "two"});

    table.clear();
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

TEST(UnorderedTabTest, CanIterateOverElements) {
    Unordered_tab<int, std::string> table;
    table.insert({1, "one"});
    table.insert({2, "two"});
    table.insert({3, "three"});

    int count = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(UnorderedTabTest, CanInsertMultipleAndFind) {
    Unordered_tab<int, std::string> table;
    std::vector<int> keys = {10, 20, 30, 40, 50};
    for(int k : keys) {
        table.insert({k, "val" + std::to_string(k)});
    }

    for(int k : keys) {
        auto it = table.find(k);
        EXPECT_NE(it, table.end());
        EXPECT_EQ(it->second, "val" + std::to_string(k));
    }
}
