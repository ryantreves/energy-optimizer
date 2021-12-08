#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
#include "vector.h"
#include "strlib.h"

using namespace std;

struct Generator {
    int capacity;
    int emissions;
    int cost;
};


// Solution B Helper
void portfolioHelperPrunes(Vector<Generator>& generators, int demand, string optimize, int current, int& best,
                     Vector<Generator>& currentPortfolio,
                     Vector<Generator>& bestPortfolio) {
    // Pruning
    if (best != -1 && current > best) {
        return;
    }

    // Base cases
    if (demand <= 0 || generators.isEmpty()) {
        if (current != 0 && (best == -1 || best > current)) {
            best = current;
            bestPortfolio = currentPortfolio;
        }
        return;
    }

    // Examine the last generator
    Generator gen = generators.remove(generators.size()-1);

    // Recurse through remaining generators without adding it
    portfolioHelper(generators, demand, optimize, current, best, currentPortfolio, bestPortfolio);

    // Add either cost or emissions to current total
    if (optimize == "budget") {
        current += gen.cost;
    }
    else {
        current += gen.emissions;
    }
    currentPortfolio.add(gen);
    // Recurse through remaining generators with adding it
    portfolioHelper(generators, demand - gen.capacity, optimize, current, best, currentPortfolio, bestPortfolio);
    currentPortfolio.remove(currentPortfolio.size() - 1);

    generators.add(gen);

}


// Solution B
Vector<Generator> buildPortfolioPrunes(Vector<Generator> generators, string optimize, int demand) {
    int best = -1;
    Vector<Generator> currentPortfolio = {};
    Vector<Generator> bestPortfolio = {};
    portfolioHelperPrunes(generators, demand, optimize, 0, best, currentPortfolio, bestPortfolio);

    return bestPortfolio;
}





// Test cases
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STUDENT_TEST("Solution A edge cases: one generator, demand = 0, or no generators") {
    Vector<Generator> generators = {};
    Vector<Generator> portfolio = buildPortfolio(generators, "budget", 0);
    EXPECT(portfolio.isEmpty());

    generators.add({1, 2, 3});
    portfolio = buildPortfolio(generators, "budget", 2);
    EXPECT_EQUAL(portfolio[0].capacity, 1);
    EXPECT_EQUAL(portfolio[0].emissions, 2);
    EXPECT_EQUAL(portfolio[0].cost, 3);

    portfolio = buildPortfolio(generators, "emissions", 0);
    EXPECT(portfolio.isEmpty());

}

STUDENT_TEST("Solution A simple test: choose best generator of two") {
    Vector<Generator> generators = {};
    Vector<Generator> exp = {};

    // Expensive, low-emitter
    Generator gen1 = {4, 2, 5};
    generators.add(gen1);

    // Cheap, high-emitter
    Generator gen2 = {4, 10, 2};
    generators.add(gen2);

    Vector<Generator> portfolio = buildPortfolio(generators, "budget", 4);
    for (int i = 0; i < portfolio.size(); i++) {
        cout << portfolio[i].capacity << portfolio[i].emissions << portfolio[i].cost << endl;
    }
    exp.add(gen2);

    EXPECT_EQUAL(exp[0].cost, portfolio[0].cost);
    EXPECT_EQUAL(exp[0].emissions, portfolio[0].emissions);
    EXPECT_EQUAL(exp[0].capacity, portfolio[0].capacity);
    exp.clear();

    portfolio = buildPortfolio(generators, "emissions", 4);
    exp.add(gen1);

    EXPECT(exp[0].cost == portfolio[0].cost);
    EXPECT(exp[0].emissions == portfolio[0].emissions);
    EXPECT(exp[0].capacity == portfolio[0].capacity);
}

STUDENT_TEST("Solution A larger test") {
    Vector<Generator> generators = {};
    Vector<Generator> exp = {};

    // Expensive, low-emitter
    Generator gen1 = {4, 2, 5};
    generators.add(gen1);

    // Cheap, high-emitter
    Generator gen2 = {3, 10, 2};
    generators.add(gen2);

    // Cheap, low-emitter
    Generator gen3 = {2, 2, 2};
    generators.add(gen3);

    // Expensive, high-emitter
    Generator gen4 = {8, 10, 15};
    generators.add(gen4);

    // Cheap, high-emitter
    Generator gen5 = {3, 17, 1};
    generators.add(gen5);

    Vector<Generator> portfolio = buildPortfolio(generators, "budget", 9);

    exp.add(gen5);
    exp.add(gen2);
    exp.add(gen1);

    for (int i = 0; i < portfolio.size(); i++) {
        EXPECT_EQUAL(portfolio[i].capacity, exp[i].capacity);
        EXPECT_EQUAL(portfolio[i].emissions, exp[i].emissions);
        EXPECT_EQUAL(portfolio[i].cost, exp[i].cost);
    }

    portfolio = buildPortfolio(generators, "emissions", 9);

    exp.clear();
    exp.add(gen3);
    exp.add(gen2);
    exp.add(gen1);

    for (int i = 0; i < portfolio.size(); i++) {
        EXPECT_EQUAL(portfolio[i].capacity, exp[i].capacity);
        EXPECT_EQUAL(portfolio[i].emissions, exp[i].emissions);
        EXPECT_EQUAL(portfolio[i].cost, exp[i].cost);
    }
}

STUDENT_TEST("Solution B simple test") {
    Vector<Generator> generators = {};
    Vector<Generator> exp = {};

    // Expensive, low-emitter
    Generator gen1 = {4, 2, 5};
    generators.add(gen1);

    // Cheap, high-emitter
    Generator gen2 = {4, 10, 2};
    generators.add(gen2);

    Vector<Generator> portfolio = buildPortfolioPrunes(generators, "budget", 4);

    exp.add(gen2);

    EXPECT_EQUAL(exp[0].cost, portfolio[0].cost);
    EXPECT_EQUAL(exp[0].emissions, portfolio[0].emissions);
    EXPECT_EQUAL(exp[0].capacity, portfolio[0].capacity);
    exp.clear();

    portfolio = buildPortfolioPrunes(generators, "emissions", 4);
    exp.add(gen1);

    EXPECT(exp[0].cost == portfolio[0].cost);
    EXPECT(exp[0].emissions == portfolio[0].emissions);
    EXPECT(exp[0].capacity == portfolio[0].capacity);
}

STUDENT_TEST("Solution B larger test") {
    Vector<Generator> generators = {};
    Vector<Generator> exp = {};

    // Expensive, low-emitter
    Generator gen1 = {4, 2, 5};
    generators.add(gen1);

    // Cheap, high-emitter
    Generator gen2 = {3, 10, 2};
    generators.add(gen2);

    // Cheap, low-emitter
    Generator gen3 = {2, 2, 2};
    generators.add(gen3);

    // Expensive, high-emitter
    Generator gen4 = {8, 10, 15};
    generators.add(gen4);

    // Cheap, high-emitter
    Generator gen5 = {3, 17, 1};
    generators.add(gen5);

    Vector<Generator> portfolio = buildPortfolioPrunes(generators, "budget", 9);

    exp.add(gen5);
    exp.add(gen2);
    exp.add(gen1);

    for (int i = 0; i < portfolio.size(); i++) {
        EXPECT_EQUAL(portfolio[i].capacity, exp[i].capacity);
        EXPECT_EQUAL(portfolio[i].emissions, exp[i].emissions);
        EXPECT_EQUAL(portfolio[i].cost, exp[i].cost);
    }

    portfolio = buildPortfolioPrunes(generators, "emissions", 9);

    exp.clear();
    exp.add(gen3);
    exp.add(gen2);
    exp.add(gen1);

    for (int i = 0; i < portfolio.size(); i++) {
        EXPECT_EQUAL(portfolio[i].capacity, exp[i].capacity);
        EXPECT_EQUAL(portfolio[i].emissions, exp[i].emissions);
        EXPECT_EQUAL(portfolio[i].cost, exp[i].cost);
    }
}

// Solution A
Vector<Generator> buildPortfolio(Vector<Generator> generators, string optimize, int demand) {
    int best = -1;
    Vector<Generator> currentPortfolio = {};
    Vector<Generator> bestPortfolio = {};
    portfolioHelper(generators, demand, optimize, 0, best, currentPortfolio, bestPortfolio);

    return bestPortfolio;
}

// Solution A Helper
void portfolioHelper(Vector<Generator>& generators, int demand, string optimize, int current, int& best,
                     Vector<Generator>& currentPortfolio,
                     Vector<Generator>& bestPortfolio) {
    // Base cases
    if (demand <= 0 || generators.isEmpty()) {
        if (current != 0 && (best == -1 || best > current)) {
            best = current;
            bestPortfolio = currentPortfolio;
        }
        return;
    }

    // Examine the first generator
    Generator gen = generators.remove(0);

    // Recurse through remaining generators without adding it
    portfolioHelper(generators, demand, optimize, current, best, currentPortfolio, bestPortfolio);

    // Add either cost or emissions to current total
    if (optimize == "budget") {
        current += gen.cost;
    }
    else {
        current += gen.emissions;
    }
    currentPortfolio.add(gen);
    // Recurse through remaining generators with adding it
    portfolioHelper(generators, demand - gen.capacity, optimize, current, best, currentPortfolio, bestPortfolio);
    currentPortfolio.remove(currentPortfolio.size() - 1);

    generators.add(gen);

}
