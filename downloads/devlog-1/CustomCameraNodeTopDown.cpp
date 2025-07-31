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
	class FCustomBoomOffsetTopDownCameraNodeEvaluator : public FCameraNodeEvaluator
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

	void FCustomBoomOffsetTopDownCameraNodeEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params,
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

		// Lấy tất cả các giá trị mong muốn từ input
		const FRotator DesiredRotation = RotationReader.Get(OutResult.VariableTable);
		const float DesiredDistance = DistanceReader.Get(OutResult.VariableTable);
		const float InterpSpeed = DistanceInterpSpeedReader.Get(OutResult.VariableTable);
		const FVector PivotLocation = Pawn->GetActorLocation();

		// Lấy các thông số của spring
		const float Stiffness = StiffnessReader.Get(OutResult.VariableTable);
		const float DampingRatio = DampingRatioReader.Get(OutResult.VariableTable);
		const float Mass = MassReader.Get(OutResult.VariableTable);

		// Xử lý lần chạy đầu tiên để camera không bay từ gốc tọa độ
		if (bIsFirstRun)
		{
			CurrentCameraRotation = DesiredRotation.Quaternion();
			CurrentSmoothDistance = DesiredDistance; // Gán giá trị ban đầu
			const FVector DirectionFromTarget = -CurrentCameraRotation.Vector();
			CurrentCameraLocation = PivotLocation + DirectionFromTarget * CurrentSmoothDistance;
			bIsFirstRun = false;
		}

		// --- BƯỚC 1: NỘI SUY GÓC QUAY (ROTATION) ---
		CurrentCameraRotation = UKismetMathLibrary::QuaternionSpringInterp(
			CurrentCameraRotation, DesiredRotation.Quaternion(),
			CameraRotationSpringState, Stiffness, DampingRatio, Params.DeltaTime, Mass);

		// --- BƯỚC 2: NỘI SUY KHOẢNG CÁCH (DISTANCE) ---
		CurrentSmoothDistance = FMath::FInterpTo(
			CurrentSmoothDistance, DesiredDistance,
			Params.DeltaTime, InterpSpeed);

		// --- BƯỚC 3: TÍNH TOÁN VỊ TRÍ MỤC TIÊU DỰA TRÊN CÁC GIÁ TRỊ ĐÃ ĐƯỢC LÀM MƯỢT ---
		const FVector SmoothedDirectionFromTarget = -CurrentCameraRotation.Vector();
		const FVector TargetLocation = PivotLocation + SmoothedDirectionFromTarget * CurrentSmoothDistance;

		// --- BƯỚC 4: NỘI SUY VỊ TRÍ (LOCATION) ĐUỔI THEO MỤC TIÊU ---
		const float TargetVelocityAmount = TargetVelocityAmountReader.Get(OutResult.VariableTable);
		CurrentCameraLocation = UKismetMathLibrary::VectorSpringInterp(
			CurrentCameraLocation, TargetLocation,
			CameraLocationSpringState, Stiffness, DampingRatio, Params.DeltaTime, Mass, TargetVelocityAmount);

		// --- BƯỚC 5: GÁN KẾT QUẢ CUỐI CÙNG ---
		OutResult.CameraPose.SetLocation(CurrentCameraLocation);
		OutResult.CameraPose.SetRotation(CurrentCameraRotation.Rotator());
	}
} // namespace UE::Cameras

FCameraNodeEvaluatorPtr UBoomArmCameraNodeTopDown::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FCustomBoomOffsetTopDownCameraNodeEvaluator>();
}
