#### tests/editor/CMakeLists.txt ###############################################
#
#  Author:      Claude AI
#  Date:        02/12/2025
#  Description: CMakeLists.txt file for the editor tests.
#
###############################################################################

set(BASEDIR ${CMAKE_CURRENT_LIST_DIR})

# Editor test files
set(EDITOR_TEST_FILES
    ${BASEDIR}/context/ActionHistory.test.cpp
    ${BASEDIR}/context/ActionGroup.test.cpp
    ${BASEDIR}/context/ActionManager.test.cpp
    ${BASEDIR}/context/StateAction.test.cpp
    ${BASEDIR}/context/DockingRegistry.test.cpp
    ${BASEDIR}/context/WindowRegistry.test.cpp
    ${BASEDIR}/context/Selector.test.cpp
    ${BASEDIR}/inputs/Command.test.cpp
    ${BASEDIR}/inputs/InputManager.test.cpp
    ${BASEDIR}/inputs/WindowState.test.cpp
    ${BASEDIR}/utils/String.test.cpp
    ${BASEDIR}/utils/TransparentStringHash.test.cpp
    ${BASEDIR}/exceptions/Exceptions.test.cpp
)

# Editor source files to compile (avoids linking full editor with OpenGL dependencies)
set(EDITOR_TESTABLE_SOURCES
    ${PROJECT_SOURCE_DIR}/editor/src/context/ActionHistory.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/context/ActionGroup.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/context/ActionManager.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/DockingRegistry.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/WindowRegistry.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/context/Selector.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/inputs/Command.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/inputs/InputManager.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/inputs/WindowState.cpp
    ${PROJECT_SOURCE_DIR}/editor/src/utils/String.cpp
    ${PROJECT_SOURCE_DIR}/common/Exception.cpp
)

# Find imgui for Command tests
find_package(imgui CONFIG REQUIRED)

add_executable(editor_tests
    ${TEST_MAIN_FILES}
    ${EDITOR_TEST_FILES}
    ${EDITOR_TESTABLE_SOURCES}
)

# Define NEXO_TESTING to disable ECS-coupled code in Selector
target_compile_definitions(editor_tests PRIVATE NEXO_TESTING)

target_link_libraries(editor_tests
    PRIVATE
    GTest::gtest
    GTest::gmock
    imgui::imgui
)

target_include_directories(editor_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/editor/src
    ${PROJECT_SOURCE_DIR}/engine/src
    ${PROJECT_SOURCE_DIR}/common/src
    ${BASEDIR}/mocks
    ${BASEDIR}/contexts
)
