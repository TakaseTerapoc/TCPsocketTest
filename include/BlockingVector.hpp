#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <optional>

using namespace std;

template<typename T>
class BlockingVector {
public:
    // デフォルトコンストラクタ
    BlockingVector() = default;

    // データを末尾に追加
    void Push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            vector_.push_back(item);
        }
        cond_.notify_one(); // 待機中のスレッドに通知
    }

    // データを末尾に追加（ムーブ対応）
    void Push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            vector_.push_back(std::move(item));
        }
        cond_.notify_one();
    }

    // n番目のデータを取り出す＆取り出した後は削除する（空ならfalse）
    bool Try_pick(size_t n) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (vector_.empty()) return false;

        // nが範囲外ならfalse
        if (n >= vector_.size()) return false; 
        vector_.erase(vector_.begin() + n);
        return true;
    }

    // 先頭データを取り出す。削除はしない。
    bool try_pop(T& result) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (vector_.empty()) return false;

        result = std::move(vector_.front());
        return true;
    }

    // vectorーをクリア
    void Clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.clear();
    }

    // vectorのサイズ
    size_t Size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.size();
    }

    // 空かどうか
    bool Empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.empty();
    }

    vector<T> getVector()  {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_;
    }

private:
    vector<T> vector_;
    mutable mutex mutex_;
    condition_variable cond_;
};