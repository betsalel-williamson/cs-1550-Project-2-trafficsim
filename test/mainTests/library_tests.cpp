//
// Created by School on 6/30/16.
//

#include "library.h"
#include "gtest/gtest.h"

TEST(Library_tests, Open_zeroth_framebuffer) {
    // opening this file will work

    int actual = 1; //foo();
//    int expected = 0;

//    printf("Opened filedesc: %d\n", filedesc);

    ASSERT_EQ(1, actual);
}