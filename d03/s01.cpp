
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>

struct Line {
    int first, second, common;
};

void fillFirstWireLinesData( std::vector< Line > &vertical, std::vector< Line > &horizontal ) {
    std::string inputLine;
    std::getline( std::cin, inputLine );
    std::stringstream ss( inputLine );

    char direction;
    int distance;

    int curx = 0, cury = 0;
    while ( ss ) {
        ss >> direction;
        ss >> distance;
        ss.ignore( 1 ); // ignore the comma

        switch ( direction ) {
            case 'R':
                horizontal.push_back( { curx, curx + distance, cury } );
                curx += distance;
                break;
            case 'L':
                horizontal.push_back( { curx, curx - distance, cury } );
                curx -= distance;
                break;
            case 'U':
                vertical.push_back( { cury, cury + distance, curx } );
                cury += distance;
                break;
            case 'D':
                vertical.push_back( { cury, cury - distance, curx } );
                cury -= distance;
                break;
            default:
                std::cerr << "Crash and burn" << std::endl;
        }
    }
}

bool isBetween( int n, int first, int second ) {
    if ( first < second ) {
        return n >= first && n <= second;
    } else {
        return n >= second && n <= first;
    }
}

int main() {
    std::vector< Line > vertical, horizontal;

    fillFirstWireLinesData( vertical, horizontal );

    int closestCrossingDistance = std::numeric_limits< int >::max();

    std::string inputLine;
    std::getline( std::cin, inputLine );
    std::stringstream ss( inputLine );
    int curx = 0, cury = 0;
    char direction;
    int distance;
    while ( ss ) {
        ss >> direction;
        ss >> distance;
        ss.ignore( 1 ); // ignore the comma

        Line secondWireLine{};
        switch ( direction ) {
            case 'R':
                secondWireLine = Line{ curx, curx + distance, cury };
                curx += distance;
                break;
            case 'L':
                secondWireLine = Line{ curx, curx - distance, cury };
                curx -= distance;
                break;
            case 'U':
                secondWireLine = Line{ cury, cury + distance, curx };
                cury += distance;
                break;
            case 'D':
                secondWireLine = Line{ cury, cury - distance, curx };
                cury -= distance;
                break;
            default:
                std::cerr << "Crash and burn" << std::endl;
        }

        std::vector< Line > *firstWireLines;
        if ( direction == 'R' || direction == 'L' ) {
            firstWireLines = &vertical;
        } else {
            firstWireLines = &horizontal;
        }

        for ( auto &firstWireLine : *firstWireLines ) {
            if ( isBetween( firstWireLine.common, secondWireLine.first, secondWireLine.second ) &&
                 isBetween( secondWireLine.common, firstWireLine.first, firstWireLine.second )) {

                if ( firstWireLine.common != 0 || secondWireLine.common != 0 ) {
                    auto crossingDistance = std::abs( firstWireLine.common ) + std::abs( secondWireLine.common );
                    closestCrossingDistance = std::min( closestCrossingDistance, crossingDistance );
                }
            }
        }
    }

    std::cout << closestCrossingDistance << std::endl;

    return 0;
}
