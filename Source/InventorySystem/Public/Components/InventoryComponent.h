// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AInventoryPC;
class AInventoryCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	void Inventory();

	FORCEINLINE TSubclassOf<UUserWidget> GetInventoryWidgetClass() const { return InventoryWidgetClass; }

protected:
	virtual void BeginPlay() override;	

private:
	AInventoryPC* PlayerController;

	AInventoryCharacter* InventoryCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
};
