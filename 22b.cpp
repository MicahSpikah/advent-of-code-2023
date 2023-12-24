#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>
#include <set>

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

    auto const make_em_fall = []( auto& bl ) {
        std::set< block* > who_fell;
        bool fell = true;
        while( fell )
        {
            fell = false;
            for( auto& b : bl )
            {
                bool rests{};
                while( b.min_z > 1 && !rests )
                {
                    for( auto const& p : bl )
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
                        who_fell.insert( &b );
                        fell = true;
                    }
                }
            }
        }
        return who_fell.size();
    };

    make_em_fall( blocks );

    int sum{};
    for( auto const& b : blocks )
    {
        auto tmp = blocks;
        std::erase_if( tmp, [ & ]( auto const& me ) { return me.x0 == b.x0 && me.y0 == b.y0 && me.z0 == b.z0; } );
        sum += make_em_fall( tmp );
    }
    return sum;
}
