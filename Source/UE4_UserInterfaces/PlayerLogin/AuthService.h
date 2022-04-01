// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"

#include "AuthService.generated.h"

USTRUCT()
struct FAccount {
	GENERATED_BODY()

	UPROPERTY() FString Username;
	UPROPERTY() FString Password;
	UPROPERTY() FString Id;

	FAccount() { Username = ""; Password = false; Id = ""; }
};

USTRUCT()
struct FRequest_Login {
	GENERATED_BODY()

	UPROPERTY() FString Username;
	UPROPERTY() FString Password;

	FRequest_Login(FString _Username, FString _Password) { Username = _Username; Password = _Password; }
	FRequest_Login() {}
};

/**
 * 
 */
DECLARE_DELEGATE(FOnLoginSuccess)
UCLASS()
class UE4_USERINTERFACES_API UAuthService : public UObject
{
	GENERATED_BODY()

public:
	UAuthService();
	~UAuthService();

	void Login(FString Username, FString Password);

	FOnLoginSuccess OnLoginSuccess;

protected:
	FString ApiBaseUrl = "http://127.0.0.1:3000";

	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	void OnLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FHttpModule* Http;

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);

	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
	
};
