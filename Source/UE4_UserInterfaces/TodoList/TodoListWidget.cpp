// Fill out your copyright notice in the Description page of Project Settings.


#include "TodoListWidget.h"
#include "TaskWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "UObject/ConstructorHelpers.h"

UTodoListWidget::UTodoListWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> TaskWidgetBPClass(TEXT("/Game/TodoList/UI/WBP_Task"));
	if (!ensure(TaskWidgetBPClass.Class != nullptr)) return;

	TaskWidgetClass = TaskWidgetBPClass.Class;
}

bool UTodoListWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	TasksService = NewObject<UTasksService>();
	if (TasksService)
	{
		UE_LOG(LogTemp, Warning, TEXT("Created a new taks service"));
		RefreshTasksLists();
	}

	if (AddTaskBtn)
	{
		AddTaskBtn->OnClicked.AddDynamic(this, &UTodoListWidget::OpenEditTaskView);
	}
	
	if (SaveTaskBtn)
	{
		SaveTaskBtn->OnClicked.AddDynamic(this, &UTodoListWidget::SaveTask);
	}
	
	if (CancelEditTaskBtn)
	{
		CancelEditTaskBtn->OnClicked.AddDynamic(this, &UTodoListWidget::OpenTaskListView);
	}

	return true;
}

void UTodoListWidget::RefreshTasksLists()
{
	if (!ensure(TodoList != nullptr)) return;
	TodoList->ClearChildren();

	if(!ensure(CompletedList != nullptr)) return;
	CompletedList->ClearChildren();

	if (!ensure(TasksService != nullptr)) return;
	TArray<FTask> tasks = TasksService->GetTasks();

	for (int32 i = 0; i < tasks.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task: %s completed: %i"), *tasks[i].Text, tasks[i].Completed);
		if (tasks[i].Completed)
		{
			AddTaskToCompletedList(tasks[i], i);
		}
		else
		{
			AddTaskToTodoList(tasks[i], i);
		}
	}
}

void UTodoListWidget::SaveTask()
{
	UE_LOG(LogTemp, Warning, TEXT("Saving task"));

	OpenTaskListView();
}

void UTodoListWidget::OpenEditTaskView()
{
	if (!ensure(ViewSwitcher != nullptr)) return;
	if (!ensure(EditTaskView != nullptr)) return;
	ViewSwitcher->SetActiveWidget(EditTaskView);
}

void UTodoListWidget::OpenTaskListView()
{
	if (!ensure(ViewSwitcher != nullptr)) return;
	if (!ensure(TaskListView != nullptr)) return;
	ViewSwitcher->SetActiveWidget(TaskListView);
}

void UTodoListWidget::AddTaskToCompletedList(FTask Task, int32 Index)
{
	UTaskWidget* TaskWidget = CreateTaskWidget(Task, Index);
	if (!ensure(TaskWidget != nullptr)) return;

	if (!ensure(CompletedList != nullptr)) return;
	CompletedList->AddChild(TaskWidget);
}

void UTodoListWidget::AddTaskToTodoList(FTask Task, int32 Index)
{
	UTaskWidget* TaskWidget = CreateTaskWidget(Task, Index);
	if (!ensure(TaskWidget != nullptr)) return;

	if (!ensure(TodoList != nullptr)) return;
	TodoList->AddChild(TaskWidget);
}

void UTodoListWidget::HandleTaskCompletedChange(bool bIsCompleted, int32 TaskIndex)
{
	if (!ensure(TasksService != nullptr)) return;

	FTask UpdatedTask = TasksService->GetTaskByIndex(TaskIndex);
	if (!UpdatedTask.Exists) return;

	UpdatedTask.Completed = bIsCompleted;

	TasksService->SaveTaskByIndex(TaskIndex, UpdatedTask);
	
	RefreshTasksLists();
}

UTaskWidget* UTodoListWidget::CreateTaskWidget(FTask Task, int32 Index)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return nullptr;

	UTaskWidget* TaskWidget = CreateWidget<UTaskWidget>(World, TaskWidgetClass);
	if (!ensure(TaskWidget != nullptr)) return nullptr;

	TaskWidget->SetText(Task.Text);
	TaskWidget->SetCompleted(Task.Completed);
	TaskWidget->SetIndex(Index);

	TaskWidget->OnCompletedChanged.BindUObject(this, &UTodoListWidget::HandleTaskCompletedChange);

	return TaskWidget;
}
