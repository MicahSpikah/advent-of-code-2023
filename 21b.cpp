#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

using found = std::vector< std::pair< int, int > >;

advent_t advent( std::vector< std::string > const& input )
{
    if( input.size() % 2 != 1 || input.front().size() % 2 != 1 || input.size() != input.front().size() || input[ input.size() / 2 ][ input.size() / 2 ] != 'S' )
        throw std::runtime_error( "Bad input" );

    std::map< std::string, std::vector< std::string > > gardens;
    gardens[ "C" ]                                         = input;
    gardens[ "C" ][ input.size() / 2 ][ input.size() / 2 ] = '.';
    gardens[ "N" ]                                         = gardens[ "C" ];
    gardens[ "E" ]                                         = gardens[ "C" ];
    gardens[ "W" ]                                         = gardens[ "C" ];
    gardens[ "S" ]                                         = gardens[ "C" ];
    gardens[ "NE" ]                                        = gardens[ "C" ];
    gardens[ "NW" ]                                        = gardens[ "C" ];
    gardens[ "SE" ]                                        = gardens[ "C" ];
    gardens[ "SW" ]                                        = gardens[ "C" ];

    // we start at x,y=65 on turn 0.
    // we hit x=131,y=65 on turn 66 (an EVEN cardinal)
    // we hit x=131,y=131 on turn 132 (an EVEN diagonal)
    // we hit x=262 on turn 197 (an ODD cardinal)
    // we hit x,y=262 on turn 263 (an ODD diagonal)

    // A new cardinal map will spawn every 132 turns starting at turn 66
    //(turn / 132) of each diag map will spawn every 132 turns starting at turn 0
    //  26501365 / 132 = 200767 cycles + 121 steps SO
    //  1 completed C
    //  200767 completed N,S,E,W and 1 more of each at step 121-66=55 (index 27)
    //  200767 completed diags and 1 more of each at step 121 (index 60)
    std::map< std::string, std::vector< int > > steps;

    std::map< std::string, std::pair< int, int > > const starting_pos{
        { "C", { input.size() / 2, input.size() / 2 } },
        { "N", { input.size() - 1, input.size() / 2 } },
        { "S", { 0, input.size() / 2 } },
        { "E", { input.size() / 2, 0 } },
        { "W", { input.size() / 2, input.size() - 1 } },
        { "NW", { input.size() - 1, input.size() - 1 } },
        { "NE", { input.size() - 1, 0 } },
        { "SW", { 0, input.size() - 1 } },
        { "SE", { 0, 0 } },
    };

    for( auto& [ type, g ] : gardens )
    {
        auto& garden                                        = g;
        found last                                          = { starting_pos.at( type ) };
        garden[ last.front().first ][ last.front().second ] = '0';
        steps[ type ].push_back( 1 );

        for( int step = 1; step < 1000; ++step )
        {
            found curr;
            for( auto const& point : last )
            {
                auto go = [ & ]( auto const row, auto const col ) {
                    if( garden[ row ][ col ] == '.' )
                    {
                        curr.push_back( { row, col } );
                        garden[ row ][ col ] = '0' + ( step % 10 );
                    }
                };
                go( point.first + 1, point.second );
                go( point.first - 1, point.second );
                go( point.first, point.second + 1 );
                go( point.first, point.second - 1 );
            }
            if( curr.empty() )
                break;

            steps[ type ].push_back( step < 2 ? curr.size() : curr.size() + steps[ type ][ step - 2 ] );

            last = curr;
        }
    }

    // ALGORITHM SKETCH:
    // There are 9 types of garden. C, N, E, W, S, NE, NW, SE, SW
    // Start to fill center garden
    // When we reach north edge, spawn a "north garden". Same for south, east, west.
    // When we reach each corner of center, spawn a "ne", "nw", "se", "sw" garden
    // While filling the non-C gardens, keep track of how many spots are filled after how many steps. If a whole round goes by with nothing being added to the garden, we can stop thinking about it
    // Those per garden maps should be enough to figure out what gets spawned by when

    /*        5
             434
            42124
           5310135
            42124
             434
              5
     0: 0
     1: 66
     2: 132
     3: 197 ([1]+131)
     4: 263 ([2]+131)
     5: 328 ([3]+131)
     6: 394 ([4]+131)
     ...

     // Turns out that cardinals take 196 steps to fill, diags take 261

     e.g. turn 400
     would mean:
     0 is full (on an even)
     1 (1 of each card) is full (on an even)
     2 (1 of each diag) is full (on an even)
     3 (1 of each card) is full (on an odd bc 400 - 197 is 203)
     4 (2 of each diag) is on (400-263) = 137
     5 (1 of each card) is on (400-328) = 72
     6 (3 of each diag) is on (400-394) = 6

     */

    auto const get_steps = [ & ]( auto const& type, int const step ) -> std::uint64_t {
        if( step < 0 )
            return 0;
        if( step >= steps[ type ].size() )
        {
            if( step % 2 != steps[ type ].size() % 2 )
                return steps[ type ].back();
            return steps[ type ][ steps[ type ].size() - 2 ];
        }
        return steps[ type ][ step ];
    };

    auto const total_steps = 26501365;

    std::uint64_t sum{ get_steps( "C", total_steps ) };

    for( int loop = 0; loop * 131 - 1000 < total_steps; ++loop )
    {
        sum += get_steps( "N", total_steps - ( 66 + loop * 131 ) );
        sum += get_steps( "S", total_steps - ( 66 + loop * 131 ) );
        sum += get_steps( "E", total_steps - ( 66 + loop * 131 ) );
        sum += get_steps( "W", total_steps - ( 66 + loop * 131 ) );

        sum += ( loop + 1 ) * get_steps( "NE", total_steps - ( 132 + loop * 131 ) );
        sum += ( loop + 1 ) * get_steps( "SE", total_steps - ( 132 + loop * 131 ) );
        sum += ( loop + 1 ) * get_steps( "NW", total_steps - ( 132 + loop * 131 ) );
        sum += ( loop + 1 ) * get_steps( "SW", total_steps - ( 132 + loop * 131 ) );
    }

    return sum;
}
