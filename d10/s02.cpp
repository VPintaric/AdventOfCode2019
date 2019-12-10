#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <numeric>
#include <cmath>
#include <algorithm>

const auto pi = std::acos( -1 );

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

std::set< Point > getAllDirections( const std::vector< std::string > &grid, int xp, int yp ) {
    std::set< Point > directions;

    for ( int y = 0; y < grid.size(); ++y ) {
        for ( int x = 0; x < grid[ y ].size(); ++x ) {
            if ( x == xp && y == yp ) {
                continue;
            }

            if ( grid[ y ][ x ] == '#' ) {
                auto dirx = x - xp;
                auto diry = y - yp;

                auto gcd = std::gcd( dirx, diry );
                dirx /= gcd;
                diry /= gcd;

                directions.insert( Point{ dirx, diry } );
            }
        }
    }

    return directions;
}

Point findNthVaporized( std::vector< std::string > grid, Point station, int which ) {
    auto directionsUnsorted = getAllDirections( grid, station.x, station.y );

    auto refAngle = std::atan2( -1, 0 );
    std::vector< Point > directions( directionsUnsorted.begin(), directionsUnsorted.end());
    std::sort( directions.begin(), directions.end(), [ &refAngle ]( const Point &dir1, const Point &dir2 ) {
        auto angle1 = std::atan2( dir1.y, dir1.x ) - refAngle;
        auto angle2 = std::atan2( dir2.y, dir2.x ) - refAngle;

        if ( angle1 < 0 ) {
            angle1 += 2 * pi;
        }
        if ( angle2 < 0 ) {
            angle2 += 2 * pi;
        }

        return angle1 < angle2;
    } );

    int vaporizedCount = 0;
    while ( true ) {
        for ( auto &dir : directions ) {
            bool gotATarget = false;
            Point target = station;
            while ( true ) {
                target.x += dir.x;
                target.y += dir.y;
                if ( target.x < 0 || target.x >= grid[ 0 ].size() || target.y < 0 || target.y >= grid.size()) {
                    break;
                }
                if ( grid[ target.y ][ target.x ] == '#' ) {
                    gotATarget = true;
                    break;
                }
            }

            if ( gotATarget ) {
                grid[ target.y ][ target.x ] = '.';
                ++vaporizedCount;
            }

            if ( vaporizedCount == which ) {
                return target;
            }
        }
    }
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
    Point bestStation{};
    for ( int y = 0; y < grid.size(); ++y ) {
        for ( int x = 0; x < grid[ y ].size(); ++x ) {
            if ( grid[ y ][ x ] == '#' ) {
                auto count = static_cast<int>(getAllDirections( grid, x, y ).size());
                if ( maxSeenCount < count ) {
                    maxSeenCount = count;
                    bestStation.x = x;
                    bestStation.y = y;
                }
            }
        }
    }

    auto vaporized200 = findNthVaporized( grid, bestStation, 200 );
    std::cout << ( vaporized200.x * 100 + vaporized200.y ) << std::endl;

    return 0;
}