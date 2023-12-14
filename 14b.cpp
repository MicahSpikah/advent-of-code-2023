#include "parse_input.h"
#include <algorithm>

advent_t advent( std::vector< std::string > const& input )
{
    auto rolled = input;
    std::vector< std::vector< std::string > > history;

    auto const north = [ & ] {
        for( int i = 0; i < rolled.size(); ++i )
        {
            for( int j = i; j > 0; --j )
            {
                for( int c = 0; c < rolled.front().size(); ++c )
                {
                    if( rolled[ j ][ c ] == 'O' && rolled[ j - 1 ][ c ] == '.' )
                    {
                        rolled[ j ][ c ]     = '.';
                        rolled[ j - 1 ][ c ] = 'O';
                    }
                }
            }
        }
    };
    auto const west = [ & ] {
        for( auto& line : rolled )
        {
            for( int x = 0; x < line.size(); ++x )
            {
                for( int c = x; c > 0; --c )
                {
                    if( line[ c ] == 'O' && line[ c - 1 ] == '.' )
                    {
                        line[ c ]     = '.';
                        line[ c - 1 ] = 'O';
                    }
                }
            }
        }
    };
    auto const south = [ & ] {
        for( int y = rolled.size() - 1; y >= 0; --y )
        {
            for( int r = y; r + 1 < rolled.size(); ++r )
            {
                for( int c = 0; c < rolled.front().size(); ++c )
                {
                    if( rolled[ r ][ c ] == 'O' && rolled[ r + 1 ][ c ] == '.' )
                    {
                        rolled[ r ][ c ]     = '.';
                        rolled[ r + 1 ][ c ] = 'O';
                    }
                }
            }
        }
    };
    auto const east = [ & ] {
        for( auto& line : rolled )
        {
            for( int x = line.size() - 1; x >= 0; --x )
            {
                for( int c = x; c + 1 < line.size(); ++c )
                {
                    if( line[ c ] == 'O' && line[ c + 1 ] == '.' )
                    {
                        line[ c ]     = '.';
                        line[ c + 1 ] = 'O';
                    }
                }
            }
        }
    };

    auto const cycle = [ & ] {
        history.push_back( rolled );
        north();
        west();
        south();
        east();
    };

    for( int i = 0; i < 1000000000; ++i )
    {
        cycle();
        for( int h = 0; h < history.size(); ++h )
        {
            if( std::ranges::equal( history[ h ], rolled ) )
            {
                auto const period = history.size() - h;
                auto const mod    = 1000000000 % period;
                auto& matching    = history[ ( history.size() / period ) * period + mod ];
                std::uint64_t sum{};
                for( int i = 0; i < input.size(); ++i )
                {
                    sum += ( input.size() - i ) * std::ranges::count( matching[ i ], 'O' );
                }
                return sum;
            }
        }
    }

    // There's no way this will ever be hit. But for completionists sake...
    std::uint64_t sum{};
    for( int i = 0; i < input.size(); ++i )
    {
        sum += ( input.size() - i ) * std::ranges::count( rolled[ i ], 'O' );
    }

    return sum;
}
