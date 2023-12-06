#include "parse_input.h"

advent_t advent( std::vector< std::string > const& input )
{
    std::uint64_t result = 1;
    std::vector< int > times( 4 );
    std::vector< int > distances( 4 );

    sscanf( input[ 0 ].c_str(), "Time: %d %d %d %d", &times[ 0 ], &times[ 1 ], &times[ 2 ], &times[ 3 ] );
    sscanf( input[ 1 ].c_str(), "Distance: %d %d %d %d", &distances[ 0 ], &distances[ 1 ], &distances[ 2 ], &distances[ 3 ] );

    for( int r = 0; r < times.size(); ++r )
    {
        int ways_to_win{};
        for( int wait_ms = 0; wait_ms < times[ r ]; ++wait_ms )
            if( ( times[ r ] - wait_ms ) * wait_ms > distances[ r ] )
                ++ways_to_win;

        result *= ways_to_win;
    }

    return result;
}
