#include "SubjectRawData.h"

#include <iostream>
#include <sstream>

SubjectRawData::SubjectRawData( size_t size )
    : m_ac( this, size )
{
}

SubjectRawData::~SubjectRawData()
{
    try
    {
        /// Чтото есть, дообрабатываем
        if( m_buffer.empty() == false )
        {
            m_ac.AppendCmd( m_buffer );
        }
        /// И завершаем, дальше ничего не будет
        m_ac.Flush();
        PrintStat();
    }
    catch( const std::exception& )
    {
    }
}


void SubjectRawData::Flush()
{
    m_ac.Flush();
}

void SubjectRawData::PushData( const std::string& msg )
{
    m_buffer += msg;
    std::string line;
    while( GetLine( line ) )
    {
        m_ac.AppendCmd( line );
    }
    /// Обновляем текущий буфер m_buffer - смещаем необработанные данные вначало, удаляем лишнее
    Update();
}

void SubjectRawData::Update()
{
    std::move( m_buffer.begin() + m_procced_symbol, m_buffer.end(), m_buffer.begin() );
    m_buffer.erase( m_buffer.size() - m_procced_symbol );
    m_procced_symbol = 0;
}

bool SubjectRawData::GetLine( std::string& line )
{
    auto b = m_buffer.find_first_of( '\n', m_procced_symbol );
    if( b == m_buffer.npos )
        return false;

    line = std::string( m_buffer.begin() + m_procced_symbol, m_buffer.begin() + b );
    m_procced_symbol = b + 1;

    return true;
}

void SubjectRawData::PrintStat() const
{
    std::stringstream ss;
    ss << "main[" << this << "]: \n";
    m_ac.GetStat().Print( ss );

    std::cout << ss.str();
}
