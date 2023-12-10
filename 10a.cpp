#include "parse_input.h"

struct done
{
};

advent_t advent( std::vector< std::string > const& input )
{
    // std::vector< std::vector< int > > steps(input.size(), std::vector< int >(input.front().size(), -1));
    std::vector< std::pair< std::size_t, std::size_t > > path;
    // Find S
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

    return path.size() / 2;
}
