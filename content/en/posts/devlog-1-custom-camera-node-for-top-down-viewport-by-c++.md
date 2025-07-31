+++
title = "DevLog: #1 Create Custom Camera Node for Top-Down View by C++"
date = "2025-07-30T10:13:42+07:00"
draft = false 
author = "Bob"
tags = ["DevLog", "Tutorial"]
categories = ["Tech"]
summary = "Create Custom Camera Node for Top-Down View by C++"
+++

A simple boom-arm camera gets the job done, but it often feels rigid and lifeless. For a player to be truly immersed in
the game world, the camera needs to feel "alive"—it needs weight, inertia, and to move with a smooth, organic motion.

Today we'll dissect a high-end C++ solution to achieve that: creating a custom `UCameraNode` for Unreal Engine's *
*Gameplay Camera System**. This node will simulate a top-down boom-arm with a sophisticated spring
physics system, delivering a camera feel that is both highly professional and easy to customize.

> The **Gameplay Camera System** provides an intuitive way for developers and designers to create complex camera
> behaviors in the editor.  
> This is a general-purpose system that contains one or more camera rigs, along with their behaviors and transitions
> inside a data asset, unlike the traditional workflow that uses a Blueprint component. This data asset can drive the
> camera behavior during gameplay when ingested by a Blueprint actor or a Blueprint component.

> To learn more about the system, read
the [Gameplay Camera System Overview](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-camera-system-overview)
and [Gameplay Camera System Quick Start](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-camera-system-quick-start)
documentation of Epic Game.

> "My goal isn't to reinvent the wheel—Epic has already engineered some excellent `UCameraNode` classes for us. Instead,
> we're going to **pop the hood and see how the engine ticks**! In this series, we'll demystify the built-in nodes and,
> more excitingly, learn how to **derive our own custom classes** with specialized logic. Honestly, that's the best part 
> about Unreal Engine, its **open-source nature** empowers you to truly understand and *take control* of what you're building. 
> It's awesome!"
> 
> See more detail of other node in '\UE_5.6\Engine\Plugins\Cameras\GameplayCameras\Source\GameplayCameras\Public\Nodes\'

#### Download:

* {{< download filename="CustomCameraNodeTopDown.h" url="/dev-blog/downloads/devlog-1/CustomCameraNodeTopDown.h"/>}}
* {{< download filename="CustomCameraNodeTopDown.cpp" url="/dev-blog/downloads/devlog-1/CustomCameraNodeTopDown.cpp"/>}}

* * *

### Part 1: The "What" - What are `UCameraNode` and the **Gameplay Camera System**?

Think of the **Gameplay Camera System** as a stack-based system, designed to be modular, with Camera Assets that can be
ingested by one or more Gameplay Camera actors and components. This means you can author your camera behavior once, and
use it on many actors in your scene.
Each node performs a specific task (camera shake, changing FOV, following a target, ...) and passes its result to the
next node in the stack.

* **`UCameraNode`** (the `.h` file): This is the UObject class that defines the properties and input pins you see in the
  Editor. It's like the "settings" panel.

* **`FCameraNodeEvaluator`** (the `.cpp` file): This is the worker struct that performs all the computational logic at
  runtime. It takes the settings from the `UCameraNode` and applies them.

* * *

### Part 2: The Header (`.h`) - Defining the "Control Knobs"

Our header file defines the parameters that a designer can tweak in the Editor.

**CustomCameraNodeTopDown.h**

```c++
UCLASS(meta=(CameraNodeCategories="Custom"))
class YOURPROJECT_API UCustomCameraNodeTopDown : public UCameraNode
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
```

**Analyzing the Properties:**

* **Core Inputs**: `DistanceFromTarget` and `Rotation` are the target values the camera will aim for.

* **Spring Physics**: `Stiffness`, `DampingRatio`, and `Mass` are the three pillars of the spring system.

    * **Stiffness**: How tight the spring is. High values make the camera react quickly and follow more rigidly.

    * **DampingRatio**: Controls overshoot. A value of `1.0` allows the camera to reach its target smoothly. A value
      `< 1.0` will create a slight bouncing effect.

    * **Mass**: A higher value creates a "heavier" camera feel with more inertia.

* **Interpolation**: `DistanceInterpolationSpeed` independently controls the zoom speed.

* * *

### Part 3: The Evaluator (`.cpp`) - The Core Logic ⚙️

This code defines the `FCustomBoomOffsetTopDownCameraNodeEvaluator` class, which is the "brain" that executes the
computational logic for a custom camera node within Unreal Engine's **Gameplay Camera System**.

### 1\. Member Variables

**CustomCameraNodeTopDown.cpp** ()

```c++
namespace UE::Cameras // Remember put class in UE::Cameras namespace
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
}
```

These variables act as the node's "memory," storing the necessary state and settings between frames.

* **`TCameraParameterReader<...>`**: These are parameter "readers." Their job is to get the values that a user (
  designer) has set on the `UCameraNode` in the Camera Stack asset. For example, `DistanceReader` will read the value
  from the `DistanceFromTarget` property.

* **`F...SpringState`**: These variables (`CameraLocationSpringState`, `CameraRotationSpringState`) store the state of
  the spring system, including the velocity and error from the previous frame. This is the core data required for the
  spring interpolation functions to work correctly.

* **`Current...`**: These variables (`CurrentCameraLocation`, `CurrentCameraRotation`, `CurrentSmoothDistance`) hold the
  camera's actual position, rotation, and distance from the previous frame. They are used as the starting point for the
  current frame's calculations to create smoothness.

* **`bIsFirstRun`**: An important flag variable to handle the first run separately. It helps prevent the camera from "
  flying" from the world origin (0,0,0) to its desired location, instead placing the camera in the correct position
  immediately.

* * *

### 2\. The `OnInitialize()` Function

Essentially, `OnInitialize` handles the "wiring" between the settings in the Editor and the logic brain that will use
them at runtime.

```c++
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
```

This is a setup function that runs only once when the node is initialized.

* **Main Purpose**: The most important task of this function is to **connect** the `TCameraParameterReader`s with their
  corresponding properties on the `UCustomCameraNodeTopDown`.

* **How it works**: The command `DistanceReader.Initialize(AttachNode->DistanceFromTarget)` means: "Hey
  `DistanceReader`, from now on, read your value from the `DistanceFromTarget` property on the camera node." This
  process is repeated for all other parameters like `Stiffness`, `Mass`, `Rotation`, etc.

* * *

### 3\. The `OnRun()` Function

This is where all the magic happens. The logic inside the `OnRun` function of
`FCustomBoomOffsetTopDownCameraNodeEvaluator` is incredibly clever.

**CustomCameraNodeTopDown.cpp** ()

```c++
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
```

* * *

### 4\. The Bridge: The `OnBuildEvaluator()` Function

This is the final piece of the puzzle, acting as the critical bridge between our `UCameraNode` (the settings part) and
our `FCameraNodeEvaluator` (the logic part).

```c++
FCameraNodeEvaluatorPtr UBoomArmCameraNodeTopDown::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
    using namespace UE::Cameras;
    return Builder.BuildEvaluator<FCustomBoomOffsetTopDownCameraNodeEvaluator>();
}
```

#### How It Works

The `OnBuildEvaluator` function is called by the **Gameplay Camera System** when it needs a "worker" to execute the
logic for this
specific node. Its job is basic:

* It receives a `Builder` object from the **Gameplay Camera System**.

* It tells the `Builder` to construct an instance of the logic class we wrote:
  `FCustomBoomOffsetTopDownCameraNodeEvaluator`.

* It then returns the newly created instance.

Essentially, this is how you tell the Engine: **"When you use this `UBoomArmCameraNodeTopDown` node, use the logic
from `FCustomBoomOffsetTopDownCameraNodeEvaluator` to do the math."** This function connects the user-facing settings
with the high-performance runtime logic.

**A Step-by-Step Analysis:**

1. **Initialization & First Run**: The code includes a critical `bIsFirstRun` check. On the very first frame, it places
   the camera directly at its desired location instead of letting it fly in from the world origin (0,0,0), preventing an
   ugly visual snap for the player.

2. **Independent Interpolation of Rotation and Distance**:

    * **Rotation** is interpolated using `QuaternionSpringInterp`, a powerful spring function for rotations that creates
      a very natural feel.

    * **Distance** (zoom) is interpolated using the simpler `FInterpTo`.

    * 💡 **Pro-Tip:** Separating these two interpolation types is a subtle but brilliant design choice. It allows a
      designer to control the camera's movement/rotation speed (via the spring) completely independently from the
      camera's zoom speed (via the InterpSpeed).

3. **Defining the "Moving Target"**: Instead of rigidly chasing the Pawn's location, the camera chases a virtual
   `TargetLocation`. This location is calculated using the rotation and distance values that were _already smoothed_ in
   the previous steps. This makes the camera's movement far more fluid.

4. **Position Interpolation with Vector Spring**: This is the main spring of the system. The camera's current position (
   `CurrentCameraLocation`) "chases" the `TargetLocation` using `VectorSpringInterp`. The `TargetVelocityAmount`
   parameter even helps the camera "lead" the target's movement, reducing the feeling of lag.

5. **Assigning the Result**: Finally, the fully smoothed location and rotation are assigned to the
   `OutResult.CameraPose`.

* * *

### Part 4: How to Use it in the Editor

The power of this method lies in how simple it is for designers to use.

1. Create a new **Camera Stack** asset.

2. In the Camera Stack Editor, click **Add Node** and search for `Custom Camera Node Top Down `.

3. Select the newly added node. You will see all the properties (`Stiffness`, `Mass`, `DampingRatio`, etc.) we defined
   in the `.h` file appear in the Details Panel.

4. Assign this **Camera Stack** asset to your **Player Camera Manager**.

Now, a designer can tweak these values and see the results instantly in-game without ever touching a line of code.

![GIF một chú chó vui mừng ngồi trước máy tính](https://media.giphy.com/media/slOhiKAVFgwr6/giphy.gif)
![GIF một chú chó vui mừng ngồi trước máy tính](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-1/output-sample-low.gif)

### Conclusion

We have just analyzed a complete C++ solution for creating a professional-feeling top-down camera using Unreal Engine's
**Gameplay Camera System**. By combining multiple layers of interpolation (springs for movement, FInterp for zoom) and
techniques like velocity prediction, we've created a camera node that is not only powerful but also incredibly flexible.

Try experimenting with the `Stiffness` and `Mass` values to create a unique camera feel for your own project!