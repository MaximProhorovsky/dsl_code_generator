#ifndef NWOCG_XML_PARSER_HPP
#define NWOCG_XML_PARSER_HPP

#include "common/i_parser.hpp"
#include "dsl_types.hpp"

#include <pugixml.hpp>

namespace dsl {

    class XmlParser : public IParser<types::Model> {
        public:
            types::Model parse(const std::filesystem::path &file_path) override;

        private:
            void parse_blocks(const pugi::xml_node &system_node, types::Model &model);
            void parse_single_block(const pugi::xml_node &block_node, types::Model &model);
            void parse_block_parameters(const pugi::xml_node &block_node, types::Block &block);
            void parse_block_ports(const pugi::xml_node &block_node, types::Block &block);

            void parse_lines(const pugi::xml_node &system_node, types::Model &model);
            void parse_single_line(const pugi::xml_node &line_node, types::Model &model);
            void parse_line_branches(const pugi::xml_node &line_node, types::Line &line);

            void parse_endpoint(const std::string &endpoint, std::string &sid, int &port);
            std::string sanitize_identifier(std::string_view name);
    };

}

#endif //NWOCG_XML_PARSER_HPP