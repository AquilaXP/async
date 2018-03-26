#pragma once

#include <iostream>
#include <sstream>

#include "IState.h"
#include "ISubject.h"

class SubjectRawData : public ISubject
{
    using lock_m = std::lock_guard<std::mutex>;
public:
    SubjectRawData( size_t size ) : m_ac(this, size)
    {
    }
    ~SubjectRawData()
    {
        try
        {
            if( m_data.empty() == false )
            {
                m_ac.AppendCmd( m_data );
            }
            m_ac.Flush();
            PrintStat();
        }
        catch( const std::exception& )
        {

        }
    }
    void PushData( const std::string& msg )
    {
        m_data += msg;
        std::string line;
        while( GetLine(line) )
        {
            m_ac.AppendCmd( line );
        }
        Update();
    }

private:
    void Update()
    {
        std::move( m_data.begin() + m_procced_line, m_data.end(), m_data.begin() );
        m_data.erase( m_data.size() - m_procced_line );
        m_procced_line = 0;
    }
    bool GetLine( std::string& line )
    {
        auto b = m_data.find_first_of( '\n', m_procced_line );
        if( b == m_data.npos )
            return false;

        line = std::string( m_data.begin() + m_procced_line, m_data.begin() + b );
        m_procced_line = b + 1;

        return true;
    }
    void PrintStat() const
    {
        std::stringstream ss;
        ss << "main[" << this << "]: \n";
        m_ac.GetStat().Print( ss );

        std::cout << ss.str();
    }

    size_t m_procced_line = 0;
    std::string m_data;
    AppenderCmd m_ac;
};