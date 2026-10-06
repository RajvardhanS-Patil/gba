# CMake generated Testfile for 
# Source directory: /home/raj/Documents/oops_game
# Build directory: /home/raj/Documents/oops_game/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test("DSATests" "/home/raj/Documents/oops_game/build/test_dsa")
set_tests_properties("DSATests" PROPERTIES  _BACKTRACE_TRIPLES "/home/raj/Documents/oops_game/CMakeLists.txt;166;add_test;/home/raj/Documents/oops_game/CMakeLists.txt;0;")
add_test("SaveTests" "/home/raj/Documents/oops_game/build/test_save")
set_tests_properties("SaveTests" PROPERTIES  _BACKTRACE_TRIPLES "/home/raj/Documents/oops_game/CMakeLists.txt;181;add_test;/home/raj/Documents/oops_game/CMakeLists.txt;0;")
subdirs("_deps/raylib-build")
subdirs("_deps/nlohmann_json-build")
