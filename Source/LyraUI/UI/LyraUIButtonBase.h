// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"

#include "LyraUIButtonBase.generated.h"

class UCommonTextBlock;

/**
 * Minimal project button base built on CommonUI.
 *
 * CommonButtonBase owns interaction, focus, selection, and state styling.
 * This class only supplies a reusable label and keeps that label synchronized
 * with the text style selected by the current CommonButtonStyle state.
 */
UCLASS(Abstract, Blueprintable)
class LYRAUI_API ULyraUIButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(const FText& InButtonText);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;

	/** Label configured per button instance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (ExposeOnSpawn = true))
	FText ButtonText;

	/** Required Common Text child in the widget blueprint. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_ButtonLabel;

private:
	void RefreshButtonText();
	void RefreshButtonTextStyle();
};
