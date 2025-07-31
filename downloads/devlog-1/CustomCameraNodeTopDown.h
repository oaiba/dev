// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CameraNode.h"
#include "Core/CameraParameters.h"

#include "CustomCameraNodeTopDown.generated.h"

class FCameraNodeEvaluator;

/**
 * 
 */
UCLASS(meta=(CameraNodeCategories="Custom"))
class GAMEANIMATIONSAMPLEDEVLOG_API UCustomCameraNodeTopDown : public UCameraNode
{
	GENERATED_BODY()
protected:
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;

public:
	// The Distance from target to camera view
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter DistanceFromTarget = 500.0f;

	// The rotation of the camera
	UPROPERTY(EditAnywhere, Category = Common)
	FRotator3dCameraParameter Rotation = FRotator(-60.0f, 0.0f, 0.0f);

	// The stiffness of the spring. Higher values are "stiffer" and follow more closely.
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter Stiffness = 45.0f;

	// The damping ratio. 1.0 is critically damped (no overshoot).
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter DampingRatio = 1.0f;

	// The mass of the camera, creating a heavier, more weighty feel.
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter Mass = 10.0f;

	// Use 100% of the target's velocity for prediction, helping the camera "lead the target".
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter TargetVelocityAmount = 1.0f;
    
	UPROPERTY(EditAnywhere, Category = Common)
	FFloatCameraParameter DistanceInterpolationSpeed = 5.0f;
};
