#include "parse_input.h"
#include <algorithm>
#include <map>
#include <numeric>
#include <ranges>

struct dir
{
    std::string l;
    std::string r;
};

advent_t advent( std::vector< std::string > const& input )
{
    std::map< std::string, dir > maps;
    std::vector< std::string > pos;
    for( auto const& line : input | std::views::drop( 2 ) )
    {
        auto a    = line.substr( 0, 3 );
        auto b    = line.substr( 7, 3 );
        auto c    = line.substr( 12, 3 );
        maps[ a ] = { b, c };
        if( a[ 2 ] == 'A' )
            pos.push_back( a );
    }
    std::vector< std::size_t > counts;
    for( auto s : pos )
    {
        std::size_t count{};
        while( s[ 2 ] != 'Z' )
        {
            if( input.front()[ count % input.front().size() ] == 'L' )
                s = maps[ s ].l;
            else
                s = maps[ s ].r;
            ++count;
        }
        counts.push_back( count );
    }

    return std::reduce( counts.cbegin() + 1, counts.cend(), counts.front(), &std::lcm< std::size_t, std::size_t > );
}
