
#include <vector>
#include <iostream>
#include <cmath>
#include <functional>
#include <sstream>
#include <limits>
#include <algorithm>
#include <queue>

class Program {
    std::vector< int > memory;
    std::queue< int > input;
    std::queue< int > output;
    int pc;

    int getParam( int n ) {
        int argMode = memory[ pc ] / 10;
        for ( int i = 0; i < n; ++i ) {
            argMode /= 10;
        }
        bool isPositionMode = ( argMode % 10 ) == 0;

        if ( isPositionMode ) {
            return memory[ memory[ pc + n ]];
        } else {
            return memory[ pc + n ];
        }
    }

public:
    void reset( const std::vector< int > &initial ) {
        pc = 0;
        memory = initial;
        std::queue< int >().swap( input );
        std::queue< int >().swap( output );
    }

    void pushInput( int in ) {
        input.push( in );
    }

    int popOutput() {
        if ( output.empty()) {
            std::cout << "Empty output queue" << std::endl;
            return 0;
        }

        auto out = output.front();
        output.pop();
        return out;
    }

    bool run() {
        bool done = false;

        while ( !done ) {
            int p1, p2, dst;

            int opcode = memory[ pc ] % 100;
            switch ( opcode ) {
                case 1:
                    p1 = getParam( 1 );
                    p2 = getParam( 2 );
                    dst = memory[ pc + 3 ];
                    memory[ dst ] = p1 + p2;

                    pc += 4;
                    break;
                case 2:
                    p1 = getParam( 1 );
                    p2 = getParam( 2 );
                    dst = memory[ pc + 3 ];
                    memory[ dst ] = p1 * p2;

                    pc += 4;
                    break;
                case 3:
                    if ( input.empty()) {
                        return false;
                    }

                    dst = memory[ pc + 1 ];
                    memory[ dst ] = input.front();
                    input.pop();

                    pc += 2;
                    break;
                case 4:
                    p1 = getParam( 1 );
                    output.push( p1 );

                    pc += 2;
                    return false;
                case 5:
                    p1 = getParam( 1 );
                    if ( p1 != 0 ) {
                        p2 = getParam( 2 );
                        pc = p2;
                    } else {
                        pc += 3;
                    }
                    break;
                case 6:
                    p1 = getParam( 1 );
                    if ( p1 == 0 ) {
                        p2 = getParam( 2 );
                        pc = p2;
                    } else {
                        pc += 3;
                    }
                    break;
                case 7:
                    p1 = getParam( 1 );
                    p2 = getParam( 2 );
                    dst = memory[ pc + 3 ];
                    if ( p1 < p2 ) {
                        memory[ dst ] = 1;
                    } else {
                        memory[ dst ] = 0;
                    }

                    pc += 4;
                    break;
                case 8:
                    p1 = getParam( 1 );
                    p2 = getParam( 2 );
                    dst = memory[ pc + 3 ];
                    if ( p1 == p2 ) {
                        memory[ dst ] = 1;
                    } else {
                        memory[ dst ] = 0;
                    }

                    pc += 4;
                    break;
                case 99:
                    done = true;
                    break;
                default:
                    std::cout << "CRASH AND BURN" << std::endl;
                    break;
            }
        }

        return true;
    }
};


int main() {
    std::vector< int > program;
    int value;

    // Read the input
    std::string line;
    std::getline( std::cin, line );
    std::stringstream ss( line );

    while ( ss >> value ) {
        program.push_back( value );
        ss.ignore( 1, ',' );
    }

    std::vector< int > phaseSetting = { 5, 6, 7, 8, 9 };
    int maxSignal = std::numeric_limits< int >::min();

    std::vector< Program > amplifiers( 5 );

    do {
        for ( int i = 0; i < amplifiers.size(); ++i ) {
            amplifiers[ i ].reset( program );
            amplifiers[ i ].pushInput( phaseSetting[ i ] );
        }

        bool done = false;
        int lastSignal = 0;
        int signal = 0;
        while ( !done ) {
            for ( int i = 0; i < amplifiers.size(); ++i ) {
                amplifiers[ i ].pushInput( signal );
                auto halted = amplifiers[ i ].run();
                if ( halted ) {
                    done = true;
                    break;
                }
                signal = amplifiers[ i ].popOutput();
                if ( i == amplifiers.size() - 1 ) {
                    lastSignal = signal;
                }
            }
        }

        maxSignal = std::max( maxSignal, lastSignal );
    } while ( std::next_permutation( phaseSetting.begin(), phaseSetting.end()));

    std::cout << maxSignal << std::endl;

    return 0;
}