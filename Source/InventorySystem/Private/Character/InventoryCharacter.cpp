// Copyright liclem97.


#include "Character/InventoryCharacter.h"

#include "Actor/Money.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/InventoryPC.h"

AInventoryCharacter::AInventoryCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureCamera"));
	SceneCapture->SetupAttachment(GetCapsuleComponent());
	SceneCapture->SetRelativeLocation(FVector(260.f, 0.f, -5.f));
	SceneCapture->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
	SceneCapture->FOVAngle = 48.f;

	SwordComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordComponent->SetupAttachment(GetMesh());
	SwordComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SwordSocket");

	ShieldComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldComponent->SetupAttachment(GetMesh());
	ShieldComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "ShieldSocket");

	MouseSensitivity = 0.6f;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AInventoryCharacter::SetOpenedWidget(bool InOpened)
{
	bOpenedWidget = InOpened;
}

void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<AInventoryPC>(Controller);
	if (PlayerController)
	{	
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		MouseSensitivity = PlayerController->GetMouseSensitivity();
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInventoryCharacter::OnCapsuleBeginOverlap);
	SceneCapture->ShowOnlyActorComponents(this);
}

void AInventoryCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMoney* Money = Cast<AMoney>(OtherActor);
	if (Money)
	{	
		if (IsValid(InventoryComponent))
		{
			InventoryComponent->PickupMoney(Money->GetMoneyAmount());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryCharacter: InventoryComponent is not valid."));
		}
		Money->Destroy();
	}
}

void AInventoryCharacter::SetSword(UStaticMesh* NewSword)
{
	SwordComponent->SetStaticMesh(NewSword);
}

void AInventoryCharacter::SetShield(UStaticMesh* NewShield)
{
	ShieldComponent->SetStaticMesh(NewShield);
}

void AInventoryCharacter::Move(const FInputActionValue& Value)
{	
	if (bOpenedWidget) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventoryCharacter::Look(const FInputActionValue& Value)
{	
	if (bOpenedWidget) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * MouseSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * MouseSensitivity);
	}
}

void AInventoryCharacter::Inventory()
{
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryCharacter: Inventory Component is not valid."));
		return;
	}
	InventoryComponent->Inventory();
	InventoryComponent->OnInventoryUpdated.Broadcast(InventoryComponent->GetAllItem());
}

void AInventoryCharacter::PressedSwordTab()
{
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryCharacter: Inventory Component is not valid."));
		return;
	}
	if (bOpenedWidget)
	{
		InventoryComponent->OnSwordTabSelected.Broadcast();
	}
}

void AInventoryCharacter::PressedShieldTab()
{
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryCharacter: Inventory Component is not valid."));
		return;
	}
	if (bOpenedWidget)
	{
		InventoryComponent->OnShieldTabSelected.Broadcast();
	}
}

void AInventoryCharacter::PressedEatableTab()
{
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryCharacter: Inventory Component is not valid."));
		return;
	}
	if (bOpenedWidget)
	{
		InventoryComponent->OnEatableTabSelected.Broadcast();
	}
}

void AInventoryCharacter::Interact()
{
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryCharacter: Inventory Component is not valid."));
		return;
	}
	FItemSearchResult ItemResult = InventoryComponent->TraceItemToPickUp();
	if (ItemResult.bFoundItem)
	{	
		if (InventoryComponent->AddItemToInventory(ItemResult.Item))
		{	
			ItemResult.ItemActor->Destroy();
			InventoryComponent->OnInventoryUpdated.Broadcast(InventoryComponent->GetAllItem());
		}
	}
}

void AInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::Look);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::Inventory);
		EnhancedInputComponent->BindAction(SwordTabAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::PressedSwordTab);
		EnhancedInputComponent->BindAction(ShieldTabAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::PressedShieldTab);
		EnhancedInputComponent->BindAction(EatableAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::PressedEatableTab);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AInventoryCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

