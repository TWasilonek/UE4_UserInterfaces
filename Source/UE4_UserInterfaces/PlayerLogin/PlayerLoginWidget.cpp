// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLoginWidget.h"
#include "AuthService.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

UPlayerLoginWidget::UPlayerLoginWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

bool UPlayerLoginWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	AuthService = NewObject<UAuthService>();
	if (AuthService)
	{
		AuthService->OnLoginSuccess.BindUObject(this, &UPlayerLoginWidget::HandleLoginSuccess);
	}

	if (LoginBtn)
	{
		LoginBtn->OnClicked.AddDynamic(this, &UPlayerLoginWidget::OnLoginBtnPressed);
	}

	SetupMouseInput();

	return true;
}

void UPlayerLoginWidget::SetupMouseInput()
{
	// this might not work right away. In the Widget BP make sure that the top-level element in the tree has isFocusable ticked
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UPlayerLoginWidget::OnLoginBtnPressed()
{
	FString Username = TEXT("");
	FString Password = TEXT("");

	if (UsernameInput)
	{
		Username = UsernameInput->GetText().ToString();
	}

	if (PasswordInput)
	{
		Password = PasswordInput->GetText().ToString();
	}

	if (AuthService)
	{
		AuthService->Login(Username, Password);
	}
}

void UPlayerLoginWidget::HandleLoginSuccess()
{
	UE_LOG(LogTemp, Warning, TEXT("Login successful!"));
}
