CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(gchd)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-reorder -Wno-missing-field-initializers")

ADD_SUBDIRECTORY(src)
