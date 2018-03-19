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

#include "Thread.hxx"

namespace utilities {

Thread::Thread(const char* name)
    : mName(name)
    , mIsShuttingDown(true) {
}

Thread::~Thread() {
    shutdown();
    join();
}

void Thread::run() {
    Mutex::Lock l(mLock);
    mIsShuttingDown = false;
    pthread_create(&mThread,
                   nullptr /* default attributes */,
                   thread_entry,
                   this);
}

int Thread::join() {
    //Mutex::Lock l(mLock);
    return pthread_join(mThread, nullptr);
}

void Thread::shutdown() {
    Mutex::Lock l(mLock);
    mIsShuttingDown = true;
}

bool Thread::isShuttingDown() const {
    Mutex::Lock l(mLock);
    return mIsShuttingDown;
}

void* Thread::thread_entry(void* param) {
    Thread* t = (Thread*)param;
    // By default, all threads created using pthread_create inherit the program name.
    // Override this if a unique name has been requested.
    if (!t->mName.empty()) {
#if defined (__APPLE__)
        pthread_setname_np(t->mName.c_str());
#else  // Linux
        pthread_setname_np(t->mThread, t->mName.c_str());
#endif
    }

    t->threadLoop();
    return nullptr;
}

}  // namespace utilities
