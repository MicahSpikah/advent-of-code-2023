#include "parse_input.h"
#include <algorithm>

struct grid
{
    std::vector< std::vector< bool > > g_smudged;

    int reflection_line() const
    {
        for( int r = 1; r < g_smudged.size(); ++r )
        {
            bool unsmudged{};
            auto g = g_smudged;
            bool good{ true };
            for( int t = 0; good && r - t > 0 && r + t < g.size(); ++t )
            {
                for( int c = 0; c < g.front().size() && good; ++c )
                {
                    if( g[ r + t ][ c ] != g[ r - ( t + 1 ) ][ c ] )
                    {
                        if( !unsmudged )
                            unsmudged = true;
                        else
                            good = false;
                    }
                }
            }
            if( good && unsmudged )
                return r * 100;
        }
        for( int c = 1; c < g_smudged.front().size(); ++c )
        {
            bool unsmudged{};
            bool good{ true };
            auto g = g_smudged;
            for( int t = 0; good && c - t > 0 && c + t < g.front().size(); ++t )
            {
                for( int r = 0; r < g.size() && good; ++r )
                {
                    if( g[ r ][ c + t ] != g[ r ][ c - ( t + 1 ) ] )
                    {
                        if( !unsmudged )
                            unsmudged = true;
                        else
                            good = false;
                    }
                }
            }
            if( good && unsmudged )
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
            grids.back().g_smudged.resize( grids.back().g_smudged.size() + 1 );
            std::ranges::transform( line, std::back_inserter( grids.back().g_smudged.back() ), []( auto in ) { return in == '#'; } );
        }
    }

    for( auto const& g : grids )
    {
        sum += g.reflection_line();
    }

    return sum;
}
