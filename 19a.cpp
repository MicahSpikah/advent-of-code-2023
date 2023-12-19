#include "parse_input.h"
#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>
struct part
{
    int x, m, a, s;
};

std::vector< part > parts{
#include "parts.h"
    // Note: parts.h was a file I generated using a vim macro which made the input valid c++ by adding 4 dots and a trailing comma
    // e.g. {x=787,m=2655,a=1222,s=2876} became {.x=787,.m=2655,.a=1222,.s=2876},
};

struct rule
{
    char c;
    int op;
    int val;
    std::string go;
};

struct rules
{
    std::vector< rule > r;
    std::string e;
};

std::map< std::string, rules > m;

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

    std::uint64_t sum{};

    for( auto const& p : parts )
    {
        std::string where = "in";
        while( true )
        {
            bool hit{};
            for( int t = 0; t < m[ where ].r.size(); ++t )
            {
                auto const& care = [ & ] {
                    switch( m[ where ].r[ t ].c )
                    {
                    case 'x':
                        return p.x;
                    case 'm':
                        return p.m;
                    case 'a':
                        return p.a;
                    case 's':
                        return p.s;
                    }
                    throw std::runtime_error( "Micah1" );
                }();
                if( care * m[ where ].r[ t ].op > m[ where ].r[ t ].val * m[ where ].r[ t ].op )
                {
                    where = m[ where ].r[ t ].go;
                    hit   = true;
                    break;
                }
            }
            if( !hit )
                where = m[ where ].e;
            if( where == "A" )
            {
                sum += p.x + p.m + p.a + p.s;
                break;
            }
            if( where == "R" )
            {
                break;
            }
        }
    }

    return sum;
}
