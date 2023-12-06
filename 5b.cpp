#include "parse_input.h"
#include <charconv>
#include <ranges>

struct thingy
{
    std::uint32_t dest;
    std::uint32_t src;
    std::uint32_t size;

    thingy( std::string const& in )
    {
        auto const split = std::views::split( in, ' ' ) |
                           std::views::transform( []( auto const thing ) {
                               std::string_view sv( thing );
                               std::uint32_t result;
                               std::from_chars( sv.data(), sv.data() + sv.size(), result );
                               return result;
                           } ) |
                           std::ranges::to< std::vector >();
        dest = split[ 0 ];
        src  = split[ 1 ];
        size = split[ 2 ];
    }
};

std::uint32_t X( std::vector< thingy > const& things, std::uint32_t in )
{
    auto const where = std::ranges::find_if( things, [ & ]( auto const& thing ) {
        return thing.src <= in && thing.src + thing.size > in;
    } );

    if( where == things.end() )
        return in;
    return where->dest + in - where->src;
}

advent_t advent( std::vector< std::string > const& input )
{
    auto const seeds = std::views::split( input.front(), ' ' ) |
                       std::views::drop( 1 ) |
                       std::views::transform( []( auto const thing ) {
                           std::string_view sv( thing );
                           std::uint32_t result;
                           std::from_chars( sv.data(), sv.data() + sv.size(), result );
                           return result;
                       } ) |
                       std::ranges::to< std::vector >();

    int next_line        = 3;
    auto const parse_map = [ & ] {
        std::vector< thingy > things;

        while( !input[ next_line ].empty() )
            things.push_back( thingy( input[ next_line++ ] ) );
        ++next_line;
        return things;
    };

    auto const seed_to_soil            = parse_map();
    auto const soil_to_fertilizer      = parse_map();
    auto const fertilizer_to_water     = parse_map();
    auto const water_to_light          = parse_map();
    auto const light_to_temperature    = parse_map();
    auto const temperature_to_humidity = parse_map();
    auto const humidity_to_location    = parse_map();

    auto const begin_to_end = [ & ]( std::uint32_t const seed ) {
        return X( humidity_to_location, X( temperature_to_humidity, X( light_to_temperature, X( water_to_light, X( fertilizer_to_water, X( soil_to_fertilizer, X( seed_to_soil, seed ) ) ) ) ) ) );
    };

    std::uint32_t min = std::numeric_limits< std::uint32_t >::max();

    for( int rrr = 0; rrr + 1 < seeds.size(); rrr += 2 )
    {
        for( int b = 0; b < seeds[ rrr + 1 ]; ++b )
        {
            min = std::min( begin_to_end( seeds[ rrr ] + b ), min );
        }
    }

    return min;
}
