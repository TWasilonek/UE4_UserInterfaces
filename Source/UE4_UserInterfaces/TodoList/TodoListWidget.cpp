// Fill out your copyright notice in the Description page of Project Settings.


#include "TodoListWidget.h"
#include "TaskWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
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

	// Fill lists
	TasksService = NewObject<UTasksService>();
	if (TasksService)
	{
		UE_LOG(LogTemp, Warning, TEXT("Created a new taks service"));
		TArray<FTask> tasks = TasksService->GetTasks();

		// TODO: create TaskWidgets for each of the task and add them to the correct Scroll widgets
		for (int32 i = 0; i < tasks.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Task: %s completed: %i"), *tasks[i].Text, tasks[i].Completed);
			if (tasks[i].Completed)
			{
				// Add it to CompletedList
				AddToCompletedList(tasks[i]);
			}
			else
			{
				// Add it to TodoList
				AddToTodoList(tasks[i]);
			}
		}
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

void UTodoListWidget::AddToCompletedList(FTask Task)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	UTaskWidget* TaskWidget = CreateWidget<UTaskWidget>(World, TaskWidgetClass);
	if (!ensure(TaskWidget != nullptr)) return;
	TaskWidget->SetText(Task.Text);
	TaskWidget->SetCompleted(Task.Completed);

	if (!ensure(CompletedList != nullptr)) return;
	CompletedList->AddChild(TaskWidget);
}

void UTodoListWidget::AddToTodoList(FTask Task)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	UTaskWidget* TaskWidget = CreateWidget<UTaskWidget>(World, TaskWidgetClass);
	if (!ensure(TaskWidget != nullptr)) return;
	TaskWidget->SetText(Task.Text);
	TaskWidget->SetCompleted(Task.Completed);

	if (!ensure(TodoList != nullptr)) return;
	TodoList->AddChild(TaskWidget);
}
