
#include <vector>
#include <iostream>
#include <cmath>
#include <functional>
#include <sstream>

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

void run( std::vector< int > &program, int input, const std::function< void( int ) > &output ) {
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
                dst = program[ pc + 3 ];
                program[ dst ] = input;

                pc += 2;
                break;
            case 4:
                p1 = getParam( program, pc, 1 );
                output( p1 );

                pc += 2;
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
    constexpr int input = 1;

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

    int nonZeroOutputsCount = 0;
    int lastNonZeroOutput;

    run( program, input, [ &nonZeroOutputsCount, &lastNonZeroOutput ]( int output ) {
        if ( output != 0 ) {
            ++nonZeroOutputsCount;
            lastNonZeroOutput = output;
        }
    } );

    std::cout << "Errors: " << nonZeroOutputsCount - 1 << std::endl;
    std::cout << "Diagnostic code: " << lastNonZeroOutput << std::endl;

    return 0;
}