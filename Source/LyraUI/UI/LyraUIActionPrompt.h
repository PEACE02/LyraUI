// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "Engine/DataTable.h"

#include "LyraUIActionPrompt.generated.h"

enum class ECommonInputType : uint8;

class UCommonActionWidget;
class UTextBlock;
class UWidget;

/**
 * Displays a CommonUI action using a generated key-cap label for keyboard
 * input and a CommonActionWidget icon for gamepad input. Touch prompts are hidden.
 */
UCLASS(Abstract, Blueprintable)
class LYRAUI_API ULyraUIActionPrompt : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** Changes the semantic action displayed by this prompt. */
	UFUNCTION(BlueprintCallable, Category = "Input Prompt")
	void SetInputAction(FDataTableRowHandle InInputAction);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Legacy CommonUI action row whose key or icon should be displayed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Prompt", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InputAction;

	/** Container for the generated keyboard key-cap. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> InputKeyContainer;

	/** Text placed over the keyboard key-cap background. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> InputKeyText;

	/** CommonUI icon used for gamepad and touch input. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> InputActionIcon;

private:
	void HandleInputMethodChanged(ECommonInputType NewInputType);
	void RefreshPrompt(ECommonInputType InputType);
};
