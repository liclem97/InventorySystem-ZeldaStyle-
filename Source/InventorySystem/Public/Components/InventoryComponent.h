// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"
#include "InventoryComponent.generated.h"

class AInventoryPC;
class AInventoryCharacter;
class UInventoryDragAndDrop;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSearchResult, FItemSearchResult, SearchedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, ChangedMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, FAllItemStruct, AllItems);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwordTabSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldTabSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEatableTabSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	void Inventory();
	void PickupMoney(int32 InMoney);

	UFUNCTION(Category = "Inventory")
	FItemSearchResult TraceItemToPickUp();

	UFUNCTION(Category = "Inventory")
	bool AddItemToInventory(const FSlotStruct &InItem);

	bool AddItemToArray(TArray<FSlotStruct> &ItemArray, const FSlotStruct &InItem);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float HealthToIncrease);

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float HealthToDecrease);

	UFUNCTION(BlueprintCallable)
	void UseEatables(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DropEatables(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DropSword(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DropShield(int32 Index);

	/** Getter */
	FORCEINLINE int32 GetMoneyAmount() const { return MoneyAmount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FAllItemStruct GetAllItem() const { return AllItem; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UUserWidget* GetInventoryWidget();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FSlotStruct GetEquippedSword() const { return EquippedSword; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FSlotStruct GetEquippedShield() const { return EquippedShield; }
	/** end Getter */

	/** Setter */
	UFUNCTION(BlueprintCallable)
	void SetEquippedSword(FSlotStruct NewSword) { EquippedSword = NewSword; }

	UFUNCTION(BlueprintCallable)
	void SetEquippedShield(FSlotStruct NewShield) { EquippedShield = NewShield; }
	/** end Setter */

	/** Delegate */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnMoneyChanged OnMoneyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSwordTabSelected OnSwordTabSelected;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnShieldTabSelected OnShieldTabSelected;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnEatableTabSelected OnEatableTabSelected;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemSearchResult OnItemSearchResult;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnEquippedItem OnEquippedItem;
	/** end Delegate */

protected:
	virtual void BeginPlay() override;	
	AController* GetOwnerController();
	void InitializeWidgets();
	void ResizeInventory();
	bool IsSameItem(FSlotStruct DragAndDropItem, FSlotStruct SlotItem);

	/** Drag and Drop */
	UFUNCTION(BlueprintCallable)
	void DeleteDraggedSword(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DeleteDraggedShield(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DeleteDraggedEatable(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DropDraggedSword(UInventoryDragAndDrop* InventoryDragAndDrop, int32 SlotIndex, FSlotStruct SlotItem);

	UFUNCTION(BlueprintCallable)
	void DropDraggedShield(UInventoryDragAndDrop* InventoryDragAndDrop, int32 SlotIndex, FSlotStruct SlotItem);

	UFUNCTION(BlueprintCallable)
	void DropDraggedEatable(UInventoryDragAndDrop* InventoryDragAndDrop, int32 SlotIndex, FSlotStruct SlotItem);
	/* end Drag and Drop*/

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

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FAllItemStruct AllItem;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 SizeOfSwords;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 SizeOfShields;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 SizeOfEatables;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	UDataTable* ItemDataTable;

	UPROPERTY()
	FSlotStruct EquippedSword;

	UPROPERTY()
	FSlotStruct EquippedShield;
};
