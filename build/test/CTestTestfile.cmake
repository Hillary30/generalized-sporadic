# CMake generated Testfile for 
# Source directory: C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test
# Build directory: C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/build/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[CMakeTest]=] "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/build/test/Debug/CMakeTest.exe")
  set_tests_properties([=[CMakeTest]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;17;add_test;C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[CMakeTest]=] "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/build/test/Release/CMakeTest.exe")
  set_tests_properties([=[CMakeTest]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;17;add_test;C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[CMakeTest]=] "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/build/test/MinSizeRel/CMakeTest.exe")
  set_tests_properties([=[CMakeTest]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;17;add_test;C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[CMakeTest]=] "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/build/test/RelWithDebInfo/CMakeTest.exe")
  set_tests_properties([=[CMakeTest]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;17;add_test;C:/Users/hilla/OneDrive/Documents/GitHub/generalized-sporadic/test/CMakeLists.txt;0;")
else()
  add_test([=[CMakeTest]=] NOT_AVAILABLE)
endif()
