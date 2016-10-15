//
// Created by User on 09.10.2016.
//

#ifndef TASKMANAGER_TASKMANAGER_HPP
#define TASKMANAGER_TASKMANAGER_HPP

#include "header.hpp"

NAMESPACE_BEGIN

class Worker;
class Task;

class TaskManager {
    friend class Worker;

public:
    TaskManager(unsigned int numberOfWorkers = std::thread::hardware_concurrency());
    ~TaskManager();

    void Init();
    void Stop();

    template<class F, class... Args>
    auto Execute(F &&function, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        std::future<return_type> futureResult;
        auto packagedTask = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(function), std::forward<Args>(args)...));

        futureResult = packagedTask->get_future();
        {
            lock(this->_cvMutex, this->_taskMutex);
            std::lock_guard<std::mutex> cvGuard(this->_cvMutex, std::adopt_lock);
            std::lock_guard<std::mutex> taskGuard(this->_taskMutex, std::adopt_lock);
            this->_tasks.emplace([this, packagedTask]() { (*packagedTask)(); });
        }
        ExecuteTasks();
        return futureResult;
    }
    void Execute(Task &task);

private:
    std::vector<std::shared_ptr<Worker>> _workers;
    std::condition_variable _cv;
    std::mutex _cvMutex;
    std::mutex _taskMutex;
    std::mutex _workerMutex;
    std::mutex _initMutex;
    std::atomic_bool _init;
    std::queue<Task> _tasks;

    void Freed();
    void ExecuteTasks();
    std::shared_ptr<Worker> GetFreeWorker();
};

NAMESPACE_END

#endif //TASKMANAGER_TASKMANAGER_HPP
