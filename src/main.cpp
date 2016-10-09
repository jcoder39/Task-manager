#include <iostream>
#include <future>
#include "../include/TaskManager.hpp"

int main() {
    using TaskManager::Task;
    using TaskManager = TaskManager::TaskManager;

    TaskManager taskManager(2);
    taskManager.Init();

    std::future<std::basic_string<char>> futureString;
    std::future<int> futureInt;

    futureString = taskManager.Execute([](const std::string &str){ return str; }, "INT:");
    int i = 10;
    futureInt = taskManager.Execute([i](){ return i; });

    Task task([&i](){ i += 10;});
    taskManager.Execute(task);

    std::cout << futureString.get() << " ";
    std::cout << futureInt.get() << " ";
    std::cout << i << std::endl;

    taskManager.Stop();
    return 0;
}