// Copyright liclem97.


#include "Components/InventoryComponent.h"

#include "Actor/Item.h"
#include "Blueprint/UserWidget.h"
#include "Character/InventoryCharacter.h"
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
	ResizeInventory();
	OnInventoryUpdated.Broadcast(AllItem);
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
		}
	}
}

void UInventoryComponent::ResizeInventory()
{
	AllItem.Swords.SetNum(SizeOfSwords);
	AllItem.Shields.SetNum(SizeOfShields);
	AllItem.Eatables.SetNum(SizeOfEatables);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TraceItemToPickUp().bFoundItem && InteractWidget)
	{
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

AController* UInventoryComponent::GetOwnerController()
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<AInventoryCharacter>(GetOwner()) : PlayerCharacter;
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetController();
	}
	return nullptr;
}
