//
// Created by User on 09.10.2016.
//

#ifndef TASKMANAGER_WORKER_HPP
#define TASKMANAGER_WORKER_HPP

#include "header.hpp"

NAMESPACE_BEGIN

class TaskManager;

class Task;

class Worker {
public:
    Worker(unsigned int id);
    ~Worker();

    enum class State {
        STOP, WAIT, START
    };

    void Start(TaskManager &taskManager);
    void Stop();
    void WaitForStopped();
    unsigned int GetId();
    void SetTask(const Task &task);
    Worker::State GetState();

private:
    unsigned int _id;
    std::thread _thread;
    std::atomic_bool _run;
    Task _task;
    std::mutex _stateMutex;
    std::atomic<State> _state;

    void Run(TaskManager &taskManager);
};

NAMESPACE_END

#endif //TASKMANAGER_WORKER_HPP
