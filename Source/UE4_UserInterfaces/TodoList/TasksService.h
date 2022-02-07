// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Runtime/Online/HTTP/Public/Http.h"

#include "TasksService.generated.h"

USTRUCT()
struct FTask {
	GENERATED_BODY()

	UPROPERTY() FString text;
	UPROPERTY() bool completed;
	UPROPERTY() FString id;

	FTask() { text = ""; completed = false; id = ""; }
};

USTRUCT()
struct FResponse_GetTasksList {
	GENERATED_BODY()
	UPROPERTY() TArray<FTask> tasks;

	FResponse_GetTasksList() {}
};


DECLARE_DELEGATE(FOnTaskListUpdated)
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

	void FetchTasksList();

	FORCEINLINE TArray<FTask> GetTasks() const { return Tasks; }

	FOnTaskListUpdated OnTaskListUpdated;

protected:
	TArray<FTask> Tasks;

private:
	FHttpModule* Http;
	FString ApiBaseUrl = "http://127.0.0.1:3000";

	void OnFetchTasksListComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);

	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
};
