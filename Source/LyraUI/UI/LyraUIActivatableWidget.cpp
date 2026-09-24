// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraUIActivatableWidget.h"

#include "Input/UIActionBindingHandle.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraUIActivatableWidget)

TOptional<FUIInputConfig> ULyraUIActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case ELyraUIWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);

	case ELyraUIWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);

	case ELyraUIWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);

	case ELyraUIWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
