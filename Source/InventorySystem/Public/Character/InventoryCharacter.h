// Copyright liclem97.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInventoryComponent;
class AInventoryPC;

UCLASS()
class INVENTORYSYSTEM_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AInventoryCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Getter */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetOpenedWidget() const { return bOpenedWidget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }	
	/** end Getter */
	
	/** Setter */
	UFUNCTION(BlueprintCallable)
	void SetOpenedWidget(bool InOpened);
	/** end Setter */
protected:
	virtual void BeginPlay() override;

	/** Player Input*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Inventory();
	void PressedSwordTab();
	void PressedShieldTab();
	void PressedEatableTab();
	void Interact();
	/** end Player Input*/

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SwordTabAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ShieldTabAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EatableAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InteractAction;
	/** end Input Actions */

	float MouseSensitivity;

	UPROPERTY()
	AInventoryPC* PlayerController;

	UPROPERTY(EditAnywhere, Category = Inventory)
	UInventoryComponent* InventoryComponent;

	bool bOpenedWidget = false;
};
