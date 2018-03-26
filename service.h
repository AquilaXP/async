#pragma once

#include <thread>
#include <map>
#include <mutex>
#include <iostream>
#include <future>

#include "async.h"
#include "threadsafe_queue.h"
#include "SubjectRawData.h"
#include "FileObserverMT.h"
#include "ConsoleObserver.h"
#include "ThreadObserver.h"

class service
{
    using lock_m = std::lock_guard<std::mutex>;
public:
    service( const service& ) = delete;
    service& operator = ( const service& ) = delete;

    static service& Instance()
    {
        static service single;
        return single;
    }
    async::handle_t Create( size_t bulk )
    {
        async::handle_t p = nullptr;
        try
        {
            std::unique_ptr<SubjectRawData> up( new SubjectRawData( bulk ) );
            up->Attach( &m_thread_obs );
            up->Attach( &m_file_obs );
            p = up.release();
        }
        catch( const std::exception& )
        {
        }
        return p;
    }
    void Receive( async::handle_t h, const char* data, size_t size )
    {
        std::string msg( data, size );
        m_worked.push( [h,msg]()
        {
            auto p = reinterpret_cast<SubjectRawData*>( h );
            p->PushData( msg );
        });
    }
    void Disconnect( async::handle_t h )
    {
        m_worked.push( [this,h]()
        {
            this->remove( h );
        });
    }
private:
    service() 
        : m_file_obs(1), m_thread_obs(&m_console_obs)
    {
        m_main_thread = std::thread( &service::Run, this );
        m_thread_obs.Start();
    }
    ~service()
    {
        m_worked.push( nullptr );
        if( m_main_thread.joinable() )
            m_main_thread.join();
    }
    void Run()
    {
        std::function<void()> del;
        while( true )
        {
            try
            {
                m_worked.wait_and_pop( del );
                if( !del )
                    break;

                del();
            }
            catch( const std::exception& )
            {
                std::cerr << "Exception!" << std::endl;
            }
        }
    }

    void remove( async::handle_t h )
    {
        std::thread( [h]{ 
            delete reinterpret_cast<SubjectRawData*>(h);
        } ).detach();
    }

    threadsafe_queue<std::function<void()> > m_worked;
    std::thread m_main_thread;
    

    FileObserverMT m_file_obs;
    ConsoleObserver m_console_obs;
    ThreadObserver m_thread_obs;
};

