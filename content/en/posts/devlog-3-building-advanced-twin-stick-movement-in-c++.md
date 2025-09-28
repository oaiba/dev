+++
title = "DevLog: #3 Building Advanced 'Twin-Stick' Movement in C++"
date = "2025-07-31T10:13:42+07:00"
draft = false
author = "Bob"
tags = ["DevLog", "Tutorial"]
categories = ["Tech"]
summary = "In this article, we will build a robust and flexible omnidirectional movement system in C++. This system allows the character to move relative to the camera's perspective and to rotate their body to look or aim independently."
showtoc = true
comments = true
cover.image = 'https://github.com/oaiba/dev/blob/gh-pages/downloads/devlog-3/sample-out-put.gif?raw=true'
cover.caption = 'Building Advanced "Twin-Stick" Movement in C++'
cover.alt = 'this is alter cover'
cover.responsiveImages = true
cover.hidden = false
weight = 4
+++

### 🎯 The Problem & The Goal

* **The Problem:** After setting up a flexible top-down camera system in the previous two articles, we face a classic
  challenge: character movement. Unreal's default movement system (`bOrientRotationToMovement`) often couples the
  movement direction with the character's facing direction. This is great for third-person adventure games but is a
  major limitation for top-down games, twin-stick shooters, or MOBAs, where the player needs to **run in one direction
  and aim or look in another**.
* **The Goal:** In this article, we will build a robust and flexible omnidirectional movement system in C++. This system
  allows the character to move relative to the camera's perspective and to rotate their body to look or aim
  independently. Most importantly, we will create a seamless state-switching mechanism between two modes: "normal
  movement" (facing the run direction) and "aiming" (facing a specific direction).

* * *

### 📚 Context & Theory

The architecture of this solution revolves around controlling two critical boolean properties of the
`UCharacterMovementComponent`. Our entire system is essentially a simple state machine that toggles them appropriately.

1. **`bOrientRotationToMovement = true`**: This is our "normal movement" state. When enabled, the Engine automatically
   rotates the character to face its current velocity vector. This is the default behavior for `ACharacter`.

2. **`bUseControllerDesiredRotation = true`**: This is our "free look/aim" state. When enabled, the Engine rotates the
   character to match the `Controller`'s `ControlRotation`. By manipulating this `ControlRotation` in code, we can make
   the character look in any direction we want, regardless of their movement.

> **Our Key:** Create a function to safely switch between these two states.
* * *

### 🚀 The Implementation Journey

We will break down each part of the system, starting with the foundational state-switching function.

#### Step 1: The Foundation—The Rotation State-Switching Function

This is the heart of the system. A simple `private` function is responsible for toggling the flags on the
`CharacterMovementComponent`. Encapsulating this logic into a single function keeps the code clean, readable, and avoids
repetition.
It also ensures that `bOrientRotationToMovement` and `bUseControllerDesiredRotation` are always in opposing states,
preventing unexpected behavior.

```c++
void ACharacterBase::SetShouldFollowMovement(const bool bShouldFollow)
{
  bShouldFollowMovement = bShouldFollow;

  if (auto* Movement = GetCharacterMovement())
  {
      // When bShouldFollow is true, we are in "running" mode.
      // When bShouldFollow is false, we are in "aiming" mode.
      const bool bIsInAimingMode = !bShouldFollowMovement;
      Movement->bUseControllerDesiredRotation = bIsInAimingMode;
      Movement->bOrientRotationToMovement = !bIsInAimingMode;
  }
}
```

#### Step 2: Camera-Relative Omnidirectional Movement

This function takes 2D input (from WASD keys or a gamepad's left stick) and translates it into movement in the game
world. We don't want the `W` key to always move the character along the world's X-axis. We want `W` to move
the character "forward" *relative to the camera's view*. To do this, we need to rotate the player's input vector by an
angle equal to the camera's yaw.

  ```c++
  void ACharacterBase::HandleMovementTrigger(const FVector2D InputAxisVector)
  {
      // Guard clauses to prevent execution when not needed.
      if (!Controller || InputAxisVector.IsNearlyZero())
      {
         return;
      }

      if (const auto* Movement = GetCharacterMovement())
      {
         if (Movement->IsFalling())
         {
            return;
         }
      }

      // The camera's rotation around the Z-axis (Yaw).
      const float CameraYawRadians = FMath::DegreesToRadians(CameraYawDegrees);

      // Rotate the input vector based on the camera's yaw using a 2D rotation matrix.
      const float CosAngle = FMath::Cos(CameraYawRadians);
      const float SinAngle = FMath::Sin(CameraYawRadians);

      const float X = InputAxisVector.X * CosAngle - InputAxisVector.Y * SinAngle;
      const float Y = InputAxisVector.X * SinAngle + InputAxisVector.Y * CosAngle;

      const FVector MoveDirection = FVector(Y, X, 0.f).GetSafeNormal();

      // Apply the calculated movement input.
      AddMovementInput(MoveDirection);

      // If the character should face its movement direction, rotate it.
      if (bShouldFollowMovement)
      {
         const float TargetYaw = MoveDirection.Rotation().Yaw;
         RotateTowardsYaw(TargetYaw, 2.5f); // Use our helper for smooth rotation
      }
  }
  ```

* **The Breakdown:**
    * The math section with `CosAngle` and `SinAngle` is the formula for a 2D rotation matrix. It transforms the input
      vector from "screen space" to "camera-relative world space."
    * When `bShouldFollowMovement` is `true`, we calculate the movement direction and use our helper function
      `RotateTowardsYaw` to turn the character accordingly.

#### Step 3: Independent Looking & Aiming

This system consists of three functions, corresponding to the `Started`, `Triggered`, and `Completed` events from the
Enhanced Input system (typically for a gamepad's right stick). When the player starts using the right stick (
`HandleLookStarted`), we switch to "aiming mode" using
`SetShouldFollowMovement(false)`. When they release it (`HandleLookCompleted`), we return to "movement mode." While
they are holding it (`HandleLookTriggered`), we continuously update the character's facing direction.

  ```c++
  void ACharacterBase::HandleLookStarted()
  {
      SetShouldFollowMovement(false); // Enter "Aiming Mode"
  }

  void ACharacterBase::HandleLookTriggered(const FVector2D InputAxisVector)
  {
	if (const auto* Movement = GetCharacterMovement())
	{
		if (Movement->IsFalling())
		{
			return;
		}
	}

	// Define the range for mapping the look input.
	constexpr float MinYaw = -90.0f;
	constexpr float MaxYaw = 90.0f;
	constexpr float MinPitch = -70.0f;
	constexpr float MaxPitch = 70.0f;

	// Scale the input from [-1, 1] to the defined min/max ranges.
	const float ScaledX = FMath::Lerp(MinYaw, MaxYaw, (InputAxisVector.X + 1.0f) / 2.0f);
	const float ScaledY = FMath::Lerp(MinPitch, MaxPitch, (InputAxisVector.Y + 1.0f) / 2.0f);

	// Calculate the target yaw relative to the screen/input axis (local yaw).
	// This is the direction the player is pushing the stick relative to the screen.
	const float LocalTargetYaw = FMath::RadiansToDegrees(FMath::Atan2(ScaledX, -ScaledY));

	// *** KEY LOGIC CHANGE IS HERE ***
	// Add the camera's yaw to the local yaw to get the final world-space target yaw.
	// This makes the character's 'look' direction relative to the camera.
	const float WorldTargetYaw = FMath::UnwindDegrees(LocalTargetYaw + CameraYawDegrees);

	// Use the unified helper function to handle the rotation towards the new world-space target.
	RotateTowardsYaw(WorldTargetYaw, 5.0f);
  }

  void ACharacterBase::HandleLookCompleted()
  {
      SetShouldFollowMovement(true); // Return to "Running Mode"
  }
  ```

* **The Breakdown:**
    * The line `WorldTargetYaw = FMath::UnwindDegrees(TargetYawFromInput + CameraYawDegrees);` is the most critical
      part. It ensures the player's "aim" direction is always consistent with the camera's perspective, creating an
      intuitive control experience.

#### Step 4: The Smooth Rotation Helper Function

Rotating the character needs to be done smoothly and independently of the frame rate. This function
allows us to reuse that logic for both movement and aiming modes.

  ```c++
  /**
   * @brief Helper function to smoothly rotate the controller towards a target yaw.
   */
  void ACharacterBase::RotateTowardsYaw(const float TargetYaw, const float RotationSpeed)
  {
      const float CurrentYaw = GetControlRotation().Yaw;

      // Find the shortest angle to rotate. Avoids rotating 350 degrees when -10 is shorter.
      const float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);

      const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

      // Add yaw input, scaled by speed and delta time for frame-rate independence.
      AddControllerYawInput(DeltaYaw * RotationSpeed * DeltaTime);
  }
  ```

#### Step 5: Exposing and Calling Functions from the Player Character Blueprint

Our C++ logic is ready, but it needs to be triggered by player input. This step connects everything. We will use the
`UFUNCTION(BlueprintCallable)` macro to "expose" our C++ functions, allowing the
`Player Character` Blueprint to see and call them. Then, we will connect the events from `Enhanced Input` to these
corresponding functions. This is a classic Unreal workflow: **Write complex, high-performance logic in C++ and use
Blueprint for flexible "wiring."**

* **Part 1: Exposing Functions to Blueprint (C++ Header)** In your character's header file (`ACharacterBase.h`), add the
  `UFUNCTION` macro before the input handling functions.

  ```c++
  // In your character's header file (e.g., ACharacterBase.h)

  public:
      /** Handles movement input. Called from Blueprint. */
      UFUNCTION(BlueprintCallable, Category = "Character | Input")
      void HandleMovementTrigger(const FVector2D InputAxisVector);

      /** Called when the look input action starts. */
      UFUNCTION(BlueprintCallable, Category = "Character | Input")
      void HandleLookStarted();

      /** Handles look input. Called every frame the input is active. */
      UFUNCTION(BlueprintCallable, Category = "Character | Input")
      void HandleLookTriggered(const FVector2D InputAxisVector);

      /** Called when the look input action completes. */
      UFUNCTION(BlueprintCallable, Category = "Character | Input")
      void HandleLookCompleted();
  ```

  > **💡 Pro-Tip:** Using a `Category` in the `UFUNCTION` macro organizes your functions neatly in the Blueprint's
  right-click menu, making them easier for designers or yourself to find.
* **Part 2: Calling the Functions from Blueprint** After compiling the C++ code, open your `Player Character`
  Blueprint (this class must inherit from `ACharacterBase`). You can now call these C++ functions directly from the
  `Enhanced Input` event nodes.

  **Event Graph in `BP_PlayerCharacter`**
* **The Breakdown:**

    1. **Event IA\_Move:** This event fires every frame that there is movement input (WASD or Left Stick). We take its
       `Action Value` (which is an `FVector2D`) and pass it directly into our C++ function `HandleMovementTrigger`.
       ![ia move](https://github.com/oaiba/dev/blob/gh-pages/downloads/devlog-3/ia_move.png?raw=true)

    2. **Event IA\_Look:**
        * The `Started` pin fires once when the player begins moving the Right Stick. It calls the `HandleLookStarted`
          function to switch the character to "aiming" mode.
        * The `Triggered` pin fires every frame the Right Stick is held. It passes the `Action Value` into the
          `HandleLookTriggered` function to update the character's rotation.
        * The `Completed` pin fires once when the player releases the Right Stick. It calls `HandleLookCompleted` to
          return the character to the normal "movement" mode.
          ![ia look](https://github.com/oaiba/dev/blob/gh-pages/downloads/devlog-3/ia_look.png?raw=true)

With this setup, we have completed the full loop: **Input -> Blueprint Event -> C++ Logic -> Character Movement.**

* * *

### 📊 Visual Results & Analysis

* **Visual Demo:**
  ![](https://github.com/oaiba/dev/blob/gh-pages/downloads/devlog-3/sample-out-put.gif?raw=true)
* **Result Analysis:**
    * **Functional:** We have successfully created a complete, flexible, and intuitive twin-stick control system that
      meets the demands of modern top-down games.
    * **Technical:** The system is driven by a simple state machine (`bShouldFollowMovement`), making the code easy to
      understand and manage. The use of helper functions like `RotateTowardsYaw` and `SetShouldFollowMovement` adheres
      to the DRY (Don't Repeat Yourself) principle.
    * **Workflow:** By connecting these `Handle...` functions to `Input Actions` in the Enhanced Input system, we have
      completely separated the processing logic from the input definition, allowing designers and programmers to work in
      parallel effectively.

* * *

### ✨ Conclusion & Key Takeaways

Here's what we learned today:

* **Lesson 1:** The power of toggling between `bOrientRotationToMovement` and `bUseControllerDesiredRotation` to create
  different movement and rotation modes.
* **Lesson 2:** How to apply vector math (a 2D rotation matrix) to create camera-relative movement, a foundational
  technique for many game genres.
* **Lesson 3:** The importance of encapsulating repeated logic into helper functions to create cleaner and more
  maintainable code.

* * *

### 💡 Challenge & Next Steps

* **A Challenge for You:** Currently, the transition between "aiming" and "movement" modes is instant. Can you modify
  the `SetShouldFollowMovement` function to add a short transition time (e.g., 0.15 seconds) and interpolate the
  `CharacterMovementComponent`'s rotation rate settings for a smoother state change?











