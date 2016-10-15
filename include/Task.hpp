//
// Created by User on 09.10.2016.
//

#ifndef TASKMANAGER_TASK_HPP
#define TASKMANAGER_TASK_HPP

#include "header.hpp"

NAMESPACE_BEGIN

class Task {
public:
    Task() : _function(nullptr) {}

    Task(const std::function<void()> &function) : _function(function) {}

    Task(const Task &other) : _function(other._function) {}

    Task(Task &&other)
    {
        this->_function = other._function;
        other._function = nullptr;
    }

    void operator()()
    {
        this->_function();
    }

    Task &operator=(const Task &other)
    {
        this->_function = other._function;
        return *this;
    }

    Task &operator=(Task &&other)
    {
        if(this == &other) return *this;
        this->_function = other._function;
        other._function = nullptr;
        return *this;
    }

    Task &operator=(const std::function<void()> function)
    {
        this->_function = function;
        return *this;
    }

    template<typename T>
    bool operator==(const T &var)
    {
        return (_function == var);
    }

    template<typename T>
    bool operator!=(const T &var)
    {
        return (_function != var);
    }

    virtual ~Task() = default;

private:
    std::function<void()> _function;
};

NAMESPACE_END

#endif //TASKMANAGER_TASK_HPP
