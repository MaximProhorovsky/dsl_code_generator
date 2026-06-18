#ifndef NWOCG_DSL_DIAGRAM_TYPES_HPP
#define NWOCG_DSL_DIAGRAM_TYPES_HPP

#include "dsl_constants.hpp"

#include <string>
#include <vector>
#include <map>

namespace dsl::types {

    enum class BlockType {
        inport,
        outport,
        sum,
        gain,
        unit_delay,
        unknown
    };

    inline BlockType string_to_block_type(std::string_view type_str) {
        if (type_str == constants::type_inport)     return BlockType::inport;
        if (type_str == constants::type_outport)    return BlockType::outport;
        if (type_str == constants::type_sum)        return BlockType::sum;
        if (type_str == constants::type_gain)       return BlockType::gain;
        if (type_str == constants::type_unit_delay) return BlockType::unit_delay;
        return BlockType::unknown;
    }

    struct Port {
        int number{0};
        std::string name;
    };

    struct Block {
        BlockType type{BlockType::unknown};
        std::string type_str;
        std::string name;
        std::string clean_name;
        std::string sid;
        std::string position;
        std::vector<Port> ports;
        std::map<std::string, std::string, std::less<>> parameters;
    };

    struct Connection {
        std::string target_sid;
        int port_number{1};
    };

    struct Line {
        std::string name;
        std::string src_sid;
        int src_port{1};
        std::string dst_sid;
        int dst_port{1};
        std::vector<Connection> branches;
    };

    struct Model {
        std::vector<Block> blocks;
        std::vector<Line> lines;
    };

}

#endif //NWOCG_DSL_DIAGRAM_TYPES_HPP