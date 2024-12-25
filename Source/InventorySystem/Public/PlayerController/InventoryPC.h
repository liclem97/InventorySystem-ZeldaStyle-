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
	void Inventory();

	float GetMouseSensitivity() const { return MouseSensitivity; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

};
