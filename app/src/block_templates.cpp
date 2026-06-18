#include "block_templates.hpp"

namespace dsl::templates{
    
    std::string SumBlockTemplate::generate_expression(const types::Block &block, const signal_map_t &signal_sources) const {
        std::string signs = "+";
        if (block.parameters.contains(constants::param_inputs)) {
            signs = block.parameters.at(constants::param_inputs.data());
        } else if (block.parameters.contains(constants::param_ports)) {
            std::string ports_str = block.parameters.at(constants::param_ports.data());
            if (!ports_str.empty() && ports_str.front() == '[') {
                int input_count = ports_str[1] - '0'; 
                if (input_count > 0) signs = std::string(input_count, '+');
            }
        }

        if (!signal_sources.contains(block.sid)) return "0.0";
        const auto & ports = signal_sources.at(block.sid);
        
        std::ostringstream expr;
        int current_port = 1;

        for (size_t i = 0; i < signs.length(); ++i) {
            char sign = signs[i];
            std::string src_signal = ports.contains(current_port) ? ports.at(current_port) : "0.0";

            if (i == 0) {
                if (sign == '-') expr << "-";
                expr << src_signal;
            } else {
                expr << (sign == '-' ? " - " : " + ") << src_signal;
            }
            current_port++;
        }
        return expr.str();
    }

    std::string GainBlockTemplate::generate_expression(const types::Block &block, const signal_map_t &signal_sources) const {
        if (!signal_sources.contains(block.sid) || !signal_sources.at(block.sid).contains(1)) {
            return "0.0";
        }
        std::string src = signal_sources.at(block.sid).at(1);
        std::string val = block.parameters.contains(constants::param_gain) ? block.parameters.at(constants::param_gain.data()) : "1.0";
        return src + " * " + val;
    }

    std::string UnitDelayBlockTemplate::generate_expression(const types::Block &block, const signal_map_t &signal_sources) const {
        return std::string(constants::code_prefix) + block.clean_name;
    }

}