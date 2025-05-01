cmake_minimum_required(VERSION 3.10)
project(NAKANISHI_IOT_SYSTEM_PROTTYPE)

# C++ 設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# インクルードディレクトリ
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/simpleini
    ${PROJECT_SOURCE_DIR}/external/csv-parser
)

# ソースファイルをまとめて取得
file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 実行ファイルのターゲットを定義
add_executable(${PROJECT_NAME} ${SRC_FILES})

# コンパイラ警告オプション
target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_libraries   (${PROJECT_NAME} PRIVATE -fsanitize=address)

# ビルド方法（プロジェクトルートで実行）
# mkdir build && cd build
# cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
# ninja