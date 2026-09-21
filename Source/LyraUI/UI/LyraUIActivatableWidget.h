// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"

#include "LyraUIActivatableWidget.generated.h"

/**
 * Minimal project base class for screens managed by CommonUI.
 *
 * This class intentionally adds no behavior yet. The first learning step is to
 * verify that the CommonUI plugin and module dependency are configured, and
 * that Blueprint screens can inherit from a project-owned native class.
 */
UCLASS(Abstract, Blueprintable)
class LYRAUI_API ULyraUIActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
};
