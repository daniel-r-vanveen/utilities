/*
 * Copyright 2018 Daniel R. Van Veen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include <mutex>
#include <thread>

#include "Condition.hxx"
#include "Mutex.hxx"
#include "Thread.hxx"
#include "TimedTask.hxx"

namespace utilities {

#define SHORT_TIMEOUT_MS 50

typedef std::lock_guard<std::mutex> Lock;

/* Mutex tests */
TEST(MutexTest, MutexBasic) {
    Mutex mutex;
    ASSERT_EQ(mutex.lock(), 0);
    ASSERT_EQ(mutex.unlock(), 0);
}

TEST(MutexTest, MutexLock) {
    EXPECT_TIMEOUT(std::chrono::milliseconds(SHORT_TIMEOUT_MS), []() {
        Mutex mutex;
        Mutex::Lock l1(mutex);
        Mutex::Lock l2(mutex);
    });
}

TEST(MutexTest, LockBasic) {
    Mutex mutex;
    Mutex::Lock l(mutex);
}

TEST(MutexTest, TryLock) {
    Mutex mutex;
    ASSERT_EQ(mutex.tryLock(), 0);
    ASSERT_EQ(mutex.tryLock(), EBUSY);
}

/* Condition tests */
TEST(ConditionTest, ConditionBasic) {
    std::mutex m;
    Lock l(m);
    Condition condition;
}

TEST(ConditionTest, ConditionWait) {
    EXPECT_TIMEOUT(std::chrono::milliseconds(SHORT_TIMEOUT_MS), []() {
        Mutex m;
        Condition c;
        m.lock();
        ASSERT_EQ(c.wait(m), 0);
    });
}

TEST(ConditionTest, Signal) {
    EXPECT_COMPLETE(std::chrono::milliseconds(SHORT_TIMEOUT_MS), []() {
        Mutex m;
        Condition c;

        const int numThreads = 5;
        int blockedThreadCount = 0;
        bool onThreadWoken = false;

        for (int i = 0; i < numThreads; ++i) {
            std::thread([&]() {
                Mutex::Lock l(m);
                blockedThreadCount++;
                c.wait(m);
                blockedThreadCount--;
                // This will only be modified by one thread at a time so does not need a mutex.
                onThreadWoken = true;
            }).detach();
        }

        for (int i = 1; i < numThreads+1; ++i) {
            {
                Mutex::Lock l(m);
                c.signal();
            }
            while (!onThreadWoken) {
                continue;
            }
            onThreadWoken = false;
            ASSERT_EQ(blockedThreadCount, (numThreads - i));
        }
    });
}

TEST(ConditionTest, Broadcast) {
    EXPECT_COMPLETE(std::chrono::milliseconds(SHORT_TIMEOUT_MS), []() {
        Mutex m;
        Condition c;

        const int numThreads = 5;
        int blockedThreadCount = 0;

        for (int i = 0; i < numThreads; ++i) {
            std::thread([&]() {
                Mutex::Lock l(m);
                blockedThreadCount++;
                c.wait(m);
                blockedThreadCount--;
            }).detach();
        }

        // Wait for all threads to be waiting.
        while (blockedThreadCount != numThreads) continue;
        {   // Scope for lock.
            Mutex::Lock l(m);
            c.broadcast();
        }
        // Expect a timed out if broadcast fails.
        while (blockedThreadCount) continue;
    });
}

// signal, broadcast, wait, timedWait

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}
