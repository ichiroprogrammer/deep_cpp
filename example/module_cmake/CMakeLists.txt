cmake_minimum_required(VERSION 3.22)
project(Cpp20ModulesExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_COMPILER clang++)

# モジュールサポートを有効化するフラグ
add_compile_options(-fmodules-ts)

# モジュールファイルとメインファイルの定義
set(MODULE_FILES math.cppm type_traits.cppm)
set(SOURCE_FILES main.cpp)

# モジュールごとにプリコンパイルとオブジェクトファイル生成
foreach(MODULE_FILE IN LISTS MODULE_FILES)
    get_filename_component(MODULE_NAME ${MODULE_FILE} NAME_WE)

    # モジュールをプリコンパイル（.pcmファイルを生成）
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${MODULE_NAME}.pcm
        COMMAND ${CMAKE_CXX_COMPILER} -std=c++20 -fmodules-ts --precompile ${CMAKE_SOURCE_DIR}/${MODULE_FILE} -o ${CMAKE_BINARY_DIR}/${MODULE_NAME}.pcm
        DEPENDS ${CMAKE_SOURCE_DIR}/${MODULE_FILE}
    )

    # プリコンパイルされたモジュールからオブジェクトファイルを生成
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${MODULE_NAME}.o
        COMMAND ${CMAKE_CXX_COMPILER} -std=c++20 -fmodules-ts -c ${CMAKE_BINARY_DIR}/${MODULE_NAME}.pcm -o ${CMAKE_BINARY_DIR}/${MODULE_NAME}.o
        DEPENDS ${CMAKE_BINARY_DIR}/${MODULE_NAME}.pcm
    )

    # モジュールをリンク対象として扱う
    add_library(${MODULE_NAME}_module OBJECT ${CMAKE_BINARY_DIR}/${MODULE_NAME}.o)
endforeach()

# メインファイルのコンパイル
add_executable(main ${SOURCE_FILES})

# メインターゲットにモジュールのオブジェクトファイルをリンク
foreach(MODULE_FILE IN LISTS MODULE_FILES)
    get_filename_component(MODULE_NAME ${MODULE_FILE} NAME_WE)
    
    # 各モジュールのプリコンパイル済みファイルを指定
    target_compile_options(main PRIVATE -fmodule-file=${CMAKE_BINARY_DIR}/${MODULE_NAME}.pcm)

    # 各モジュールのオブジェクトファイルをリンク
    target_link_libraries(main PRIVATE ${CMAKE_BINARY_DIR}/${MODULE_NAME}.o)
endforeach()

