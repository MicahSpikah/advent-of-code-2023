#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

using weighted_graph = std::map< char, std::map< char, int > >;
int go( weighted_graph const& graph, char const from, char const to, int const overhead )
{
    int best{};
    for( auto const& [ next_k, len ] : graph.at( from ) )
    {
        if( next_k == to )
        {
            best = std::max( best, overhead + len );
            continue;
        }
        auto tmp_graph = graph;
        tmp_graph.erase( from );
        for( auto const& thing : graph.at( from ) )
        {
            tmp_graph.at( thing.first ).erase( from );
        }

        auto tmp_overhead = overhead + len;
        best              = std::max( best, go( tmp_graph, next_k, to, tmp_overhead ) );
    }
    return best;
}

advent_t advent( std::vector< std::string > const& input )
{
    std::map< char, std::pair< int, int > > id_to_loc;
    std::map< std::pair< int, int >, char > loc_to_id;
    weighted_graph you_can_go;

    auto modified = input;
    for( auto& line : modified )
    {
        std::ranges::replace( line, '>', '.' );
        std::ranges::replace( line, '<', '.' );
        std::ranges::replace( line, '^', '.' );
        std::ranges::replace( line, 'v', '.' );
    }

    for( int row = 1; row < modified.size(); ++row )
    {
        for( int col = 1; col < modified.front().size(); ++col )
        {
            if( modified[ row ][ col ] == '.' )
            {
                int paths{};
                if( modified[ row - 1 ][ col ] == '.' )
                    ++paths;
                if( modified[ row + 1 ][ col ] == '.' )
                    ++paths;
                if( modified[ row ][ col - 1 ] == '.' )
                    ++paths;
                if( modified[ row ][ col + 1 ] == '.' )
                    ++paths;
                if( paths < 3 )
                    continue;
                modified[ row ][ col ]    = 'F'; // Fork
                std::pair< int, int > loc = { row, col };
                char id;
                if( id_to_loc.size() < 10 )
                {
                    id = id_to_loc.size() + '0';
                }
                else
                {
                    id = id_to_loc.size() + 'A' - 10;
                }
                id_to_loc[ id ]  = loc;
                loc_to_id[ loc ] = id;
            }
        }
    }
    int overhead{};
    char source;
    char sink;
    auto wander = [ & ]( int row, int col ) {
        auto tmp = modified;
        int len{ 1 };
        while( row > 0 && row + 1 < modified.size() && tmp[ row ][ col ] != 'F' )
        {
            tmp[ row ][ col ] = 'O';
            ++len;
            if( tmp[ row - 1 ][ col ] == '.' || ( len > 2 && tmp[ row - 1 ][ col ] == 'F' ) )
                --row;
            else if( tmp[ row + 1 ][ col ] == '.' || ( len > 2 && tmp[ row + 1 ][ col ] == 'F' ) )
                ++row;
            else if( tmp[ row ][ col - 1 ] == '.' || ( len > 2 && tmp[ row ][ col - 1 ] == 'F' ) )
                --col;
            else
                ++col;
        }
        if( row == 0 )
            return std::make_pair( 'Y', len );
        if( row + 1 == modified.size() )
            return std::make_pair( 'Z', len );
        return std::make_pair( loc_to_id[ std::make_pair( row, col ) ], len );
    };

    for( auto const& [ k, v ] : id_to_loc )
    {
        auto const tell = [ &, k = k ]( auto const row, auto const col ) {
            if( modified[ row ][ col ] == '.' )
            {
                auto const result = wander( row, col );
                if( result.first == 'Y' )
                {
                    source = k;
                    overhead += result.second;
                }
                else if( result.first == 'Z' )
                {
                    sink = k;
                    overhead += result.second;
                }
                else
                {
                    you_can_go[ k ][ result.first ] = result.second;
                    you_can_go[ result.first ][ k ] = result.second;
                }
            }
        };
        tell( v.first - 1, v.second );
        tell( v.first + 1, v.second );
        tell( v.first, v.second - 1 );
        tell( v.first, v.second + 1 );
    }

    return go( you_can_go, source, sink, overhead );
}
