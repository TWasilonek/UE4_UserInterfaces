// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "TasksService.generated.h"

USTRUCT()
struct FTask {
	GENERATED_BODY()

	UPROPERTY() FString Text;
	UPROPERTY() bool Completed;
	UPROPERTY() FString Id;

	FTask() { Text = ""; Completed = false; Id = ""; }
};


/**
 * 
 */
UCLASS()
class UE4_USERINTERFACES_API UTasksService : public UObject
{
	GENERATED_BODY()

public:
	UTasksService();
	~UTasksService();

	void AddTask(FTask* task);

	void UpdateTaskById(FString TaskId, FTask* Task);

	void DeleteTaskById(FString TaskId);

	FTask* GetTaskById(FString TaskId);

	FORCEINLINE TArray<FTask> GetTasks() const { return Tasks; }

protected:
	TArray<FTask> Tasks;
};
