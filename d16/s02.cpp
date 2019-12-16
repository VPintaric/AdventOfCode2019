#include <iostream>
#include <vector>

std::vector< short > findEightDigits( std::vector< short > signal, int iterations, int from ) {
    std::vector< short > digits;
    digits.reserve( 8 );

    for ( int iteration = 0; iteration < iterations; ++iteration ) {
        std::vector< short > reversePartialSums( signal.size() - from, 0 );
        short current = 0;
        for ( int i = static_cast<int>(signal.size() - 1), j = 0; i >= from; --i, ++j ) {
            current = static_cast<short>(( current + signal[ i ] ) % 10);
            reversePartialSums[ j ] = current;
        }

        for ( int i = static_cast<int>(signal.size() - 1), j = 0; i >= from; --i, ++j ) {
            signal[ i ] = reversePartialSums[ j ];
        }
    }

    digits.insert( digits.end(), signal.begin() + from, signal.begin() + from + 8 );
    return digits;
}

int main() {
    std::vector< short > signal;
    char inputDigit;
    while ( std::cin.get( inputDigit )) {
        if ( inputDigit == '\n' ) {
            break;
        }
        signal.push_back( inputDigit - '0' );
    }

    int offset = 0;
    {
        int multiplier = 1000000;
        for ( int i = 0; i < 7; ++i, multiplier /= 10 ) {
            offset += signal[ i ] * multiplier;
        }
    }

    {
        std::vector< short > realSignal;
        realSignal.reserve( signal.size() * 10000 );
        for ( int i = 0; i < 10000; ++i ) {
            realSignal.insert( realSignal.end(), signal.begin(), signal.end());
        }
        signal = std::move( realSignal );
    }

    auto digits = findEightDigits( signal, 100, offset );

    for ( auto &d : digits ) {
        std::cout << d;
    }
    std::cout << std::endl;

    return 0;
}