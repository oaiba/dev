// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CameraNode.h"
#include "Core/CameraParameters.h"

#include "BoomArmCameraNodeTopDown.generated.h"

class FCameraNodeEvaluator;

/**
 *
 */
UCLASS()
class GASEXTRACTION_API UBoomArmCameraNodeTopDown : public UCameraNode
{
	GENERATED_BODY()

protected:

	// UCameraNode interface.
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;

public:

	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter DistanceFromTarget = 500.0f;

	UPROPERTY(EditAnywhere, Category = Common)
	FRotator3dCameraParameter Rotation = FRotator::ZeroRotator;

	// Độ cứng của lò xo. Càng cao càng "cứng", bám sát hơn.
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter Stiffness = 45.0f;

	// Tỉ lệ giảm chấn. 1.0 = tới hạn (không vọt lố).
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter DampingRatio = 1.0f;

	// Khối lượng của camera, tạo cảm giác ì, đầm hơn.
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter Mass = 10.0f;

	// Sử dụng 100% vận tốc của mục tiêu để dự đoán, giúp camera "đón đầu".
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter TargetVelocityAmount = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter DistanceInterpolationSpeed = 5.0f;
	
};
