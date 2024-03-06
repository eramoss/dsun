#include "vec.h"
#include "iostream"

class NumericSet {
public:
  NumericSet() : sum_(0), data(), max(0), min(0) {}

  void Add(int value) {
    if (data.len() == 0) {
      min = value;
      max = value;
    }
    else {
      if (value < min) {
        min = value;
      }
      if (value > max) {
        max = value;
      }
    }
    sum_ += value;
    data.push(value);
  }

  int GetSum() const {
    return sum_;
  }
  int GetMax() const {
    return max;
  }
  int GetMin() const {
    return min;
  }
  std::optional<int> get(int index) const {
    return data.get(index);
  }
  uint32_t len() const {
    return data.len();
  }

private:
  int sum_;
  dsun::Vec<int> data;
  int max;
  int min;
};

int main() {
  NumericSet set;
  set.Add(1);
  set.Add(2);
  set.Add(3);
  std::cout << std::endl << "numeric set example: " << std::endl;
  std::cout << "sum: " << set.GetSum() << std::endl;
  std::cout << "max: " << set.GetMax() << std::endl;
  std::cout << "min: " << set.GetMin() << std::endl;
  std::cout << "values: ";
  for (size_t i = 0; i < set.len(); ++i) {
    std::cout << "," << set.get(i).value();
  }
  std::cout << std::endl;

  return 0;
}