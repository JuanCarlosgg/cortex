//
// Created by jc on 5/11/24.
//

#include "../utils.h"
#include "catch2/catch_test_macros.hpp"

#include <cstdint>
#include <dsr/core/crdt/delta_crdt.h>


struct dt{

    std::string m_str;

    dt(const std::string &str) : m_str(str) {} 
    dt(std::string &&str) : m_str(str) {}
    dt(const char *str) : m_str(str) {}

    auto agent_id() -> uint32_t { return m_str.size();}
    auto operator<=>(const dt&) const = default;

};

TEST_CASE("CRDT operations and state", "[CRDT][CONSISTENCY]") {

  mvreg<dt> o1, o2, o3;
  mvreg<dt> delta, delta2, delta3, delta4;
  
  delta = o1.write("hello");
  delta2 = delta3 = delta;

  o2.join(std::move(delta));
  o3.join(std::move(delta2));
  //o1 and o2 and o3 are in the same state.
  REQUIRE(o1.read_reg().m_str == "hello");
  REQUIRE(o2.read_reg().m_str == "hello");
  REQUIRE(o3.read_reg().m_str == "hello");


  //check out of order joins. 
  delta = o2.write("world");
  REQUIRE(o2.read_reg().m_str == "world");
  delta = o2.write("hello world");
  delta3 = delta;
  REQUIRE(o2.read_reg().m_str == "hello world");

  delta2 = o3.write("I'm o3");
  REQUIRE(o3.read_reg().m_str == "I'm o3");

  o1.join(std::move(delta));
  REQUIRE(o1.read_reg().m_str == "hello world");
  o1.join(std::move(delta2));
  //o2 is newer than o3 operation (the implementation uses a counter).
  REQUIRE(o1.read_reg().m_str == "hello world");
  o3.join(std::move(delta3));
  REQUIRE(o3.read_reg().m_str == "hello world");

  //check that in can solve conflicts (same counter) automatically using the smaller agent_id() method of the datatype. 
  delta  = o2.write("~~~~~~~~~~~~");
  delta2 = o3.write("I win");
  delta3 = delta2;
  delta4 = delta;
  o1.join(std::move(delta));
  REQUIRE(o1.read_reg().m_str == "~~~~~~~~~~~~");
  o1.join(std::move(delta2));
  o2.join(std::move(delta3));  
  o3.join(std::move(delta4));
  REQUIRE(o1.read_reg().m_str == "I win");
  REQUIRE(o2.read_reg().m_str == "I win");
  REQUIRE(o3.read_reg().m_str == "I win");

}
