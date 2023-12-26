#include "parse_input.h"

#include <algorithm>
#include <array>
#include <map>
#include <numeric>
#include <ranges>

std::int64_t const min = 200000000000000ll;
std::int64_t const max = 400000000000000ll;

struct hail
{
    std::int64_t x, y, z;
    int dx, dy, dz;

    std::int64_t at_min_x() const
    {
        if( dx == 0 )
            throw std::runtime_error( "Infinite slope sorry" );
        return ( dy / dx ) * ( min - x ) + y;
    }

    std::int64_t at_max_x() const
    {
        if( dx == 0 )
            throw std::runtime_error( "Infinite slope sorry" );
        return ( dy / dx ) * ( max - x ) + y;
    }
};

advent_t advent( std::vector< std::string > const& input )
{
    std::vector< hail > hails;

    for( auto const& line : input )
    {
        hail b;
        sscanf( line.c_str(), "%lli, %lli, %lli @ %d, %d, %d", &b.x, &b.y, &b.z, &b.dx, &b.dy, &b.dz );
        hails.push_back( b );
    }

    int intersecting{};

    for( int i = 0; i < hails.size(); ++i )
    {
        for( int j = i + 1; j < hails.size(); ++j )
        {
            auto const& h1 = hails[ i ];
            auto const& h2 = hails[ j ];
            if( h1.dy * h2.dx == h2.dy * h1.dx )
            {
                if( h1.dx == 0 )
                {
                    if( h1.x == h2.x && min <= h1.x && h1.x <= max )
                        ++intersecting;
                }
                else if( h1.y - h2.y == ( h1.dy * ( h1.x - h2.x ) ) / h1.dx )
                {
                    if( !( ( h1.at_min_x() > max && h1.at_max_x() > max ) || ( h1.at_min_x() < min && h1.at_max_x() < min ) ) )
                        ++intersecting;
                }
            }
            else
            {
                // Single intersection point:
                // h1.y = dy/dx * x + b
                // yi = (xi - h1.x) * h1.dy/h1.dx + h1.y
                // yi = (xi - h2.x) * h2.dy/h2.dx + h2.y
                // 0 = xi * h1.m - h1.x * h1.m + h1.y -xi * h2.m + h2.x * h2.m - h2.y
                // 0 = xi (h1.m - h2.m) - h1.x * h1.m + h1.y + h2.x * h2.m - h2.y
                // h1.x * h1.m + h2.y - h2.x * h2.m - h1.y  = x1 (h1.m - h2.m)
                double const m1  = static_cast< double >( h1.dy ) / static_cast< double >( h1.dx );
                double const m2  = static_cast< double >( h2.dy ) / static_cast< double >( h2.dx );
                double const x_i = ( h1.x * m1 + h2.y - h2.x * m2 - h1.y ) / ( m1 - m2 );
                double const y_i = m1 * ( x_i - h1.x ) + h1.y;
                double const t1  = ( x_i - h1.x ) / h1.dx;
                double const t2  = ( x_i - h2.x ) / h2.dx;
                if( min <= x_i && x_i < max && min <= y_i && y_i < max && t1 >= 0 && t2 >= 0 )
                {
                    ++intersecting;
                }
            }
        }
    }

    return intersecting;
}
