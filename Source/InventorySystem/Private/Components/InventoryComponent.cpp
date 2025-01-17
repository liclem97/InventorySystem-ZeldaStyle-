// Copyright liclem97.


#include "Components/InventoryComponent.h"

#include "Actor/Item.h"
#include "Blueprint/UserWidget.h"
#include "Character/InventoryCharacter.h"
#include "DragAndDrop/InventoryDragAndDrop.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerController/InventoryPC.h"

UInventoryComponent::UInventoryComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;
	ItemTraceRange = 120.f;

	SizeOfSwords = 5;
	SizeOfShields = 8;
	SizeOfEatables = 7;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeWidgets();
	SetInventorySize(SizeOfSwords, SizeOfShields, SizeOfEatables);
}

void UInventoryComponent::InitializeWidgets()
{
	PlayerController = PlayerController == nullptr ? Cast<AInventoryPC>(GetOwnerController()) : PlayerController;
	if (PlayerController)
	{
		if (IsValid(InventoryWidgetClass))
		{
			InventoryWidget = InventoryWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass) : InventoryWidget;

			// To construct InventoryWidget.
			InventoryWidget->AddToViewport();
			InventoryWidget->RemoveFromParent();
		}
		if (IsValid(HealthBarWidgetClass))
		{
			HealthBarWidget = HealthBarWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), HealthBarWidgetClass) : HealthBarWidget;
			HealthBarWidget->SetPositionInViewport(FVector2D(5.f, 5.f));
			HealthBarWidget->AddToViewport();
		}
		if (IsValid(InteractWidgetClass))
		{
			InteractWidget = InteractWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), InteractWidgetClass) : InteractWidget;
			
			// To construct InteractWidget.
			InteractWidget->AddToViewport();
			InteractWidget->RemoveFromParent();
		}
	}
}

void UInventoryComponent::SetInventorySize(int32 InSizeOfSwords, int32 InSizeOfShields, int32 InSizeOfEatables)
{
	AllItem.Swords.SetNum(InSizeOfSwords);
	AllItem.Shields.SetNum(InSizeOfShields);
	AllItem.Eatables.SetNum(InSizeOfEatables);

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::UpgradeInventory(int32 SwordAmount, int32 ShieldAmount, int32 EatableAmount)
{	
	int32 LengthOfSwords = AllItem.Swords.Num() + SwordAmount;
	int32 LengthOfShields = AllItem.Shields.Num() + ShieldAmount;
	int32 LengthOfEatables = AllItem.Eatables.Num() + EatableAmount;

	SetInventorySize(LengthOfSwords, LengthOfShields, LengthOfEatables);
}

bool UInventoryComponent::IsSameItem(FSlotStruct DragAndDropItem, FSlotStruct SlotItem)
{	
	return DragAndDropItem.ItemID.RowName == SlotItem.ItemID.RowName;
}

void UInventoryComponent::DeleteDraggedSword(int32 Index)
{
	AllItem.Swords[Index].ItemID.DataTable = nullptr;
	AllItem.Swords[Index].ItemID.RowName = NAME_None;
	AllItem.Swords[Index].Quantity = 0;
	AllItem.Swords[Index].ItemType = EItemTypes::Sword;

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::DeleteDraggedShield(int32 Index)
{
	AllItem.Shields[Index].ItemID.DataTable = nullptr;
	AllItem.Shields[Index].ItemID.RowName = NAME_None;
	AllItem.Shields[Index].Quantity = 0;
	AllItem.Shields[Index].ItemType = EItemTypes::Shield;

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::DeleteDraggedEatable(int32 Index)
{
	AllItem.Eatables[Index].ItemID.DataTable = nullptr;
	AllItem.Eatables[Index].ItemID.RowName = NAME_None;
	AllItem.Eatables[Index].Quantity = 0;
	AllItem.Eatables[Index].ItemType = EItemTypes::Eatable;

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::DropDraggedSword(UInventoryDragAndDrop* InventoryDragAndDrop, int32 SlotIndex, FSlotStruct SlotItem)
{	
	if (!IsValid(InventoryDragAndDrop)) return;

	int32 DraggedIndex = InventoryDragAndDrop->GetIndex();
	FSlotStruct DraggedItem = InventoryDragAndDrop->GetItem();

	if (IsSameItem(DraggedItem, SlotItem))
	{	
		AllItem.Swords[DraggedIndex] = DraggedItem;		
		OnInventoryUpdated.Broadcast(AllItem);
		return;
	}
	else
	{
		if (SlotItem.Quantity == 0) // Empty Slot.
		{	
			AllItem.Swords[SlotIndex] = DraggedItem;
			OnInventoryUpdated.Broadcast(AllItem);
			return;
		}
		else // Swap Item.
		{
			AllItem.Swords[DraggedIndex] = SlotItem;
			AllItem.Swords[SlotIndex] = DraggedItem;
			OnInventoryUpdated.Broadcast(AllItem);
			return;
		}
	}
}

void UInventoryComponent::DropDraggedShield(UInventoryDragAndDrop* InventoryDragAndDrop, int32 SlotIndex, FSlotStruct SlotItem)
{
	if (!IsValid(InventoryDragAndDrop)) return;

	int32 DraggedIndex = InventoryDragAndDrop->GetIndex();
	FSlotStruct DraggedItem = InventoryDragAndDrop->GetItem();

	if (IsSameItem(DraggedItem, SlotItem))
	{
		AllItem.Shields[DraggedIndex] = DraggedItem;
		OnInventoryUpdated.Broadcast(AllItem);
		return;
	}
	else
	{
		if (SlotItem.Quantity == 0) // Empty Slot.
		{
			AllItem.Shields[SlotIndex] = DraggedItem;
			OnInventoryUpdated.Broadcast(AllItem);
			return;
		}
		else // Swap Item.
		{
			AllItem.Shields[DraggedIndex] = SlotItem;
			AllItem.Shields[SlotIndex] = DraggedItem;
			OnInventoryUpdated.Broadcast(AllItem);
			return;
		}
	}
}

void UInventoryComponent::DropDraggedEatable(UInventoryDragAndDrop* InventoryDragAndDrop, int32 SlotIndex, FSlotStruct SlotItem)
{
	if (!IsValid(InventoryDragAndDrop)) return;

	int32 DraggedIndex = InventoryDragAndDrop->GetIndex();
	FSlotStruct DraggedItem = InventoryDragAndDrop->GetItem();

	if (IsSameItem(DraggedItem, SlotItem))
	{
		AllItem.Eatables[DraggedIndex] = DraggedItem;
		OnInventoryUpdated.Broadcast(AllItem);
		return;
	}
	else
	{
		if (SlotItem.Quantity == 0) // Empty Slot.
		{
			AllItem.Eatables[SlotIndex] = DraggedItem;
			OnInventoryUpdated.Broadcast(AllItem);
			return;
		}
		else // Swap Item.
		{
			AllItem.Eatables[DraggedIndex] = SlotItem;
			AllItem.Eatables[SlotIndex] = DraggedItem;
			OnInventoryUpdated.Broadcast(AllItem);
			return;
		}
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TraceItemToPickUp().bFoundItem && InteractWidget)
	{	
		OnItemSearchResult.Broadcast(TraceItemToPickUp());
		InteractWidget->AddToViewport();
	}
	else if (!TraceItemToPickUp().bFoundItem)
	{
		InteractWidget->RemoveFromParent();
	}
}

void UInventoryComponent::Inventory()
{	
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<AInventoryCharacter>(GetOwner()) : PlayerCharacter;
	if (PlayerCharacter && InventoryWidget)
	{
		if (PlayerCharacter->GetOpenedWidget()) // true
		{
			PlayerCharacter->SetOpenedWidget(false);
			InventoryWidget->RemoveFromParent();
			PlayerController->bShowMouseCursor = false;
		}
		else // false
		{
			PlayerCharacter->SetOpenedWidget(true);
			OnMoneyChanged.Broadcast(MoneyAmount);
			InventoryWidget->AddToViewport();
			PlayerController->bShowMouseCursor = true;
		}
	}	
}

void UInventoryComponent::PickupMoney(int32 InMoney)
{
	MoneyAmount += InMoney;
	OnMoneyChanged.Broadcast(MoneyAmount);
}

FItemSearchResult UInventoryComponent::TraceItemToPickUp()
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<AInventoryCharacter>(GetOwner()) : PlayerCharacter;
	FItemSearchResult Result;

	FVector Start = PlayerCharacter->GetActorLocation() - FVector(0, 0, 60.f);
	FVector End = (PlayerCharacter->GetActorForwardVector() * ItemTraceRange) + Start;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		30.f,
		Channel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::ForOneFrame,
		HitResult,
		true
	);

	if (bHit)
	{
		if (AItem* CastedItem = Cast<AItem>(HitResult.GetActor()))
		{
			Result.Item = CastedItem->GetItemData();
			Result.bFoundItem = true;
			Result.ItemActor = CastedItem;
		}
	}
	return Result;
}

bool UInventoryComponent::AddItemToInventory(const FSlotStruct& InItem)
{
	if (!IsValid(ItemDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Item DataTable is not valid."));
		return false;
	}

	switch (InItem.ItemType)
	{
	case EItemTypes::Sword:
		if (AddItemToArray(AllItem.Swords, InItem)) return true;
		break;
	case EItemTypes::Shield:
		if (AddItemToArray(AllItem.Shields, InItem)) return true;
		break;
	case EItemTypes::Eatable:
		if (AddItemToArray(AllItem.Eatables, InItem)) return true;
		break;		
	}
	return false;
}

bool UInventoryComponent::AddItemToArray(TArray<FSlotStruct>& ItemArray, const FSlotStruct& InItem)
{	
	int32 Index = 0;
	for (FSlotStruct ItemToAdd : ItemArray)
	{
		if (ItemToAdd.ItemID.RowName == InItem.ItemID.RowName)
		{
			FString ContextString;
			FItemStruct* RowData = ItemDataTable->FindRow<FItemStruct>(InItem.ItemID.RowName, ContextString);

			if (RowData->StackSize >= ItemToAdd.Quantity + InItem.Quantity)
			{
				ItemArray[Index].ItemID = InItem.ItemID;
				ItemArray[Index].Quantity = ItemToAdd.Quantity + InItem.Quantity;
				ItemArray[Index].ItemType = ItemToAdd.ItemType;

				return true;
			}
		}
		else if (ItemToAdd.Quantity == 0)
		{
			ItemArray[Index] = InItem;
			return true;
		}
		Index++;
	}
	return false;
}

void UInventoryComponent::IncreaseHealth(float HealthToIncrease)
{
	Health += HealthToIncrease;
	OnHealthChanged.Broadcast();
}

void UInventoryComponent::DecreaseHealth(float HealthToDecrease)
{
	Health -= HealthToDecrease;
	OnHealthChanged.Broadcast();
}

void UInventoryComponent::UseEatables(int32 Index)
{	
	FSlotStruct& EatableRef = AllItem.Eatables[Index];
	
	AllItem.Eatables[Index].ItemID = EatableRef.ItemID;
	AllItem.Eatables[Index].Quantity = EatableRef.Quantity - 1;
	AllItem.Eatables[Index].ItemType = EatableRef.ItemType;

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::DropEatables(int32 Index)
{	
	//Todo : 아이템을 한번에 드랍하는 버그가 있음.
	AllItem.Eatables[Index].ItemID.DataTable = nullptr;
	AllItem.Eatables[Index].ItemID.RowName = NAME_None;
	AllItem.Eatables[Index].Quantity = 0;	
	AllItem.Eatables[Index].ItemType = EItemTypes::Eatable;

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::DropSword(int32 Index)
{
	AllItem.Swords[Index].ItemID.DataTable = nullptr;
	AllItem.Swords[Index].ItemID.RowName = NAME_None;
	AllItem.Swords[Index].Quantity = 0;
	AllItem.Swords[Index].ItemType = EItemTypes::Sword;

	OnInventoryUpdated.Broadcast(AllItem);
}

void UInventoryComponent::DropShield(int32 Index)
{
	AllItem.Shields[Index].ItemID.DataTable = nullptr;
	AllItem.Shields[Index].ItemID.RowName = NAME_None;
	AllItem.Shields[Index].Quantity = 0;
	AllItem.Shields[Index].ItemType = EItemTypes::Shield;

	OnInventoryUpdated.Broadcast(AllItem);
}

UUserWidget* UInventoryComponent::GetInventoryWidget()
{	
	return InventoryWidget = InventoryWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass) : InventoryWidget;
}

AController* UInventoryComponent::GetOwnerController()
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<AInventoryCharacter>(GetOwner()) : PlayerCharacter;
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetController();
	}
	return nullptr;
}
