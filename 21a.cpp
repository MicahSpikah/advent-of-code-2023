#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

using found = std::vector< std::pair< int, int > >;

advent_t advent( std::vector< std::string > const& input )
{
    auto modified     = input;
    std::uint64_t sum = 1;

    found last = { { 65, 65 } };

    for( int step = 0; step < 64; ++step )
    {
        found curr;
        for( auto const& point : last )
        {
            auto go = [ & ]( auto const row, auto const col ) {
                if( modified[ row ][ col ] == '.' )
                {
                    curr.push_back( { row, col } );
                    modified[ row ][ col ] = step % 2 ? 'O' : 'E';
                }
            };
            go( point.first + 1, point.second );
            go( point.first - 1, point.second );
            go( point.first, point.second + 1 );
            go( point.first, point.second - 1 );
        }
        if( step % 2 )
            sum += curr.size();
        last = curr;
    }

    return sum;
}
