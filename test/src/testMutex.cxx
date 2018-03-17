#include <gtest/gtest.h>

#include "Mutex.hxx"

TEST(MutexTest, Basic) { 
    ASSERT_EQ(0, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}