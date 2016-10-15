//
// Created by User on 15.10.2016.
//

#ifndef TASK_MANAGER_HEADER_HPP
#define TASK_MANAGER_HEADER_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <future>
#include <chrono>
#include <c++/functional>

#define NAMESPACE_BEGIN namespace name { namespace bvv { namespace TaskManager {
#define NAMESPACE_END   } } }

#include "../include/Task.hpp"
#include "../include/TaskManager.hpp"
#include "../include/Worker.hpp"

#endif //TASK_MANAGER_HEADER_HPP
