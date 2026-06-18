#ifndef NWOCG_GRAPH_PROCESSOR_HPP
#define NWOCG_GRAPH_PROCESSOR_HPP

#include "dsl_types.hpp"

#include <unordered_map>
#include <queue>

namespace dsl {

    class GraphProcessor {
        public:
            static std::vector<std::string> get_execution_order(const types::Model &model);

        private:
            using adjacency_map = std::unordered_map<std::string, std::vector<std::string>>;
            using degree_map    = std::unordered_map<std::string, int>;
            using block_ref_map  = std::unordered_map<std::string, const types::Block *>;

            static void initialize_maps(const types::Model &model, block_ref_map &block_map, degree_map &in_degree);
            static void build_graph_edges(const types::Model &model, const block_ref_map &block_map, adjacency_map &adj, degree_map &in_degree);
            static void add_single_edge(const std::string &from, const std::string &to, const block_ref_map &block_map, adjacency_map &adj, degree_map &in_degree);
            
            static std::vector<std::string> traverse_graph(std::queue<std::string> &zero_degree_queue, adjacency_map &adj, degree_map &in_degree);
            static void validate_cycles(size_t sorted_count, size_t total_count);
    };

}

#endif //NWOCG_GRAPH_PROCESSOR_HPP