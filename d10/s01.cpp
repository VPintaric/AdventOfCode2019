#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <numeric>
#include <cmath>

struct Point {
    int x, y;

    bool operator<( const Point &o ) const {
        if ( x == o.x ) {
            return y < o.y;
        } else {
            return x < o.x;
        }
    }
};

int countSeen( const std::vector< std::string > &grid, int x, int y ) {
    std::set< Point > usedDirections;
    int count = 0;

    for ( int i = 0; i < grid.size(); ++i ) {
        for ( int j = 0; j < grid[ i ].size(); ++j ) {
            if ( i == x && j == y ) {
                continue;
            }

            if ( grid[ i ][ j ] == '#' ) {
                auto dirx = i - x;
                auto diry = j - y;

                auto gcd = std::gcd( dirx, diry );
                dirx /= gcd;
                diry /= gcd;

                Point dir{ dirx, diry };

                if ( usedDirections.find( dir ) == usedDirections.end()) {
                    usedDirections.insert( dir );
                    ++count;
                }
            }
        }
    }

    return count;
}

int main() {
    std::vector< std::string > grid;
    std::string line;

    while ( std::getline( std::cin, line )) {
        if ( line.empty()) {
            break;
        }
        grid.push_back( line );
    }

    int maxSeenCount = 0;
    for ( int i = 0; i < grid.size(); ++i ) {
        for ( int j = 0; j < grid[ i ].size(); ++j ) {
            if ( grid[ i ][ j ] == '#' ) {
                auto count = countSeen( grid, i, j );
                maxSeenCount = std::max( maxSeenCount, count );
            }
        }
    }

    std::cout << maxSeenCount << std::endl;
}