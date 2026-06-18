#include "block_templates.hpp"
#include "dsl_constants.hpp"
#include "c_code_generator.hpp"

namespace dsl {

    CCodeGenerator::CCodeGenerator() {
        templates_registry_[types::BlockType::sum]  = std::make_unique<templates::SumBlockTemplate>();
        templates_registry_[types::BlockType::gain] = std::make_unique<templates::GainBlockTemplate>();
        templates_registry_[types::BlockType::unit_delay] = std::make_unique<templates::UnitDelayBlockTemplate>();
    }

    void CCodeGenerator::generate(const types::Model & model, const std::vector<std::string> & order, IWriter &writer) {
        block_map_t block_map;
        for (const auto & b : model.blocks) {
            block_map[b.sid] = &b;
        }

        signal_map_t signal_sources;
        build_signal_maps(model, block_map, signal_sources);

        writer.open();
        write_header_includes(writer);
        write_struct_definition(writer, model);
        write_init_function(writer, model);
        write_step_function(writer, block_map, order, signal_sources);
        write_ext_ports(writer, model);
        writer.close();
    }

    void CCodeGenerator::build_signal_maps(const types::Model &model, const block_map_t &block_map, signal_map_t &signal_sources) {
        for (const auto & line : model.lines) {
            if (!block_map.contains(line.src_sid)) continue;
            
            auto src_block = block_map.at(line.src_sid);

            std::string expr = std::string(constants::code_prefix) + src_block->clean_name;

            if (!line.dst_sid.empty()) {
                signal_sources[line.dst_sid][line.dst_port] = expr;
            }
            for (const auto & br : line.branches) {
                signal_sources[br.target_sid][br.port_number] = expr;
            }
        }
    }

    void CCodeGenerator::write_header_includes(IWriter &writer) {
        writer.write("#include \"nwocg_run.h\"\n#include <math.h>\n\n");
    }

    void CCodeGenerator::write_struct_definition(IWriter &writer, const types::Model & model) {
        writer.write("static struct\n{\n");
        for (const auto & b : model.blocks) {
            if (b.type == types::BlockType::outport) continue;
            writer.write("  double " + b.clean_name + ";\n");
        }
        writer.write("} nwocg;\n\n");
    }

    void CCodeGenerator::write_init_function(IWriter &writer, const types::Model & model) {
        writer.write("void nwocg_generated_init()\n{\n");
        for (const auto & b : model.blocks) {
            if (b.type == types::BlockType::unit_delay) {
                writer.write("  " + std::string(constants::code_prefix) + b.clean_name + " = 0;\n");
            }
        }
        writer.write("}\n\n");
    }

    void CCodeGenerator::write_step_function(IWriter &writer, const block_map_t &block_map, const std::vector<std::string> & order, const signal_map_t & signal_sources) {
        writer.write("void nwocg_generated_step()\n{\n");
        std::vector<const types::Block *> delays_to_update;

        for (const auto & sid : order) {
            if (block_map.contains(sid)) {
                process_single_block_step(writer, block_map.at(sid), signal_sources, delays_to_update);
            }
        }

        for (auto delay_blk : delays_to_update) {
            if (signal_sources.contains(delay_blk->sid) && signal_sources.at(delay_blk->sid).contains(1)) {
                writer.write("  " + std::string(constants::code_prefix) + delay_blk->clean_name + " = " + signal_sources.at(delay_blk->sid).at(1) + ";\n");
            }
        }
        writer.write("}\n\n");
    }

    void CCodeGenerator::process_single_block_step(IWriter &writer, const types::Block * b, const signal_map_t & signal_sources, std::vector<const types::Block *> & delays) {
        if (b->type == types::BlockType::inport || b->type == types::BlockType::outport) return;

        if (b->type == types::BlockType::unit_delay) {
            delays.push_back(b);
            return;
        }

        if (templates_registry_.contains(b->type)) {
            std::string math_expr = templates_registry_.at(b->type)->generate_expression(*b, signal_sources);
            writer.write("  " + std::string(constants::code_prefix) + b->clean_name + " = " + math_expr + ";\n");
        }
    }

    void CCodeGenerator::write_ext_ports(IWriter &writer, const types::Model & model) {
        writer.write("static const nwocg_ExtPort ext_ports[] =\n{\n");
        
        std::unordered_map<std::string, std::string> outport_sources;
        for (const auto & line : model.lines) {
            if (!line.dst_sid.empty()) outport_sources[line.dst_sid] = line.src_sid;
            for (const auto & br : line.branches) outport_sources[br.target_sid] = line.src_sid;
        }

        std::unordered_map<std::string, const types::Block *> block_by_sid;
        for (const auto & b : model.blocks) block_by_sid[b.sid] = &b;

        for (const auto & b : model.blocks) {
            if (b.type == types::BlockType::inport) {
                writer.write("  { \"" + b.name + "\", &" + std::string(constants::code_prefix) + b.clean_name + ", 1 },\n");
            } 
            else if (b.type == types::BlockType::outport) {
                std::string src_ptr = "0";
                if (outport_sources.contains(b.sid)) {
                    std::string src_sid = outport_sources.at(b.sid);
                    if (block_by_sid.contains(src_sid)) {
                        src_ptr = "&" + std::string(constants::code_prefix) + block_by_sid.at(src_sid)->clean_name;
                    }
                }
                writer.write("  { \"" + b.name + "\", " + src_ptr + ", 0 },\n");
            }
        }
        writer.write("  { 0, 0, 0 }\n};\n\n"
                    "const nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;\n\n"
                    "const size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);\n");
    }

}
