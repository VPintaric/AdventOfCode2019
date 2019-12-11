
#include <vector>
#include <iostream>
#include <cmath>
#include <functional>
#include <sstream>
#include <limits>
#include <algorithm>
#include <queue>
#include <list>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <map>

using bigint = long long;

constexpr int INITIAL_MEMORY = 10000;
constexpr int BLOCKS_MIN_DISTANCE = 100;

class Memory {
    std::list< std::vector< bigint > > blocks;
    std::list< bigint > blocksStartAddr;

public:
    Memory() {
        blocks.emplace_back( INITIAL_MEMORY );
        blocksStartAddr.emplace_back( 0 );
    }

    void write( const bigint &addr, const bigint &v ) {
        auto block = blocks.begin();
        auto startAddr = blocksStartAddr.begin();
        for ( ; block != blocks.end(); ++block, ++startAddr ) {
            if ( addr < *startAddr ) {

                blocks.insert( block, std::vector< bigint >( 1 ));
                blocksStartAddr.insert( startAddr, addr );
                --block;
                --startAddr;
                break;
            }

            auto blockEnd = *startAddr + block->size();
            if ( *startAddr <= addr && addr < blockEnd + BLOCKS_MIN_DISTANCE ) {
                if ( addr >= blockEnd ) {
                    block->insert( block->end(), BLOCKS_MIN_DISTANCE, bigint( 0 ));
                }
                ( *block )[ static_cast<uint32_t>( addr - *startAddr ) ] = v;
                break;
            }
        }

        if ( block == blocks.end()) {
            blocks.emplace_back( 1 );
            blocksStartAddr.emplace_back( addr );

            blocks.back()[ 0 ] = v;
        } else {
            auto nextBlock = std::next( block );
            auto nextStartAddr = std::next( startAddr );

            if ( nextBlock != blocks.end()) {
                auto blockEnd = *startAddr + block->size();
                auto sizeToIncreaseBy = static_cast<uint32_t >(*nextStartAddr - blockEnd);

                block->insert( block->end(), sizeToIncreaseBy, bigint( 0 ));
                block->insert( block->end(), nextBlock->begin(), nextBlock->end());

                blocks.erase( nextBlock );
                blocksStartAddr.erase( nextStartAddr );
            }
        }
    }

    bigint read( const bigint &addr ) {
        auto block = blocks.begin();
        auto startAddr = blocksStartAddr.begin();
        for ( ; block != blocks.end(); ++block, ++startAddr ) {
            if ( *startAddr <= addr && addr < *startAddr + block->size()) {
                return ( *block )[ static_cast<uint32_t >( addr - *startAddr) ];
            }
        }

        return bigint( 0 );
    }
};

enum ArgMode {
    POSITION = 0,
    IMMEDIATE = 1,
    RELATIVE = 2
};

enum Opcode {
    ADD = 1,
    MUL = 2,
    INP = 3,
    OUT = 4,
    JIT = 5,
    JIF = 6,
    JLT = 7,
    JEQ = 8,
    ARB = 9,
    HLT = 99,
};

class Program {
    Memory memory;
    std::queue< bigint > input;
    std::queue< bigint > output;
    bigint pc{};
    bigint rbo{};
    std::mutex iMut, oMut;
    std::condition_variable iCv, oCv;
    bool isRunning{ false };

    bigint getParam( const bigint &command, int n ) {
        auto argMode = ( command / static_cast<int>(std::pow( 10, n + 1 ))) % 10;

        switch ( static_cast<uint32_t >(argMode)) {
            case ArgMode::POSITION: {
                auto addr = memory.read( pc + n );
                return memory.read( addr );
            }
            case ArgMode::IMMEDIATE: {
                return memory.read( pc + n );
            }
            case ArgMode::RELATIVE: {
                auto addr = memory.read( pc + n );
                return memory.read( rbo + addr );
            }
            default: {
                std::cout << "Invalid arg mode" << std::endl;
                return 0;
            }
        }
    }

    bigint getDstAddr( const bigint &command, int n ) {
        auto argMode = ( command / static_cast<int>(std::pow( 10, n + 1 ))) % 10;

        switch ( static_cast<uint32_t >(argMode)) {
            case ArgMode::POSITION: {
                auto addr = memory.read( pc + n );
                return addr;
            }
            case ArgMode::IMMEDIATE: {
                std::cout << "Dst params should never be in immediate mode" << std::endl;
                return 0;
            }
            case ArgMode::RELATIVE: {
                auto addr = memory.read( pc + n );
                return rbo + addr;
            }
            default: {
                std::cout << "Invalid arg mode" << std::endl;
                return 0;
            }
        }
    }

    void run() {
        bool done = false;

        while ( !done ) {
            bigint p1, p2, dst;
            auto command = memory.read( pc );

            int opcode = command % 100;
            switch ( opcode ) {
                case Opcode::ADD:
                    p1 = getParam( command, 1 );
                    p2 = getParam( command, 2 );
                    dst = getDstAddr( command, 3 );
                    memory.write( dst, p1 + p2 );

                    pc += 4;
                    break;
                case Opcode::MUL:
                    p1 = getParam( command, 1 );
                    p2 = getParam( command, 2 );
                    dst = getDstAddr( command, 3 );
                    memory.write( dst, p1 * p2 );

                    pc += 4;
                    break;
                case Opcode::INP:
                    p1 = popInput();
                    dst = getDstAddr( command, 1 );
                    memory.write( dst, p1 );

                    pc += 2;
                    break;
                case Opcode::OUT:
                    p1 = getParam( command, 1 );
                    pushOutput( p1 );

                    pc += 2;
                    break;
                case Opcode::JIT:
                    p1 = getParam( command, 1 );
                    if ( p1 != 0 ) {
                        p2 = getParam( command, 2 );
                        pc = p2;
                    } else {
                        pc += 3;
                    }
                    break;
                case Opcode::JIF:
                    p1 = getParam( command, 1 );
                    if ( p1 == 0 ) {
                        p2 = getParam( command, 2 );
                        pc = p2;
                    } else {
                        pc += 3;
                    }
                    break;
                case Opcode::JLT:
                    p1 = getParam( command, 1 );
                    p2 = getParam( command, 2 );
                    dst = getDstAddr( command, 3 );
                    if ( p1 < p2 ) {
                        memory.write( dst, 1 );
                    } else {
                        memory.write( dst, 0 );
                    }

                    pc += 4;
                    break;
                case Opcode::JEQ:
                    p1 = getParam( command, 1 );
                    p2 = getParam( command, 2 );
                    dst = getDstAddr( command, 3 );
                    if ( p1 == p2 ) {
                        memory.write( dst, 1 );
                    } else {
                        memory.write( dst, 0 );
                    }

                    pc += 4;
                    break;
                case Opcode::ARB:
                    p1 = getParam( command, 1 );
                    rbo += p1;

                    pc += 2;
                    break;
                case Opcode::HLT:
                    done = true;
                    break;
                default:
                    std::cout << "CRASH AND BURN" << std::endl;
                    break;
            }
        }

        setIsRunning( false );
    }

    bigint popInput() {
        std::unique_lock< std::mutex > lock( iMut );
        iCv.wait( lock, [ this ] { return !input.empty(); } );

        auto v = input.front();
        input.pop();
        return v;
    }

    void pushOutput( const bigint &out ) {
        {
            std::lock_guard< std::mutex > lock( oMut );
            output.push( out );
        }
        oCv.notify_one();
    }

    void setIsRunning( bool to ) {
        {
            std::lock_guard< std::mutex > lock( oMut );
            isRunning = to;
        }
        oCv.notify_one();
    }

public:
    void reset( const Memory &initial ) {
        pc = 0;
        rbo = 0;
        memory = initial;
        std::queue< bigint >().swap( input );
        std::queue< bigint >().swap( output );
    }

    void pushInput( const bigint &in ) {
        {
            std::lock_guard< std::mutex > lock( iMut );
            input.push( in );
        }
        iCv.notify_one();
    }

    bool popOutput( bigint &out ) {
        std::unique_lock< std::mutex > lock( oMut );
        oCv.wait( lock, [ this ] { return !output.empty() || !isRunning; } );

        if ( output.empty()) {
            return false;
        }

        out = output.front();
        output.pop();
        return true;
    }

    std::thread start() {
        isRunning = true;
        return std::thread( [ this ] { this->run(); } );
    }

    bool isDone() {
        std::lock_guard< std::mutex > lock( oMut );
        return !isRunning;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Point {
    int x, y;

    bool operator<( const Point &o ) const {
        if ( x == o.x ) {
            return y < o.y;
        } else {
            return x < o.x;
        }
    }
};

int main() {
    Memory memory;
    Program program;

    std::string line;
    std::getline( std::cin, line );
    std::stringstream ss( line );

    int i = 0;
    for ( std::string s; std::getline( ss, s, ',' ); ++i ) {
        memory.write( i, std::stoll( s ));
    }

    program.reset( memory );
    auto execution = program.start();

    std::map< Point, char > grid;
    Point pos{ 0, 0 }, dir{ 0, 1 };
    grid[ pos ] = '#';
    while ( true ) {
        auto iter = grid.find( pos );
        if ( iter == grid.end() || iter->second == '.' ) {
            program.pushInput( 0 );
        } else {
            program.pushInput( 1 );
        }

        bigint colorToUse;
        if ( !program.popOutput( colorToUse )) {
            break;
        }
        if ( colorToUse == 0 ) {
            grid[ pos ] = '.';
        } else {
            grid[ pos ] = '#';
        }

        bigint turnDirection;
        if ( !program.popOutput( turnDirection )) {
            break;
        }
        if ( turnDirection == 0 ) {
            dir = Point{ -dir.y, dir.x };
        } else {
            dir = Point{ dir.y, -dir.x };
        }

        pos.x += dir.x;
        pos.y += dir.y;
    }
    execution.join();

    int maxx = -99999, minx = 99999, maxy = -99999, miny = 99999;
    for ( auto &tile : grid ) {
        maxx = std::max( tile.first.x, maxx );
        minx = std::min( tile.first.x, minx );
        maxy = std::max( tile.first.y, maxy );
        miny = std::min( tile.first.y, miny );
    }

    for ( int y = maxy; y >= miny; --y ) {
        for ( int x = minx; x <= maxx; ++x ) {
            auto iter = grid.find( Point{ x, y } );
            char color;
            if ( iter == grid.end()) {
                color = '.';
            } else {
                color = iter->second;
            }

            if ( color == '.' ) {
                color = ' ';
            }

            std::cout << color;
        }
        std::cout << std::endl;
    }

    return 0;
}