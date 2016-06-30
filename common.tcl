#! /usr/bin/env tclsh8.5

set BUILD_DIR _build

set PROJECT_NAME Project2_Main

set TEST_SUITE_NAME TestSuite

set BUILD_SCRIPT_FILE ./build.tcl

set TEST_SCRIPT_FILE ./test.tcl

set TEST_EXE $BUILD_DIR/test/mainTests/$TEST_SUITE_NAME

set MAIN_EXE $BUILD_DIR/$PROJECT_NAME