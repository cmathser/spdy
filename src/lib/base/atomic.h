/*
 * Copyright (c) 2012 James Peach
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ATOMIC_H_A14B912F_B134_4D38_8EC1_51C50EC0FBE6
#define ATOMIC_H_A14B912F_B134_4D38_8EC1_51C50EC0FBE6

#include <mutex>
#include <atomic>
#include <thread>

// Increment @val by @amt, returning the previous value.
#define atomic_increment(val, amt) __sync_fetch_and_add(&(val), amt)

// Decrement @val by @amt, returning the previous value.
#define atomic_decrement(val, amt) __sync_fetch_and_sub(&(val), amt)

struct countable
{
    countable() : refcnt(0) {}
    virtual ~countable() {}

private:
    std::atomic<unsigned> refcnt;

    template <typename T> friend T * retain(T * ptr);
    template <typename T> friend void release(T * ptr);
};

template <typename T> T * retain(T * ptr) {
    std::atomic_fetch_add_explicit(&ptr->refcnt, 1u, std::memory_order_acq_rel);
    return ptr;
}

template <typename T> void release(T * ptr) {
    unsigned count = std::atomic_fetch_sub_explicit(&ptr->refcnt, 1u, std::memory_order_acq_rel);
    // If the previous refcount was 1, then we have decremented it to 0. We
    // want to delete in that case.
    if (count == 1) {
        delete ptr;
    }
}

#endif /* ATOMIC_H_A14B912F_B134_4D38_8EC1_51C50EC0FBE6 */
/* vim: set sw=4 ts=4 tw=79 et : */
