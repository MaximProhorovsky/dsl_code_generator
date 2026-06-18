#ifndef NWOCG_I_SOURCE_BLOCK_HPP
#define NWOCG_I_SOURCE_BLOCK_HPP

#include "dsl_types.hpp"

#include <string>
#include <unordered_map>

namespace dsl{
    using signal_map_t = std::unordered_map<std::string, std::unordered_map<int, std::string>>;

    class IBlockTemplate {
        public:
            virtual ~IBlockTemplate() = default;
            virtual std::string generate_expression(
                const dsl::types::Block &block, 
                const signal_map_t &signal_sources
            ) const = 0;
        };
}

#endif //NWOCG_I_SOURCE_BLOCK_HPP