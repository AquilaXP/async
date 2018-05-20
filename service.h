#pragma once

#include <thread>
#include <mutex>

#include "async.h"
#include "threadsafe_queue.h"
#include "FileObserverMT.h"
#include "ConsoleObserver.h"
#include "ThreadObserver.h"

/// Класс обрабатывающий запросы, является потокобезопасным.
class service
{
    using lock_m = std::lock_guard<std::mutex>;
public:
    service( const service& ) = delete;
    service& operator = ( const service& ) = delete;

    static service& Instance();

    /// Создает handle_t
    async::handle_t Create( size_t bulk );

    /// Помещает в очередь на обработку
    void Receive( async::handle_t h, const char* data, size_t size );

    /// Заставляем сбросить данные
    void Flush( async::handle_t h );

    /// Помещает в очередь на удаление
    void Disconnect( async::handle_t h );

private:
    service();
    ~service();

    /// Функция для обработки очереди
    void Run();

    /// Функция для удаления контекста
    void Remove( async::handle_t h );

    threadsafe_queue<std::function<void()> > m_queue_commands;
    std::thread m_main_thread;

    FileObserverMT m_file_obs;
    ConsoleObserver m_console_obs;
    ThreadObserver m_thread_obs;
};

