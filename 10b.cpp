#include "parse_input.h"

struct done
{
};

advent_t advent( std::vector< std::string > const& input )
{
    std::vector< std::pair< std::size_t, std::size_t > > path;

    std::size_t x;
    std::size_t y;
    std::size_t px;
    std::size_t py;

    auto const next_is = [ & ]( std::size_t const nx, std::size_t const ny ) {
        if( ( !path.empty() ) && nx == path.front().first && ny == path.front().second )
            throw done{};
        path.push_back( { nx, ny } );
        px = x;
        py = y;
        x  = nx;
        y  = ny;
    };

    auto const grow_first_step = [ & ] {
        if( y > 0 && ( input[ y - 1 ][ x ] == '|' || input[ y - 1 ][ x ] == '7' || input[ y - 1 ][ x ] == 'F' ) )
            next_is( x, y - 1 );
        else if( y < input.size() - 1 && ( input[ y + 1 ][ x ] == '|' || input[ y + 1 ][ x ] == 'J' || input[ y + 1 ][ x ] == 'L' ) )
            next_is( x, y + 1 );
        else
            next_is( x - 1, y );
    };

    auto const grow = [ & ] {
        switch( input[ y ][ x ] )
        {
        case '|':
            if( py < y )
                next_is( x, y + 1 );
            else
                next_is( x, y - 1 );
            break;
        case '-':
            if( px < x )
                next_is( x + 1, y );
            else
                next_is( x - 1, y );
            break;
        case 'L':
            if( py < y )
                next_is( x + 1, y );
            else
                next_is( x, y - 1 );
            break;
        case 'J':
            if( py < y )
                next_is( x - 1, y );
            else
                next_is( x, y - 1 );
            break;
        case '7':
            if( py > y )
                next_is( x - 1, y );
            else
                next_is( x, y + 1 );
            break;
        case 'F':
            if( py > y )
                next_is( x + 1, y );
            else
                next_is( x, y + 1 );
            break;
        default:
            throw std::runtime_error( "How" );
        }
    };

    try
    {
        for( std::size_t row = 0; row < input.size(); ++row )
            for( std::size_t col = 0; col < input[ row ].size(); ++col )
                if( input[ row ][ col ] == 'S' )
                {
                    next_is( col, row );
                    grow_first_step();
                    while( true )
                        grow();
                }
    }
    catch( done )
    {
    }

    std::vector< std::string > clean( input.size(), std::string( input.front().size(), '.' ) );

    for( auto const p : path )
    {
        clean[ p.second ][ p.first ] = input[ p.second ][ p.first ];
    }

    // Clean up the 'S' as well
    {
        auto const ax = path[ 1 ].first;
        auto const ay = path[ 1 ].second;
        auto const bx = path.back().first;
        auto const by = path.back().second;
        auto const sx = path.front().first;
        auto const sy = path.front().second;
        if( ax == bx )
            clean[ sy ][ sx ] = '-';
        else if( ay == by )
            clean[ sy ][ sx ] = '|';
        else if( ( ay < sy && ax == sx && bx > sx && by == sy ) || ( by < sy && bx == sx && ax > sx && ay == sy ) )
            clean[ sy ][ sx ] = 'L';
        else if( ( ay < sy && ax == sx && bx < sx && by == sy ) || ( by < sy && bx == sx && ax < sx && ay == sy ) )
            clean[ sy ][ sx ] = 'J';
        else if( ( ay > sy && ax == sx && bx > sx && by == sy ) || ( by > sy && bx == sx && ax > sx && ay == sy ) )
            clean[ sy ][ sx ] = 'F';
        else
            clean[ sy ][ sx ] = '7';
    }

    std::size_t inside{};

    // Check each point
    for( std::size_t sy = 0; sy < input.size(); ++sy )
    {
        auto const& clean_line = clean[ sy ];
        for( std::size_t sx = 0; sx < clean_line.size(); ++sx )
            if( clean_line[ sx ] == '.' )
            {
                std::size_t crossings{};
                bool above{};
                for( std::size_t x = 0; x < sx; ++x )
                {
                    if( clean_line[ x ] == '|' )
                        ++crossings;
                    else if( clean_line[ x ] == 'L' )
                    {
                        above = false;
                    }
                    else if( clean_line[ x ] == 'J' )
                    {
                        if( above )
                            ++crossings;
                    }
                    else if( clean_line[ x ] == 'F' )
                    {
                        above = true;
                    }
                    else if( clean_line[ x ] == '7' )
                    {
                        if( !above )
                            ++crossings;
                    }
                }
                if( crossings % 2 )
                {
                    ++inside;
                    clean[ sy ][ sx ] = 'I';
                }
                else
                    clean[ sy ][ sx ] = 'O';
            }
    }

    return inside;
}
