#include "parse_input.h"
#include <algorithm>
#include <numeric>
#include <set>

advent_t advent( std::vector< std::string > const& input )
{
    std::vector<int> cards(input.size(), 1);
    for( auto const& line : input )
    {
        int a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
        int b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24;
        int card;
        sscanf( line.c_str(), "Card %d: %d %d %d %d %d %d %d %d %d %d | %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &card, &a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &b0, &b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9, &b10, &b11, &b12, &b13, &b14, &b15, &b16, &b17, &b18, &b19, &b20, &b21, &b22, &b23, &b24 );
        std::set< int > a{ a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 };
        std::set< int > b{ b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24 };
        std::vector< int > w;
        std::ranges::set_intersection( a, b, std::back_inserter( w ) );

        for(int micah_is_lazy = 0; micah_is_lazy < w.size(); ++micah_is_lazy)
        {
            cards[card + micah_is_lazy] += cards[card-1];
        }
    }
    return std::reduce(cards.cbegin(), cards.cend());
}
