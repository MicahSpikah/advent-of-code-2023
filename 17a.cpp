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
        /* 1000 is an upper bound. If the program runs with no path hitting the bottom, raise this limit */
        if( heat_loss > 1000 || count >= max_count || row < 0 || col < 0 || row >= input.size() || col >= input.front().size() )
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

        if( v != up )
            grow( row + 1, col, down, ( v == down ) ? ( count + 1 ) : 0, heat_loss );
        if( v != right )
            grow( row, col - 1, left, ( v == left ) ? ( count + 1 ) : 0, heat_loss );
        if( v != down )
            grow( row - 1, col, up, ( v == up ) ? ( count + 1 ) : 0, heat_loss );
        if( v != left )
            grow( row, col + 1, right, ( v == right ) ? ( count + 1 ) : 0, heat_loss );
    }

    void report( int row, int col )
    {
        if( row == 0 && col == 0 )
            return;
        auto min_heat_loss = std::numeric_limits< std::uint64_t >::max();
        for( auto const v : { up, down, left, right } )
            for( auto const c : tiles[ row ][ col ][ v ] )
                min_heat_loss = std::min( min_heat_loss, c );
        std::cerr << "Best at " << row << ", " << col << " is " << min_heat_loss;
        if( tiles[ row ][ col ][ up ].back() == min_heat_loss )
        {
            std::cerr << " from down\n";
            report( row + 1, col );
        }
        else if( tiles[ row ][ col ][ down ].back() == min_heat_loss )
        {
            std::cerr << " from up\n";
            report( row - 1, col );
        }
        else if( tiles[ row ][ col ][ left ].back() == min_heat_loss )
        {
            std::cerr << " from right\n";
            report( row, col + 1 );
        }
        else if( tiles[ row ][ col ][ right ].back() == min_heat_loss )
        {
            std::cerr << " from left\n";
            report( row, col - 1 );
        }
        else
            throw std::runtime_error( "Why" );
    }

    std::vector< std::string > const& input;
    std::vector< std::vector< tile > > tiles;
};

advent_t advent( std::vector< std::string > const& input )
{
    problem p( input );

    // p.report( input.size() - 1, input.front().size() - 1 );

    return std::min(
        p.tiles[ input.size() - 1 ][ input.front().size() - 1 ][ down ].back(),
        p.tiles[ input.size() - 1 ][ input.front().size() - 1 ][ right ].back() );
}
