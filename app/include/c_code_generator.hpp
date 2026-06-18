#ifndef NWOCG_C_CODE_GENERATOR_HPP
#define NWOCG_C_CODE_GENERATOR_HPP

#include "dsl_types.hpp"
#include "common/i_code_generator.hpp"
#include "block_templates.hpp"

#include <unordered_map>
#include <memory>
#include <map>

namespace dsl {

    class CCodeGenerator : public ICodeGenerator<types::Model> {
        public:
            CCodeGenerator();
            ~CCodeGenerator() override = default;

            void generate(const types::Model &model, const std::vector<std::string> &order, IWriter&output) override;

        private:
            using block_map_t = std::unordered_map<std::string, const types::Block *>;

            std::map<types::BlockType, std::unique_ptr<IBlockTemplate>> templates_registry_;

            void build_signal_maps(const types::Model &model, const block_map_t &block_map, signal_map_t &signal_sources);
            
            void write_header_includes(IWriter &writer);
            void write_struct_definition(IWriter &writer, const types::Model &model);
            void write_init_function(IWriter &writer, const types::Model &model);
            void write_step_function(IWriter &writer, const block_map_t &block_map, const std::vector<std::string> &order, const signal_map_t &signal_sources);
            void write_ext_ports(IWriter &writer, const types::Model &model);

            void process_single_block_step(IWriter &writer, const types::Block *b, const signal_map_t &signal_sources, std::vector<const types::Block *> &delays);
    };

}

#endif //NWOCG_C_CODE_GENERATOR_HPP
