// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, ChangedMoney);

class AInventoryPC;
class AInventoryCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	void Inventory();
	void PickupMoney(int32 InMoney);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnMoneyChanged OnMoneyChanged;

	FORCEINLINE int32 GetMoneyAmount() { return MoneyAmount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

protected:
	virtual void BeginPlay() override;	
	AController* GetOwnerController();

private:
	UPROPERTY()
	AInventoryPC* PlayerController;

	UPROPERTY()
	AInventoryCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	int32 MoneyAmount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;
};
