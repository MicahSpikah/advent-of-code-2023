#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>

using namespace std::literals;

using effect  = std::pair< bool, std::vector< std::string > >;
using effects = std::vector< effect >;

struct thing
{
    std::vector< std::string > inputs;
    std::vector< std::string > outputs;

    thing( std::vector< std::string > outs ) :
        outputs( std::move( outs ) )
    {
    }

    virtual effect pulse( bool const in, std::string const& from )
    {
        return { in, outputs };
    }
};

struct flipflop : thing
{
    using thing::thing;
    bool state{};
    effect pulse( bool const in, std::string const& ) override
    {
        if( in )
            return { false, {} };
        state = !state;
        return { state, outputs };
    }
};

struct conjunction : thing
{
    using thing::thing;
    std::map< std::string, bool > last_signal;
    effect pulse( bool const in, std::string const& from ) override
    {
        last_signal[ from ] = in;
        if( std::ranges::all_of( last_signal, []( auto const& kv ) { return kv.second; } ) )
            return { false, outputs };
        return { true, outputs };
    }
};

advent_t advent( std::vector< std::string > const& input )
{
    std::map< std::string, std::unique_ptr< thing > > things;

    for( auto const& line : input )
    {
        char a[ 100 ];
        int n;
        sscanf( line.c_str(), "%s -> %n", a, &n );
        std::string source( a );
        std::vector< std::string > outputs;
        auto const val_sub = line.substr( n );
        for( auto const word : std::views::split( std::string_view( val_sub ), ", "sv ) )
        {
            outputs.push_back( std::string( std::string_view( word ) ) );
        }

        if( source[ 0 ] == '%' )
            things[ source.substr( 1 ) ] = std::make_unique< flipflop >( std::move( outputs ) );
        else if( source[ 0 ] == '&' )
            things[ source.substr( 1 ) ] = std::make_unique< conjunction >( std::move( outputs ) );
        else
            things[ source ] = std::make_unique< thing >( std::move( outputs ) );
    }

    for( auto const& [ k, v ] : things )
        for( auto const& o : v->outputs )
            if( !things.contains( o ) )
                things[ o ] = std::make_unique< thing >( std::vector< std::string >{} );

    for( auto const& [ k, v ] : things )
        for( auto const& o : v->outputs )
            things[ o ]->inputs.push_back( k );

    for( auto const& [ k, v ] : things )
        if( auto const c = dynamic_cast< conjunction* >( v.get() ) )
            for( auto const& i : v->inputs )
                c->last_signal[ i ] = false;

    std::uint64_t highs{};
    std::uint64_t lows{};

    for( int b = 0; b < 1000; ++b )
    {
        std::queue< effect > e;
        std::queue< std::string > effect_source;
        ++lows; // Button push
        e.push( things[ "broadcaster" ]->pulse( false, "button" ) );
        std::cerr << "Button press #" << b << " -low->broadcaster\n";
        effect_source.push( "broadcaster" );
        while( !e.empty() )
        {
            //            if(e.front().first) highs += e.front().second.size();
            //            else lows += e.front().second.size();

            for( auto const& d : e.front().second )
            {
                if( e.front().first )
                    ++highs;
                else
                    ++lows;
                e.push( things[ d ]->pulse( e.front().first, effect_source.front() ) );
                std::cerr << effect_source.front() << ( e.front().first ? " -high"s : " -low"s ) + " -> " << d << '\n';
                effect_source.push( d );
            }

            e.pop();
            effect_source.pop();
        }
    }
    std::cerr << highs << 'x' << lows << '\n';

    return highs * lows;
}
