// Fill out your copyright notice in the Description page of Project Settings.


#include "TasksService.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"

// TODO: Just for testing
int32 Id_Base = 0;

UTasksService::UTasksService()
{
	Http = &FHttpModule::Get();
}

UTasksService::~UTasksService()
{
}

void UTasksService::AddTask(FTask *Task)
{
	Task->id = FString::FromInt(++Id_Base);
	Tasks.Emplace(*Task);
}

void UTasksService::UpdateTaskById(FString TaskId, FTask* Task)
{
	int32 TaskIndex = Tasks.IndexOfByPredicate([TaskId](const FTask& CurrentTask) {
		return CurrentTask.id == TaskId;
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
		return CurrentTask.id == TaskId;
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
			return Task.id == TaskId;
		}
	);
}

void UTasksService::FetchTasksList()
{
	if (!ensure(Http != nullptr)) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/tasks");
	SetRequestHeaders(Request);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &UTasksService::OnFetchTasksListComplete);
	Request->ProcessRequest();
}

void UTasksService::OnFetchTasksListComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_GetTasksList GetTasksListResponse;
	GetStructFromJsonString<FResponse_GetTasksList>(Response, GetTasksListResponse);

	Tasks = GetTasksListResponse.tasks;

	// Broadcast
	OnTaskListUpdated.ExecuteIfBound();
}

void UTasksService::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request)
{
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

bool UTasksService::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}

/*
* Utilities - TODO: Move to separate class
*/
template <typename StructType>
void  UTasksService::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput) {
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void  UTasksService::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}