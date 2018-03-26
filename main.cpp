#include <iostream>

#include "async.h"
#include <fstream>

#include <thread>

int main(int, char *[]) {
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    std::ifstream f( R"(S:\Projects\OUTS\DZ\10.bulkmt-build\test_cmd1.txt)" );
    std::ifstream f1( R"(S:\Projects\OUTS\DZ\10.bulkmt-build\test_cmd1.txt)" );
   /* while( true )
    {
        char buf[12];
        f.read( buf, 12 );        
        async::receive( h, buf, f.gcount() );
        if( f.good() == false )
            break;

        f1.read( buf, 12 );
        async::receive( h2, buf, f1.gcount() );
        if( f1.good() == false )
            break;
    }*/
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive( h, "v\nn", 3 );
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::receive( h, "{\nc\nd\n2\n89\n", 11 );
    std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
    async::disconnect(h);
    async::disconnect(h2);
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    return 0;
}
