// Fill out your copyright notice in the Description page of Project Settings.


#include "TasksService.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"

// For in-memory updates
//int32 Id_Base = 0;

UTasksService::UTasksService()
{
	Http = &FHttpModule::Get();
}

UTasksService::~UTasksService()
{
}

void UTasksService::AddTask(FTask *Task)
{
	CreateTaskRequest(Task);

	/* In-memory way */
	/*Task->id = FString::FromInt(++Id_Base);
	Tasks.Emplace(*Task);*/
}

void UTasksService::UpdateTaskById(FString TaskId, FTask* Task)
{
	UpdateTaskRequest(TaskId, Task);

	/* In-memory way */
	/*int32 TaskIndex = Tasks.IndexOfByPredicate([TaskId](const FTask& CurrentTask) {
		return CurrentTask.id == TaskId;
		});

	if (TaskIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find task with Id: %s"), *TaskId);
		return;
	}

	Tasks[TaskIndex] = *Task;*/
}

void UTasksService::DeleteTaskById(FString TaskId)
{
	DeleteTaskRequest(TaskId);

	/* In-memory way */
	/*int32 TaskIndex = Tasks.IndexOfByPredicate([TaskId](const FTask& CurrentTask) {
		return CurrentTask.id == TaskId;
		});

	if (TaskIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find task with Id: %s"), *TaskId);
		return;
	}

	Tasks.RemoveAt(TaskIndex);*/
}

FTask* UTasksService::GetTaskById(FString TaskId)
{
	return Tasks.FindByPredicate([TaskId](const FTask& Task) 
		{
			return Task.id == TaskId;
		}
	);
}

void UTasksService::FetchTasksRequest()
{
	if (!ensure(Http != nullptr)) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/tasks");
	SetRequestHeaders(Request);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &UTasksService::OnFetchTasksRequestComplete);
	Request->ProcessRequest();
}

void UTasksService::OnFetchTasksRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_FetchTasksList GetTasksListResponse;
	GetStructFromJsonString<FResponse_FetchTasksList>(Response, GetTasksListResponse);

	Tasks = GetTasksListResponse.tasks;

	// Broadcast
	OnTaskListUpdated.ExecuteIfBound();
}

void UTasksService::CreateTaskRequest(FTask* Task)
{
	if (!ensure(Http != nullptr)) return;

	FString ContentJsonString;
	FRequest_CreateTask RequestBody = FRequest_CreateTask();
	RequestBody.data = *Task;
	GetJsonStringFromStruct<FRequest_CreateTask>(RequestBody, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/tasks");
	SetRequestHeaders(Request);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UTasksService::OnCreateTaskRequestComplete);
	Request->ProcessRequest();
}

void UTasksService::OnCreateTaskRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;
	FetchTasksRequest();
}

void UTasksService::UpdateTaskRequest(FString TaskId, FTask* Task)
{
	if (!ensure(Http != nullptr)) return;

	FString ContentJsonString;
	FRequest_UpdateTask RequestBody = FRequest_UpdateTask();
	RequestBody.data = *Task;
	GetJsonStringFromStruct<FRequest_UpdateTask>(RequestBody, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/tasks/" + TaskId);
	SetRequestHeaders(Request);
	Request->SetVerb("PUT");
	Request->SetContentAsString(ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UTasksService::OnUpdateTaskRequestComplete);
	Request->ProcessRequest();
}

void UTasksService::OnUpdateTaskRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;
	FetchTasksRequest();
}

void UTasksService::DeleteTaskRequest(FString TaskId)
{
	if (!ensure(Http != nullptr)) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/tasks/" + TaskId + "/delete");
	SetRequestHeaders(Request);
	Request->SetVerb("POST");
	Request->OnProcessRequestComplete().BindUObject(this, &UTasksService::OnDeleteTaskRequestComplete);
	Request->ProcessRequest();
}

void UTasksService::OnDeleteTaskRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;
	FetchTasksRequest();
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
* Utilities
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