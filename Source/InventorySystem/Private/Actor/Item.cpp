// Copyright liclem97.


#include "Actor/Item.h"

#include "Components/StaticMeshComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None, 25.f);
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
		Mesh->SetStaticMesh(RowData->Mesh);
		ItemName = FName(*RowData->Name.ToString());
		ItemData.ItemType = RowData->ItemType;
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



