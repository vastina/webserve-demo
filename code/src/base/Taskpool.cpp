#include "base/Taskpool.hpp"

#include "base/loger.hpp"

namespace vastina
{

    void Taskpool::Start()
    {
        {
            std::unique_lock<std::mutex> lk(tmutex);
            common.stop = false;
            common.work = std::packaged_task<void()>{[&]() -> void
            {
                for (;;)
                {
                    if (tasks2.empty())
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(tmutex);
                        if (common.stop && tasks.empty())
                            break;
                        if (!tasks2.empty())
                        {
                            task = std::move(tasks2.front());
                            tasks2.pop();
                        }
                    }
                    task();
                }
            }};
            common.worker = std::thread{[this]() { common.work(); }};
        }
        SubmitTask([] { vastina::logtest("taskpool Init"); }, importance::lowest);
    }

    void Taskpool::LastWork()
    {
        std::unique_lock<std::mutex> lock(tmutex);
        std::function<void()> task;
        while (!tasks.empty())
        {
            task = std::move(tasks.front());
            tasks.pop();
            task();
        }
        // print("(commonfuture: {})\n",common._future.get());
    }

    void Taskpool::setStop()
    {
        std::unique_lock<std::mutex> lk(tmutex);
        common.stop = true;
    }

} // namespace vastina