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

#ifndef THREAD_H
#define THREAD_H

// Support for UNIX platforms only.
#include <pthread.h>
#include <string>

#include "Mutex.hxx"

namespace utilities {

/*
 * pthread based Thread implementation.
 *
 */
class Thread {
public:
    Thread(const char* name = "");
    ~Thread();

    void run();
    int join();
    void shutdown();

    bool isShuttingDown() const;

    // TODO - Sleep, priority.

private:
    Thread(const Thread&);
    Thread& operator=(const Thread&);

    virtual void threadLoop() = 0;

    pthread_t mThread;
    std::string mName;
    bool mIsShuttingDown;
    mutable Mutex mLock;

    static void* thread_entry(void* param);
};

}  // namespace utilities

#endif  // THREAD_H
