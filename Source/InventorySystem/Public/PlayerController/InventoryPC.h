// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryPC.generated.h"

class UInventoryComponent;
class UUserWidget;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInventoryPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	float GetMouseSensitivity() const { return MouseSensitivity; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float MouseSensitivity;

private:

};
