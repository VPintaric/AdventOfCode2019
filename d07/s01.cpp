
#include <vector>
#include <iostream>
#include <cmath>
#include <functional>
#include <sstream>
#include <limits>
#include <algorithm>

int getParam( const std::vector< int > &program, int pc, int n ) {
    int argMode = program[ pc ] / 10;
    for ( int i = 0; i < n; ++i ) {
        argMode /= 10;
    }
    bool isPositionMode = ( argMode % 10 ) == 0;

    if ( isPositionMode ) {
        return program[ program[ pc + n ]];
    } else {
        return program[ pc + n ];
    }
}

void run( std::vector< int > program,
          const std::function< int( void ) > &input,
          const std::function< void( int ) > &output ) {

    int pc = 0; // program counter
    bool done = false;

    while ( !done ) {
        int p1, p2, dst;

        int opcode = program[ pc ] % 100;
        switch ( opcode ) {
            case 1:
                p1 = getParam( program, pc, 1 );
                p2 = getParam( program, pc, 2 );
                dst = program[ pc + 3 ];
                program[ dst ] = p1 + p2;

                pc += 4;
                break;
            case 2:
                p1 = getParam( program, pc, 1 );
                p2 = getParam( program, pc, 2 );
                dst = program[ pc + 3 ];
                program[ dst ] = p1 * p2;

                pc += 4;
                break;
            case 3:
                dst = program[ pc + 1 ];
                program[ dst ] = input();

                pc += 2;
                break;
            case 4:
                p1 = getParam( program, pc, 1 );
                output( p1 );

                pc += 2;
                break;
            case 5:
                p1 = getParam( program, pc, 1 );
                if ( p1 != 0 ) {
                    p2 = getParam( program, pc, 2 );
                    pc = p2;
                } else {
                    pc += 3;
                }
                break;
            case 6:
                p1 = getParam( program, pc, 1 );
                if ( p1 == 0 ) {
                    p2 = getParam( program, pc, 2 );
                    pc = p2;
                } else {
                    pc += 3;
                }
                break;
            case 7:
                p1 = getParam( program, pc, 1 );
                p2 = getParam( program, pc, 2 );
                dst = program[ pc + 3 ];
                if ( p1 < p2 ) {
                    program[ dst ] = 1;
                } else {
                    program[ dst ] = 0;
                }

                pc += 4;
                break;
            case 8:
                p1 = getParam( program, pc, 1 );
                p2 = getParam( program, pc, 2 );
                dst = program[ pc + 3 ];
                if ( p1 == p2 ) {
                    program[ dst ] = 1;
                } else {
                    program[ dst ] = 0;
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
}

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

    std::vector< int > phaseSetting = { 0, 1, 2, 3, 4 };
    std::vector< int > bestPhaseSetting;
    int maxSignal = std::numeric_limits< int >::min();

    do {
        int input = 0;
        for ( auto &phase : phaseSetting ) {
            int nInput = 0;

            run( program,
                 [ &input, &nInput, &phase ]() {
                     if ( nInput == 0 ) {
                         ++nInput;
                         return phase;
                     } else {
                         return input;
                     }
                 },
                 [ &input ]( int output ) {
                     input = output;
                 } );
        }

        if ( maxSignal < input ) {
            maxSignal = input;
            bestPhaseSetting = phaseSetting;
        }

    } while ( std::next_permutation( phaseSetting.begin(), phaseSetting.end()));

    std::cout << maxSignal << std::endl;
    for ( auto &phase : bestPhaseSetting ) {
        std::cout << phase << " ";
    }
    std::cout << std::endl;

    return 0;
}