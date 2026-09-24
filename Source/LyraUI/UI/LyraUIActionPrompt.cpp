// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraUIActionPrompt.h"

#include "CommonActionWidget.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "CommonUITypes.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraUIActionPrompt)

void ULyraUIActionPrompt::SetInputAction(FDataTableRowHandle InInputAction)
{
	InputAction = InInputAction;

	if (InputActionIcon)
	{
		InputActionIcon->SetInputAction(InputAction);
	}

	const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();
	RefreshPrompt(InputSubsystem ? InputSubsystem->GetCurrentInputType() : ECommonInputType::MouseAndKeyboard);
}

void ULyraUIActionPrompt::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (InputActionIcon)
	{
		InputActionIcon->SetInputAction(InputAction);
	}

	if (IsDesignTime())
	{
		RefreshPrompt(ECommonInputType::MouseAndKeyboard);
	}
}

void ULyraUIActionPrompt::NativeConstruct()
{
	Super::NativeConstruct();

	if (InputActionIcon)
	{
		InputActionIcon->SetInputAction(InputAction);
	}

	if (UCommonInputSubsystem* InputSubsystem = GetInputSubsystem())
	{
		InputSubsystem->OnInputMethodChangedNative.RemoveAll(this);
		InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);
		RefreshPrompt(InputSubsystem->GetCurrentInputType());
	}
	else
	{
		RefreshPrompt(ECommonInputType::MouseAndKeyboard);
	}
}

void ULyraUIActionPrompt::NativeDestruct()
{
	if (UCommonInputSubsystem* InputSubsystem = GetInputSubsystem())
	{
		InputSubsystem->OnInputMethodChangedNative.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void ULyraUIActionPrompt::HandleInputMethodChanged(ECommonInputType NewInputType)
{
	RefreshPrompt(NewInputType);
}

void ULyraUIActionPrompt::RefreshPrompt(ECommonInputType InputType)
{
	const bool bUseGeneratedKeyCap = InputType == ECommonInputType::MouseAndKeyboard;
	const bool bUseInputIcon = InputType == ECommonInputType::Gamepad;

	if (InputActionIcon)
	{
		InputActionIcon->SetVisibility(bUseInputIcon ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (!InputKeyContainer || !InputKeyText)
	{
		return;
	}

	if (!bUseGeneratedKeyCap)
	{
		InputKeyContainer->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	const FCommonInputActionDataBase* ActionData = CommonUI::GetInputActionData(InputAction);
	const FKey Key = ActionData
		? ActionData->GetInputTypeInfo(ECommonInputType::MouseAndKeyboard, NAME_None).GetKey()
		: EKeys::Invalid;

	if (!Key.IsValid())
	{
		InputKeyContainer->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	InputKeyText->SetText(Key.GetDisplayName(false));
	InputKeyContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
