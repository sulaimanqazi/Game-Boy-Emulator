# CMake generated Testfile for 
# Source directory: /home/sulaiman/Documents/gb emulator
# Build directory: /home/sulaiman/Documents/gb emulator/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(check_gbe "/home/sulaiman/Documents/gb emulator/build/tests/check_gbe")
set_tests_properties(check_gbe PROPERTIES  _BACKTRACE_TRIPLES "/home/sulaiman/Documents/gb emulator/CMakeLists.txt;96;add_test;/home/sulaiman/Documents/gb emulator/CMakeLists.txt;0;")
subdirs("lib")
subdirs("gbemu")
subdirs("tests")
