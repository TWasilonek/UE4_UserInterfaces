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
struct FResponse_FetchTasksList {
	GENERATED_BODY()
	UPROPERTY() TArray<FTask> tasks;

	FResponse_FetchTasksList() {}
};

USTRUCT()
struct FRequest_CreateTask {
	GENERATED_BODY()
	UPROPERTY() FTask data;

	FRequest_CreateTask() {}
};

USTRUCT()
struct FRequest_UpdateTask {
	GENERATED_BODY()
	UPROPERTY() FTask data;

	FRequest_UpdateTask() {}
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

	void FetchTasksRequest();

	FORCEINLINE TArray<FTask> GetTasks() const { return Tasks; }

	FOnTaskListUpdated OnTaskListUpdated;

protected:
	TArray<FTask> Tasks;

	FString ApiBaseUrl = "http://127.0.0.1:3000";

	void OnFetchTasksRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void CreateTaskRequest(FTask* Task);
	void UpdateTaskRequest(FString TaskId, FTask* Task);
	void DeleteTaskRequest(FString TaskId);

	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FHttpModule* Http;

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);

	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
};
