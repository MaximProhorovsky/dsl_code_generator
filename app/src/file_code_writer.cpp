#include "file_code_writer.hpp"

#include <stdexcept>

namespace dsl {

FileCodeWriter::FileCodeWriter(std::filesystem::path file_path) : file_path_(std::move(file_path)) {}

FileCodeWriter::~FileCodeWriter() {
    close();
}

void FileCodeWriter::open() {
    if (!file_stream_.is_open()) {
        file_stream_.open(file_path_);
        if (!file_stream_) {
            throw std::runtime_error("Failed to open file for writing: " + file_path_.string());
        }
    }
}

void FileCodeWriter::write(std::string_view data) {
    if (file_stream_.is_open()) {
        file_stream_ << data;
    }
}

void FileCodeWriter::close() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

} // namespace dsl
