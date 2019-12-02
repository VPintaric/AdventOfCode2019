//
// Created by wex on 12/2/19.
//

#include <vector>
#include <iostream>

int main() {
    std::vector< int > values;
    int value;

    // Read the input
    while ( std::cin >> value ) {
        values.push_back( value );
        std::cin.ignore( 1, ',' );
    }

    // hardcoded values stated in the task text
    values[ 1 ] = 12;
    values[ 2 ] = 2;

    // Run the code
    int pc = 0; // program counter
    bool done = false;
    while ( !done ) {
        int v1, v2, dst;

        switch ( values[ pc ] ) {
            case 1:
                v1 = values[ values[ pc + 1 ]];
                v2 = values[ values[ pc + 2 ]];
                dst = values[ pc + 3 ];
                values[ dst ] = v1 + v2;
                break;
            case 2:
                v1 = values[ values[ pc + 1 ]];
                v2 = values[ values[ pc + 2 ]];
                dst = values[ pc + 3 ];
                values[ dst ] = v1 * v2;
                break;
            case 99:
                done = true;
                break;
            default:
                break;
        }

        pc += 4;
    }

    std::cout << values[ 0 ] << std::endl;

    return 0;
}