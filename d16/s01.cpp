#include <iostream>
#include <vector>

void nextPhase( std::vector< int > &signal ) {
    const std::vector< int > pattern{ 0, 1, 0, -1 };

    std::vector< int > newSignal;
    newSignal.reserve( signal.size());

    for ( int i = 0; i < signal.size(); ++i ) {
        int patternRepeats = i + 1;
        int currentRepeat = ( patternRepeats == 1 ) ? 0 : 1;
        int currentPatternDigit = ( patternRepeats == 1 ) ? 1 : 0;

        int newDigit = 0;
        for ( auto &d : signal ) {
            newDigit = ( newDigit + d * pattern[ currentPatternDigit ] );

            ++currentRepeat;
            if ( currentRepeat == patternRepeats ) {
                currentRepeat = 0;
                currentPatternDigit = static_cast<int>(( currentPatternDigit + 1 ) % pattern.size());
            }
        }

        newSignal.push_back( std::abs( newDigit ) % 10 );
    }

    signal = std::move( newSignal );
}

int main() {
    std::vector< int > signal;
    char inputDigit;
    while ( std::cin.get( inputDigit )) {
        if ( inputDigit == '\n' ) {
            break;
        }
        signal.push_back( inputDigit - '0' );
    }

    for ( int i = 0; i < 100; ++i ) {
        nextPhase( signal );
    }

    for ( int i = 0; i < 8; ++i ) {
        std::cout << signal[ i ];
    }
    std::cout << std::endl;

    return 0;
}