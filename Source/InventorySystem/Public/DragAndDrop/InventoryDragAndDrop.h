// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory.h"
#include "InventoryDragAndDrop.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryDragAndDrop : public UDragDropOperation
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FSlotStruct Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 Index;
};
