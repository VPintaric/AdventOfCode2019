#include <iostream>
#include <vector>
#include <map>
#include <sstream>

struct Ingredient {
    long long amount;
    std::string name;
};

struct Recipe {
    long long amount;
    std::string name;
    std::vector< Ingredient > ingredients;

    void print() {
        for ( auto &ingredient : ingredients ) {
            std::cout << ingredient.amount << " " << ingredient.name << ", ";
        }
        std::cout << "=> " << amount << " " << name << std::endl;
    }
};

using Recipes = std::map< std::string, Recipe >;
using Leftovers = std::map< std::string, long long >;

long long
howMuchOreFor( const Recipes &recipes, long long amount, const std::string &materialName, Leftovers &leftovers ) {
    const std::string ORE = "ORE";

    if ( amount <= 0 ) {
        return 0;
    }

    // check if any leftovers
    auto leftover = leftovers.find( materialName );
    if ( leftover != leftovers.end() && leftover->second > 0 ) {
        auto leftoversToTake = std::min( amount, leftover->second );

        leftover->second -= leftoversToTake;
        amount -= leftoversToTake;
    }

    if ( amount <= 0 ) {
        return 0;
    }

    auto recipe = recipes.find( materialName );
    auto nReactions = ( amount / recipe->second.amount ) + (( amount % recipe->second.amount ) != 0 );
    auto actualAmount = nReactions * recipe->second.amount;

    long long oreAmount = 0;
    for ( auto &ingredient : recipe->second.ingredients ) {
        if ( ingredient.name == ORE ) {
            oreAmount += nReactions * ingredient.amount;
        } else {
            oreAmount += howMuchOreFor( recipes, nReactions * ingredient.amount, ingredient.name, leftovers );
        }
    }

    leftovers[ materialName ] = actualAmount - amount;

    return oreAmount;
}

int main() {
    Recipes recipes;

    {
        std::string reaction;
        while ( std::getline( std::cin, reaction )) {
            if ( reaction.empty()) {
                break;
            }

            Recipe recipe = Recipe{};

            std::stringstream reactionStream( reaction );

            std::string ingredients;
            std::getline( reactionStream, ingredients, '=' );

            std::stringstream ingredientsStream( ingredients );

            std::string ingredient;
            while ( std::getline( ingredientsStream, ingredient, ',' )) {
                std::stringstream ingredientStream( ingredient );

                long long amnt;
                std::string name;
                ingredientStream >> amnt >> name;
                recipe.ingredients.push_back( Ingredient{ amnt, name } );
            }

            reactionStream.ignore( 1 ); // ignore the '>'

            reactionStream >> recipe.amount >> recipe.name;
            recipes[ recipe.name ] = recipe;
        }
    }

    Leftovers leftovers;
    long long lo = 1, hi = 1000000000;
    constexpr long long ORE_MAX = 1000000000000;
    while ( lo < hi ) {
        auto fuelAmount = ( lo + hi + 1 ) / 2;

        leftovers.clear();
        auto neededOre = howMuchOreFor( recipes, fuelAmount, "FUEL", leftovers );

        if ( neededOre < ORE_MAX ) {
            lo = fuelAmount;
        } else {
            hi = fuelAmount - 1;
        }
    }

    std::cout << lo << std::endl;

    return 0;
}