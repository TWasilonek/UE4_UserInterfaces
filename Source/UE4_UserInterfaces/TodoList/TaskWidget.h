// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4_USERINTERFACES_API UTaskWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void SetText(FString text);

	UFUNCTION()
		void SetCompleted(bool Completed); 

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextField;

	UPROPERTY(meta = (BindWidget))
		class UCheckBox* CompletedCheckbox;

	UPROPERTY(meta = (BindWidget))
		class UButton* EditBtn;

	UPROPERTY(meta = (BindWidget))
		UButton* DeleteBtn;
};
