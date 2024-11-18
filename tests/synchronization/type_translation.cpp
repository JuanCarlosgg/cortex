
#include "dsr/api/dsr_api.h"
#include "../utils.h"
#include <cstdint>
#include <optional>

#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "dsr/core/crdt/delta_crdt.h"
#include "dsr/core/topics/IDLGraph.hpp"
#include "dsr/core/types/common_types.h"
#include "dsr/core/types/crdt_types.h"
#include "dsr/core/types/translator.h"
#include "dsr/core/types/type_checking/dsr_edge_type.h"
#include "dsr/core/types/type_checking/dsr_node_type.h"
#include "dsr/core/types/user_types.h"

using namespace DSR;



TEST_CASE("NODE: from DSR representation to CRDT to IDL", "[TRANSLATION][NODE]"){

    uint32_t agent_id = random_number();
    auto id = random_number();
    auto name = random_string();


    static auto new_attribute__ = [&]() -> std::pair<std::string, DSR::Attribute> {

        auto val = random_choose(std::vector<ValType>{
            (int)12, 
            random_string(), 
            std::vector<float>{1.0, 2.0, 3.0}
        });
        Attribute attr(val, random_number(), (uint32_t)random_number());
        return std::make_pair(random_string(), attr);
    };


    auto attributes = GENERATE(std::map<std::string, DSR::Attribute>{}, 
                               std::map<std::string, DSR::Attribute>{new_attribute__(), new_attribute__(), new_attribute__(), new_attribute__()});

    static auto new_edge__ = [&]() -> std::pair<std::pair<uint64_t, std::string>, DSR::Edge> {

        auto type = random_choose(std::vector<std::string>{"in", "RT", "reachable", "visible"});
        auto to = random_number();
        auto key = std::pair<uint64_t, std::string>{to, type};
        auto edge = Edge();
        edge.to(to);
        edge.from(id);
        edge.type(type);
        edge.agent_id(random_number());
        edge.attrs(attributes);
        return std::make_pair(key, edge);
    };

    auto fano = GENERATE(std::map<std::pair<uint64_t, std::string>, DSR::Edge>{}, 
                         std::map<std::pair<uint64_t, std::string>, DSR::Edge>{
                            new_edge__(), new_edge__(),new_edge__()
                            });

    SECTION("User Node representation"){
    
        auto node = Node::create<robot_node_type> (attributes, fano);
        node.id(id);
        node.name(name);
        node.agent_id(agent_id);

        REQUIRE(node.fano() == fano);
        REQUIRE(node.attrs() == attributes);
        REQUIRE(node.id() == id);
        REQUIRE(node.name() == name);
        REQUIRE(node.agent_id() == agent_id);
        
        
        SECTION("Copy node"){
            CRDTNode crdt_node = user_node_to_crdt(node);
            REQUIRE(crdt_node.attrs().size() == attributes.size());
            REQUIRE(crdt_node.fano().size() == fano.size());
            REQUIRE(crdt_node.id() == id);
            REQUIRE(crdt_node.name() == name);
            REQUIRE(crdt_node.agent_id() == agent_id);
    
            mvreg<CRDTNode> mvreg_node;
            auto delta = mvreg_node.write(crdt_node);
            
            REQUIRE(mvreg_node.read_reg() == crdt_node);
            REQUIRE(delta.read_reg() == crdt_node);

            auto& reg = mvreg_node.read_reg();
            REQUIRE(reg.attrs().size() == attributes.size());
            REQUIRE(reg.fano().size() == fano.size());
            REQUIRE(reg.id() == id);
            REQUIRE(reg.name() == name);
            REQUIRE(reg.agent_id() == agent_id);


            IDL::IDLNode idl_node = reg.to_IDL_node(id);
            REQUIRE(idl_node.attrs().size() == attributes.size());
            REQUIRE(idl_node.fano().size() == fano.size());
            REQUIRE(idl_node.id() == id);
            REQUIRE(idl_node.name() == name);
            REQUIRE(idl_node.agent_id() == agent_id);
            REQUIRE(idl_node.type() == robot_node_type::attr_name);

        }

        SECTION("Move node"){
            
            CRDTNode crdt_node = user_node_to_crdt(std::move(node));
            REQUIRE(crdt_node.attrs().size() == attributes.size());
            REQUIRE(crdt_node.fano().size() == fano.size());
            REQUIRE(crdt_node.id() == id);
            REQUIRE(crdt_node.name() == name);
            REQUIRE(crdt_node.agent_id() == agent_id);

            mvreg<CRDTNode> mvreg_node;
            auto delta = mvreg_node.write(std::move(crdt_node));

            auto& reg = mvreg_node.read_reg();
            REQUIRE(delta.read_reg() == reg);
            REQUIRE(reg.attrs().size() == attributes.size());
            REQUIRE(reg.fano().size() == fano.size());
            REQUIRE(reg.id() == id);
            REQUIRE(reg.name() == name);
            REQUIRE(reg.agent_id() == agent_id);

            IDL::IDLNode idl_node = reg.to_IDL_node(id);
            REQUIRE(idl_node.attrs().size() == attributes.size());
            REQUIRE(idl_node.fano().size() == fano.size());
            REQUIRE(idl_node.id() == id);
            REQUIRE(idl_node.name() == name);
            REQUIRE(idl_node.agent_id() == agent_id);
            REQUIRE(idl_node.type() == robot_node_type::attr_name);
        }

    }


}



TEST_CASE("EDGE: from DSR representation to CRDT to IDL", "[TRANSLATION][EDGE]"){

    uint32_t agent_id = random_number();
    auto from = random_number();
    auto to = random_number();


    static auto new_attribute__ = [&]() -> std::pair<std::string, DSR::Attribute> {

        auto val = random_choose(std::vector<ValType>{
            (int)12, 
            random_string(), 
            std::vector<float>{1.0, 2.0, 3.0}
        });
        Attribute attr(val, random_number(), (uint32_t)random_number());
        return std::make_pair(random_string(), attr);
    };


    auto attributes = GENERATE(std::map<std::string, DSR::Attribute>{}, 
                               std::map<std::string, DSR::Attribute>{new_attribute__(), new_attribute__(), new_attribute__(), new_attribute__()});

    SECTION("User Edge representation"){
    
        auto edge = Edge::create<in_edge_type> (from, to, attributes);
        edge.agent_id(agent_id);

        REQUIRE(edge.attrs() == attributes);
        REQUIRE(edge.from() == from);
        REQUIRE(edge.to() == to);
        REQUIRE(edge.type() == in_edge_type_str);
        REQUIRE(edge.agent_id() == agent_id);
        
        
        SECTION("Copy edge"){
            CRDTEdge crdt_edge = user_edge_to_crdt(edge);
            REQUIRE(crdt_edge.attrs().size() == attributes.size());
            REQUIRE(crdt_edge.from() == from);
            REQUIRE(crdt_edge.to() == to);
            REQUIRE(crdt_edge.type() == in_edge_type_str);
            REQUIRE(crdt_edge.agent_id() == agent_id);
    
            mvreg<CRDTEdge> mvreg_edge;
            auto delta = mvreg_edge.write(crdt_edge);
            
            REQUIRE(mvreg_edge.read_reg() == crdt_edge);
            REQUIRE(delta.read_reg() == crdt_edge);

            auto& reg = mvreg_edge.read_reg();
            REQUIRE(reg.attrs().size() == attributes.size());
            REQUIRE(reg.from() == from);
            REQUIRE(reg.to() == to);
            REQUIRE(reg.type() == in_edge_type_str);
            REQUIRE(reg.agent_id() == agent_id);


            IDL::IDLEdge idl_node = reg.to_IDL_edge(from);
            REQUIRE(idl_node.attrs().size() == attributes.size());
            REQUIRE(idl_node.from() == from);
            REQUIRE(idl_node.to() == to);
            REQUIRE(idl_node.agent_id() == agent_id);
            REQUIRE(idl_node.type() == in_edge_type_str);

        }

        SECTION("Move edge"){
            
            CRDTEdge crdt_edge = user_edge_to_crdt(edge);
            REQUIRE(crdt_edge.attrs().size() == attributes.size());
            REQUIRE(crdt_edge.from() == from);
            REQUIRE(crdt_edge.to() == to);
            REQUIRE(crdt_edge.type() == in_edge_type_str);
            REQUIRE(crdt_edge.agent_id() == agent_id);
    
            mvreg<CRDTEdge> mvreg_edge;
            auto delta = mvreg_edge.write(crdt_edge);
            
            REQUIRE(delta.read_reg() == mvreg_edge.read_reg());

            auto& reg = mvreg_edge.read_reg();
            REQUIRE(reg.attrs().size() == attributes.size());
            REQUIRE(reg.from() == from);
            REQUIRE(reg.to() == to);
            REQUIRE(reg.type() == in_edge_type_str);
            REQUIRE(reg.agent_id() == agent_id);


            IDL::IDLEdge idl_node = reg.to_IDL_edge(from);
            REQUIRE(idl_node.attrs().size() == attributes.size());
            REQUIRE(idl_node.from() == from);
            REQUIRE(idl_node.to() == to);
            REQUIRE(idl_node.agent_id() == agent_id);
            REQUIRE(idl_node.type() == in_edge_type_str);
        }

    }


}