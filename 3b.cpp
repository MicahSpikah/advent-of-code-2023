#include "parse_input.h"
#include <algorithm>

advent_t advent( std::vector< std::string > const& input )
{
    auto const num_at = [ & ]( int row, int col ) {
        if( row < 0 || col < 0 || row >= input.size() || col >= input[ row ].size() || input[ row ][ col ] < '0' || input[ row ][ col ] > '9' )
        {
            return std::pair< int, int >{ 0, col + 1 };
        }
        while( col > 0 && input[ row ][ col - 1 ] >= '0' && input[ row ][ col - 1 ] <= '9' )
            --col;

        std::size_t size{};

        int num = std::stoi( input[ row ].substr( col ), &size );
        return std::pair< int, int >{ num, col + size };
    };
    int sum{};

    for( int i = 0; i < input.size(); ++i )
    {
        for( auto gear = input[ i ].find( '*' ); gear != std::string::npos; gear = input[ i ].find( '*', gear + 1 ) )
        {
            std::vector< int > found;
            for( auto row = i - 1; row < i + 2; ++row )
                for( auto col = gear - 1; col < gear + 2; ++col )
                {
                    auto const num = num_at( row, col );
                    if( num.first > 0 )
                    {
                        found.push_back( num.first );
                        col = num.second;
                    }
                }

            if( found.size() == 2 )
            {
                std::cerr << "Gear at " << i << " " << gear << " connects " << found[ 0 ] << " " << found[ 1 ] << '\n';
                sum += found[ 0 ] * found[ 1 ];
            }
        }
    }

    return sum;
}
