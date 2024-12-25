// Copyright liclem97.


#include "PlayerController/InventoryPC.h"

#include "Blueprint/UserWidget.h"
#include "Character/InventoryCharacter.h"
#include "Components/InventoryComponent.h"

void AInventoryPC::BeginPlay()
{
	Super::BeginPlay();
}

void AInventoryPC::Inventory()
{
	AInventoryCharacter* InventoryCharacter = Cast<AInventoryCharacter>(GetPawn());
	if (InventoryCharacter)
	{
		InventoryComponent = InventoryCharacter->FindComponentByClass<UInventoryComponent>();
		if (InventoryComponent)
		{
			InventoryWidget = InventoryWidget == nullptr ? CreateWidget<UUserWidget>(GetWorld(), InventoryComponent->GetInventoryWidgetClass()) : InventoryWidget;
			InventoryWidget->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly().SetWidgetToFocus(InventoryWidget->TakeWidget()));
		}
	}
}
