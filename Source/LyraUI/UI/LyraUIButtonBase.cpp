// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraUIButtonBase.h"

#include "CommonTextBlock.h"

void ULyraUIButtonBase::SetButtonText(const FText& InButtonText)
{
	ButtonText = InButtonText;
	RefreshButtonText();
}

void ULyraUIButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	RefreshButtonText();
	RefreshButtonTextStyle();
}

void ULyraUIButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	RefreshButtonTextStyle();
}

void ULyraUIButtonBase::RefreshButtonText()
{
	if (Text_ButtonLabel)
	{
		Text_ButtonLabel->SetText(ButtonText);
	}
}

void ULyraUIButtonBase::RefreshButtonTextStyle()
{
	if (Text_ButtonLabel)
	{
		Text_ButtonLabel->SetStyle(GetCurrentTextStyleClass());
	}
}
