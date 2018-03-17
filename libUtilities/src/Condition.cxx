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

#include "Condition.hxx"
#include "Mutex.hxx"

namespace utilities {

Condition::Condition() {
    pthread_cond_init(&mCondition, nullptr);
}

Condition::~Condition() {
    pthread_cond_destroy(&mCondition);
}

int Condition::signal() {
    return pthread_cond_signal(&mCondition);
}

int Condition::broadcast() {
    return pthread_cond_broadcast(&mCondition);
}

int Condition::wait(Mutex& mutex) {
    return pthread_cond_wait(&mCondition, mutex.getMutex());
}

int Condition::timedWait(Mutex& mutex, const struct timespec *releaseTime) {
    return pthread_cond_timedwait(&mCondition, mutex.getMutex(), releaseTime);
}

int Condition::timedWaitMs(Mutex& mutex, long ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = ms % 1000;
    return timedWait(mutex, &ts);
}

int Condition::timedWaitUs(Mutex& mutex, long us) {
    struct timespec ts;
    ts.tv_sec = us / 1000000;
    ts.tv_nsec = us % 1000000;
    return timedWait(mutex, &ts);
}

int Condition::timedWaitNs(Mutex& mutex, long ns) {
    struct timespec ts;
    ts.tv_sec = ns / 1000000000;
    ts.tv_nsec = ns % 1000000000;
    return timedWait(mutex, &ts);
}

}  // namespace utilities
