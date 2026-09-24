// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"

#include "LyraUIActivatableWidget.generated.h"

struct FUIInputConfig;

/** Input behavior requested while an activatable screen is the active UI leaf. */
UENUM(BlueprintType)
enum class ELyraUIWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * Project base class for screens managed by CommonUI.
 * Screens declare their desired input behavior as class defaults; CommonUI
 * applies that configuration while the screen is the active input leaf.
 */
UCLASS(Abstract, Blueprintable)
class LYRAUI_API ULyraUIActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

protected:
	/** Controls whether this screen routes input to the game, UI, or both. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	ELyraUIWidgetInputMode InputConfig = ELyraUIWidgetInputMode::Default;

	/** Mouse capture behavior used by Game and GameAndMenu modes. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
