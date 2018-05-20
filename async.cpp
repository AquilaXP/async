#include "async.h"

#include <string>
#include <algorithm>
#include <iostream>

#include "service.h"

namespace async {

handle_t connect(std::size_t bulk)
{
    return service::Instance().Create( bulk );
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    service::Instance().Receive( handle, data, size );
}


void flush( handle_t handle )
{
    service::Instance().Flush( handle );
}

void disconnect(handle_t handle) {
    service::Instance().Disconnect( handle );
}

}
