#include "parse_input.h"
#include <algorithm>
#include <array>
#include <numeric>

enum dir
{
    up,
    down,
    left,
    right
};

int nx( int x, dir moving )
{
    switch( moving )
    {
    case up:
    case down:
        return x;
    case left:
        return x - 1;
    case right:
        return x + 1;
    }
}

int ny( int y, dir moving )
{
    switch( moving )
    {
    case up:
        return y - 1;
    case down:
        return y + 1;
    case left:
    case right:
        return y;
    }
}

struct from_dir
{
    std::array< bool, 4 > dir{};
};

std::size_t count_energized( std::vector< from_dir > const& row )
{
    return std::ranges::count_if( row, []( from_dir const& tile ) { return tile.dir[ up ] || tile.dir[ down ] || tile.dir[ left ] || tile.dir[ right ]; } );
}

struct beams
{
    beams( std::vector< std::string > const& the_input, int x, int y, dir moving ) :
        input( the_input ),
        rays( input.size(), std::vector< from_dir >( input.front().size() ) )
    {
        go( x, y, moving );
    }

    std::vector< std::string > const& input;
    std::vector< std::vector< from_dir > > rays;

    void go( int x, int y, dir moving )
    {
        int x1 = nx( x, moving );
        int y1 = ny( y, moving );

        if( x1 < 0 || x1 >= rays.front().size() || y1 < 0 || y1 >= rays.size() || rays[ y1 ][ x1 ].dir[ moving ] )
        {
            return;
        }

        rays[ y1 ][ x1 ].dir[ moving ] = true;

        auto const tile = input[ y1 ][ x1 ];

        switch( tile )
        {
        case '.':
            return go( x1, y1, moving );
        case '|':
            if( moving == left || moving == right )
            {
                go( x1, y1, up );
                go( x1, y1, down );
                return;
            }
            return go( x1, y1, moving );
        case '-':
            if( moving == left || moving == right )
                return go( x1, y1, moving );
            else
            {
                go( x1, y1, left );
                go( x1, y1, right );
                return;
            }
        case '/':
            switch( moving )
            {
            case up:
                return go( x1, y1, right );
            case down:
                return go( x1, y1, left );
            case left:
                return go( x1, y1, down );
            case right:
                return go( x1, y1, up );
            }
        case '\\':
            switch( moving )
            {
            case up:
                return go( x1, y1, left );
            case down:
                return go( x1, y1, right );
            case left:
                return go( x1, y1, up );
            case right:
                return go( x1, y1, down );
            }
        }
    }

    std::size_t count()
    {
        return std::transform_reduce( rays.cbegin() + 1, rays.cend(), count_energized( rays.front() ), std::plus<>(), count_energized );
    }
};

advent_t advent( std::vector< std::string > const& input )
{
    std::size_t max{};

    for( int x = 0; x < input.front().size(); ++x )
    {
        max = std::max( max, beams( input, x, -1, down ).count() );
        max = std::max( max, beams( input, x, input.size(), up ).count() );
    }
    for( int y = 0; y < input.size(); ++y )
    {
        max = std::max( max, beams( input, -1, y, right ).count() );
        max = std::max( max, beams( input, input.front().size(), y, left ).count() );
    }

    return max;
}
