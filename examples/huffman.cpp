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


freq_table_t build_freq_table(const std::string& input) {
  freq_table_t freq_table;
  for (char c : input) {
    freq_table.entry(c).and_modify([&](uint64_t& freq) {
      freq += 1;
      }).or_insert(1);
  }
  return freq_table;
}


void insert_in_order(dsun::LinkedList<huffman_node_t*>& nodes, huffman_node_t* node) {
  for (int i = 0; i < nodes.len(); i++) {
    if (node->value.frequency < nodes[i]->value.frequency) {
      nodes.insert(i, node);
      return;
    }
  }
  nodes.push_back(node);
}


dsun::LinkedList<huffman_node_t*> build_huffman_nodes(freq_table_t& freq_table) {
  dsun::LinkedList<huffman_node_t*> nodes;
  for (auto entry : freq_table) {
    huffman_node_t* node = new huffman_node_t(huffman_value_t(entry.first, entry.second));
    insert_in_order(nodes, node);
  }
  return nodes;
}

void build_walk_table(huffman_node_t* root, dsun::Vec<uint8_t>* bits, dsun::HashMap<char, dsun::Vec<uint8_t>>& walk_table) {
  dsun::Vec<uint8_t> bits_copy(*bits);

  if (root->left) {
    bits_copy.push(0);
    build_walk_table(root->left, &bits_copy, walk_table);
  }
  if (root->right) {
    bits->push(1);
    build_walk_table(root->right, bits, walk_table);
  }
  if (root->left == NULL && root->right == NULL) {
    walk_table.insert(root->value.value, bits_copy);
  }
}

int main(int argc, char const* argv[]) {
  std::string input = "aaaaabbbbbbbbbccccccccccccdddddddddddddeeeeeeeeeeeeeeeefffffffffffffffffffffffffffffffffffffffffffff";
  freq_table_t freq_table = build_freq_table(input);


  dsun::LinkedList<huffman_node_t*> nodes = build_huffman_nodes(freq_table);
  std::cout << "[";
  for (int i = 0; i < nodes.len(); i++) {
    std::cout << "{" << nodes[i]->value.value << ", " << nodes[i]->value.frequency << "}";
  }
  std::cout << "]" << std::endl;

  while (nodes.len() > 1) {
    auto first = nodes.pop_front().value();
    auto second = nodes.pop_front().value();
    auto root = new huffman_node_t(huffman_value_t('\0', first->value.frequency + second->value.frequency));
    root = huffman_tree_t::merge(first, second, root);
    std::cout << "Merged: " << first->value.value << " " << second->value.value << std::endl;
    std::cout << "Root: " << root->value.frequency << std::endl;

    insert_in_order(nodes, root);
    std::cout << "[";
    for (int i = 0; i < nodes.len(); i++) {
      std::cout << "{" << nodes[i]->value.value << ", " << nodes[i]->value.frequency << "}";
    }
    std::cout << "]" << nodes.len() << std::endl;

  }

  std::cout << "\n\nHuffman tree:" << std::endl;
  huffman_tree_t::pre_order_traversal(nodes[nodes.len() - 1], [](huffman_value_t value) {
    if (value.value == '\0') {
      std::cout << "Internal Node: " << value.frequency << std::endl;
    }
    else {
      std::cout << "Leaf Node: " << value.value << " " << value.frequency << std::endl;
    }
    });

  std::cout << "\n\nWalk Table:" << std::endl;
  dsun::Vec<uint8_t>* bits = new dsun::Vec<uint8_t>();
  auto walk_table = dsun::HashMap<char, dsun::Vec<uint8_t>>();
  build_walk_table(nodes[0], bits, walk_table);
  for (auto entry : walk_table) {
    std::cout << entry.first << ": ";
    for (size_t i = 0; i < entry.second.len(); i++) {
      std::cout << (int)entry.second[i];
    }
    std::cout << std::endl;
  }
  return 0;
}
