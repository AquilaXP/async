#include "service.h"

#include <iostream>

#include "SubjectRawData.h"

size_t count_thread = 4;

#ifdef WIN32
std::mutex mut;
#endif

service& service::Instance()
{
#ifdef WIN32
    static service* single = nullptr;
    if( single == nullptr )
    {
        std::lock_guard<std::mutex> lk(mut);
        if( single == nullptr )
        {
            single = new service;
        }
    }
    return *single;
#else
    static service single;
    return single;
#endif
}

async::handle_t service::Create( size_t bulk )
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
void service::Receive( async::handle_t h, const char* data, size_t size )
{
    std::string msg( data, size );
    m_queue_commands.push( [h, msg]()
    {
        auto p = reinterpret_cast<SubjectRawData*>( h );
        p->PushData( msg );
    } );
}
void service::Disconnect( async::handle_t h )
{
    m_queue_commands.push( [this, h]()
    {
        this->Remove( h );
    } );
}

service::service()
    : m_file_obs( count_thread ), m_thread_obs( &m_console_obs )
{
    m_main_thread = std::thread( &service::Run, this );
    m_thread_obs.Start();
}

service::~service()
{
    // Записываем пустую комманду, чтобы завершить поток.
    m_queue_commands.push( nullptr );
    if( m_main_thread.joinable() )
        m_main_thread.join();
}

void service::Run()
{
    std::function<void()> del;
    while( true )
    {
        try
        {
            m_queue_commands.wait_and_pop( del );
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

void service::Remove( async::handle_t h )
{
    std::thread( [h]{
        delete reinterpret_cast<SubjectRawData*>( h );
    } ).detach();
}
