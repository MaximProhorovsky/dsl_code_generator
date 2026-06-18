#include "xml_parser.hpp"
#include <stdexcept>
#include <cctype>

namespace dsl {

    types::Model XmlParser::parse(const std::filesystem::path &file_path) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(file_path.c_str());
        
        if (!result) {
            throw std::runtime_error("Failed to parse XML: " + std::string(result.description()));
        }

        types::Model model;
        auto system_node = doc.child(constants::tag_system.data());
        
        parse_blocks(system_node, model);
        parse_lines(system_node, model);

        return model;
    }

    void XmlParser::parse_blocks(const pugi::xml_node &system_node, types::Model &model) {
        for (auto block_node : system_node.children(constants::tag_block.data())) {
            parse_single_block(block_node, model);
        }
    }

    void XmlParser::parse_single_block(const pugi::xml_node &block_node, types::Model &model) {
        types::Block block;
        block.type_str = block_node.attribute(constants::attr_block_type.data()).as_string();
        block.type = types::string_to_block_type(block.type_str);
        block.name = block_node.attribute(constants::attr_name.data()).as_string();
        block.clean_name = sanitize_identifier(block.name);
        block.sid = block_node.attribute(constants::attr_sid.data()).as_string();

        parse_block_parameters(block_node, block);
        parse_block_ports(block_node, block);

        model.blocks.push_back(block);
    }

    void XmlParser::parse_block_parameters(const pugi::xml_node &block_node, types::Block &block) {
        for (auto p_node : block_node.children(constants::tag_p.data())) {
            std::string param_name = p_node.attribute(constants::attr_name.data()).as_string();
            if (param_name == constants::param_position) {
                block.position = p_node.child_value();
            } else {
                block.parameters[param_name] = p_node.child_value();
            }
        }
    }

    void XmlParser::parse_block_ports(const pugi::xml_node &block_node, types::Block &block) {
        for (auto port_node : block_node.children(constants::tag_port.data())) {
            types::Port port;
            for (auto p_node : port_node.children(constants::tag_p.data())) {
                std::string p_name = p_node.attribute(constants::attr_name.data()).as_string();
                if (p_name == constants::param_port_number) {
                    port.number = p_node.text().as_int();
                } else if (p_name == constants::attr_name) {
                    port.name = p_node.child_value();
                }
            }
            block.ports.push_back(port);
        }
    }

    void XmlParser::parse_endpoint(const std::string &endpoint, std::string &sid, int &port) {
        auto hash_pos = endpoint.find('#');
        if (hash_pos == std::string::npos) return;

        sid = endpoint.substr(0, hash_pos);
        auto colon_pos = endpoint.find(':', hash_pos);
        if (colon_pos != std::string::npos) {
            port = std::stoi(endpoint.substr(colon_pos + 1));
        }
    }

    std::string XmlParser::sanitize_identifier(std::string_view name) {
        std::string clean;
        for (char c : name) {
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
                clean += c;
            }
        }
        return clean;
    }

    void XmlParser::parse_lines(const pugi::xml_node &system_node, types::Model &model) {
        for (auto line_node : system_node.children(constants::tag_line.data())) {
            parse_single_line(line_node, model);
        }
    }

    void XmlParser::parse_single_line(const pugi::xml_node &line_node, types::Model &model) {
        types::Line line;
        auto name_node = line_node.find_child_by_attribute(constants::tag_p.data(), constants::attr_name.data(), constants::attr_name.data());
        if (name_node) {
            line.name = name_node.child_value();
        }

        for (auto p_node : line_node.children(constants::tag_p.data())) {
            std::string p_name = p_node.attribute(constants::attr_name.data()).as_string();
            if (p_name == constants::param_src) parse_endpoint(p_node.child_value(), line.src_sid, line.src_port);
            if (p_name == constants::param_dst) parse_endpoint(p_node.child_value(), line.dst_sid, line.dst_port);
        }

        parse_line_branches(line_node, line);
        model.lines.push_back(line);
    }

    void XmlParser::parse_line_branches(const pugi::xml_node &line_node, types::Line &line) {
        for (auto branch_node : line_node.children(constants::tag_branch.data())) {
            for (auto p_node : branch_node.children(constants::tag_p.data())) {
                if (std::string(p_node.attribute(constants::attr_name.data()).as_string()) == constants::param_dst) {
                    types::Connection conn;
                    parse_endpoint(p_node.child_value(), conn.target_sid, conn.port_number);
                    line.branches.push_back(conn);
                }
            }
        }
    }

}
