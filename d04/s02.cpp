
#include <iostream>
#include <vector>

void increment( std::vector< int > &digits ) {
    for ( auto iter = digits.rbegin(); iter != digits.rend(); ++iter ) {
        auto &digit = *iter;
        ++digit;
        if ( digit == 10 ) {
            digit = 0;
        } else {
            break;
        }
    }
}

bool repeatingCriterionChecksOut( const std::vector< int > &digits ) {
    auto previous = digits.front();
    int repeats = 1;
    for ( int i = 1; i < digits.size(); ++i ) {
        if ( digits[ i ] == previous ) {
            ++repeats;
        } else {
            if ( repeats == 2 ) {
                return true;
            }
            repeats = 1;
        }
        previous = digits[ i ];
    }
    return repeats == 2;
}

void setToNextValidNumber( std::vector< int > &digits ) {
    increment( digits );

    int previous = 0;
    bool hasRepeating = false;
    int i;
    for ( i = 0; i < digits.size(); ++i ) {
        if ( digits[ i ] < previous ) {
            break;
        }
        if ( digits[ i ] == previous ) {
            hasRepeating = true;
        }
        previous = digits[ i ];
    }

    for ( ; i < digits.size(); ++i ) {
        digits[ i ] = previous;
        hasRepeating = true;
    }

    if ( !hasRepeating ) {
        digits[ digits.size() - 2 ] = digits[ digits.size() - 1 ];
    }

    if ( !repeatingCriterionChecksOut( digits )) {
        setToNextValidNumber( digits );
    }
}

int digitsToInt( const std::vector< int > &digits ) {
    int base = 1;
    int num = 0;
    for ( auto iter = digits.rbegin(); iter != digits.rend(); ++iter ) {
        num += *iter * base;
        base *= 10;
    }

    return num;
}

int main() {
    constexpr int N_DIGITS = 6;

    int limit;

    std::vector< int > digits;
    digits.reserve( N_DIGITS );
    for ( int i = 0; i < N_DIGITS; ++i ) {
        digits.push_back( std::cin.get() - '0' );
    }
    std::cin.ignore( 1 );
    std::cin >> limit;

    setToNextValidNumber( digits );
    int count = 0;
    while ( digitsToInt( digits ) < limit ) {
        ++count;
        std::cout << digitsToInt( digits ) << std::endl;
        setToNextValidNumber( digits );
    }

    std::cout << count << std::endl;

    return 0;
}
