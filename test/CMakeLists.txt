
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(THIS ${PROJECT_NAME}-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
    ../arpc/source/message-handler.cpp
    ../arpc/source/message.cpp
    source/client-socket-connection.cpp
    source/server-socket-connection.cpp
    source/message-handler-test.cpp
    source/connection-test.cpp)

include_directories(../arpc/include)
include_directories(include)

add_executable(${THIS} ${SOURCE_FILES})

target_link_libraries(${THIS} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

add_test(NAME ${THIS} COMMAND ${THIS})

