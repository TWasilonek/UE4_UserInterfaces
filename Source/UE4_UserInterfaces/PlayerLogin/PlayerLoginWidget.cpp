// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLoginWidget.h"
#include "AuthService.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

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
		AuthService->OnLoginError.BindUObject(this, &UPlayerLoginWidget::HandleLoginError);
	}

	if (LoginBtn)
	{
		LoginBtn->OnClicked.AddDynamic(this, &UPlayerLoginWidget::OnLoginBtnPressed);
	}

	if (ErrorText)
	{
		ErrorText->SetVisibility(ESlateVisibility::Collapsed);
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
	FString current = GetWorld()->GetMapName();
	current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // needed when in editor mode

	UGameplayStatics::OpenLevel(this, FName(TEXT("ProtectedMap")));
}

void UPlayerLoginWidget::HandleLoginError(int32 HTTPCode)
{
	if (ErrorText)
	{
		if (HTTPCode == 401)
		{
			ErrorText->SetText(FText::AsCultureInvariant(TEXT("You entered the wrong credentials.")));
		}
		else
		{
			ErrorText->SetText(FText::AsCultureInvariant(TEXT("Something went wrong, try again later.")));
		}

		ErrorText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
