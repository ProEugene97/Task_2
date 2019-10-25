#include "gtest/gtest.h"

extern "C" {
#include "../static_lib/include/count.h"
#include "../dynamic_lib/include/parallel_count.h"
#include "generate.h"
}

class Tests : public ::testing::Test
{
protected:
    void SetUp()
    {
        a = (char *)calloc(SIZE, sizeof(char));
    }
    void TearDown()
    {
        free(a);
    }
    char *a;
};

TEST_F(Tests, test1)
{
    generate_array(a, SIZE);
    ASSERT_EQ(count(a, SIZE), parallel_count(a, SIZE));
}

TEST_F(Tests, test2)
{
    generate_array(a, SIZE);
    ASSERT_EQ(count(a, SIZE), parallel_count(a, SIZE));
}

TEST_F(Tests, test3)
{
    generate_array(a, SIZE);
    ASSERT_EQ(count(a, SIZE), parallel_count(a, SIZE));
}

TEST_F(Tests, test4)
{
    for (int i = 0; i < SIZE; ++i) {
        a[i] = 'a';
    }
    ASSERT_EQ(count(a, SIZE), 1);
}

TEST_F(Tests, test5)
{
    for (int i = 0; i < SIZE; ++i) {
        a[i] = 'a';
    }
    ASSERT_EQ(parallel_count(a, SIZE), 1);
}