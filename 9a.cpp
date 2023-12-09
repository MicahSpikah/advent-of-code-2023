#include "parse_input.h"
#include <algorithm>
#include <charconv>
#include <numeric>
#include <ranges>
#include <string_view>

advent_t advent( std::vector< std::string > const& input )
{
    std::int64_t sum{};
    for( auto const& line : input )
    {
        std::vector< std::vector< int > > vecs;
        vecs.push_back( std::views::split( line, ' ' ) |
                        std::views::transform( []( auto const thing ) {
                            std::string_view sv( thing );
                            int result;
                            std::from_chars( sv.data(), sv.data() + sv.size(), result );
                            return result;
                        } ) |
                        std::ranges::to< std::vector >() );
        while( !std::equal( vecs.back().cbegin(), vecs.back().cend() - 1, vecs.back().cbegin() + 1 ) )
        {
            std::vector< int > diffs;
            std::adjacent_difference( vecs.back().cbegin(), vecs.back().cend(), std::back_inserter( diffs ) );
            vecs.push_back( std::vector< int >( diffs.cbegin() + 1, diffs.cend() ) );
        }
        while( vecs.size() > 1 )
        {
            auto const to_add = vecs.back().back();
            vecs.pop_back();
            vecs.back().push_back( vecs.back().back() + to_add );
        }
        sum += vecs.back().back();
    }
    return sum;
}
