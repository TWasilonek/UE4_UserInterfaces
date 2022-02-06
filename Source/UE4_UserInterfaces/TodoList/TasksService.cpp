// Fill out your copyright notice in the Description page of Project Settings.


#include "TasksService.h"

// TODO: Just for testing
int32 Id_Base = 0;

UTasksService::UTasksService()
{
	// TODO: Add some tasks for testing
	FTask task1 = FTask();
	task1.Text = TEXT("Finish the Task Service");
	task1.Completed = true;
	task1.Id = FString::FromInt(++Id_Base);
	Tasks.Emplace(task1);

	FTask task2 = FTask();
	task2.Text = TEXT("Add Server and HTTP functionality");
	task2.Id = FString::FromInt(++Id_Base);
	Tasks.Emplace(task2);

	FTask task3 = FTask();
	task3.Text = TEXT("Make sure all buttons are doing what they should");
	task3.Id = FString::FromInt(++Id_Base);
	Tasks.Emplace(task3);
	
}

UTasksService::~UTasksService()
{
}

void UTasksService::AddTask(FTask *Task)
{
	Task->Id = FString::FromInt(++Id_Base);
	Tasks.Emplace(*Task);
}

void UTasksService::UpdateTaskById(FString TaskId, FTask* Task)
{
	int32 TaskIndex = Tasks.IndexOfByPredicate([TaskId](const FTask& CurrentTask) {
		return CurrentTask.Id == TaskId;
		});

	if (TaskIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find task with Id: %s"), *TaskId);
		return;
	}

	Tasks[TaskIndex] = *Task;
}

void UTasksService::DeleteTaskById(FString TaskId)
{
	int32 TaskIndex = Tasks.IndexOfByPredicate([TaskId](const FTask& CurrentTask) {
		return CurrentTask.Id == TaskId;
		});

	if (TaskIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find task with Id: %s"), *TaskId);
		return;
	}

	Tasks.RemoveAt(TaskIndex);
}

FTask* UTasksService::GetTaskById(FString TaskId)
{
	return Tasks.FindByPredicate([TaskId](const FTask& Task) 
		{
			return Task.Id == TaskId;
		}
	);
}