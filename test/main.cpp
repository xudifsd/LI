#include <gtest/gtest.h>

#include "test_utils.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    if (testResult == 0)
    {
        LI_test::RunPerf();
    }

    return testResult;
}
