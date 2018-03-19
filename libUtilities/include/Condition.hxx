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

#ifndef CONDITION_H
#define CONDITION_H

// Support for UNIX platforms only.
#include <pthread.h>

namespace utilities {

class Mutex;

/*
 * pthread based Condition implementation.
 */
class Condition {
public:
    Condition();
    ~Condition();

    /*
     * Unblock at least one of the threads that are blocked by this Condition.
     * returns 0 on success, otherwise an error.
     */
    int signal();

    /*
     * Unblock all of the threads that are blocked by this Condition.
     * returns 0 on success, otherwise an error.
     */
    int broadcast();

    /*
     * Block thread on Condition variable. Mutex must be locked before calling.
     * returns 0 on success, otherwise an error.
     */
    int wait(Mutex& mutex);

    /*
     * Block thread on Condition variable, automatically releasing after releaseTime.
     * Mutex must be locked before calling.
     * returns 0 on success, otherwise an error. Returns ETIMEDOUT on releaseTime elapsed.
     */
    int timedWait(Mutex& mutex, const struct timespec *releaseTime);

    /*
     * Convenience functions to wait for a specified number of milliseconds, microseconds
     * or nanoseconds. Follows timedWait.
     */
    int timedWaitMs(Mutex& mutex, long ms);
    int timedWaitUs(Mutex& mutex, long us);
    int timedWaitNs(Mutex& mutex, long ns);

private:
    Condition(const Condition&);
    Condition& operator=(const Condition&);

    pthread_cond_t mCondition;
};

}  // namespace utilities

#endif  // CONDITION_H
