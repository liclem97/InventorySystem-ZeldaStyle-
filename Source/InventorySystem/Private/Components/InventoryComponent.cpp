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
	if (IsValid(InventoryWidgetClass) && PlayerController)
	{
		InventoryWidget = InventoryWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass) : InventoryWidget;
		InventoryWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(InventoryWidget->TakeWidget()));
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	

	PlayerController = PlayerController == nullptr ? Cast<AInventoryPC>(GetOwnerController()) : PlayerController;
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
