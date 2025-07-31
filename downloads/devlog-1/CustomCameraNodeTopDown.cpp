// Fill out your copyright notice in the Description page of Project Settings.

#include "Cameras/CustomNode/BoomArmCameraNodeTopDown.h"

#include "Core/CameraEvaluationContext.h"
#include "Core/CameraParameterReader.h"
#include "GameFramework/Pawn.h"
#include "Logging/LogMacros.h"
#include "GameFramework/PlayerController.h"
#include "Nodes/Input/Input2DCameraNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"
#include "Math/Rotator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BoomArmCameraNodeTopDown)

namespace UE::Cameras
{
	class FCustomCameraNodeTopDownEvaluator : public FCameraNodeEvaluator
	{
	protected:
		virtual void OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params,
		                          FCameraNodeEvaluationResult& OutResult) override;
		virtual void OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult) override;

	private:
		TCameraParameterReader<float> DistanceReader;
		TCameraParameterReader<FRotator3d> RotationReader;
		TCameraParameterReader<float> StiffnessReader;
		TCameraParameterReader<float> DampingRatioReader;
		TCameraParameterReader<float> MassReader;
		TCameraParameterReader<float> TargetVelocityAmountReader;
		TCameraParameterReader<float> DistanceInterpSpeedReader;

		FVectorSpringState CameraLocationSpringState = FVectorSpringState();
		FVector CurrentCameraLocation = FVector::ZeroVector;

		FQuaternionSpringState CameraRotationSpringState = FQuaternionSpringState();
		FQuat CurrentCameraRotation = FQuat::Identity;

		float CurrentSmoothDistance = 500.0f;
		bool bIsFirstRun = true;
	};

	void FCustomCameraNodeTopDownEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params,
	                                                               FCameraNodeEvaluationResult& OutResult)
	{
		const UBoomArmCameraNodeTopDown* AttachNode = GetCameraNodeAs<UBoomArmCameraNodeTopDown>();
		DistanceReader.Initialize(AttachNode->DistanceFromTarget);
		RotationReader.Initialize(AttachNode->Rotation);
		StiffnessReader.Initialize(AttachNode->Stiffness);
		DampingRatioReader.Initialize(AttachNode->DampingRatio);
		MassReader.Initialize(AttachNode->Mass);
		TargetVelocityAmountReader.Initialize(AttachNode->TargetVelocityAmount);
		DistanceInterpSpeedReader.Initialize(AttachNode->DistanceInterpolationSpeed);
	}

	void FCustomBoomOffsetTopDownCameraNodeEvaluator::OnRun(const FCameraNodeEvaluationParams& Params,
	                                                        FCameraNodeEvaluationResult& OutResult)
	{
	    const APlayerController* PlayerController = Params.EvaluationContext->GetPlayerController();
	    if (!PlayerController) return;

	    const APawn* Pawn = PlayerController->GetPawnOrSpectator();
	    if (!Pawn) return;

	    // Get data from input
	    const FRotator DesiredRotation = RotationReader.Get(OutResult.VariableTable);
	    const float DesiredDistance = DistanceReader.Get(OutResult.VariableTable);
	    const float InterpSpeed = DistanceInterpSpeedReader.Get(OutResult.VariableTable);
	    const FVector PivotLocation = Pawn->GetActorLocation();

	    // Get data of Spring
	    const float Stiffness = StiffnessReader.Get(OutResult.VariableTable);
	    const float DampingRatio = DampingRatioReader.Get(OutResult.VariableTable);
	    const float Mass = MassReader.Get(OutResult.VariableTable);

	    // Handle the first run to prevent the camera from flying in from the world origin
	    if (bIsFirstRun)
	    {
	        CurrentCameraRotation = DesiredRotation.Quaternion();
	        CurrentSmoothDistance = DesiredDistance;
	        const FVector DirectionFromTarget = -CurrentCameraRotation.Vector();
	        CurrentCameraLocation = PivotLocation + DirectionFromTarget * CurrentSmoothDistance;
	        bIsFirstRun = false;
	    }

	    // --- STEP 1: INTERPOLATE ROTATION ---
	    CurrentCameraRotation = UKismetMathLibrary::QuaternionSpringInterp(
	        CurrentCameraRotation, DesiredRotation.Quaternion(),
	        CameraRotationSpringState, Stiffness, DampingRatio, Params.DeltaTime, Mass);

	    // --- STEP 2: INTERPOLATE DISTANCE ---
	    CurrentSmoothDistance = FMath::FInterpTo(
	        CurrentSmoothDistance, DesiredDistance,
	        Params.DeltaTime, InterpSpeed);

	    // --- STEP 3: CALCULATE THE TARGET LOCATION BASED ON SMOOTHED VALUES ---
	    const FVector SmoothedDirectionFromTarget = -CurrentCameraRotation.Vector();
	    const FVector TargetLocation = PivotLocation + SmoothedDirectionFromTarget * CurrentSmoothDistance;

	    // --- STEP 4: INTERPOLATE LOCATION, CHASING THE TARGET ---
	    const float TargetVelocityAmount = TargetVelocityAmountReader.Get(OutResult.VariableTable);
	    CurrentCameraLocation = UKismetMathLibrary::VectorSpringInterp(
	        CurrentCameraLocation, TargetLocation,
	        CameraLocationSpringState, Stiffness, DampingRatio, Params.DeltaTime, Mass, TargetVelocityAmount);

	    // --- STEP 5: ASSIGN THE FINAL RESULT ---
	    OutResult.CameraPose.SetLocation(CurrentCameraLocation);
	    OutResult.CameraPose.SetRotation(CurrentCameraRotation.Rotator());
	}
} // namespace UE::Cameras

FCameraNodeEvaluatorPtr UBoomArmCameraNodeTopDown::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FCustomCameraNodeTopDownEvaluator>();
}
