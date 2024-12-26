// Copyright liclem97.


#include "Components/InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Character/InventoryCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerController/InventoryPC.h"

UInventoryComponent::UInventoryComponent()
{

}

void UInventoryComponent::Inventory()
{
	if (InventoryWidget)
	{
		OnMoneyChanged.Broadcast(MoneyAmount);
		InventoryWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(InventoryWidget->TakeWidget()));
	}	
}

void UInventoryComponent::PickupMoney(int32 InMoney)
{
	MoneyAmount += InMoney;
	OnMoneyChanged.Broadcast(MoneyAmount);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	

	PlayerController = PlayerController == nullptr ? Cast<AInventoryPC>(GetOwnerController()) : PlayerController;
	if (IsValid(InventoryWidgetClass) && PlayerController)
	{
		InventoryWidget = InventoryWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass) : InventoryWidget;
		
		// To construct InventoryWidget.
		InventoryWidget->AddToViewport();
		InventoryWidget->RemoveFromParent();
	}
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
