#pragma once

#include "IState.h"
#include "ISubject.h"

/// Данный класс выполняет сборку сырых данных в строки
/// и отправку "слушателям"
class SubjectRawData : public ISubject
{
public:
    SubjectRawData( size_t size );
    ~SubjectRawData();

    void PushData( const std::string& msg );
private:
    void Update();
    bool GetLine( std::string& line );
    void PrintStat() const;

    size_t m_procced_symbol = 0;
    std::string m_buffer;
    AppenderCmd m_ac;
};