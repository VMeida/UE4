// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
// #include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    // FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    // Isograms = GetValidWords(Words);

    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word){
        return Word.Len() <= 10 && Word.Len() >= 4 && IsIsogram(Word); 
    });

    SetupGame();

    PrintLine(TEXT("The Hidden word is %s"), *HiddenWord);
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver == true)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PLayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcome the player
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word"),HiddenWord.Len());
    PrintLine(TEXT("To play, press TAB,\nguess the word then press enter\n"));
    PrintLine(TEXT("You have %i Lives"),Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("To play again press ENTER"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You win!"));
        EndGame();
        return;
    }

    // Check Right Number Of Characters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Wrong!"));
        PrintLine(TEXT("The Hidden word is %i letters long!\n"), HiddenWord.Len());
        return;
    }

    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    // Remove Life
    --Lives;
    PrintLine(TEXT("Lost a life"));

    // Check If Lives > 0
    if (Lives == 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("You Lost!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Wrong!"));
    PrintLine(TEXT("You have %i lives left"),Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 Index = 0; Index < Word.Len();Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
    
    // For each letter, if Guess word position equals to HiddenWord then true
    // Else, for each letter remaining in Guess, check if exists in HiddenWord
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        
        for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenWordIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}