// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLoginWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UE4_USERINTERFACES_API UPlayerLoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerLoginWidget(const FObjectInitializer& ObjectInitializer);
	
private:
	class UAuthService* AuthService = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* UsernameInput;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordInput;

	UPROPERTY(meta = (BindWidget))
	UButton* LoginBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorText;

	virtual bool Initialize() override;

	void SetupMouseInput();

	UFUNCTION()
	void OnLoginBtnPressed();

	UFUNCTION()
	void HandleLoginSuccess();

	UFUNCTION()
	void HandleLoginError(int32 HTTPCode);
};
