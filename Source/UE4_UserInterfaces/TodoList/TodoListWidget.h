// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TasksService.h"
#include "TaskInterface.h"
#include "Blueprint/UserWidget.h"
#include "TodoListWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4_USERINTERFACES_API UTodoListWidget : public UUserWidget, public ITaskInterface
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

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* TaskTextInput;

	virtual bool Initialize() override;

	UFUNCTION()
	void OnSaveTaskPressed();

	UFUNCTION()
	void OnCancelEditTaskPressed();

	/* ITaskInterface implementation */
	UFUNCTION()
		void OnCompletedChange(bool bIsCompleted, FString TaskId) override;

	UFUNCTION()
		void OnEditTask(FString TaskId) override;

	UFUNCTION()
		void OnDeleteTask(FString TaskId) override;
	/* ITaskInterface implementation END */

private:
	TSubclassOf<class UUserWidget> TaskWidgetClass;

	FTask* EditedTask = nullptr;

	UFUNCTION()
		void OpenEditTaskView();

	UFUNCTION()
		void OpenTaskListView();

	UFUNCTION()
		void AddTaskToCompletedList(FTask Task, FString TaskId);

	UFUNCTION()
		void AddTaskToTodoList(FTask Task, FString TaskId);

	UFUNCTION()
		void RefreshTasksLists();

	UFUNCTION()
		void ResetEditedTask();

	UFUNCTION()
	UTaskWidget* CreateTaskWidget(FTask Task, FString TaskId);
};
