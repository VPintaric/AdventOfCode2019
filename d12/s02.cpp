#include <cstdio>
#include <vector>
#include <cmath>
#include <set>
#include <numeric>

struct Moon {
    int pos, vel;

    bool operator==( const Moon &other ) const {
        return pos == other.pos && vel == other.vel;
    }

    bool operator<( const Moon &other ) const {
        if ( pos == other.pos ) {
            return vel < other.vel;
        } else {
            return pos < other.pos;
        }
    }
};

struct State {
    std::vector< Moon > moons;

    bool operator<( const State &other ) const {
        // No bounds checking - crash and burn if vector sizes are different
        for ( int i = 0; i < moons.size(); ++i ) {
            if ( !( moons[ i ] == other.moons[ i ] )) {
                return moons[ i ] < other.moons[ i ];
            }
        }
        return false;
    }
};

void applyGravity( int pa, int pb, int &va, int &vb ) {
    if ( pa < pb ) {
        ++va;
        --vb;
    } else if ( pa > pb ) {
        --va;
        ++vb;
    }
}

void next( State &state ) {
    for ( int i = 0; i < state.moons.size(); ++i ) {
        for ( int j = i + 1; j < state.moons.size(); ++j ) {
            auto &first = state.moons[ i ];
            auto &second = state.moons[ j ];

            applyGravity( first.pos, second.pos, first.vel, second.vel );
        }
    }

    for ( auto &moon : state.moons ) {
        moon.pos += moon.vel;
    }
}

int main() {
    State xstate, ystate, zstate;
    int x, y, z;
    while ( std::scanf( "<x=%d, y=%d, z=%d>\n", &x, &y, &z ) == 3 ) {
        xstate.moons.push_back( { x, 0 } );
        ystate.moons.push_back( { y, 0 } );
        zstate.moons.push_back( { z, 0 } );
    }

    std::set< State > xhistory, yhistory, zhistory;
    xhistory.insert( xstate );
    yhistory.insert( ystate );
    zhistory.insert( zstate );

    bool xdone, ydone, zdone;
    xdone = ydone = zdone = false;

    while ( !xdone || !ydone || !zdone ) {
        next( xstate );
        next( ystate );
        next( zstate );

        auto check = xhistory.insert( xstate );
        if ( !check.second ) {
            xdone = true;
        }

        check = yhistory.insert( ystate );
        if ( !check.second ) {
            ydone = true;
        }

        check = zhistory.insert( zstate );
        if ( !check.second ) {
            zdone = true;
        }
    }

    auto firstRepeatIteration = std::lcm( xhistory.size(), std::lcm( yhistory.size(), zhistory.size()));
    std::printf( "%ld\n", firstRepeatIteration );

    return 0;
}