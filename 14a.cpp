#include "parse_input.h"
#include <algorithm>

advent_t advent( std::vector< std::string > const& input )
{
    auto rolled = input;

    for( int i = 0; i < rolled.size(); ++i )
    {
        for( int j = i; j > 0; --j )
        {
            for( int c = 0; c < rolled.front().size(); ++c )
            {
                if( rolled[ j ][ c ] == 'O' && rolled[ j - 1 ][ c ] == '.' )
                {
                    rolled[ j ][ c ]     = '.';
                    rolled[ j - 1 ][ c ] = 'O';
                }
            }
        }
    }

    std::uint64_t sum{};
    for( int i = 0; i < input.size(); ++i )
    {
        sum += ( input.size() - i ) * std::ranges::count( rolled[ i ], 'O' );
    }

    return sum;
}
