// Copyright liclem97.


#include "Components/InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Character/InventoryCharacter.h"
#include "PlayerController/InventoryPC.h"

UInventoryComponent::UInventoryComponent()
{

}

void UInventoryComponent::Inventory()
{
	InventoryCharacter = InventoryCharacter == nullptr ? Cast<AInventoryCharacter>(GetOwner()) : InventoryCharacter;
	if (InventoryCharacter)
	{
		PlayerController = PlayerController == nullptr ? Cast<AInventoryPC>(InventoryCharacter->GetController()) : PlayerController;
		if (PlayerController && IsValid(InventoryWidgetClass))
		{
			PlayerController->Inventory();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: PlayerController or InventoryWidgetClass is nullptr."));
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
}