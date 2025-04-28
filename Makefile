# コンパイラ
CXX = g++

# 基本コンパイルオプション
INCLUDE_DIRS = -I./include -I./external/spdlog/include -I./external/simpleini
SPDLOG_DEFINE = -DSPDLOG_HEADER_ONLY
WARNING_FLAGS = -Wall -Wextra -Werror

# 出力する実行ファイル名
TARGET = main

# ソースファイル一覧（src以下のcppを全部）
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# ====== デフォルトはリリースビルド ======
CXXFLAGS = -std=c++17 $(INCLUDE_DIRS) $(SPDLOG_DEFINE) $(WARNING_FLAGS) -O2

# ビルドターゲット
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# クリーンターゲット
clean:
	rm -f $(TARGET) $(OBJS)

# ====== デバッグビルドモード ======
debug: CXXFLAGS = -std=c++17 $(INCLUDE_DIRS) $(SPDLOG_DEFINE) $(WARNING_FLAGS) -g
debug: clean all
