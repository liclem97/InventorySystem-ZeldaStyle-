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

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	

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
