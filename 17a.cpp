#include "parse_input.h"
#include <array>
#include <limits>
#include <map>
#include <ranges>

enum dir
{
    up,
    down,
    left,
    right
};

constexpr std::size_t max_count = 3;

using tile =
    std::map< dir, std::array< std::uint64_t, max_count > >;

constexpr std::array< std::uint64_t, max_count > upper_limit = [] {
    std::array< std::uint64_t, max_count > x;
    x.fill(std::numeric_limits<std::uint64_t>::max());
    return x; }();

struct problem
{
    problem( std::vector< std::string > const& i ) :
        input( i ),
        tiles( input.size(), std::vector< tile >( input.front().size(), tile{ { up, upper_limit }, { down, upper_limit }, { left, upper_limit }, { right, upper_limit } } ) )
    {
        for( auto d : { up, down, left, right } )
            for( auto i : std::views::iota( 0, static_cast< int >( max_count ) ) )
                tiles[ 0 ][ 0 ][ d ][ i ] = 0;

        grow( 0, 1, right, 0, 0 );
        grow( 1, 0, down, 0, 0 );
    }

    void grow( int const row, int const col, dir const v, int const count, std::uint64_t heat_loss )
    {
        if( count >= max_count || row < 0 || col < 0 || row >= input.size() || col >= input.front().size() )
            return;

        bool improvement{};
        heat_loss += static_cast< std::uint64_t >( input[ row ][ col ] - '0' );
        for( auto c = count; c < max_count; ++c )
        {
            if( heat_loss < tiles[ row ][ col ][ v ][ c ] )
            {
                tiles[ row ][ col ][ v ][ c ] = heat_loss;
                improvement                   = true;
            }
        }
        if( !improvement )
            return;

        grow( row + 1, col, down, v == down ? count + 1 : 0, heat_loss );
        grow( row, col - 1, left, v == left ? count + 1 : 0, heat_loss );
        grow( row - 1, col, up, v == up ? count + 1 : 0, heat_loss );
        grow( row, col + 1, right, v == right ? count + 1 : 0, heat_loss );
    }

    std::vector< std::string > const& input;
    std::vector< std::vector< tile > > tiles;
};

advent_t advent( std::vector< std::string > const& input )
{
    problem p( input );

    return std::max(
        p.tiles[ input.size() - 1 ][ input.front().size() - 1 ][ down ][ 2 ],
        p.tiles[ input.size() - 1 ][ input.front().size() - 1 ][ right ][ 2 ] );
}
