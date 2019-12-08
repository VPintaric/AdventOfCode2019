#include <cstdio>
#include <limits>
#include <vector>

int main() {
    std::vector< int > digits;

    char c;
    while ( c = std::fgetc( stdin )) {
        if ( c == EOF || c == '\n' ) {
            break;
        }
        c -= '0';
        digits.push_back( c );
    }

    std::vector< int > fewestZerosCounter( 10 ), layerCounter( 10 );
    fewestZerosCounter[ 0 ] = std::numeric_limits< int >::max();
    for ( int i = 0; i < digits.size(); i += 25 * 6 ) {
        layerCounter = std::vector< int >( 10 );

        for ( int j = 0; j < 25 * 6; ++j ) {
            layerCounter[ digits[ i + j ]] += 1;
        }

        if ( layerCounter[ 0 ] < fewestZerosCounter[ 0 ] ) {
            fewestZerosCounter = layerCounter;
        }
    }

    std::printf( "%d\n", fewestZerosCounter[ 1 ] * fewestZerosCounter[ 2 ] );

    return 0;
}