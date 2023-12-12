#include "parse_input.h"
#include <algorithm>
#include <charconv>
#include <ranges>
#include <set>

struct thingy
{
    std::string data;
    std::vector< int > counts;

    thingy( std::string const& in )
    {
        data   = in.substr( 0, in.find( ' ' ) );
        counts = std::views::split( in.substr( data.size() + 1 ), ',' ) |
                 std::views::transform( []( auto const thing ) {
                     std::string_view sv( thing );
                     int result;
                     std::from_chars( sv.data(), sv.data() + sv.size(), result );
                     return result;
                 } ) |
                 std::ranges::to< std::vector >();
    }

    bool valid() const
    {
        std::vector< int > counts_alt( 1 );
        for( auto const c : data )
        {
            if( c == '#' )
                ++counts_alt.back();
            else if( counts_alt.back() > 0 )
                counts_alt.push_back( 0 );
        }
        if( counts_alt.back() == 0 )
            counts_alt.pop_back();

        return std::ranges::equal( counts_alt, counts );
    }

    void print()
    {
        std::cout << data;

        if( !counts.empty() )
        {
            std::cout << " " + std::to_string( counts.front() );

            for( auto const& x : counts | std::views::drop( 1 ) )
                std::cout << "," + std::to_string( x );
        }
        std::cout << '\n';
    }
};

int possible{};

void try_possible( thingy const& p )
{

    auto first_q = p.data.find( '?' );
    if( first_q == std::string::npos )
    {
        if( p.valid() )
            ++possible;
        return;
    }

    auto p_dot            = p;
    p_dot.data[ first_q ] = '.';
    try_possible( p_dot );
    auto p_hash            = p;
    p_hash.data[ first_q ] = '#';
    try_possible( p_hash );
};

advent_t advent( std::vector< std::string > const& input )
{
    int sum{};

    for( auto const& line : input )
    {
        possible = 0;
        thingy t( line );

        try_possible( t );

        sum += possible;
    }

    return sum;
}
