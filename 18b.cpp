#include "parse_input.h"

// NOTE: I converted the input using a vim macro rather than writing c++ to do it

advent_t advent( std::vector< std::string > const& input )
{
    std::int64_t y = 0;
    std::int64_t area{};
    std::int64_t edge_area_x2 = 2;

    for( auto const& line : input )
    {
        char dir;
        int len;
        sscanf( line.c_str(), "%c %x", &dir, &len );
        switch( dir )
        {
        case 'U':
            y += len;
            break;
        case 'D':
            y -= len;
            break;
        case 'L':
            area -= y * len;
            break;
        case 'R':
            area += y * len;
            break;
        }
        edge_area_x2 += len;
    }

    return std::abs( area ) + edge_area_x2 / 2;
}
