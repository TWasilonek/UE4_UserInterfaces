// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TodoListWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4_USERINTERFACES_API UTodoListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTodoListWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY()
	class UTasksService* TasksService = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UButton* AddTaskBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelEditTaskBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* SaveTaskBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ViewSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UWidget* TaskListView = nullptr;

	UPROPERTY(meta = (BindWidget))
		UWidget* EditTaskView = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* TodoList;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* CompletedList;

	virtual bool Initialize() override;

	UFUNCTION()
	void SaveTask();

private:
	UFUNCTION()
		void OpenEditTaskView();

	UFUNCTION()
		void OpenTaskListView();
};
