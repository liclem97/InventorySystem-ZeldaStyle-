// Copyright liclem97.


#include "Actor/Item.h"

#include "Components/StaticMeshComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AItem::OnConstruction(const FTransform& Transform)
{
	if (!IsValid(ItemDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item: Item DataTable is not valid."));
		return;
	}

	FString ContextString;
	FItemStruct* RowData = ItemDataTable->FindRow<FItemStruct>(ItemData.ItemID.RowName, ContextString);

	if (RowData)
	{
		/*MoneyComponent->SetStaticMesh(RowData->Mesh);
		MoneyType = RowData->Name;
		MoneyAmount = RowData->Amount;*/

		Mesh->SetStaticMesh(RowData->Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item: Can't find RowData."));
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}



