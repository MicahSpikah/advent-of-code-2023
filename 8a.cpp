#include "parse_input.h"
#include <map>
#include <ranges>

struct dir
{
    std::string l;
    std::string r;
};

advent_t advent( std::vector< std::string > const& input )
{
    std::map< std::string, dir > maps;
    for( auto const& line : input | std::views::drop( 2 ) )
    {
        auto a    = line.substr( 0, 3 );
        auto b    = line.substr( 7, 3 );
        auto c    = line.substr( 12, 3 );
        maps[ a ] = { b, c };
    }
    std::string pos = "AAA";
    std::size_t count{};
    while( pos != "ZZZ" )
    {
        if( input.front()[ count % input.front().size() ] == 'L' )
            pos = maps[ pos ].l;
        else
            pos = maps[ pos ].r;
        ++count;
    }

    return count;
}
