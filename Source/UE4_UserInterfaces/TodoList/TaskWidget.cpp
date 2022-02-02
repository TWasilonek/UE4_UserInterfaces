// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskWidget.h"

#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"

void UTaskWidget::SetText(FString Text)
{
	if (!ensure(TextField != nullptr)) return;
	TextField->SetText(FText::AsCultureInvariant(Text));
}

void UTaskWidget::SetCompleted(bool Completed)
{
	if (!ensure(CompletedCheckbox != nullptr)) return;

	if (Completed)
	{
		CompletedCheckbox->SetCheckedState(ECheckBoxState::Checked);
	}
	else
	{
		CompletedCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}
