use std::cmp::Eq;
use std::ops::{Add, Div, Mul, Sub};

const INITIAL_SIZE: u8 = 10;

pub struct NumberSet<A>
where
    A: Eq
        + Add<Output = A>
        + Mul<Output = A>
        + Sub<Output = A>
        + Div<Output = A>
        + PartialOrd
        + Default
        + Copy,
{
    numbers: Vec<A>,
    max: A,
    min: A,
}

impl<
        A: Eq
            + Add<Output = A>
            + Mul<Output = A>
            + Sub<Output = A>
            + Div<Output = A>
            + PartialOrd
            + Default
            + Copy,
    > NumberSet<A>
{
    pub fn new() -> Self {
        NumberSet {
            numbers: Vec::with_capacity(INITIAL_SIZE as usize),
            max: A::default(),
            min: A::default(),
        }
    }

    pub fn insert(&mut self, number: A) -> bool {
        if self.numbers.contains(&number) {
            return false;
        }
        if self.len() == 0 {
            self.max = number;
            self.min = number;
        } else {
            if number > self.max {
                self.max = number;
            }
            if number < self.min {
                self.min = number;
            }
        }
        self.numbers.push(number);
        true
    }

    pub fn max(&self) -> A {
        self.max
    }

    pub fn min(&self) -> A {
        self.min
    }

    pub fn len(&self) -> usize {
        self.numbers.len()
    }

    pub fn mul(&mut self, factor: A) {
        for number in self.numbers.iter_mut() {
            *number = *number * factor;
        }
    }
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_insert() {
        let mut number_set = NumberSet::<i64>::new();
        assert_eq!(number_set.insert(5), true);
        assert_eq!(number_set.insert(10), true);
        assert_eq!(number_set.insert(5), false);
        assert_eq!(number_set.insert(15), true);
    }

    #[test]
    fn test_max_min() {
        let mut number_set = NumberSet::new();
        number_set.insert(5);
        number_set.insert(10);
        number_set.insert(15);
        assert_eq!(number_set.max(), 15);
        assert_eq!(number_set.min(), 5);
    }

    #[test]
    fn test_len() {
        let mut number_set = NumberSet::new();
        assert_eq!(number_set.len(), 0);
        number_set.insert(5);
        number_set.insert(10);
        assert_eq!(number_set.len(), 2);
    }

    #[test]
    fn test_mul() {
        let mut number_set = NumberSet::new();
        number_set.insert(5);
        number_set.insert(10);
        number_set.mul(2);
        assert_eq!(number_set.numbers, vec![10, 20]);
    }
}
