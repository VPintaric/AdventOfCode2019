//
// Created by wex on 12/2/19.
//

#include <cstdio>
#include <vector>
#include <map>
#include <memory>

class Node {
public:
    std::vector< Node * > children;
    Node *parent{ nullptr };

    Node *findParent() {
        if ( parent == nullptr ) {
            return this;
        }
        return parent->findParent();
    }

    int getNumberOfOrbits() {
        int sum = 0;
        for ( auto &child : children ) {
            sum += child->orbits( 1 );
        }
        return sum;
    }

private:

    int orbits( int depth ) {
        int sum = 0;
        for ( auto &child : children ) {
            sum += child->orbits( depth + 1 );
        }
        return depth + sum;
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

    auto head = objects.begin()->second->findParent();

    std::printf( "%d\n", head->getNumberOfOrbits());

    return 0;
}