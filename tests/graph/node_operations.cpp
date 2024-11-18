//
// Created by jc on 5/11/24.
//


#include "catch2/catch_test_macros.hpp"

#include "dsr/core/types/user_types.h"

#include "dsr/api/dsr_api.h"
#include "../utils.h"
#include <optional>


using namespace DSR;


TEST_CASE("Graph node operations", "[NODE]") {


    auto filename = make_empty_config_file();
    DSRGraph G(random_string(10), rand() % 1200, filename);

    SECTION("Try to get a node that does not exists by a id") {
        std::optional<Node> n_id = G.get_node(random_number());
        REQUIRE_FALSE(n_id.has_value());
    }

    SECTION("Try to get a node that does not exists by a name") {
        std::optional<Node> n_name = G.get_node(random_string());
        REQUIRE_FALSE(n_name.has_value());
    }

    SECTION("Insert a new node and get it by id and name") {
        auto node_name = random_string();
        auto n = Node::create<testtype_node_type>(node_name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());
        std::optional<Node> n_id = G.get_node(r.value());
        REQUIRE(n_id.has_value());
        std::optional<Node> n_name = G.get_node(node_name);
        REQUIRE(n_name.has_value());
    }

    SECTION("Create a node with an invalid type") {
        Node n;
        REQUIRE_THROWS(n.type(random_string(100)));
    }

    SECTION("Update existing node") {
        auto node_name = random_string();
        auto n = Node::create<testtype_node_type>(node_name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());

        std::optional<Node> n_id = G.get_node(node_name);
        REQUIRE(n_id.has_value());
        G.add_attrib_local<level_att>(n_id.value(), 1);
        r = G.update_node(n_id.value());
        REQUIRE(r);

        REQUIRE(G.remove_attrib_local(*n_id, "level"));
        G.update_node(*n_id);
        REQUIRE(n_id->attrs().find("level") == n_id->attrs().end());
        REQUIRE_FALSE(G.remove_attrib_local(n_id.value(), "level"));
    }

    SECTION("Can't update an existent node with different id") {
        auto node_name = random_string();
        auto n = Node::create<testtype_node_type>(node_name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());

        n.id(random_number());

        REQUIRE_THROWS(G.update_node(n));
    }

    SECTION("Can't update an existent node with different name") {
        auto node_name = random_string();
        auto n = Node::create<testtype_node_type>(node_name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());

        n.name(random_string());

        REQUIRE_THROWS(G.update_node(n));
    }

    SECTION("Delete existing node by id") {
        auto node_name = random_string();
        auto n = Node::create<testtype_node_type>(node_name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());

        std::optional<Node> n_ = G.get_node(node_name);
        REQUIRE(n_.has_value());
        bool deleted = G.delete_node(n_->id());
        REQUIRE(deleted);
        REQUIRE(G.get_node(n_->name()) == std::nullopt);
    }


    SECTION("Delete existing node by name") {
        auto node_name = random_string();
        auto n = Node::create<testtype_node_type>(node_name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());

        std::optional<Node> n_ = G.get_node(node_name);
        REQUIRE(n_.has_value());
        bool deleted = G.delete_node(node_name);
        REQUIRE(deleted);
        REQUIRE(G.get_node(n_->name()) == std::nullopt);

    }

    SECTION("Delete a node that does not exists by id") {
        bool r = G.delete_node(rand());
        REQUIRE_FALSE(r);
    }

    SECTION("Create a node with an user defined name") {
        auto name = random_string();
        Node n;
        n.id(1550);
        n.type( "testtype");
        n.name(name);
        std::optional<uint64_t> r  = G.insert_node(n);
        REQUIRE(r.has_value());
        REQUIRE(G.get_node(name).has_value());
    }
}


TEST_CASE("Node creation", 
          "[Node]") {


    SECTION("Runtime set of a node type") {
        Node n;
        REQUIRE_NOTHROW(n.type("testtype"));
        REQUIRE_THROWS(n.type("invalidtype"));
    }
}