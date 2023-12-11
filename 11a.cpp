#include "parse_input.h"
#include <ranges>
#include <set>

advent_t advent( std::vector< std::string > const& input )
{

    std::set< int > empty_rows( std::from_range, std::views::iota( 0, static_cast< int >( input.size() ) ) );
    std::set< int > empty_cols( std::from_range, std::views::iota( 0, static_cast< int >( input.front().size() ) ) );
    std::vector< std::pair< std::size_t, std::size_t > > galaxies;
    for( std::size_t row = 0; row < input.size(); ++row )
    {
        auto const& line = input.at( row );
        for( std::size_t col = 0; col < input.front().size(); ++col )
        {
            if( line[ col ] == '#' )
            {
                galaxies.push_back( { col, row } );
                empty_rows.erase( row );
                empty_cols.erase( col );
            }
        }
    }

    std::size_t sum{};

    for( std::size_t g0 = 0; g0 < galaxies.size(); ++g0 )
    {
        for( std::size_t g1 = g0 + 1; g1 < galaxies.size(); ++g1 )
        {
            auto const x0 = std::min( galaxies[ g0 ].first, galaxies[ g1 ].first );
            auto const x1 = std::max( galaxies[ g0 ].first, galaxies[ g1 ].first );
            auto const y0 = std::min( galaxies[ g0 ].second, galaxies[ g1 ].second );
            auto const y1 = std::max( galaxies[ g0 ].second, galaxies[ g1 ].second );

            std::uint64_t expanded_space{};
            for( int x = x0 + 1; x < x1; ++x )
                if( empty_cols.contains( x ) )
                    ++expanded_space;
            for( int y = y0 + 1; y < y1; ++y )
                if( empty_rows.contains( y ) )
                    ++expanded_space;

            sum += ( x1 - x0 ) + ( y1 - y0 ) + expanded_space;
        }
    }

    return sum;
}
