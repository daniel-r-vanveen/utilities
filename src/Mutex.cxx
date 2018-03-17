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

#include "Mutex.hxx"

namespace utilities {

Mutex::Mutex() {
    pthread_mutex_init(&mMutex, nullptr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mMutex);
}

int Mutex::lock() {
    return pthread_mutex_lock(&mMutex);
}

int Mutex::unlock() {
    return pthread_mutex_unlock(&mMutex);
}

int Mutex::tryLock() {
    return pthread_mutex_trylock(&mMutex);
}

Mutex::Lock::Lock(Mutex& mutex) : mLock(mutex) {
    mLock.lock();
}

Mutex::Lock::~Lock() {
    mLock.unlock();
}

}  // namespace utilities
