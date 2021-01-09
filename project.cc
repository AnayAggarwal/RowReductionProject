#include <algorithm>
#include <array>
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

int counter = 9;                      // For 2x2 it's 9 moves at max
std::vector<std::vector<int>> Moves;  // Vector of moves

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
// x and y are the essential coefficients. The rest is looping through Dummy. We
// are using a recursion (calling the function inside itself) with a break
// condition.
void recursionfordataset(int range, std::vector<std::vector<int>> Dummy) {
  if (counter != 0) {
    for (int x = -range; x <= range; x++) {
      for (int y = -range; y <= range; y++) {
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 2; j++) {
            if (i != j) {
              Dummy = add(Dummy[i], Dummy[j], Dummy, i, x, y);
              std::vector<int> move = {x, y};
              Moves.push_back(move);
              counter -= 1;
              recursionfordataset(range, Dummy);
            }
          }
        }
      }
    }
  } else {
    bool yesno = TestIfIdentity(Dummy);
    if (yesno == true) {
      return;
    } else {
      Moves.clear();
      counter = 9;
      recursionfordataset(range, Dummy);
    }
  }
}

bool TestIfIdentity(std::vector<std::vector<int>> I) {
  int counter = 0;
  for (int iterator = 0; iterator < I.size(); iterator++) {
    std::vector<int> Dummy = I[iterator];
    for (int iterator2 = 0; iterator2 < Dummy.size(); iterator2++) {
      if (iterator = iterator2) {
        if (Dummy[iterator2] == 1) {
          counter++;
        }
      } else {
        if (Dummy[iterator2] == 0) {
          counter++;
        }
      }
    }
  }
  int desired = I.size() * I.size();
  if (counter == desired) {
    return true;
  } else {
    return false;
  }
}

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
  bool value = TestIfIdentity(B);
  bool value2 = TestIfIdentity(Bfinal);
  std::cout << value << " " << value2 << std::endl;
  // Everything in the main section is testing the program on a random scenario.
}
