// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();    

    TArray<FString> Words;
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    //FFileHelper::LoadFileToStringArray(Words, *WordListPath);    
    FFileHelper::LoadFileToStringArrayWithPredicate(this->ValidWords, *WordListPath, [](const FString& Word) { return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word); });

    FBullCowCount Count = {1, 1};

    PrintLine(TEXT("Bulls = %i, Cows = %i"), Count.Bulls, Count.Cows);

    SetupGame(); // Set up the game    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{        
    if (this->bGameOver == true)
    {
        ClearScreen();
        SetupGame();        
    }    
    else
    {
        if ( IsIsogram(Input))
        {
            //PrintLine("Its a isogram!!");
        }
        ProcessGuess(Input);
    }    
}

void UBullCowCartridge::SetupGame()
{        
    this->HiddenWord = this->ValidWords[FMath::RandRange(0, this->ValidWords.Num()-1)];
    this->Lives = this->HiddenWord.Len(); 
    this->bGameOver = false;

    // Welcoming the player
    PrintLine(TEXT("Welcome to Bull Cows! %s"), *HiddenWord);
    PrintLine(TEXT("Guess the letter word!!")); // Magic Number Remove!
    PrintLine(TEXT("Press enter to play again!"));

    const char HW[] = "cake";
    const TCHAR HW2[] = TEXT("CAKE");
    //PrintLine(TEXT("%c"), HW2[0]);
    for (FString Word : ValidWords)
    {
        //PrintLine(Word);
    }
    ShowHUD(TEXT(""));
}

void UBullCowCartridge::GameOver()
{    
    this->bGameOver = true;
    PrintLine(TEXT("Press ENTER to continue"));
}

void UBullCowCartridge::ShowHUD(const FString& Guess) 
{
    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    PrintLine(TEXT("You have %i lives!"), this->Lives);
    PrintLine(TEXT("Type the word:"));    
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have won!"));
        GameOver();
    } 
    else 
    {
        this->Lives -= 1;
        if (this->Lives >= 0)
        {
            if (Guess.Len() == HiddenWord.Len()) {
                PrintLine(TEXT("Have the same length!"));
            }
            ShowHUD(Guess);
        }
        else
        {
            PrintLine(TEXT("You have lost!"));
            GameOver();
        }            
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }   
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words)
{
    TArray<FString> ValidWordsTmp;
    for (FString Word : Words)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWordsTmp.Add(Word);
        }
    }
    return ValidWordsTmp;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == this->HiddenWord[i])
        {
            BullCount++;
            continue;
        }
        for (int32 j = 0; j < Guess.Len(); j++)
        {
            if (Guess[j] == this->HiddenWord[i])
            {
                CowCount++;
                break;
            }
        }
    }
}