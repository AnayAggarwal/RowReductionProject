#include <math.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
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

void MultiplyVectorByScalar(std::vector<int>& entry, int k) {
  for (int i = 0; i < entry.size(); i++) {
    entry[i] *= k;
  }
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
  MultiplyVectorByScalar(R1, x);
  MultiplyVectorByScalar(R2, y);
  for (size_t i = 0; i < R1.size(); i++) {
    R1[i] += R2[i];  // Adding R1 and R2
  }
  A.erase(A.begin() + index);       // Taking away old R1
  A.insert(A.begin() + index, R1);  // Adding new R1
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
            // Dummy = add(Dummy[i], Dummy[j], Dummy, i, x, y); (removed because
            // it is bashing)
            std::cout << "before push_back\n";
            Moves.push_back(std::make_pair(x, y));
            --counter;
            std::cout << "about to recurse again\n";
            recursionfordataset(range, Dummy);
            if (x != 0 && y != 0) {
              std::cout << "x, y are non-zero, calling add again\n";
              // add(Dummy[i], Dummy[j], Dummy, i, 1 / x, 1 / y); (removed
              // because it is bashing)
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

void generatelooserdataset(std::vector<std::vector<int>> NewDummy) {
  // This uses my conjecture, now proven
  std::vector<std::vector<int>> I;
  std::vector<std::vector<int>> Moves2;
  create_identity_matrix(I, NewDummy.size());
  for (int row = 0; row < NewDummy.size(); row++) {
    for (int column = 0; column < NewDummy.size(); column++) {
      if (row < column) {
        if (NewDummy[row + 1][column] == 0) {
          // No dividing by 0
          NewDummy[row + 1][column] = 1;
        }
        // I = add(
        //  I[row], I[row + 1], I, row, 1,
        // float(-NewDummy[row][column]) / float(NewDummy[row + 1][column]));
        // (we don't need the inverse for our dataset!)
        Moves2.push_back(
            {row, row + 1, 1,
             float(-NewDummy[row][column]) / float(NewDummy[row + 1][column])});
      }
      if (row == column) {
        if (NewDummy[row][column] == 0) {
          // No dividing by 0
          NewDummy[row][column] = 1;
          std::cout << "In here!" << std::endl;
        };
        //        I = add(I[row], I[row + 1], I, row,
        //             float(1) / float(NewDummy[row][column]), 0);
        Moves2.push_back(
            {row, row + 1, float(1) / float(NewDummy[row][column]), 0});
      }
      if (row > column) {
        //  I = add(I[row], I[column], I, row, 1, -NewDummy[row][column]);
        Moves2.push_back({row, column, 1, -NewDummy[row][column]});
      }
    }
  }
  // Moves is filled with entries of the form {row1, row2, constant1,
  // constant2}, which we will use to train our model!
  std::ofstream bufferfile;
  bufferfile.open("dataset.txt");
  for (auto& entry : NewDummy) {
    for (int i = 0; i < entry.size(); i++) {
      bufferfile << entry[i] << " ";
    }
    bufferfile << "\n";
  }
  bufferfile << "\n";
  for (auto& entry : Moves2) {
    for (int i = 0; i < entry.size(); i++) {
      bufferfile << entry[i] << " ";
    }
    bufferfile << "\n";
  }
  bufferfile << "\n"
             << "\n";
  bufferfile.close();
  for (auto& entry : NewDummy) {
    for (int i = 0; i < entry.size(); i++) {
      std::cout << entry[i] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
  for (auto& entry : Moves2) {
    for (int i = 0; i < entry.size(); i++) {
      std::cout << entry[i] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n"
            << "\n";
  Moves2.clear();
}

int scoringalg(std::vector<std::vector<float>> Dummy) {
  // We will score this matrix
  int score = 0;
  for (int i = 0; i < Dummy.size(); i++) {
    for (int j = 0; j < Dummy.size(); j++) {
      // Compare against the identity matrix
      if (i == j) {
        // should be 1
        score += pow(Dummy[i][j] - 1, 2);
      }
      if (i != j) {
        // should be 0
        score += pow(Dummy[i][j], 2);
      }
    }
  }
  return score;
}
int counter1 = 0;
std::vector<float> scores;
std::vector<std::vector<float>> record;
int minimax(std::vector<std::vector<float>> Dummy, int min, int max) {
  // Picture a tree. Suppose Dummy's minimum element is a, maximum is b. The set
  // of constants we can use is {a,a+1,...,b,1/a,1/(a+1),...,1/b}. So there are
  // 2*(b-a+1) branches from each node of the tree. The tree ends once n^2
  // levels have been completed (Dummy is n x n)
  std::vector<float> Constants;
  for (int i = min; i <= max; i++) {
    Constants.push_back(float(i));
    if (i != 0) {
      Constants.push_back(float(1) / float(i));
    }
  }
  Constants.push_back(float(0));
  if (counter1 == pow(Dummy.size(), 2)) {
    // Hit the end of a part on the tree
    scores.push_back(scoringalg(Dummy));
  }
  // Number of total nodes: |Constants| for each level, n^2 levels, so
  // |Constants|^(n^2), very large!
  if (counter1 == pow(Constants.size(), pow(Dummy.size(), 2))) {
    // Find minimum element, corresponding moves
    // In the record, there are n^2 moves for each element in scores
    int minimum_element = 1000;
    for (int i = 0; i < scores.size(); i++) {
      if (scores[i] < minimum_element) {
        minimum_element = scores[i];
      }
    }
    for (int i = 0; i < scores.size(); i++) {
      if (scores[i] == minimum_element) {
        return record[i];
      }
    }
  }
  for (auto& entry : Constants) {
    for (auto& entry2 : Constants) {
      for (int i = 0; i < Dummy.size(); i++) {
        for (int j = 0; j < Dummy.size(); j++) {
          Dummy = add(Dummy[i], Dummy[j], Dummy, entry, entry2);
          record.push_back({i, j, entry, entry2});
          counter1++;
          minimax(Dummy, min,
                  max);  // Recursion here, counter is breaking point
        }
      }
    }
  }
}
int main() {
  std::vector<std::vector<int>> A = {{1, 2}, {3, 4}};

  std::vector<std::vector<int>> B = add(A[0], A[1], A, 0, 1, 1);
  for (auto& entry : B) {
    std::cout << entry[0] << " " << entry[1] << std::endl;
  }

  std::vector<std::vector<int>> K;
  create_identity_matrix(K, 2);

  std::vector<std::vector<int>> Bfinal = add(K[0], K[1], K, 0, 1, 1);
  for (auto& entry : Bfinal) {
    std::cout << entry[0] << " " << entry[1] << std::endl;
  }

  bool value = TestIfIdentity(B);
  bool value2 = TestIfIdentity(Bfinal);
  std::cout << value << " " << value2 << std::endl;

  // Everything in the main section is testing the program on a random scenario.

  // recursionfordataset(5, Dummy); Removed because it is too bashy
  // Call new function on this
  for (int a = 1; a < 10; a++) {
    for (int b = 1; b < 10; b++) {
      for (int c = 1; c < 10; c++) {
        for (int d = 1; d < 10; d++) {
          generatelooserdataset({{a, b}, {c, d}});
        }
      }
    }
  }
}
