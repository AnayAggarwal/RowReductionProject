#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

/*
 INSTRUCTIONS
 ------------
 Construct a Matrix A. Given this matrix, find all the given parameters needed
 for the add function (see comment under the function). For the altered matrix,
 call it A', run the add function with said parameters. Then run the same
 add function, except with Identity(K,A.size()), where K is a dummy matrix. (For
 reference, the 'matrices' we are using are essentially a vector of a vector)
 Assign that to some matrix I (keep this as your identity). Then you're done
 with a row operation!
 */

using Matrix_entry = std::pair<int, int>;
using Matrix = std::vector<Matrix_entry>;

uint16_t counter = 9;  // For 2x2 it's 9 moves at max
Matrix Moves;          // Vector of moves

void MultiplyVectorByScalar(Matrix_entry& matrix_entry, int k) {
  matrix_entry.first *= k;
  matrix_entry.second *= k;
}

Matrix create_identity_matrix() {
  // Create n x n identity matrix
  // XXX because we're initially working on a 2x2, set the
  // diagnals to 1.
  return {std::make_pair(1, 0), std::make_pair(0, 1)};
}

Matrix add(Matrix_entry& R1, Matrix_entry& R2, const Matrix& A, int index,
           int x, int y) {
  Matrix_entry A1, A2;
  auto multiply_by_scalar = [](Matrix_entry& matrix_entry, int scalar) {
    matrix_entry.first *= scalar;
    matrix_entry.second *= scalar;
  };

  multiply_by_scalar(R1, x);
  A1 = R1;
  std::cout << "in add, A1: " << A1.first << " " << A1.second << "\n";
  std::cout << "x = " << x << "\n";
  multiply_by_scalar(R2, y);
  A2 = R2;
  std::cout << "in add, A2: " << A2.first << " " << A2.second << "\n";
  std::cout << "y = " << y << "\n";

  A1.first += A2.first;
  A1.second += A2.second;
  std::cout << "in add, A1: " << A1.first << " " << A1.second << "\n";

  Matrix return_matrix{A};
  std::cout << "in add, before erase, index: " << index << "\n";
  return_matrix.erase(return_matrix.begin() + index);
  std::cout << "in add, after erase\n";
  return_matrix.insert(return_matrix.begin() + index, A1);
  std::cout << "in add, after insert. return_matrix size: "
            << return_matrix.size() << "\n";
  for (auto& entry : return_matrix) {
    std::cout << entry.first << " " << entry.second << "\n";
  }
  return return_matrix;
}

bool test_if_identity(Matrix& input) {
  // input_(row, column) if row = column then we need input_(row,column)=1.
  // Otherwise, input_(row,column)=0.
  // assert that the input matrix is always 2x2
  assert(input.size() == 2);
  if (input[0] == std::make_pair(1, 0) && input[1] == std::make_pair(0, 1)) {
    return true;
  }
  return false;
}

bool TestIfIdentity(std::vector<std::vector<int>> I) {
  uint32_t identity_counter = 0;
  for (int iterator = 0; iterator < I.size(); iterator++) {
    std::vector<int> Dummy = I[iterator];
    for (int iterator2 = 0; iterator2 < Dummy.size(); iterator2++) {
      if (iterator = iterator2) {
        if (Dummy[iterator2] == 1) {
          identity_counter++;
        }
      } else {
        if (Dummy[iterator2] == 0) {
          identity_counter++;
        }
      }
    }
  }
  int desired = I.size() * I.size();
  if (identity_counter == desired) {
    return true;
  } else {
    return false;
  }
}

void recursionfordataset(int range, Matrix& Dummy) {
  if (counter == 0) {
    std::cout << "counter is zero\n";
    if (test_if_identity(Dummy) == true) {
      std::cout << "counter is zero, identity true. returning\n";
      return;
    }

    // if it isn't an identity matrix, clear the moves and reset the
    // counter. and, start over.
    Moves.clear();
    counter = 9;
    std::cout << "reset to 9, recursing\n";
    recursionfordataset(range, Dummy);
  }

  // 4^9*range^18/9
  // the global 'counter' is non-zero
  std::cout << "recursing for dataset\n";
  for (int x = -range; x <= range; x++) {
    for (int y = -range; y <= range; y++) {
      for (uint32_t i = 0; i < 2; i++) {
        for (uint32_t j = 0; j < 2; j++) {
          if (i != j) {
            std::cout << "i: " << i << ",j: " << j << " range: " << range
                      << "\n";
            Dummy = add(Dummy[i], Dummy[j], Dummy, i, x, y);
            std::cout << "before push_back\n";
            Moves.push_back(std::make_pair(x, y));
            --counter;
            std::cout << "about to recurse again\n";
            recursionfordataset(range, Dummy);
            if (x != 0 && y != 0) {
              std::cout << "x, y are non-zero, calling add again\n";
              add(Dummy[i], Dummy[j], Dummy, i, 1 / x, 1 / y);
            }
            std::cout << "recursing again a second time\n";
            recursionfordataset(range, Dummy);
          }
        }
      }
    }
  }
}

#if 0
std::vector<std::vector<int>> generateDataset(
    std::vector<std::vector<int>> Dummy) {
  // Dummy is 2x2 --> It needs 9 moves at max. Time complexity is
  // not of the essence here, since we just want to generate the dataset.
  int maximum = 0;
  int minimum = 1000000;
  // Finding minimum and maximum values to determine the range (so it won't run
  // forever)
  for (auto& entry : Dummy) {
    for (auto& entry2 : entry) {
      if (minimum > entry2) {
        minimum = entry2;
      }
      if (maximum < entry2) {
        maximum = entry2;
      }
    }
  }
  int range = maximum / minimum;
  recursionfordataset(range, Dummy);
  return Moves;
}
#endif

// x and y are the essential coefficients. The rest is looping through Dummy. We
// are using a recursion (calling the function inside itself) with a break
// condition.
/*
 Here is how we do the add function. It takes multiple parameters. The first is
 the first row it uses, the second is the second row it uses. The first row is
 the one that we will affect. Next is the major matrix that we are doing work
 on. Then, we have the index of the first row in the matrix. Eg. in 12,34 the
 index of 34 is 2. Finally, we have the multiplicative constants x,y. We are
 affecting R1 with R1*x+R2*y.
   */

int main() {
  Matrix A;
  A = {std::make_pair(1, 2), std::make_pair(3, 4)};

  Matrix B = add(A[1], A[0], A, 1, 2, 1);
  for (const auto& [x, y] : B) {
    std::cout << x << " " << y << std::endl;
  }

  // change number entry for A size
  Matrix K = create_identity_matrix();
  // Change 3 for A size
  Matrix Bfinal = add(K[1], K[0], K, 1, 2, 1);
  for (const auto& [x, y] : Bfinal) {
    std::cout << x << " " << y << std::endl;
  }

  bool value = test_if_identity(B);
  bool value2 = test_if_identity(Bfinal);
  std::cout << value << " " << value2 << std::endl;

  // Everything in the main section is testing the program on a random scenario.
  Matrix Dummy = {std::make_pair(1, 2), std::make_pair(3, 4)};
  recursionfordataset(5, Dummy);
  for (const auto& [x, y] : Moves) {
    std::cout << x << " " << y << std::endl;
  }
}
