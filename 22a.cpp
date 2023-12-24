#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

struct block
{
    int x0, y0, z0;
    int x1, y1, z1;
    int min_x, min_y, min_z;
    int max_x, max_y, max_z;

    void cache()
    {
        min_x = std::min( x0, x1 );
        min_y = std::min( y0, y1 );
        min_z = std::min( z0, z1 );
        max_x = std::max( x0, x1 );
        max_y = std::max( y0, y1 );
        max_z = std::max( z0, z1 );
    }

    bool rests_on( block const& b ) const
    {
        return ( b.max_z + 1 == min_z ) && ( ( b.max_x >= min_x ) && ( max_x >= b.min_x ) ) && ( ( b.max_y >= min_y ) && ( max_y >= b.min_y ) );
    }

    void drop()
    {
        --max_z;
        --min_z;
        --z0;
        --z1;
    }
};

advent_t advent( std::vector< std::string > const& input )
{
    std::vector< block > blocks;
    for( auto const& line : input )
    {
        block b;
        sscanf( line.c_str(), "%d,%d,%d~%d,%d,%d", &b.x0, &b.y0, &b.z0, &b.x1, &b.y1, &b.z1 );
        b.cache();
        blocks.push_back( b );
    }

    bool fell = true;
    while( fell )
    {
        fell = false;
        for( auto& b : blocks )
        {
            bool rests{};
            while( b.min_z > 1 && !rests )
            {
                for( auto const& p : blocks )
                {
                    if( b.rests_on( p ) )
                    {
                        rests = true;
                        break;
                    }
                }
                if( !rests )
                {
                    b.drop();
                    fell = true;
                }
            }
        }
    }

    int sum{};
    for( auto const& b : blocks )
    {
        bool could_go = true;
        for( auto const& c : blocks )
        {
            if( !could_go )
                break;
            if( c.rests_on( b ) )
            {
                could_go = false;
                for( auto const& d : blocks )
                {
                    if( c.rests_on( d ) && &d != &b )
                    {
                        could_go = true;
                        break;
                    }
                }
            }
        }
        if( could_go )
        {
            ++sum;
        }
    }
    return sum;
}
