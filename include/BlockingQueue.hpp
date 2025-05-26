#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>
#include <optional>

//型のテンプレート宣言
template<typename T>
class BlockingQueue {
public:
    // データを末尾に追加
    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push_back(item);
        }
        cond_.notify_one(); // 待機中のスレッドに通知
    }

    // データを末尾に追加（ムーブ対応）
    void push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push_back(std::move(item));
        }
        cond_.notify_one();
    }

    // データを取り出す（空ならブロックして待機）
    T wait_and_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty(); });

        T item = std::move(queue_.front());
        queue_.pop_front();
        return item;
    }

    // データを取り出す（空ならfalse）
    bool try_pop(T& result) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;

        result = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    // キューをクリア
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.clear();
    }

    // キューのサイズ
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    // 空かどうか
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    std::deque<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};
