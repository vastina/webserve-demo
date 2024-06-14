#ifndef _TASKPOOL_H_
#define _TASKPOOL_H_

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

namespace vastina {

enum importance
{
  maintk = 1,
  common,
  lowest
};

class Taskpool
{
private:
  struct common_
  {
    std::thread worker;
    std::packaged_task<void()> work;
    bool stop;
  } common;
  std::mutex tmutex;
  std::queue<std::function<void()>> tasks2;
  std::queue<std::function<void()>> tasks;

public:
  void start();
  template<typename F, typename... Args>
  void submittask( F&& f, Args&&... args, int level );
  void lastwork();
  void setstoped();
};

template<typename F, typename... Args>
void Taskpool::submittask( F&& f, Args&&... args, int level )
{

  std::function<decltype( f( args... ) )()> func = std::bind( std::forward<F>( f ), std::forward<Args>( args )... );

  auto task_ptr = std::make_shared<std::packaged_task<decltype( f( args... ) )()>>( func );

  if ( level == importance::lowest ) {
    std::unique_lock<std::mutex> lock( tmutex );
    tasks.emplace( [task_ptr]() { ( *task_ptr )(); } );
  } else if ( level == importance::common ) {
    std::unique_lock<std::mutex> lk( tmutex );
    tasks2.emplace( [task_ptr]() { ( *task_ptr )(); } );

    if ( common.worker.joinable() )
      common.worker.join();
  }
};

} // namespace vastina

#endif