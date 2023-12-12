#include "parse_input.h"
#include <algorithm>
#include <charconv>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <utility>

struct thingy
{
    std::string data;
    std::vector< int > counts;
    int to_fill;
    int available_to_fill;

    thingy( std::string const& in )
    {
        auto const folded_data   = in.substr( 0, in.find( ' ' ) );
        data                     = folded_data + "?" + folded_data + "?" + folded_data + "?" + folded_data + "?" + folded_data;
        auto const folded_counts = std::views::split( in.substr( folded_data.size() + 1 ), ',' ) |
                                   std::views::transform( []( auto const thing ) {
                                       std::string_view sv( thing );
                                       int result;
                                       std::from_chars( sv.data(), sv.data() + sv.size(), result );
                                       return result;
                                   } ) |
                                   std::ranges::to< std::vector >();
        counts.insert_range( counts.end(), folded_counts );
        counts.insert_range( counts.end(), folded_counts );
        counts.insert_range( counts.end(), folded_counts );
        counts.insert_range( counts.end(), folded_counts );
        counts.insert_range( counts.end(), folded_counts );

        to_fill           = std::reduce( counts.cbegin(), counts.cend() );
        available_to_fill = std::ranges::count( data, '#' ) + std::ranges::count( data, '?' );
        std::ranges::reverse( counts );
    }
};

std::map< std::pair< int, int >, std::uint64_t > cache;

std::uint64_t try_possible( thingy const& p )
{
    std::uint64_t possible{};

    if( p.to_fill > p.available_to_fill )
    {
        return 0;
    }

    if( p.to_fill == 0 )
    {
        if( !p.data.contains( '#' ) )
            ++possible;

        return possible;
    }

    auto const first           = p.data.find_first_of( "?#" );
    auto const remaining_count = p.counts.size();

    if( auto const found = cache.find( { first, remaining_count } ); found != cache.cend() )
    {
        return found->second;
    }

    if( p.data[ first ] == '?' )
    {
        auto p_dot          = p;
        p_dot.data[ first ] = '.';
        --p_dot.available_to_fill;
        possible += try_possible( p_dot );
    }

    auto p_X                  = p;
    auto const attempted_size = p.counts.back();
    p_X.data[ first ]         = '.';
    for( int i = 1; i < attempted_size; ++i )
    {
        if( p.data[ first + i ] == '.' )
        {
            cache[ { first, remaining_count } ] = possible;
            return possible;
        }

        p_X.data[ first + i ] = '.';
    }
    p_X.available_to_fill -= attempted_size;
    if( first + attempted_size < p.data.size() )
    {
        if( p.data[ first + attempted_size ] == '#' )
        {
            cache[ { first, remaining_count } ] = possible;
            return possible;
        }
        if( p.data[ first + attempted_size ] == '?' )
        {
            p_X.data[ first + attempted_size ] = '.';
            --p_X.available_to_fill;
        }
    }
    p_X.to_fill -= attempted_size;
    p_X.counts.pop_back();
    possible += try_possible( p_X );

    cache[ { first, remaining_count } ] = possible;
    return possible;
};

advent_t advent( std::vector< std::string > const& input )
{
    std::uint64_t sum{};

    for( auto const& line : input )
    {
        cache.clear();
        thingy t( line );

        auto const possible = try_possible( t );

        sum += possible;
    }

    return sum;
}
