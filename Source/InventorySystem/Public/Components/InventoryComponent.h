// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, ChangedMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwordTabSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldTabSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEatableTabSelected);

class AInventoryPC;
class AInventoryCharacter;

USTRUCT(BlueprintType)
struct FItemSearchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FSlotStruct Item;

	UPROPERTY(BlueprintReadWrite)
	bool bFoundItem;

	UPROPERTY(BlueprintReadWrite)
	AActor* ItemActor;

	FItemSearchResult()
		: bFoundItem(false)
		, ItemActor(nullptr)
	{}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	void Inventory();
	void PickupMoney(int32 InMoney);
	FItemSearchResult TraceItemToPickUp();

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float HealthToIncrease);

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float HealthToDecrease);

	/** Getter */
	FORCEINLINE int32 GetMoneyAmount() { return MoneyAmount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	/** end Getter */

	/** Delegate */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnMoneyChanged OnMoneyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSwordTabSelected OnSwordTabSelected;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnShieldTabSelected OnShieldTabSelected;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnEatableTabSelected OnEatableTabSelected;
	/** end Delegate */

protected:
	virtual void BeginPlay() override;	
	AController* GetOwnerController();
	void InitializeWidgets();

private:
	UPROPERTY()
	AInventoryPC* PlayerController;

	UPROPERTY()
	AInventoryCharacter* PlayerCharacter;

	/** Widgets */
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY()
	UUserWidget* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> InteractWidgetClass;

	UPROPERTY()
	UUserWidget* InteractWidget;
	/** end Widgets */

	int32 MoneyAmount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float ItemTraceRange;

	FItemSearchResult FoundItem;
};
