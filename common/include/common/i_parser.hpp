#ifndef NWOCG_I_PARSER_HPP
#define NWOCG_I_PARSER_HPP

#include <filesystem>

namespace dsl {

    template <typename ModelType>
    class IParser {
        public:
            virtual ~IParser() = default;
            virtual ModelType parse(const std::filesystem::path &file_path) = 0;
    };

}

#endif //NWOCG_I_PARSER_HPP