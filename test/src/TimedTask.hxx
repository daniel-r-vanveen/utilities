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

#include <functional>
#include <future>
#include <thread>

enum class TimedTaskType {
    ExpectComplete,
    ExpectTimeout
};

template<class Rep, class Period>
static void EXPECT_COMPLETE(const std::chrono::duration<Rep, Period>& dur,
                            std::function<void()> f) {
    EXPECT_TIMED_TASK(TimedTaskType::ExpectComplete, dur, f);
}

template<class Rep, class Period>
static void EXPECT_TIMEOUT(const std::chrono::duration<Rep, Period>& dur,
                            std::function<void()> f) {
    EXPECT_TIMED_TASK(TimedTaskType::ExpectTimeout, dur, f);
}

template <class Rep, class Period>
void EXPECT_TIMED_TASK(TimedTaskType type, const std::chrono::duration<Rep, Period>& dur, std::function<void()> f) {
    std::promise<bool> funcFinished;
    auto futureResult = funcFinished.get_future();

    std::thread([](std::promise<bool>& finished, std::function<void()> func) {
        func();
        finished.set_value(true);
    }, std::ref(funcFinished), f).detach();

    switch (type) {
        case TimedTaskType::ExpectComplete:
            if (futureResult.wait_for(dur) == std::future_status::timeout) {
                FAIL() << "Test failed due to time out";
            }
            break;
        case TimedTaskType::ExpectTimeout:
            if (futureResult.wait_for(dur) != std::future_status::timeout) {
                FAIL() << "Test failed as timeout was expected";
            }
            break;
    }
}
