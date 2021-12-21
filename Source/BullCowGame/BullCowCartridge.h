// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;

	void GetBullCows(const FString& HiddenWord, const FString& Guess, int32& BullCount, int32& CowCount) const
	{
		BullCount = 0;
		CowCount = 0;

		for (int32 i = 0; i < Guess.Len(); i++)
		{
			if (Guess[i] == HiddenWord[i])
			{
				BullCount++;
				continue;
			}
			for (int32 j = 0; j < Guess.Len(); j++)
			{
				if (Guess[j] == HiddenWord[i])
				{
					CowCount++;
					break;
				}
			}
		}
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void ShowHUD(const FString& Guess);
	void GameOver();
	void ProcessGuess(const FString& Input);
	static bool IsIsogram(const FString& Word);
	TArray<FString> GetValidWords(const TArray<FString>& Words);
	FBullCowCount GetBullCows(const FString& Guess) const;

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 Lives;
	bool bGameOver;
	TArray<FString> ValidWords;
	
};
