#include "parse_input.h"
#include <algorithm>

struct grid
{
    std::vector< std::vector< bool > > g;

    int reflection_line() const
    {
        for( int r = 1; r < g.size(); ++r )
        {
            bool good{ true };
            for( int t = 0; good && r - t > 0 && r + t < g.size(); ++t )
            {
                if( !std::ranges::equal( g[ r + t ], g[ r - ( 1 + t ) ] ) )
                    good = false;
            }
            if( good )
                return r * 100;
        }
        for( int c = 1; c < g.front().size(); ++c )
        {
            bool good{ true };
            for( int t = 0; good && c - t > 0 && c + t < g.front().size(); ++t )
            {
                for( int r = 0; r < g.size() && good; ++r )
                {
                    if( g[ r ][ c + t ] != g[ r ][ c - ( t + 1 ) ] )
                        good = false;
                }
            }
            if( good )
                return c;
        }
        throw std::runtime_error( "Illegal" );
    }
};

advent_t advent( std::vector< std::string > const& input )
{
    std::uint64_t sum{};

    std::vector< grid > grids( 1 );

    for( auto const& line : input )
    {
        if( line.empty() )
        {
            grids.resize( grids.size() + 1 );
        }
        else
        {
            grids.back().g.resize( grids.back().g.size() + 1 );
            std::ranges::transform( line, std::back_inserter( grids.back().g.back() ), []( auto in ) { return in == '#'; } );
        }
    }

    for( auto const& g : grids )
    {
        sum += g.reflection_line();
    }

    return sum;
}
