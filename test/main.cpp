#include <gtest/gtest.h>

#include "test_utils.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    LI_test::RunPerf();

    return testResult;
}
