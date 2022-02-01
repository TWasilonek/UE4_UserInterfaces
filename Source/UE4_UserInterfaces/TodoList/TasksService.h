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

	FTask() { Completed = false; }
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

	void AddTask(FTask task);

	void SaveTaskByIndex(int32 index, FTask task);

	FTask DeleteTaskByIndex(int32 index);
	
	FTask GetTaskByIndex(int32 index);

	FORCEINLINE TArray<FTask> GetTasks() const { return Tasks; }

protected:
	TArray<FTask> Tasks;
};
