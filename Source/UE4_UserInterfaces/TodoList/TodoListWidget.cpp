// Fill out your copyright notice in the Description page of Project Settings.


#include "TodoListWidget.h"
#include "TaskWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/EditableTextBox.h"
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
		SaveTaskBtn->OnClicked.AddDynamic(this, &UTodoListWidget::OnSaveTaskPressed);
	}
	
	if (CancelEditTaskBtn)
	{
		CancelEditTaskBtn->OnClicked.AddDynamic(this, &UTodoListWidget::OnCancelEditTaskPressed);
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
			AddTaskToCompletedList(tasks[i], tasks[i].Id);
		}
		else
		{
			AddTaskToTodoList(tasks[i], tasks[i].Id);
		}
	}
}

void UTodoListWidget::OpenEditTaskView()
{
	if (!ensure(ViewSwitcher != nullptr)) return;
	if (!ensure(EditTaskView != nullptr)) return;

	if (EditedTask == nullptr)
	{
		EditedTask = new FTask();
	}
	else
	{
		if (TaskTextInput)
		{
			TaskTextInput->SetText(FText::FromString(EditedTask->Text));
		}
	}

	ViewSwitcher->SetActiveWidget(EditTaskView);
}

void UTodoListWidget::OpenTaskListView()
{
	if (!ensure(ViewSwitcher != nullptr)) return;
	if (!ensure(TaskListView != nullptr)) return;
	ViewSwitcher->SetActiveWidget(TaskListView);
}

void UTodoListWidget::AddTaskToCompletedList(FTask Task, FString TaskId)
{
	UTaskWidget* TaskWidget = CreateTaskWidget(Task, TaskId);
	if (!ensure(TaskWidget != nullptr)) return;

	if (!ensure(CompletedList != nullptr)) return;
	CompletedList->AddChild(TaskWidget);
}

void UTodoListWidget::AddTaskToTodoList(FTask Task, FString TaskId)
{
	UTaskWidget* TaskWidget = CreateTaskWidget(Task, TaskId);
	if (!ensure(TaskWidget != nullptr)) return;

	if (!ensure(TodoList != nullptr)) return;
	TodoList->AddChild(TaskWidget);
}

void UTodoListWidget::OnCancelEditTaskPressed()
{
	ResetEditedTask();
	OpenTaskListView();
	RefreshTasksLists();
}

void UTodoListWidget::OnSaveTaskPressed()
{
	if (!ensure(TasksService != nullptr)) return;

	FTask NewTask = *EditedTask;
	
	NewTask.Text = TaskTextInput->GetText().ToString();

	if (NewTask.Id == "") {
		NewTask.Completed = false;
		TasksService->AddTask(&NewTask);
	}
	else
	{
		TasksService->UpdateTaskById(NewTask.Id, &NewTask);
	}

	ResetEditedTask();
	OpenTaskListView();
	RefreshTasksLists();
}


void UTodoListWidget::OnCompletedChange(bool bIsCompleted, FString TaskId)
{
	if (!ensure(TasksService != nullptr)) return;

	FTask* FoundTask = TasksService->GetTaskById(TaskId);
	if (!FoundTask) return;

	FTask NewTask = *FoundTask;
	NewTask.Completed = bIsCompleted;

	TasksService->UpdateTaskById(TaskId, &NewTask);

	RefreshTasksLists();
}

void UTodoListWidget::OnEditTask(FString TaskId)
{
	if (!ensure(TasksService != nullptr)) return;
	EditedTask = TasksService->GetTaskById(TaskId);

	if (EditedTask != nullptr)
	{
		OpenEditTaskView();
	}
}

void UTodoListWidget::OnDeleteTask(FString TaskId)
{
	if (!ensure(TasksService != nullptr)) return;
	TasksService->DeleteTaskById(TaskId);

	RefreshTasksLists();
}

UTaskWidget* UTodoListWidget::CreateTaskWidget(FTask Task, FString TaskId)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return nullptr;

	UTaskWidget* TaskWidget = CreateWidget<UTaskWidget>(World, TaskWidgetClass);
	if (!ensure(TaskWidget != nullptr)) return nullptr;

	TaskWidget->SetText(Task.Text);
	TaskWidget->SetCompleted(Task.Completed);
	TaskWidget->SetTaskId(TaskId);

	/* If you want tot go the Task Interface way (DI) set the interface in TaskWidget */
	TaskWidget->SetTaskInterface(this);

	/* If you want to go the Event Deletages way - you should register you event handlers in each task */
	//TaskWidget->OnCompletedChanged.BindUObject(this, &UTodoListWidget::OnCompletedChange);

	return TaskWidget;
}

void UTodoListWidget::ResetEditedTask()
{
	if (TaskTextInput)
	{
		TaskTextInput->SetText(FText::AsCultureInvariant(""));
	}

	EditedTask = nullptr;
}
