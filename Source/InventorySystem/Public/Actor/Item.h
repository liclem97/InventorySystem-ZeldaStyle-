// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.h"
#include "Item.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void OnConstruction(const FTransform& Transform) override;

	FORCEINLINE FName GetItemName() const { return ItemName; }
	FORCEINLINE FSlotStruct GetItemData() const { return ItemData; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FSlotStruct ItemData;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	FName ItemName;
};
