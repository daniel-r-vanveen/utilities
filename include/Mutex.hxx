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

#ifndef MUTEX_H
#define MUTEX_H

namespace utilities {

// Support for UNIX platforms only.
#include <pthread.h>

class Condition;

/*
 * pthread based Mutex implementation.
 *
 * Implementation does not currently support Mutex sharing across processes.
 */
class Mutex {
    Mutex();
    ~Mutex();

    /*
     * Lock the Mutex.
     * returns 0 on success, otherwise an error.
     */
    int lock();

    /*
     * Unlock the Mutex.
     * returns 0 on success, otherwise an error.
     */
    int unlock();

    /*
     * Try to lock the Mutex.
     * returns 0 on success, otherwise an error. Returns EBUSY if Mutex already locked.
     */
    int tryLock();

    // Lock is a scoped Mutex implementation. The Mutex is locked on construction
    // and released when Lock goes out of scope.
    class Lock {
    public:
        Lock(Mutex& mutex);
        ~Lock();
    private:
        Mutex& mLock;
    };

private:
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);

    friend class Condition;
    pthread_mutex_t* getMutex();

    pthread_mutex_t mMutex;
};

}  // namespace utilities

#endif  // MUTEX_H
