#ifndef NWOCG_DSL_DIAGRAM_CONSTANTS_HPP
#define NWOCG_DSL_DIAGRAM_CONSTANTS_HPP

#include <string_view>

namespace dsl::constants {
    
    using std::string_view_literals::operator""sv;

    constexpr auto tag_system = "System"sv;
    constexpr auto tag_block  = "Block"sv;
    constexpr auto tag_port   = "Port"sv;
    constexpr auto tag_line   = "Line"sv;
    constexpr auto tag_p      = "P"sv;
    constexpr auto tag_branch = "Branch"sv;

    constexpr auto attr_block_type = "BlockType"sv;
    constexpr auto attr_name       = "Name"sv;
    constexpr auto attr_sid        = "SID"sv;

    constexpr auto type_inport     = "Inport"sv;
    constexpr auto type_outport    = "Outport"sv;
    constexpr auto type_sum        = "Sum"sv;
    constexpr auto type_gain       = "Gain"sv;
    constexpr auto type_unit_delay = "UnitDelay"sv;

    constexpr auto param_position    = "Position"sv;
    constexpr auto param_port_number = "PortNumber"sv;
    constexpr auto param_ports       = "Ports"sv;
    constexpr auto param_inputs      = "Inputs"sv;
    constexpr auto param_gain        = "Gain"sv;
    constexpr auto param_src         = "Src"sv;
    constexpr auto param_dst         = "Dst"sv;

    constexpr auto code_prefix  = "nwocg."sv;

}

#endif //NWOCG_DSL_DIAGRAM_CONSTANTS_HPP