#include "parse_input.h"
#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

enum seg_id
{
    x_seg,
    m_seg,
    a_seg,
    s_seg
};

struct segment
{
    int min{};
    int max{};
};

using chunk =
    std::array< segment, 4 >;

struct rule
{
    char c;
    int op;
    int val;
    std::string go;
};

std::map< char, seg_id > char_to_id{
    { 'x', x_seg },
    { 'm', m_seg },
    { 'a', a_seg },
    { 's', s_seg },
};

struct rules
{
    std::vector< rule > r;
    std::string e;
};

std::map< std::string, rules > m;

std::uint64_t sum{};

void handle_seg( chunk c, std::string const& key )
{
    if( key == "R" )
        return;
    if( key == "A" )
    {
        sum +=
            static_cast< std::uint64_t >( 1 + c[ x_seg ].max - c[ x_seg ].min ) *
            static_cast< std::uint64_t >( 1 + c[ m_seg ].max - c[ m_seg ].min ) *
            static_cast< std::uint64_t >( 1 + c[ a_seg ].max - c[ a_seg ].min ) *
            static_cast< std::uint64_t >( 1 + c[ s_seg ].max - c[ s_seg ].min );
        return;
    }
    for( auto const& r : m[ key ].r )
    {
        auto const id = char_to_id.at( r.c );
        if( ( r.op == 1 && c[ id ].min > r.val ) || ( r.op == -1 && c[ id ].max < r.val ) )
        {
            // All input goes to this rule
            handle_seg( c, r.go );
            return;
        }
        else if( ( r.op == 1 && c[ id ].max <= r.val ) || ( r.op == -1 && c[ id ].min >= r.val ) )
        {
            // No input goes to this rule
            continue;
        }
        else
        {
            auto split = c;
            // Split the seg
            if( r.op == 1 )
            {
                split[ id ].min = r.val + 1;
                c[ id ].max     = r.val;
                handle_seg( split, r.go );
            }
            else
            {
                split[ id ].max = r.val - 1;
                c[ id ].min     = r.val;
                handle_seg( split, r.go );
            }
        }
    }
    handle_seg( c, m[ key ].e );
}

advent_t advent( std::vector< std::string > const& input )
{
    for( auto const& line : input )
    {
        auto k = line.substr( 0, line.find( '{' ) );
        auto v = line.substr( line.find( '{' ) + 1 );
        v.pop_back();
        rules r;
        for( int f = v.find( ',' ); f != std::string::npos; f = v.find( ',' ) )
        {
            char go_buf[ 10 ];
            rule the_rule;
            char op_c;
            sscanf( v.substr( 0, f ).c_str(), "%c%c%d:%s", &the_rule.c, &op_c, &the_rule.val, go_buf );
            the_rule.go = std::string( go_buf );
            if( op_c == '>' )
                the_rule.op = 1;
            else
                the_rule.op = -1;
            r.r.push_back( the_rule );
            v = v.substr( f + 1 );
        }
        r.e    = v;
        m[ k ] = r;
    }

    handle_seg( chunk{ { { 1, 4000 }, { 1, 4000 }, { 1, 4000 }, { 1, 4000 } } }, "in" );

    return sum;
}
