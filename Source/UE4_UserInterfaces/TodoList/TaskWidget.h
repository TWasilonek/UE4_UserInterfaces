// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskInterface.h"
#include "TaskWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_TwoParams(FOnCompletedChanged, bool, FString) // Param1 = bool bIsCompleted, Param2 = FString TaskId
UCLASS()
class UE4_USERINTERFACES_API UTaskWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void SetText(FString text);

	UFUNCTION()
		void SetCompleted(bool Completed);

	void SetTaskInterface(ITaskInterface* TaskInterface);

	FORCEINLINE void SetTaskId(FString _TaskId) { TaskId = _TaskId;  }

	FOnCompletedChanged OnCompletedChanged;

protected:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextField;

	UPROPERTY(meta = (BindWidget))
		class UCheckBox* CompletedCheckbox;

	UPROPERTY(meta = (BindWidget))
		class UButton* EditBtn;

	UPROPERTY(meta = (BindWidget))
		UButton* DeleteBtn;

	virtual bool Initialize() override;

private:
	FString TaskId;
	
	ITaskInterface* TaskInterface = nullptr;

	UFUNCTION()
	void HandleCompleteCheckboxChange(bool bIsChecked);

	UFUNCTION()
	void HandleEditBtnClicked();

	UFUNCTION()
	void HandleDeleteBtnClicked();
};
