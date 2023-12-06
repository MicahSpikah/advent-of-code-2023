#include "parse_input.h"
#include <algorithm>

advent_t advent( std::vector< std::string > const& input )
{
    int sum{};
    for( int i = 0; i < input.size(); ++i )
    {
        auto pos = input[ i ].find_first_of( "1234567890" );
        while( pos != std::string::npos )
        {
            bool good{};
            auto end = input[ i ].find_first_not_of( "1234567890", pos );
            if( end == std::string::npos )
                end = input[ i ].size();
            int start = std::clamp< int >( pos - 1, 0, static_cast< int >( input[ i ].size() ) );
            int stop  = std::clamp< int >( end + 1, 0, static_cast< int >( input[ i ].size() ) );
            for( int j = std::max( i - 1, 0 ); j < std::min< int >( i + 2, input.size() ); ++j )
            {
                auto const substring = input[ j ].substr( start, stop - start );
                if( substring.find_first_not_of( "1234567890." ) != std::string::npos )
                    good = true;
            }
            if( good )
            {
                auto const good_num = std::stoi( input[ i ].substr( pos, end ) );
                sum += good_num;
                std::cerr << good_num << '\n';
            }
            if( end < input[ i ].size() )
                pos = input[ i ].find_first_of( "1234567890", end );
            else
                break;
        }
    }

    return sum;
}
