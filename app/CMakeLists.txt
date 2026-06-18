add_executable(app 
    src/xml_parser.cpp
    src/graph_processor.cpp
    src/c_code_generator.cpp
    src/file_code_writer.cpp
    src/block_templates.cpp
    src/main.cpp
)

target_include_directories(app PRIVATE include)
target_link_libraries(app PRIVATE common pugixml)
