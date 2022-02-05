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
			AddTaskToCompletedList(tasks[i], i);
		}
		else
		{
			AddTaskToTodoList(tasks[i], i);
		}
	}
}


void UTodoListWidget::OpenEditTaskView()
{
	if (!ensure(ViewSwitcher != nullptr)) return;
	if (!ensure(EditTaskView != nullptr)) return;

	if (TaskTextInput && 
		EditedTask.Index > -1) // TODO: need a better way to identify Edit Mode and Add Mode
	{
		TaskTextInput->SetText(FText::FromString(EditedTask.Text));
	}

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

void UTodoListWidget::OnCompletedChange(bool bIsCompleted, int32 TaskIndex)
{
	if (!ensure(TasksService != nullptr)) return;

	FTask UpdatedTask = TasksService->GetTaskByIndex(TaskIndex);
	if (!UpdatedTask.Exists) return;

	UpdatedTask.Completed = bIsCompleted;

	TasksService->SaveTaskByIndex(TaskIndex, UpdatedTask);

	RefreshTasksLists();
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
	
	EditedTask.Text = TaskTextInput->GetText().ToString();

	if (EditedTask.Index > -1) { // TODO: Need a better way to identify Edit Mode
		// TODO: change to SaveTaskByID
		TasksService->SaveTaskByIndex(EditedTask.Index, EditedTask);
	}
	else
	{
		EditedTask.Exists = true;
		EditedTask.Completed = false;
		EditedTask.Index = TasksService->GetTasks().Num() + 1; // TODO: Remove this if possible
		
		TasksService->AddTask(EditedTask);
	}

	ResetEditedTask();
	OpenTaskListView();
	RefreshTasksLists();
}

void UTodoListWidget::OnEditTask(int32 Index)
{
	if (!ensure(TasksService != nullptr)) return;
	EditedTask = TasksService->GetTaskByIndex(Index);

	OpenEditTaskView();
}

void UTodoListWidget::OnDeleteTask(int32 Index)
{
	if (!ensure(TasksService != nullptr)) return;
	TasksService->DeleteTaskByIndex(Index);

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

	EditedTask = FTask();
}
