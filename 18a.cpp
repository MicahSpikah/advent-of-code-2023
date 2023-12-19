#include "parse_input.h"
#include <algorithm>
#include <numeric>
#include <ranges>

advent_t advent( std::vector< std::string > const& input )
{
    std::vector< std::string > g( 10000, std::string( 10000, '.' ) );
    int row         = g.size() / 2;
    int col         = g.front().size() / 2;
    auto min_row    = row;
    auto max_row    = row;
    auto min_col    = col;
    auto max_col    = col;
    g[ row ][ col ] = '#';
    for( auto const& line : input )
    {
        char dir;
        int d_row{}, d_col{};
        int len;
        sscanf( line.c_str(), "%c %d", &dir, &len );
        switch( dir )
        {
        case 'U':
            d_row = -1;
            break;
        case 'D':
            d_row = 1;
            break;
        case 'L':
            d_col = -1;
            break;
        case 'R':
            d_col = 1;
            break;
        }
        for( int c = 0; c < len; ++c )
        {
            row += d_row;
            col += d_col;

            if( row < 0 || row >= g.size() || col < 0 || col >= g.front().size() )
                throw std::runtime_error( "Pick better limits Micah" );
            g[ row ][ col ] = '#';
            min_row         = std::min( min_row, row );
            max_row         = std::max( max_row, row );
            min_col         = std::min( min_col, col );
            max_col         = std::max( max_col, col );
        }
    }

    std::cout << min_row << '-' << max_row << " x " << min_col << '-' << max_col << '\n';

    /*
    for(int row = min_row; row < max_row; ++row)
    {
        bool in{};
        for(int col = min_row - 1; col <= max_col + 1; ++col)
        {
            if(!in)
            {
                if(g[row+1][col] == '#')
                {
                    if(g[row+1][col+1] == '.' && g[row][col+1] == '#')
                        in = true;
                }
            }
            else
            {
                if(g[row+1][col] == '.')
                {
                    g[row+1][col] = '#';
                }
                else if(g[row+1][col+1] == '.') in =false;
            }

        }
    }
    */
    std::uint64_t area{};
    auto dbg = g;

    for( int row = min_row; row <= max_row; ++row )
    {
        bool in{};
        bool above{};
        bool along{};
        for( int col = min_col - 1; col <= max_col + 1; ++col )
        {
            if( in )
            {
                if( g[ row ][ col ] == '#' )
                {
                    ++area;
                    dbg[ row ][ col ] = 'X';
                    if( !along )
                    {
                        if( g[ row ][ col + 1 ] == '.' )
                            in = false;
                        else
                        {
                            along = true;
                            if( g[ row + 1 ][ col ] == '#' )
                                above = true;
                            else
                                above = false;
                        }
                    }
                    if( along && g[ row ][ col + 1 ] == '.' )
                    {
                        along = false;
                        if( above && g[ row - 1 ][ col ] == '#' )
                            in = false;
                        else if( ( !above ) && g[ row + 1 ][ col ] == '#' )
                            in = false;
                    }
                }
                else
                {
                    ++area;
                    dbg[ row ][ col ] = 'X';
                }
            }
            else
            {
                if( g[ row ][ col ] == '#' )
                {
                    ++area;
                    dbg[ row ][ col ] = 'X';
                    if( !along )
                    {
                        if( g[ row ][ col + 1 ] == '.' )
                            in = true;
                        else
                        {
                            along = true;
                            if( g[ row + 1 ][ col ] == '#' )
                                above = true;
                            else
                                above = false;
                        }
                    }
                    if( along && g[ row ][ col + 1 ] == '.' )
                    {
                        along = false;
                        if( above && g[ row - 1 ][ col ] == '#' )
                            in = true;
                        else if( ( !above ) && g[ row + 1 ][ col ] == '#' )
                            in = true;
                    }
                }
            }
        }
    }
    {
        for( int row = min_row - 1; row <= max_row + 1; ++row )
        {
            for( int col = min_col - 1; col <= max_col + 1; ++col )
            {
                std::cout << dbg[ row ][ col ];
            }
            std::cout << '\n';
        }
    }
    return area;
}
