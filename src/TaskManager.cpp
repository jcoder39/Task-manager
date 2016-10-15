//
// Created by User on 09.10.2016.
//

#include "../include/TaskManager.hpp"

NAMESPACE_BEGIN

TaskManager::TaskManager(unsigned int numberOfWorkers) : _init(false)
{
    if (numberOfWorkers == 0) {
        numberOfWorkers = 1;
    }
    _workers.reserve(numberOfWorkers);
    for (unsigned int i = 0; i < numberOfWorkers; i++) {
        _workers.emplace_back(new Worker(i));
    }
}

TaskManager::~TaskManager()
{
    _workers.clear();
}

void TaskManager::ExecuteTasks()
{
    using namespace std;
    {
        lock_guard<mutex> initGuard(this->_initMutex);
        if (!this->_init.load()) {
            throw runtime_error("Task manager should be initialized");
        }
    }
    lock(this->_cvMutex, this->_taskMutex, this->_workerMutex);
    lock_guard<mutex> cvGuard(this->_cvMutex, adopt_lock);
    lock_guard<mutex> taskGuard(this->_taskMutex, adopt_lock);
    lock_guard<mutex> workerGuard(this->_workerMutex, adopt_lock);
    auto worker = GetFreeWorker();
    if (worker == nullptr) {
        return;
    }
    if (_tasks.empty()) {
        return;
    }
    Task task = move(_tasks.front());
    _tasks.pop();
    worker->SetTask(task);
    _cv.notify_all();
}

void TaskManager::Execute(Task &task)
{
    using namespace std;
    {
        lock_guard<mutex> initGuard(this->_initMutex);
        if (!this->_init.load()) {
            throw runtime_error("Task manager should be initialized");
        }
    }
    {
        lock_guard<mutex> taskGuard(this->_taskMutex);
        this->_tasks.push(task);
    }
    ExecuteTasks();
}

std::shared_ptr<Worker> TaskManager::GetFreeWorker()
{
    for (auto worker : _workers) {
        if (worker->GetState() == Worker::State::WAIT) {
            return worker;
        }
    }
    return std::shared_ptr<Worker>();
}

void TaskManager::Freed()
{
    ExecuteTasks();
}

void TaskManager::Init()
{
    std::lock_guard<std::mutex> initGuard(this->_initMutex);
    this->_init.store(true);
    for (auto worker : _workers) {
        worker->Start(*this);
    }
}

void TaskManager::Stop()
{
    std::lock_guard<std::mutex> cvGuard(this->_cvMutex);
    for (auto worker : _workers) {
        worker->Stop();
    }
    this->_cv.notify_all();
}

NAMESPACE_END