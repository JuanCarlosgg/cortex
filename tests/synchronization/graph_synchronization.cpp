//
// Created by jc on 5/11/24.
//




#include "dsr/api/dsr_api.h"
#include "../utils.h"
#include <thread>

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"

using namespace DSR;
using namespace std::chrono_literals;

TEST_CASE("Connect and receive the graph from other agent", "[SYNCHRONIZATION][GRAPH]"){


    auto filename = GENERATE(make_edge_config_file, make_empty_config_file);
    auto ctx = filename();
    auto id1 = rand() % 1000;
    auto id2 = id1 + 1;
    DSRGraph G(random_string(10), id1, ctx);
    DSRGraph G2(random_string(11), id2);
    std::this_thread::sleep_for(200ms);
    REQUIRE(G2.size() == G.size());
    
}