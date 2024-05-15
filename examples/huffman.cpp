#include "dsun.h"
#include "memory"
struct huffman_value_t {
  char value;
  int frequency;
  huffman_value_t(char value, int frequency) : value(value), frequency(frequency) {}
  huffman_value_t() {
    value = '\0';
    frequency = 0;
  }
};

typedef dsun::raw::node_t<huffman_value_t> huffman_node_t;
typedef dsun::raw::BinaryTree<huffman_value_t> huffman_tree_t;
typedef dsun::HashMap<char, uint64_t> freq_table_t;


freq_table_t build_freq_table(const std::string& text) {
  freq_table_t freq_table;
  for (char c : text) {
    freq_table.entry(c).and_modify([&](uint64_t& freq) {
      freq += 1;
      }).or_insert(1);
  }
  return freq_table;
}

int main(int argc, char const* argv[]) {
  std::string text = "hello world";
  freq_table_t freq_table = build_freq_table(text);
  std::cout << "Frequency table:" << std::endl;
  for (auto c : freq_table) {
    std::cout << c.first << ": " << c.second << std::endl;
  }



  return 0;
}
