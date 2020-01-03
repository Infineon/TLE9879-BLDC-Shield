#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/library-ino.h"
#include "../src/utils/utility.h"

using ::testing::Return;
//using ::testing::Matcher;
// using ::testing::NotNull;
// using ::testing::Sequence;
using ::testing::_;

/**
 * Class Arduino API Test Suite
 */
class ArduinoAPI: public ::testing::Test
{
    public:
    	
        LibraryClass object;

        void setUp()
        {

        }

        void TearDown()
        {

        }
};

/**
 * Mock Utility Class
 */
class MockUtility: public Utility
{
    public:
        MOCK_METHOD(char, someUtilFunction, (char a),(override));
};

TEST_F(ArduinoAPI, checkBegin)
{
    ASSERT_EQ(object.begin(),0);
}

TEST_F(ArduinoAPI, checkEnd)
{
    ASSERT_EQ(object.end(),0);
}

TEST_F(ArduinoAPI, checkSomeFunctionError)
{
    MockUtility mocku;

    EXPECT_CALL(mocku, someUtilFunction(_))
        .WillOnce(Return((char)'e'));

    ASSERT_EQ(object.someFunction(mocku),1);
}

TEST_F(ArduinoAPI, checkSomeFunctionOK)
{
    MockUtility mocku;

    EXPECT_CALL(mocku, someUtilFunction(_))
        .WillOnce(Return((char)'a'));

    ASSERT_EQ(object.someFunction(mocku),0);
}