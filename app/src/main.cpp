#include "dsl_types.hpp"
#include "xml_parser.hpp"
#include "graph_processor.hpp"
#include "c_code_generator.hpp"
#include "file_code_writer.hpp"

#include <iostream>
#include <memory>

int main() {
    try {
        std::unique_ptr<dsl::IParser<dsl::types::Model>> parser = std::make_unique<dsl::XmlParser>();
        dsl::types::Model model = parser->parse("shared/data/diagram.xml");

        auto execution_order = dsl::GraphProcessor::get_execution_order(model);

        std::unique_ptr<dsl::IWriter> output_target = std::make_unique<dsl::FileCodeWriter>("nwocg_generated.c");

        std::unique_ptr<dsl::ICodeGenerator<dsl::types::Model>> generator = std::make_unique<dsl::CCodeGenerator>();
        generator->generate(model, execution_order, *output_target);

    } catch (const std::exception &ex) {
        std::cerr << "Application failure: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
