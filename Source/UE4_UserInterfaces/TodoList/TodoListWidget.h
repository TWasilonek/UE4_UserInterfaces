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

	virtual bool Initialize() override;

	UFUNCTION()
	void SaveTask();

	/* ITaskInterface implementation */
	UFUNCTION()
		void OnCompletedChange(bool bIsCompleted, int32 TaskIndex) override;

private:
	TSubclassOf<class UUserWidget> TaskWidgetClass;

	UFUNCTION()
		void OpenEditTaskView();

	UFUNCTION()
		void OpenTaskListView();

	UFUNCTION()
		void AddTaskToCompletedList(FTask Task, int32 Index);

	UFUNCTION()
		void AddTaskToTodoList(FTask Task, int32 Index);

	UFUNCTION()
		void RefreshTasksLists();

	UFUNCTION()
	UTaskWidget* CreateTaskWidget(FTask Task, int32 Index);
};
