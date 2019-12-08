#include <cstdio>
#include <limits>
#include <vector>

int main() {
    constexpr int width = 25;
    constexpr int height = 6;

    std::vector< int > digits;

    char c;
    while ( c = std::fgetc( stdin )) {
        if ( c == EOF || c == '\n' ) {
            break;
        }
        c -= '0';
        digits.push_back( c );
    }
    std::printf( "%d\n", digits.size());

    std::vector< std::vector< int > > layers;

    for ( int layerIdx = 0; layerIdx < digits.size(); layerIdx += width * height ) {
        std::vector< int > layer;
        layer.reserve( width * height );
        for ( int i = 0; i < width * height; ++i ) {
            layer.push_back( digits[ layerIdx + i ] );
        }

        layers.push_back( layer );
    }

    std::vector< int > image( width * height );
    for ( int i = 0; i < width * height; ++i ) {
        for ( auto &layer : layers ) {
            if ( layer[ i ] == 2 ) {
                continue;
            } else {
                image[ i ] = layer[ i ];
                break;
            }
        }
    }

    for ( int i = 0; i < height; ++i ) {
        for ( int j = 0; j < width; ++j ) {
            if ( image[ i * width + j ] == 0 ) {
                std::printf( " " );
            } else {
                std::printf( "X" );
            }
        }
        std::printf( "\n" );
    }

    return 0;
}