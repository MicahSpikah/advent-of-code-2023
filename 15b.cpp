#include "parse_input.h"
#include <algorithm>
#include <list>
#include <map>
#include <print>

advent_t advent( std::vector< std::string > const& input )
{
    std::map< std::uint8_t, std::list< std::pair< std::string, int > > > m;

    std::uint8_t reg{};
    std::uint64_t sum{};
    bool chomp{};
    std::string label;

    auto const print_state = [ & ] {
        for( auto const& [ k, v ] : m )
        {
            if( v.empty() )
                continue;
            std::print( "Box {}:", k );
            for( auto const& lens : v )
            {
                std::print( " [{} {}]", lens.first, lens.second );
            }
            std::print( "\n" );
        }
        std::print( "\n" );
    };

    for( auto const c : input.front() )
    {
        if( chomp )
        {
            auto found = std::ranges::find_if(
                m[ reg ], [ & ]( auto const& item ) { return item.first == label; } );
            if( found != m[ reg ].end() )
            {
                found->second = c - '0';
            }
            else
                m[ reg ].push_back( { label, c - '0' } );

            // std::print("After \"{}={}\":\n",label,c);
            // print_state();

            chomp = false;
        }
        else if( c == ',' )
        {
            reg = 0;
            label.clear();
        }
        else if( c == '=' )
        {
            chomp = true;
        }
        else if( c == '-' )
        {
            m[ reg ].remove_if( [ & ]( auto const& item ) { return item.first == label; } );
            // std::print("After \"{}-\":\n",label);
            // print_state();
        }
        else
        {
            label += c;
            reg += static_cast< std::uint8_t >( c );
            reg *= 17;
        }
    }

    for( auto const& [ k, v ] : m )
    {
        int box_multiplier = static_cast< int >( k ) + 1;
        int lens_multiplier{};
        for( auto const& lens : v )
        {
            sum += box_multiplier * ++lens_multiplier * lens.second;
        }
    }

    return sum;
}
