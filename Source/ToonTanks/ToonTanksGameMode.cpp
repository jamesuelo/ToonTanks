// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Turret.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::BeginPlay() {
	Super::BeginPlay();
	HandleGameStart();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor) {
	if (DeadActor == Tank) {
		Tank->HandleDestruction();
		if (ToonTanksPlayerController) {
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATurret* DestroyedTower = Cast<ATurret>(DeadActor)) {
		DestroyedTower->HandleDestruction();
		--TargetTurrets;
		if (TargetTurrets == 0) {
			GameOver(true);
		}
	}
}
void AToonTanksGameMode::HandleGameStart() {
	TargetTurrets = GetTargetTurretCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	StartGame();
	if (ToonTanksPlayerController) {
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController, 
			&AToonTanksPlayerController::SetPlayerEnabledState, 
			true);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);
	}
}

int32 AToonTanksGameMode::GetTargetTurretCount(){
	TArray<AActor*> Turrets;
	UGameplayStatics::GetAllActorsOfClass(this, ATurret::StaticClass(),Turrets );
	return Turrets.Num();
}