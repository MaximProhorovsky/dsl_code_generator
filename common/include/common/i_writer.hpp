#ifndef NWOCG_I_CODEGEN_WRITER_HPP
#define NWOCG_I_CODEGEN_WRITER_HPP

#include <string_view>

namespace dsl {

    class IWriter {
        public:
            virtual ~IWriter() = default;
            virtual void open() = 0;
            virtual void write(std::string_view data) = 0;
            virtual void close() = 0;
    };

}

#endif //NWOCG_I_CODEGEN_WRITER_HPP