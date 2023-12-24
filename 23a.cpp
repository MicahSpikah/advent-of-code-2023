#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

int count( std::vector< std::string > const& input )
{
    int sum{};

    for( auto const& line : input )
        sum += std::ranges::count( line, 'O' );

    return sum;
}

int best_route( std::vector< std::string > const& input, int row, int col )
{
    auto tmp          = input;
    tmp[ row ][ col ] = 'O';

    std::vector< std::pair< int, int > > options;

    while( row + 1 < input.size() )
    {
        options.clear();
        if( tmp[ row - 1 ][ col ] == '^' || tmp[ row - 1 ][ col ] == '.' )
            options.push_back( { row - 1, col } );
        if( tmp[ row + 1 ][ col ] == 'v' || tmp[ row + 1 ][ col ] == '.' )
            options.push_back( { row + 1, col } );
        if( tmp[ row ][ col - 1 ] == '<' || tmp[ row ][ col - 1 ] == '.' )
            options.push_back( { row, col - 1 } );
        if( tmp[ row ][ col + 1 ] == '>' || tmp[ row ][ col + 1 ] == '.' )
            options.push_back( { row, col + 1 } );

        if( options.empty() )
            return 0;
        if( options.size() == 1 )
        {
            row               = options[ 0 ].first;
            col               = options[ 0 ].second;
            tmp[ row ][ col ] = 'O';
        }
        else
        {
            int best{};
            for( auto const& o : options )
            {
                best = std::max( best, best_route( tmp, o.first, o.second ) );
            }
            return best;
        }
    }

    return count( tmp );
}

advent_t advent( std::vector< std::string > const& input )
{
    auto take_first_step = input;
    for( int col = 0; col < input.front().size(); ++col )
    {
        if( input.front()[ col ] == '.' )
        {
            take_first_step.front()[ col ] = 'O';
            return best_route( input, 1, 1 );
        }
    }

    throw std::runtime_error( "Bad input" );
}
