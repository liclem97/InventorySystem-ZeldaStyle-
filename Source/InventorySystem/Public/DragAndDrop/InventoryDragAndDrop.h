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

public:
	FORCEINLINE FSlotStruct GetItem() const { return Item; }
	FORCEINLINE int32 GetIndex() const { return Index; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FSlotStruct Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 Index;
};
