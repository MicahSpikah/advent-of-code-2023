#include "parse_input.h"

advent_t advent( std::vector< std::string > const& input )
{
    std::uint8_t reg{};
    std::uint64_t sum{};
    for( auto const c : input.front() )
    {
        if( c == ',' )
        {
            sum += reg;
            reg = 0;
        }
        else
        {
            reg += static_cast< std::uint8_t >( c );
            reg *= 17;
        }
    }
    sum += reg;

    return sum;
}
