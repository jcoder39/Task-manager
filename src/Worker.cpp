//
// Created by User on 09.10.2016.
//

#include "../include/Worker.hpp"

namespace TaskManager {

    Worker::Worker(unsigned int id) : _run(false), _thread(), _task(nullptr), _id(id), _state(State::WAIT) {}

    Worker::~Worker()
    {
        if (this->_run.load()) this->Stop();
        this->WaitForStopped();
    }

    void Worker::Start(TaskManager &taskManager)
    {
        this->_run.store(true);
        _thread = std::thread(&Worker::Run, this, std::ref(taskManager));
    }

    void Worker::Run(TaskManager &taskManager)
    {
        using namespace std;
        while (this->_run.load()) {
            unique_lock<mutex> stateLock(this->_stateMutex, defer_lock);
            {
                unique_lock<mutex> cvLock(taskManager._cvMutex);
                taskManager._cv.wait(cvLock,
                                     [this, &taskManager] {
                                         return (!this->_run.load() || _task != nullptr);
                                     });
                if (!this->_run.load()) return;
            }
//            this_thread::sleep_for(chrono::seconds(5));
            this->_task();
            this->_task = nullptr;
            stateLock.lock();
            this->_state.store(State::WAIT);
            stateLock.unlock();
            taskManager.Freed();
        }
    }

    void Worker::WaitForStopped()
    {
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void Worker::Stop()
    {
        this->_run.store(false);
    }

    void Worker::SetTask(const Task &task)
    {
        std::lock_guard<std::mutex> stateGuard(this->_stateMutex);
        this->_state.store(State::START);
        this->_task = task;
    }

    Worker::State Worker::GetState()
    {
        std::lock_guard<std::mutex> stateGuard(this->_stateMutex);
        return _state.load();
    }

    unsigned int Worker::GetId()
    {
        return _id;
    }

}