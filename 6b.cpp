#include <cstdint>

int main()
{
    std::uint64_t time     = 60947882;
    std::uint64_t distance = 475213810151650;
    int ways_to_win{};
    for( std::uint64_t wait_ms = 0; wait_ms < time; ++wait_ms )
        if( ( time - wait_ms ) * wait_ms > distance )
            ++ways_to_win;

    return ways_to_win;
}
