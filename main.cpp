#include <iostream>

#include "async.h"
#include <fstream>

#include <thread>

int main(int, char *[]) {
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive( h, "v\nn", 3 );
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::receive( h, "{\nc\nd\n2\n89\n", 11 );

    std::thread t1( [&]{
        for( int i = 0; i < 1000; ++i )
            async::receive( h, "1\n", 2 );
    } );
    std::thread t2( [&]{
        for( int i = 0; i < 1000; ++i )
            async::receive( h2, "1\n", 2 );
    } );
    t1.join();
    t2.join();

    async::disconnect(h);
    async::disconnect(h2);
    std::this_thread::sleep_for( std::chrono::seconds( 4 ) );
    return 0;
}
