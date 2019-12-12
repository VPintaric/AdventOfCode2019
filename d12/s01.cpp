#include <cstdio>
#include <vector>
#include <cmath>

struct Vec {
    int x, y, z;
};

struct Moon {
    Vec pos, vel;
};

struct State {
    std::vector< Moon > moons;

    void print() const {
        for ( const auto &moon : moons ) {
            std::printf( "pos: <x=%3d, y=%3d, z=%3d>, vel: <x=%3d, y=%3d, z=%3d>\n",
                         moon.pos.x, moon.pos.y, moon.pos.z, moon.vel.x, moon.vel.y, moon.vel.z );
        }
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

            applyGravity( first.pos.x, second.pos.x, first.vel.x, second.vel.x );
            applyGravity( first.pos.y, second.pos.y, first.vel.y, second.vel.y );
            applyGravity( first.pos.z, second.pos.z, first.vel.z, second.vel.z );
        }
    }

    for ( auto &moon : state.moons ) {
        moon.pos.x += moon.vel.x;
        moon.pos.y += moon.vel.y;
        moon.pos.z += moon.vel.z;
    }
}

int energy( const State &state ) {
    int total = 0;
    for ( const auto &moon : state.moons ) {
        auto potential = std::abs( moon.pos.x ) + std::abs( moon.pos.y ) + std::abs( moon.pos.z );
        auto kinetic = std::abs( moon.vel.x ) + std::abs( moon.vel.y ) + std::abs( moon.vel.z );

        total += potential * kinetic;
    }

    return total;
}

int main() {
    Vec pos{};
    State state;
    while ( std::scanf( "<x=%d, y=%d, z=%d>\n", &pos.x, &pos.y, &pos.z ) == 3 ) {
        Moon moon{ pos, { 0, 0, 0 }};
        state.moons.push_back( moon );
    }

    for ( int i = 0; i < 1000; ++i ) {
        next( state );
    }

    state.print();
    std::printf( "Energy: %d\n", energy( state ));

    return 0;
}