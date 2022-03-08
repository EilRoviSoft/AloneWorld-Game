/*********************************************************
*
*  Copyright (C) 2014 by Vitaliy Vitsentiy
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*********************************************************/


#ifndef __ctpl_stl_thread_pool_H__
#define __ctpl_stl_thread_pool_H__

#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <exception>
#include <future>
#include <mutex>
#include <queue>

// thread pool to run user's functors with signature
//      ret func(size_t id, other_params)
// where id is the index of the thread that runs the functor
// ret is some return type

namespace ctpl {
    namespace detail {
        template <typename T>
        class Queue {
        public:
            bool push(T const & value) {
                std::unique_lock<std::mutex> lock(this->_mutex);
                this->_c.push(value);
                return true;
            }
            // deletes the retrieved element, do not use for non integral types
            bool pop(T & v) {
                std::unique_lock<std::mutex> lock(this->_mutex);
                if (this->_c.empty())
                    return false;
                v = this->_c.front();
                this->_c.pop();
                return true;
            }
            bool empty() {
                std::unique_lock<std::mutex> lock(this->_mutex);
                return this->_c.empty();
            }
        private:
            std::queue<T> _c;
            std::mutex _mutex;
        };
    }

    class thread_pool {
    public:

        thread_pool() { this->_Init(); }
        thread_pool(size_t size) { this->_Init(); this->resize(size); }

        // the destructor waits for all the functions in the queue to be finished
        ~thread_pool() {
            this->stop(true);
        }

        // get the number of running _threads in the pool
        size_t size() const { return this->_threads.size(); }

        // number of idle _threads
        size_t idle() const { return this->_idle; }
        std::thread & get_thread(size_t i) { return *this->_threads[i]; }

        // change the number of _threads in the pool
        // should be called from one thread, otherwise be careful to not interleave, also with this->stop()
        // size must be >= 0
        void resize(size_t size) {
            if (!this->_isStop && !this->_isDone) {
                size_t before = this->_threads.size();
                if (before <= size) {  // if the number of _threads is increased
                    this->_threads.resize(size);
                    this->_flags.resize(size);

                    for (size_t i = before; i < size; ++i) {
                        this->_flags[i] = std::make_shared<std::atomic<bool>>(false);
                        this->_SetThread(i);
                    }
                }
                else {  // the number of _threads is decreased
                    for (size_t i = before - 1; i >= size; --i) {
                        *this->_flags[i] = true;  // this thread will finish
                        this->_threads[i]->detach();
                    }
                    {
                        // stop the detached _threads that were waiting
                        std::unique_lock<std::mutex> lock(this->_mutex);
                        this->_cv.notify_all();
                    }
                    this->_threads.resize(size);  // safe to delete because the _threads are detached
                    this->_flags.resize(size);  // safe to delete because the _threads have copies of shared_ptr of the _flags, not originals
                }
            }
        }

        // empty the queue
        void clear_queue() {
            std::function<void(size_t id)> * _f;
            while (this->_queue.pop(_f))
                delete _f; // empty the queue
        }

        // pops a functional wrapper to the original function
        std::function<void(size_t)> pop() {
            std::function<void(size_t id)> * _f = nullptr;
            this->_queue.pop(_f);
            std::unique_ptr<std::function<void(size_t id)>> func(_f); // at return, delete the function even if an exception occurred
            std::function<void(size_t)> f;
            if (_f)
                f = *_f;
            return f;
        }

        // wait for all computing _threads to finish and stop all _threads
        // may be called asynchronously to not pause the calling thread while waiting
        // if isWait == true, all the functions in the queue are run, otherwise the queue is cleared without running the functions
        void stop(bool isWait = false) {
            if (!isWait) {
                if (this->_isStop)
                    return;
                this->_isStop = true;
                for (size_t i = 0, n = this->size(); i < n; ++i) {
                    *this->_flags[i] = true;  // command the _threads to stop
                }
                this->clear_queue();  // empty the queue
            }
            else {
                if (this->_isDone || this->_isStop)
                    return;
                this->_isDone = true;  // give the waiting _threads a command to finish
            }
            {
                std::unique_lock<std::mutex> lock(this->_mutex);
                this->_cv.notify_all();  // stop all waiting _threads
            }
            for (size_t i = 0; i < this->_threads.size(); ++i) {  // wait for the computing _threads to finish
                    if (this->_threads[i]->joinable())
                        this->_threads[i]->join();
            }
            // if there were no _threads in the pool but some functors in the queue, the functors are not deleted by the _threads
            // therefore delete them here
            this->clear_queue();
            this->_threads.clear();
            this->_flags.clear();
        }

        template<typename F, typename... Rest>
        auto push(F && f, Rest&&... rest) ->std::future<decltype(f(0, rest...))> {
            auto pck = std::make_shared<std::packaged_task<decltype(f(0, rest...))(size_t)>>(
                std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...)
                );
            auto _f = new std::function<void(size_t id)>([pck](size_t id) {
                (*pck)(id);
            });
            this->_queue.push(_f);
            std::unique_lock<std::mutex> lock(this->_mutex);
            this->_cv.notify_one();
            return pck->get_future();
        }

        // run the user's function that excepts argument size_t - id of the running thread. returned value is templatized
        // operator returns std::future, where the user can get the result and rethrow the catched exceptins
        template<typename F>
        auto push(F && f) ->std::future<decltype(f(0))> {
            auto pck = std::make_shared<std::packaged_task<decltype(f(0))(size_t)>>(std::forward<F>(f));
            auto _f = new std::function<void(size_t id)>([pck](size_t id) {
                (*pck)(id);
            });
            this->_queue.push(_f);
            std::unique_lock<std::mutex> lock(this->_mutex);
            this->_cv.notify_one();
            return pck->get_future();
        }


    private:
        // deleted
        thread_pool(const thread_pool &);// = delete;
        thread_pool(thread_pool &&);// = delete;
        thread_pool & operator=(const thread_pool &);// = delete;
        thread_pool & operator=(thread_pool &&);// = delete;

        void _SetThread(size_t i) {
            std::shared_ptr<std::atomic<bool>> flag(this->_flags[i]); // a copy of the shared ptr to the flag
            auto f = [this, i, flag/* a copy of the shared ptr to the flag */]() {
                std::atomic<bool> & _flag = *flag;
                std::function<void(size_t id)> * _f;
                bool isPop = this->_queue.pop(_f);
                while (true) {
                    while (isPop) {  // if there is anything in the queue
                        std::unique_ptr<std::function<void(size_t id)>> func(_f); // at return, delete the function even if an exception occurred
                        (*_f)(i);
                        if (_flag)
                            return;  // the thread is wanted to stop, return even if the queue is not empty yet
                        else
                            isPop = this->_queue.pop(_f);
                    }
                    // the queue is empty here, wait for the next command
                    std::unique_lock<std::mutex> lock(this->_mutex);
                    ++this->_idle;
                    this->_cv.wait(lock, [this, &_f, &isPop, &_flag](){ isPop = this->_queue.pop(_f); return isPop || this->_isDone || _flag; });
                    --this->_idle;
                    if (!isPop)
                        return;  // if the queue is empty and this->_isDone == true or *flag then return
                }
            };
            this->_threads[i].reset(new std::thread(f)); // compiler may not support std::make_unique()
        }

        void _Init() { this->_idle = 0; this->_isStop = false; this->_isDone = false; }

        std::vector <std::unique_ptr <std::thread>> _threads;
        std::vector <std::shared_ptr <std::atomic<bool>>> _flags;
        detail::Queue <std::function <void(size_t id)>*> _queue;
        std::atomic <bool> _isDone;
        std::atomic <bool> _isStop;
        // how many _threads are waiting
        std::atomic <size_t> _idle;

        std::mutex _mutex;
        std::condition_variable _cv;
    };

}

#endif // __ctpl_stl_thread_pool_H__
