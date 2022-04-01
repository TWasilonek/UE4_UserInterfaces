// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

UPlayerLoginWidget::UPlayerLoginWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

bool UPlayerLoginWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (LoginBtn)
	{
		LoginBtn->OnClicked.AddDynamic(this, &UPlayerLoginWidget::OnLoginBtnPressed);
	}

	return true;
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

	UE_LOG(LogTemp, Warning, TEXT("Login btn pressed. Username: %s Password: %s"), *Username, *Password);
}
