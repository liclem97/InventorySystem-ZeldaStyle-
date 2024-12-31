// Copyright liclem97.


#include "Actor/Item.h"

#include "Components/StaticMeshComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}



