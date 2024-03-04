cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(qtwidgetsinspector)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(DOXYGEN_EXECUTABLE "C:/Program Files/doxygen/bin/doxygen.exe")
find_package(Doxygen REQUIRED)

# set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase")

## @brief Enable target sanatization
## @param TARGET_NAME The target's name
function   ( sanatize_target     TARGET_NAME)
    target_compile_definitions(${TARGET_NAME}
        PRIVATE
            /Zi /fsanitize=address) #/fno-sanitize-address-vcasan-lib

    target_compile_options(${TARGET_NAME}
        PRIVATE
            /Zi /fsanitize=address) #/fno-sanitize-address-vcasan-lib

    target_link_options(qtwidgetsinspector
        PRIVATE
            "/INCREMENTAL:NO")

endfunction()

## @brief Example macro documentation
## @param X x
## @param Y y
## @param Z z
macro(example_macro X Y Z)
endmacro()

## @brief Example function documentation
## @param A a
## @param B b
## @param C c
function(example_function A B C)
endfunction()

add_executable(qtwidgetsinspector
    WIN32
        MainWindow.h
        MainWindow.cpp
        MainWindow.ui        
        main.cpp)

target_link_libraries(qtwidgetsinspector
    PRIVATE
        Qt5::Widgets)

set_target_properties(qtwidgetsinspector
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON)

sanatize_target(qtwidgetsinspector)

message(STATUS "${CMAKE_CXX_FLAGS}")

# doxygen 
set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
set(DOXYGEN_WARN_AS_ERROR FAIL_ON_WARNINGS_PRINT)
# VS Log Message Format: https://learn.microsoft.com/en-us/cpp/build/formatting-the-output-of-a-custom-build-step-or-build-event?view=msvc-170&redirectedfrom=MSDN
set(DOXYGEN_WARN_FORMAT [[$file ($line,0): $text]]) 
set(DOXYGEN_WARN_NO_PARAMDOC YES) 
set(DOXYGEN_WARN_IF_UNDOC_ENUM_VAL YES) 
set(DOXYGEN_ALWAYS_DETAILED_SEC YES) # create links to e.g. functions
set(DOXYGEN_GENERATE_TODOLIST YES)

set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXTRACT_PACKAGE YES)
set(DOXYGEN_EXTRACT_STATIC YES)
set(DOXYGEN_EXTRACT_ANON_NSPACES YES)
set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)

set(DOXYGEN_FULL_PATH_NAMES NO)

set(DOXYGEN_FILE_PATTERNS 
    [[*.h]]    
    [[*.txt]]
)

set(DOXYGEN_ALIASES
    [["getter{1}=@brief Getter for @ref \1^^@returns Returns the value of property @ref \1"]]
    [["setter=@brief Setter for @ref "]]
    [["signal=@brief Signal for @ref "]]
    [["ownercaller=@warning Return value owner is caller!"]]
    [["ownercallee=@warning Return value owner is callee!"]]
)

set(DOXYGEN_EXTENSION_MAPPING
    [[txt=Python]]
)

set(DOXYGEN_EXCLUDE ${CMAKE_CURRENT_LIST_FILE})

get_filename_component(FILENAME ${CMAKE_CURRENT_LIST_FILE} NAME)
set(PYTHON_FILENAME ${CMAKE_CURRENT_BINARY_DIR}/doxygen/generated/${FILENAME})

doxygen_add_docs(
    doxygen
    ${CMAKE_CURRENT_LIST_DIR}    
    ${CMAKE_CURRENT_LIST_DIR}/cmake
    ${CMAKE_CURRENT_BINARY_DIR}/doxygen/generated
    ${PYTHON_FILENAME} # TODO why list this file explicitly
    COMMENT "Generate doxygen"
)

option(EXAMPLE_OPTION1 "Description of this option")
option(EXAMPLE_OPTION2 "Description of this option" OFF)

## @cond DISABLE_DOXYGEN
function(doxygen_generate_cmake FILENAME OUTPUT_FILENAME)
    file(STRINGS ${FILENAME} LINES REGEX [[^ *##.*|^ *(function|macro|option) *\(]])
    file(WRITE  ${OUTPUT_FILENAME} "# THIS FILE IS GENERATED! \n\n")
    file(APPEND ${OUTPUT_FILENAME} "## @file\n\n")

    set(CMAKE_DECLARATION_TYPE)
    foreach(LINE IN LISTS LINES)         
        string(STRIP ${LINE} LINE)        
        string(STRIP ${LINE} LINESTRIPPED)

        if(LINE MATCHES [[^##.*]])
            set(CMAKE_DECLARATION_TYPE "comment")
            set(CMAKE_DECLARATION_GROUP "")
            file(APPEND ${OUTPUT_FILENAME} "${LINESTRIPPED}\n")
            continue()
        elseif(LINE MATCHES [[^function.*]])
            set(CMAKE_DECLARATION_TYPE "function")
            set(CMAKE_DECLARATION_GROUP "cmake_functions")
        elseif(LINE MATCHES [[^macro.*]])
            set(CMAKE_DECLARATION_TYPE "macro")
            set(CMAKE_DECLARATION_GROUP "cmake_macros")
        elseif(LINE MATCHES [[^option.*]])
            set(CMAKE_DECLARATION_TYPE "option")
            set(CMAKE_DECLARATION_GROUP "cmake_options")
        else()
            continue()
        endif()
              
        string(REGEX REPLACE [[.*\(]] "" LINE ${LINE})
        string(REGEX REPLACE [[\).*]] "" LINE ${LINE})
        string(STRIP ${LINE} LINE)        

        if(CMAKE_DECLARATION_TYPE STREQUAL "option")
            string(REGEX REPLACE "([a-zA-Z0-9_]+) +\"(.*)\" *([a-zA-Z0-9_]*)" "\\2" PYTHON_DOXYGEN ${LINE})
            string(REGEX REPLACE "([a-zA-Z0-9_]+) +\"(.*)\" *([a-zA-Z0-9_]*)" "\\1 : bool = \\3" PYTHON_VARIABLE ${LINE})
            file(APPEND ${OUTPUT_FILENAME} "## @brief ${PYTHON_DOXYGEN}\n")
            file(APPEND ${OUTPUT_FILENAME} "##\n")
            file(APPEND ${OUTPUT_FILENAME} "## @ingroup ${CMAKE_DECLARATION_GROUP}\n")
            file(APPEND ${OUTPUT_FILENAME} "## @ref ${CMAKE_DECLARATION_GROUP}\n")            
            file(APPEND ${OUTPUT_FILENAME} "${PYTHON_VARIABLE}\n\n")
        else()
            string(REGEX REPLACE " +"  [[;]] LINE ${LINE})
            list(POP_FRONT LINE PYTHON_FUNCTION_NAME)
            string(JOIN ", " PYTHON_FUNCTION_ARGS ${LINE})
            file(APPEND ${OUTPUT_FILENAME} "##\n")
            file(APPEND ${OUTPUT_FILENAME} "## @ingroup ${CMAKE_DECLARATION_GROUP}\n")
            file(APPEND ${OUTPUT_FILENAME} "## @ref ${CMAKE_DECLARATION_GROUP}\n")
            file(APPEND ${OUTPUT_FILENAME} "def ${PYTHON_FUNCTION_NAME}(${PYTHON_FUNCTION_ARGS}):\n\tpass\n\n")
        endif()        
    endforeach()
endfunction()
## @endcond

doxygen_generate_cmake(${CMAKE_CURRENT_LIST_FILE} ${PYTHON_FILENAME})