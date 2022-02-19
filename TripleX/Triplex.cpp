#include <iostream>
#include <cstdlib>
#include <ctime>

void PrintIntroduction(int Difficulty)
{
  //Game intro
  std::cout << "\n\nYou are a secret agent breaking into a level " << Difficulty;
  std::cout << "\nEnter the correct code to continue...\n\n";
}

bool PlayGame(int Difficulty)
{
  PrintIntroduction(Difficulty);
  // Init numbers
  const int CodeA = rand() % Difficulty + Difficulty;
  const int CodeB = rand() % Difficulty + Difficulty;
  const int CodeC = rand() % Difficulty + Difficulty;

  // Calculation
  const int CodeSum = CodeA + CodeB + CodeC;
  const int CodeProduct = CodeA * CodeB * CodeC; 
  
  // Output hints
  std::cout << "+ There are 3 numbers in the code\n";
  std::cout << "+ The codes add up to: " << CodeSum;
  std::cout << "\n+ The codes multiply to give: " << CodeProduct << std::endl;
  //Input player guess
  int GuessA, GuessB, GuessC;
  
  std::cin >> GuessA;
  std::cin >> GuessB;
  std::cin >> GuessC;
  std::cout << "\nPlayer Guess is: " << GuessA << GuessB << GuessC;

  // Calculate player guess
  int GuessSum = GuessA + GuessB + GuessC;
  int GuessProduct = GuessA * GuessB * GuessC;

  // Check if player wins
  if (GuessSum == CodeSum && GuessProduct == CodeProduct)
  {
    std::cout << "\nGreat, prepare for the next level!";
    return true;
  }
  else
  {
    std::cout << "\nWrong! Try again before they notice you!";
    return false;
  }
}

int main()
{
  srand(time(NULL)); // cerates new random sequence based on time of day
  
  int LevelDifficulty = 1;
  int MaxDifficulty = 5;

  while (LevelDifficulty <= MaxDifficulty) // Loop game until all levels are competed
  {
    bool bLevelComplete = PlayGame(LevelDifficulty);
    std::cin.clear(); // clears any errors 
    std::cin.ignore(); // discards the buffer

    if (bLevelComplete)
    {
      ++LevelDifficulty;
    }
  }
  std::cout << "\n Great work agent! You got all the files, now get out of there!"; 

  return 0;
}