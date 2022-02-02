// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskWidget.h"

#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"

bool UTaskWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	CompletedCheckbox->OnCheckStateChanged.AddDynamic(this, &UTaskWidget::HandleCompleteCheckboxChange);

	return true;
}

void UTaskWidget::SetText(FString Text)
{
	if (!ensure(TextField != nullptr)) return;
	TextField->SetText(FText::AsCultureInvariant(Text));
}

void UTaskWidget::SetCompleted(bool Completed)
{
	if (!ensure(CompletedCheckbox != nullptr)) return;
	CompletedCheckbox->SetIsChecked(Completed);
}

void UTaskWidget::HandleCompleteCheckboxChange(bool bIsChecked)
{
	// broadcast
	OnCompletedChanged.ExecuteIfBound(bIsChecked, Index);
}