//
// Created by wex on 12/2/19.
//

#include <vector>
#include <iostream>

int run( std::vector< int > program, int noun, int verb ) {
    program[ 1 ] = noun;
    program[ 2 ] = verb;

    // Run the code
    int pc = 0; // program counter
    bool done = false;
    while ( !done ) {
        int v1, v2, dst;

        switch ( program[ pc ] ) {
            case 1:
                v1 = program[ program[ pc + 1 ]];
                v2 = program[ program[ pc + 2 ]];
                dst = program[ pc + 3 ];
                program[ dst ] = v1 + v2;
                break;
            case 2:
                v1 = program[ program[ pc + 1 ]];
                v2 = program[ program[ pc + 2 ]];
                dst = program[ pc + 3 ];
                program[ dst ] = v1 * v2;
                break;
            case 99:
                done = true;
                break;
            default:
                break;
        }

        pc += 4;
    }

    return program[ 0 ];
}

int main() {
    std::vector< int > initialMemory;
    int value;

    // Read the input
    while ( std::cin >> value ) {
        initialMemory.push_back( value );
        std::cin.ignore( 1, ',' );
    }

    constexpr int expectedResult = 19690720;

    // Search for correct input parameters
    int mergedNounVerb;
    for ( int noun = 0; noun < 100; ++noun ) {
        for ( int verb = 0; verb < 100; ++verb ) {
            auto result = run( initialMemory, noun, verb );
            if ( result == expectedResult ) {
                mergedNounVerb = 100 * noun + verb;
            }
        }
    }
    std::cout << mergedNounVerb << std::endl;

    return 0;
}