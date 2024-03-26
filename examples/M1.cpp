#include "utils.h"
#include <math.h>
#include <fstream>
#include <string>

class ProductItem : public Serializable {
public:
  ProductItem() = default;
  ProductItem(int id, std::string name, double price) : id(id), name(name), price(price) {}

  bool operator==(const ProductItem& other) const {
    return id == other.id;
  }
  friend std::ostream& operator<<(std::ostream& os, const ProductItem& item) {
    os << "ID: " << item.id << ", Name: " << item.name << ", Price: " << item.price;
    return os;
  }
  void serialize(std::ostream& file) const override {
    file.write(reinterpret_cast<const char*>(&id), sizeof(int));

    int nameLength = name.length();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
    file.write(name.c_str(), nameLength);
    file.write(reinterpret_cast<const char*>(&price), sizeof(double));
  }

  bool deserialize(std::istream& file) override {
    if (!file.read(reinterpret_cast<char*>(&id), sizeof(int)))
      return false;

    int nameLength;
    if (!file.read(reinterpret_cast<char*>(&nameLength), sizeof(int)))
      return false;

    char* nameBuffer = new char[nameLength + 1];
    if (!file.read(nameBuffer, nameLength)) {
      delete[] nameBuffer;
      return false;
    }
    nameBuffer[nameLength] = '\0';
    name = std::string(nameBuffer);
    delete[] nameBuffer;

    if (!file.read(reinterpret_cast<char*>(&price), sizeof(double)))
      return false;

    return true;
  }

  int id;
  std::string name;
  double price;
};

class Products {
public:
  Products() = default;
  Products(dsun::LinkedList<ProductItem> storage) : storage(storage), total_price(0) {}

  double get_total_price() {
    return total_price;
  }

  void add_product(ProductItem item) {
    storage.push_back(item);
    total_price += item.price;
  }

  void remove_product(int id) {
    for (auto it = storage.begin(); it != storage.end(); ++it) {
      if (it.get_mut().id == id) {
        total_price -= it.get_mut().price;
        storage.remove(*it);
        break;
      }
    }
  }

  ProductItem get_product(int id) {
    for (auto it = storage.begin(); it != storage.end(); ++it) {
      if (it.get_mut().id == id) {
        return it.get_mut();
      }
    }
    return ProductItem();
  }

  void print_products() {
    for (auto it = storage.begin(); it != storage.end(); ++it) {
      std::cout << "ID: " << it.get_mut().id << ", Name: " << it.get_mut().name << ", Price: " << it.get_mut().price << std::endl;
    }
  }

  void clean() {
    storage.clean();
    total_price = 0;
  }

  void serialize() {
    serialize_list_on_disk(storage);
  }

  void deserialize() {
    if (std::ifstream(LIST_FILE).good()) {
      storage = deserialize_list_from_disk<ProductItem>();
      std::cout << storage.len() << std::endl;
      total_price = 0;
      for (auto it = storage.begin(); it != storage.end(); ++it) {
        total_price += (*it).price;
      }
    }
  }

  dsun::LinkedList<ProductItem> storage;
private:
  double total_price;
};


int main() {
  Products shop;
  Products costumer_cart;
  shop.deserialize();

  while (true) {
    std::cout << "1. Add product to cart" << std::endl;
    std::cout << "2. Remove product from cart" << std::endl;
    std::cout << "3. Print products in cart" << std::endl;
    std::cout << "4. Checkout" << std::endl;
    std::cout << "5. Admin" << std::endl;
    std::cout << "6. Exit" << std::endl;

    int option;
    std::cin >> option;


    if (option == 1) {
      shop.print_products();
      int id;
      std::cout << "Enter product ID: ";
      std::cin >> id;
      auto item = shop.get_product(id);
      costumer_cart.add_product(item);
    }
    else if (option == 2) {
      costumer_cart.print_products();
      int id;
      std::cout << "Enter product ID: ";
      std::cin >> id;
      costumer_cart.remove_product(id);
    }
    else if (option == 3) {
      costumer_cart.print_products();
    }
    else if (option == 4) {
      std::cout << "Total price: " << costumer_cart.get_total_price() << std::endl;
      costumer_cart.clean();
      break;
    }
    else if (option == 5) {
      std::cout << "1. Add product" << std::endl;
      std::cout << "2. Remove product" << std::endl;
      std::cout << "3. Print products" << std::endl;
      std::cout << "4. Exit" << std::endl;

      int admin_option;
      std::cin >> admin_option;

      if (admin_option == 1) {
        int id;
        std::string name;
        double price;
        std::cout << "Enter product ID: ";
        std::cin >> id;
        std::cout << "Enter product name: ";
        getline(std::cin >> std::ws, name); // to ignore the newline character (https://stackoverflow.com/a/20728910/1092820
        std::cout << "Enter product price: ";
        std::cin >> price;
        shop.add_product(ProductItem(id, name, price));
      }
      else if (admin_option == 2) {
        shop.print_products();
        int id;
        std::cout << "Enter product ID: ";
        std::cin >> id;
        shop.remove_product(id);
      }
      else if (admin_option == 3) {
        shop.print_products();
      }
      else if (admin_option == 4) {
        break;
      }
    }
    else if (option == 6) {
      shop.serialize();
      return 0;
    }

  }
}