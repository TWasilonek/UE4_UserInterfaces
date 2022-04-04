// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthService.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"

UAuthService::UAuthService()
{
	Http = &FHttpModule::Get();
}

UAuthService::~UAuthService()
{
}

void UAuthService::Login(FString Username, FString Password)
{
	if (!ensure(Http != nullptr)) return;

	FString ContentJsonString;
	FRequest_Login RequestBody = FRequest_Login(Username, Password);
	GetJsonStringFromStruct<FRequest_Login>(RequestBody, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/auth/login");
	SetRequestHeaders(Request);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &UAuthService::OnLoginComplete);
	Request->ProcessRequest();
}

void UAuthService::OnLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful))
	{
		OnLoginError.ExecuteIfBound(Response->GetResponseCode());
		return;
	}
	
	// Broadcast
	OnLoginSuccess.ExecuteIfBound();
}

void UAuthService::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request)
{
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

bool UAuthService::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
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
void  UAuthService::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput) {
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void  UAuthService::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}
