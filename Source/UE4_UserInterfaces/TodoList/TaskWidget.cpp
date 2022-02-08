// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskWidget.h"

#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"

bool UTaskWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (CompletedCheckbox)
	{
		CompletedCheckbox->OnCheckStateChanged.AddDynamic(this, &UTaskWidget::HandleCompleteCheckboxChange);
	}
	
	if (EditBtn)
	{
		EditBtn->OnClicked.AddDynamic(this, &UTaskWidget::HandleEditBtnClicked);
	}

	if (DeleteBtn)
	{
		DeleteBtn->OnClicked.AddDynamic(this, &UTaskWidget::HandleDeleteBtnClicked);
	}

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

void UTaskWidget::SetTaskInterface(ITaskInterface* _TaskInterface)
{
	this->TaskInterface = _TaskInterface;
}

void UTaskWidget::HandleCompleteCheckboxChange(bool bIsChecked)
{
	// broadcast event
	//OnCompletedChanged.ExecuteIfBound(bIsChecked, TaskId);
	
	if (TaskInterface) 
	{
		TaskInterface->OnCompletedChange(bIsChecked, TaskId);
	}
}

void UTaskWidget::HandleEditBtnClicked()
{
	if (TaskInterface)
	{
		TaskInterface->OnEditTask(TaskId);
	}
}

void UTaskWidget::HandleDeleteBtnClicked()
{
	if (TaskInterface)
	{
		TaskInterface->OnDeleteTask(TaskId);
	}
}