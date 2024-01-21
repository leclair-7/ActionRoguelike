// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InputComponent.h"

// have to add the module to the build file
// as seen here
// https://docs.unrealengine.com/4.27/en-US/API/Plugins/EnhancedInput/UEnhancedInputLocalPlayerSubsyst-/
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController * PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// when you declare a pointer the expression returns a pointer
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			// might have to set BirdMappingContext to IMC_PlayerContext
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	if (DirectionValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA MOVE TRIGGERED %f"), DirectionValue)

		if (Controller && (DirectionValue != 0.f))
		{
			FVector Forward = GetActorForwardVector();
			AddMovementInput(Forward, DirectionValue);
		}
	}
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		// AddControllerPitchInput(LookAxisValue.Y);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	}

}

