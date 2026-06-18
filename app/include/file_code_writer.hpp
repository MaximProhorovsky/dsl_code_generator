#ifndef NWOCG_FILE_CODE_WRITER_HPP
#define NWOCG_FILE_CODE_WRITER_HPP

#include "common/i_writer.hpp"
#include <filesystem>
#include <fstream>

namespace dsl {

    class FileCodeWriter : public IWriter {
        public:
            explicit FileCodeWriter(std::filesystem::path file_path);
            ~FileCodeWriter() override;

            void open() override;
            void write(std::string_view data) override;
            void close() override;

        private:
            std::filesystem::path file_path_;
            std::ofstream file_stream_;
    };

}

#endif //NWOCG_FILE_CODE_WRITER_HPP
