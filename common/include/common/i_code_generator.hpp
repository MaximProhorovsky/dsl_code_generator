#ifndef NWOCG_I_CODE_GENERATOR_HPP
#define NWOCG_I_CODE_GENERATOR_HPP

#include "i_writer.hpp"

namespace dsl {
    
    template <typename ModelType>
    class ICodeGenerator {
        public:
            virtual ~ICodeGenerator() = default;
            virtual void generate(const ModelType &model, const std::vector<std::string> &order, IWriter &writer) = 0;
    };

}

#endif //NWOCG_I_CODE_GENERATOR_HPP
