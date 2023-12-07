#include "parse_input.h"
#include <algorithm>
#include <array>
#include <charconv>
#include <map>
#include <ranges>

struct Hand
{
    std::array< int, 5 > hand_vals;
    int bid;

    int strength{};

    Hand( std::string const& from )
    {
        std::map< char, int > card_count;
        auto const contains_val = [ & ]( auto const val ) {
            for( auto const& [ k, v ] : card_count )
                if( v == val )
                    return true;
            return false;
        };

        auto const hand = from.substr( 0, 5 );
        std::ranges::transform( hand, hand_vals.begin(), []( auto const card ) {
            std::map< char, int > t;
            t[ '2' ] = 2;
            t[ '3' ] = 3;
            t[ '4' ] = 4;
            t[ '5' ] = 5;
            t[ '6' ] = 6;
            t[ '7' ] = 7;
            t[ '8' ] = 8;
            t[ '9' ] = 9;
            t[ 'T' ] = 10;
            t[ 'J' ] = 11;
            t[ 'Q' ] = 12;
            t[ 'K' ] = 13;
            t[ 'A' ] = 14;
            return t[ card ];
        } );
        std::from_chars( from.data() + 6, from.data() + from.size(), bid );

        for( auto const card : hand )
            ++card_count[ card ];
        if( contains_val( 5 ) )
            strength = 6;
        else if( contains_val( 4 ) )
            strength = 5;
        else if( contains_val( 3 ) && card_count.size() == 2 )
            strength = 4;
        else if( contains_val( 3 ) )
            strength = 3;
        else if( contains_val( 2 ) && card_count.size() == 3 )
            strength = 2;
        else if( contains_val( 2 ) )
            strength = 1;
    }
};

bool hand_less( Hand const& a, Hand const& b )
{
    if( a.strength < b.strength )
        return true;
    if( a.strength > b.strength )
        return false;
    for( std::size_t pos = 0; pos < 5; ++pos )
        if( a.hand_vals[ pos ] < b.hand_vals[ pos ] )
            return true;
        else if( a.hand_vals[ pos ] > b.hand_vals[ pos ] )
            return false;
    return false;
}

advent_t advent( std::vector< std::string > const& input )
{
    std::vector< Hand > hands;
    std::ranges::transform( input, std::back_inserter( hands ), []( auto const& in ) { return Hand( in ); } );
    std::ranges::sort( hands, &hand_less );
    int sum{};

    for( int rank = 1; rank <= hands.size(); ++rank )
    {
        sum += hands[ rank - 1 ].bid * rank;
    }

    return sum;
}
