// Fill out your copyright notice in the Description page of Project Settings.


#include "TasksService.h"

UTasksService::UTasksService()
{
	// TODO: Add some tasks for testing
	FTask task1 = FTask();
	task1.Text = TEXT("Finish the Task Service");

	FTask task2 = FTask();
	task2.Text = TEXT("Add Firebase and HTTP functionality");

	Tasks.Emplace(task1);
	Tasks.Emplace(task2);
}

UTasksService::~UTasksService()
{
}

void UTasksService::AddTask(FTask task)
{
	Tasks.Emplace(task);
}

void UTasksService::SaveTaskByIndex(int32 index, FTask task)
{

	if (!Tasks.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Error, TEXT("TasksService: invalid index %i"), index);
		return;
	}
	
	Tasks[index] = task;
}

FTask UTasksService::DeleteTaskByIndex(int32 index)
{
	if (!Tasks.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Error, TEXT("TasksService: invalid index %i"), index);
		return FTask();
	}

	FTask deletedTask = Tasks[index];
	Tasks.RemoveAt(index);

	return deletedTask;
}

FTask UTasksService::GetTaskByIndex(int32 index)
{
	if (!Tasks.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Error, TEXT("TasksService: invalid index %i"), index);
		return FTask();
	}

	return Tasks[index];
}
