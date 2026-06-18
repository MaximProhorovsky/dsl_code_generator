#ifndef NWOCG_SOURCE_BLOCK_TEMPLATES_HPP
#define NWOCG_SOURCE_BLOCK_TEMPLATES_HPP

#include "i_source_block.hpp"
#include "dsl_constants.hpp"

#include <sstream>

namespace dsl::templates{
    class SumBlockTemplate : public IBlockTemplate {
        public:
            std::string generate_expression(const types::Block &block, const signal_map_t &signal_sources) const override;
    };


    class GainBlockTemplate : public IBlockTemplate {
        public:
            std::string generate_expression(const types::Block &block, const signal_map_t &signal_sources) const override;
    };

    class UnitDelayBlockTemplate : public IBlockTemplate {
        public:
            std::string generate_expression(const types::Block &block, const signal_map_t &signal_sources) const override;
    };
}

#endif //NWOCG_SOURCE_BLOCK_TEMPLATES_HPP