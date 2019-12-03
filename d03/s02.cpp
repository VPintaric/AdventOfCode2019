//
// Created by wex on 12/2/19.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <limits>

struct Line {
    int first, second, common, steps;
};

void fillFirstWireLinesData( std::vector< Line > &vertical, std::vector< Line > &horizontal ) {
    std::string inputLine;
    std::getline( std::cin, inputLine );
    std::stringstream ss( inputLine );

    char direction;
    int distance;

    int curx = 0, cury = 0;
    int steps = 0;
    while ( ss ) {
        ss >> direction;
        ss >> distance;
        ss.ignore( 1 ); // ignore the comma

        switch ( direction ) {
            case 'R':
                horizontal.push_back( { curx, curx + distance, cury, steps } );
                curx += distance;
                break;
            case 'L':
                horizontal.push_back( { curx, curx - distance, cury, steps } );
                curx -= distance;
                break;
            case 'U':
                vertical.push_back( { cury, cury + distance, curx, steps } );
                cury += distance;
                break;
            case 'D':
                vertical.push_back( { cury, cury - distance, curx, steps } );
                cury -= distance;
                break;
            default:
                std::cerr << "Crash and burn" << std::endl;
        }
        steps += distance;
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

    int closestCrossingSteps = std::numeric_limits< int >::max();

    std::string inputLine;
    std::getline( std::cin, inputLine );
    std::stringstream ss( inputLine );
    int curx = 0, cury = 0;
    char direction;
    int distance;
    int steps = 0;
    while ( ss ) {
        ss >> direction;
        ss >> distance;
        ss.ignore( 1 ); // ignore the comma

        Line secondWireLine{};
        switch ( direction ) {
            case 'R':
                secondWireLine = Line{ curx, curx + distance, cury, steps };
                curx += distance;
                break;
            case 'L':
                secondWireLine = Line{ curx, curx - distance, cury, steps };
                curx -= distance;
                break;
            case 'U':
                secondWireLine = Line{ cury, cury + distance, curx, steps };
                cury += distance;
                break;
            case 'D':
                secondWireLine = Line{ cury, cury - distance, curx, steps };
                cury -= distance;
                break;
            default:
                std::cerr << "Crash and burn" << std::endl;
        }
        steps += distance;

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
                    auto crossingStepsFirstLine =
                        firstWireLine.steps + std::abs( firstWireLine.first - secondWireLine.common );
                    auto crossingStepsSecondLine =
                        secondWireLine.steps + std::abs( secondWireLine.first - firstWireLine.common );

                    auto crossingSteps = crossingStepsFirstLine + crossingStepsSecondLine;
                    closestCrossingSteps = std::min( crossingSteps, closestCrossingSteps );
                }
            }
        }
    }

    std::cout << closestCrossingSteps << std::endl;

    return 0;
}
