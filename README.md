Task manager
===============================
Task manager aimed to managing asynchronous tasks, based on C++11

## Use sample
```C++
using TaskManager::Task;
using TaskManager = TaskManager::TaskManager;

TaskManager taskManager(2);
taskManager.Init();

auto future = taskManager.Execute([](const std::string &str){ return str; }, "string");
std::cout << future.get() << std::endl;

taskManager.Stop();
```

## License

[The MIT License (MIT)](LICENSE.md)