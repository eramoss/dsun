#include "dsun.h"

struct huffman_node_t {
  char c;
  int freq;
  huffman_node_t(char c, int freq) : c(c), freq(freq) {}
  huffman_node_t(int freq) : c('\0'), freq(freq) {}

  std::strong_ordering operator<=>(const huffman_node_t& other) const {
    return freq <=> other.freq;
  }
};

using huffman_tree = dsun::BSTree<huffman_node_t>;
using freq_table = dsun::HashMap<char, uint64_t>;

huffman_tree build_huffman_tree(const freq_table& table) {
  huffman_tree tree;
  for (const auto& [c, freq, _] : table) {
    tree.insert(huffman_node_t(c, freq), true);
  }
  return tree;
}

freq_table build_freq_table(const std::string& input) {
  freq_table table;
  for (const auto& c : input) {
    table.entry(c).and_modify([&](uint64_t& freq) {
      freq += 1;
      }).or_insert(1);
  }
  return table;
}

int main(int argc, char const* argv[]) {
  auto input = "this is an example for huffman encoding";
  auto table = build_freq_table(input);
  auto tree = build_huffman_tree(table);
  std::cout << "pre: [";
  tree.pre_order([](const huffman_node_t& node) {
    std::cout << node.c << ": " << node.freq << ", ";
    });
  std::cout << "]" << std::endl;
  std::cout << "in: [";
  tree.in_order([](const huffman_node_t& node) {
    std::cout << node.c << ": " << node.freq << ", ";
    });
  std::cout << "]" << std::endl;
  std::cout << "post: [";
  tree.post_order([](const huffman_node_t& node) {
    std::cout << node.c << ": " << node.freq << ", ";
    });
  std::cout << "]" << std::endl;

  return 0;
}
