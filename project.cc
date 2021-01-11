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

using Matrix_entry = std::pair<int, int>;  // This is for 2x2 matrix
using Matrix = std::vector<Matrix_entry>;  // This is for 2x2 matrix

uint16_t counter = 9;  // For 2x2 it's 9 moves at max, with the MIT algorithm
Matrix Moves;          // Vector of moves, used in recursion for dataset

void MultiplyVectorByScalar(Matrix_entry& matrix_entry, int k) {
  matrix_entry.first *= k;
  matrix_entry.second *= k;
}

void create_identity_matrix(std::vector<std::vector<int>>& I, int n) {
  // Create n x n identity matrix
  I = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
  for (unsigned int t = 0; t < n; t++) {
    I[t][t] = 1;
  }
}

std::vector<std::vector<int>> add(std::vector<int> R1, std::vector<int> R2,
                                  std::vector<std::vector<int>> A, int index,
                                  int x, int y) {
  std::vector<int> A1, A2;
  MultiplyVectorByScalar(R1, x);
  std::copy(R1.begin(), R1.end(),
            std::back_inserter(A1));  // Copying R1*x into A1
  MultiplyVectorByScalar(R2, y);
  std::copy(
      R2.begin(), R2.end(),
      std::back_inserter(
          A2));  // Copying R2*y into A2 (this can be optimized, of course)
  for (size_t i = 0; i < A1.size(); i++) {
    A1[i] += A2[i];  // Adding A1 and A2
  }
  A.erase(A.begin() + index - 1);       // Taking away old A1
  A.insert(A.begin() + index - 1, A1);  // Adding new A1
  return A;
}

bool test_if_identity(Matrix& input) {
  // Simply testing if it is {{1,0}, {0,1}} (2x2 specifically)
  assert(input.size() == 2);
  if (input[0] == std::make_pair(1, 0) && input[1] == std::make_pair(0, 1)) {
    return true;
  }
  return false;
}

bool TestIfIdentity(std::vector<std::vector<int>> I) {
  // Testing if all A_(i,i)=1, and A(i,j)=0 with i=/=j for the general case
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

// x and y are the essential coefficients. The rest is looping through Dummy. We
// are using a recursion (calling the function inside itself) with a break
// condition.

std::vector<std::vector<int>> generatelooserdataset(
    std::vector<std::vector<int>> Dummy) {
  // This uses my conjecture, now proven
  std::vector<std::vector<int>> I;
  std::vector<std::vector<int>> Moves;
  create_identity_matrix(I, Dummy.size());
  for (int row = 0; row < n; row++) {
    for (int column = 0; column < n; column++) {
      if (row < column) {
        I = add(I[row], I[row + 1], 1,
                -Dummy[row][column] / Dummy[row + 1][column]);
        Moves.push_back(
            {row, row + 1, 1, -Dummy[row][column] / Dummy[row + 1][column]});
      }
      if (row == column) {
        I = add(I[row], I[row + 1], 1 / Dummy[row][column], 0);
        Moves.push_back({row, row + 1, 1 / Dummy[row][column], 0});
      }
      if (row > column) {
        I = add(I[row], I[column], 1, -Dummy[row][column]);
        Moves.push_back({row, column, 1, -Dummy[row][column]});
      }
    }
  }
  // I is now the inverse of dummy
  // Moves is filled with entries of the form {row1, row2, constant1,
  // constant2}, which we will use to train our model!
  return I, Moves;
}

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
  std::vector<std::vector<int>> Dummy = {{1, 2}, {3, 4}};
  // recursionfordataset(5, Dummy); Removed because it is too bashy
  // Call new function on this
}
