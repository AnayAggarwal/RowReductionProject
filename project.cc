#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <vector>

// A function to multiply a vector by a constant
void MultiplyVectorByScalar(std::vector<int>& v, int k) {
  for (auto& entry : v) {
    entry *= k;
  }
}

std::vector<std::vector<int>> add(std::vector<int> R1, std::vector<int> R2,
                                  std::vector<std::vector<int>> A, int index,
                                  int x, int y) {
  std::vector<int> A1, A2;

  MultiplyVectorByScalar(R1, x);
  std::copy(R1.begin(), R1.end(), std::back_inserter(A1));

  MultiplyVectorByScalar(R2, y);
  std::copy(R2.begin(), R2.end(), std::back_inserter(A2));

  for (size_t i = 0; i < A1.size(); i++) {
    A1[i] += A2[i];
  }
  A.erase(A.begin() + index - 1);
  A.insert(A.begin() + index - 1, A1);
  return A;
}
/*
 Here is how we do the add function. It takes multiple parameters. The first is
 the first row it uses, the second is the second row it uses. The first row is
 the one that we will affect. Next is the major matrix that we are doing work
 on. Then, we have the index of the first row in the matrix. Eg. in 12,34 the
 index of 34 is 2. Finally, we have the multiplicative constants x,y. We are
 affecting R1 with R1*x+R2*y.
   */

void Identity(std::vector<std::vector<int>>& I, int n) {
  // Create n x n identity matrix
  I = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
  for (unsigned int t = 0; t < n; t++) {
    I[t][t] = 1;
  }
}

int main() {
  std::vector<std::vector<int>> A;
  A = {{1, 2, 3}, {3, 4, 5}, {6, 7, 8}};
  std::vector<std::vector<int>> B = add(A[1], A[0], A, 2, 2, 1);
  for (auto& entry : B) {
    std::cout << entry[0] << " " << entry[1] << " " << entry[2] << std::endl;
  }
  // change number entry for A size
  std::vector<std::vector<int>> K;
  Identity(K, 3);
  // Change 3 for A size
  std::vector<std::vector<int>> Bfinal = add(K[1], K[0], K, 2, 2, 1);
  for (auto& entry : Bfinal) {
    std::cout << entry[0] << " " << entry[1] << " " << entry[2] << std::endl;
  }
}
