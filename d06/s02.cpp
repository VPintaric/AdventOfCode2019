//
// Created by wex on 12/2/19.
//

#include <cstdio>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include <limits>

class Node {
public:
    std::vector< Node * > children;
    Node *parent{ nullptr };

    int findShortestPathTo( Node *destination ) {
        std::set< Node * > visited;
        visited.insert( this );

        return parent->findShortestPathToWithVisited( destination, visited ) + 1;
    }

private:

    int findShortestPathToWithVisited( Node *destination, std::set< Node * > &visited ) {
        if ( this == destination ) {
            return 0;
        }

        visited.insert( this );

        std::vector< Node * > toVisit;
        if ( parent != nullptr && visited.find( parent ) == visited.end()) {
            toVisit.push_back( parent );
        }

        for ( auto &child : children ) {
            if ( visited.find( child ) == visited.end()) {
                toVisit.push_back( child );
            }
        }

        auto minDistance = -1;
        for ( auto &node : toVisit ) {
            auto distance = node->findShortestPathToWithVisited( destination, visited );
            if ( distance != -1 ) {
                if ( minDistance == -1 ) {
                    minDistance = distance;
                } else {
                    minDistance = std::min( minDistance, distance );
                }
            }
        }

        visited.erase( this );

        if ( minDistance == -1 ) {
            return -1;
        }
        return minDistance + 1;
    }
};

int convertToLabel( const char *name ) {
    int label = 0;

    label += name[ 0 ] << 16;
    label += name[ 1 ] << 8;
    label += name[ 2 ];

    return label;
}

int main() {
    char orbiter[4] = { 0 }, orbited[4] = { 0 };
    std::map< int, std::unique_ptr< Node > > objects;

    while ( std::scanf( "%3s)%3s\n", orbited, orbiter ) == 2 ) {
        auto orbitedLabel = convertToLabel( orbited );
        auto orbiterLabel = convertToLabel( orbiter );

        Node *parent, *child;

        auto ret = objects.insert( std::make_pair( orbitedLabel, std::make_unique< Node >()));
        parent = ret.first->second.get();

        ret = objects.insert( std::make_pair( orbiterLabel, std::make_unique< Node >()));
        child = ret.first->second.get();

        parent->children.push_back( child );
        child->parent = parent;
    }

    auto youLabel = convertToLabel( "YOU" );
    auto santaLabel = convertToLabel( "SAN" );

    auto me = objects.find( youLabel )->second.get();
    auto santa = objects.find( santaLabel )->second.get();

    std::printf( "%d\n", me->findShortestPathTo( santa ) - 2 );

    return 0;
}