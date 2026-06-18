#include "graph_processor.hpp"

#include <stdexcept>

namespace dsl {

    std::vector<std::string> GraphProcessor::get_execution_order(const types::Model &model) {
        block_ref_map block_map;
        degree_map in_degree;
        initialize_maps(model, block_map, in_degree);

        adjacency_map adj;
        build_graph_edges(model, block_map, adj, in_degree);

        std::queue<std::string> zero_degree_queue;
        for (const auto &[sid, degree] : in_degree) {
            if (degree == 0) {
                zero_degree_queue.push(sid);
            }
        }

        auto execution_order = traverse_graph(zero_degree_queue, adj, in_degree);
        validate_cycles(execution_order.size(), model.blocks.size());

        return execution_order;
    }

    void GraphProcessor::initialize_maps(const types::Model &model, block_ref_map &block_map, degree_map &in_degree) {
        for (const auto &block : model.blocks) {
            block_map[block.sid] = &block;
            in_degree[block.sid] = 0;
        }
    }

    void GraphProcessor::build_graph_edges(const types::Model &model, const block_ref_map &block_map, adjacency_map &adj, degree_map &in_degree) {
        for (const auto &line : model.lines) {
            if (!line.dst_sid.empty()) {
                add_single_edge(line.src_sid, line.dst_sid, block_map, adj, in_degree);
            }
            for (const auto &branch : line.branches) {
                add_single_edge(line.src_sid, branch.target_sid, block_map, adj, in_degree);
            }
        }
    }

    void GraphProcessor::add_single_edge(const std::string &from, const std::string &to, const block_ref_map &block_map, adjacency_map &adj, degree_map &in_degree) {
        if (!block_map.contains(from) || !block_map.contains(to)) return;
        if (block_map.at(from)->type == types::BlockType::unit_delay) return;

        adj[from].push_back(to);
        in_degree[to]++;
    }

    std::vector<std::string> GraphProcessor::traverse_graph(std::queue<std::string> &zero_degree_queue, adjacency_map &adj, degree_map &in_degree) {
        std::vector<std::string> order;
        while (!zero_degree_queue.empty()) {
            auto current_sid = zero_degree_queue.front();
            zero_degree_queue.pop();
            order.push_back(current_sid);

            for (const auto &neighbor_sid : adj[current_sid]) {
                if (--in_degree[neighbor_sid] == 0) {
                    zero_degree_queue.push(neighbor_sid);
                }
            }
        }
        return order;
    }

    void GraphProcessor::validate_cycles(size_t sorted_count, size_t total_count) {
        if (sorted_count != total_count) {
            throw std::runtime_error("Algebraic loop detected! The diagram contains an invalid loop dependency.");
        }
    }

}
