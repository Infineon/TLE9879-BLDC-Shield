#include <gtest/gtest.h>
extern "C"
{
#include "../src/library/library.h"
#include "fakes/fff.h"
}

Library_Handle_t handle = {.pal = NULL};

/**
 * Class C Library API Test Suite
 */
class CLibraryAPI: public ::testing::Test
{
    public:

        void SetUp()
        {
            Library_Config(&handle);
        }

        void TearDown()
        {

        }
};

/**
 * FFF fakes
 */
DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(Library_PAL_XX_FunctionA);

TEST_F(CLibraryAPI, checkInit)
{
    ASSERT_EQ(Library_Init(&handle),LIBRARY_OK);
}

TEST_F(CLibraryAPI, checkDeinit)
{
    ASSERT_EQ(Library_Deinit(&handle),LIBRARY_OK);
}

TEST_F(CLibraryAPI, checkSomeFunction)
{
    ASSERT_EQ(Library_SomeFunction(&handle),LIBRARY_OK);
    ASSERT_EQ(Library_PAL_XX_FunctionA_fake.call_count, 1);
}