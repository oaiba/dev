+++
title = "Dev Log: Building a Dynamic and Scalable Grid Inventory System in Unreal Engine C++"
date = "2025-09-11T10:00:00-05:00"
draft = false
author = "Bob"
tags = ["DevLog", "Tutorial", "Unreal Engine", "C++", "GameDev", "UI", "Inventory System"]
categories = ["Tech"]
summary = "The new grid inventory system is a UMG widget designed to be highly configurable and responsive."
showtoc = true
comments = true
cover.image = 'https://github.com/oaiba/dev-blog/blob/gh-pages/downloads/devlog-3/sample-out-put.gif?raw=true'
cover.caption = 'The new grid inventory system is a UMG widget designed to be highly configurable and responsive.'
cover.alt = 'this is alter cover'
cover.responsiveImages = true
cover.hidden = false
weight = 5
+++

### Overview

The new grid inventory system is a UMG widget designed to be highly configurable and responsive. The primary goal was to create a component that could handle items of varying sizes, automatically scale to fit its container while preserving its aspect ratio, and provide clear visual feedback in both the game and the Unreal Editor.

The system is composed of three main C++ classes that work together:

1.  `UOBGridInventoryWidget`: The main user-facing widget that orchestrates the entire inventory.
2.  `UOBGridBackgroundWidget`: A lightweight, dedicated widget for drawing the grid background and border.
3.  `FOBGridInventoryConfig`: A simple struct to hold all the configuration data for the grid.

This separation of concerns allows the main widget to focus on logic (item placement, scaling calculations) while the background widget handles the rendering details.

### Key Classes and Responsibilities

#### `FOBGridInventoryConfig`
This is the data backbone of our system. It’s a `USTRUCT` exposed to Blueprints, allowing designers to tweak the inventory's appearance and behavior directly in the editor.

```c++
USTRUCT(BlueprintType)
struct FOBGridInventoryConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ...)
	int32 NumRows = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ...)
	int32 NumColumns = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ...)
	float CellSize = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ...)
	FLinearColor GridLineColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.5f);
    
    // ... and other properties like line thickness
};
```
Key properties include grid dimensions (`NumRows`, `NumColumns`), base `CellSize`, and visual parameters like line color and thickness.

#### `UOBGridBackgroundWidget`
This widget has one job: draw the grid. It overrides the `NativePaint` function to draw lines based on the `FOBGridInventoryConfig` it receives. This is far more performant than creating a grid out of individual image widgets.

A neat feature is its design-time support. When viewing the widget in the UMG editor, it will display the name of its owning widget and its dimensions, making it easier for designers to work with.

```c++
// In UOBGridBackgroundWidget.cpp
int32 UOBGridBackgroundWidget::NativePaint(...) const
{
    // ... scaling logic ...

	// --- Draw Vertical Lines & Left/Right Border ---
	for (int32 X = 0; X <= NumColumns; ++X)
	{
        // ... draw logic using FSlateDrawElement::MakeLines ...
	}

	// --- Draw Horizontal Lines & Top/Bottom Border ---
	for (int32 Y = 0; Y <= NumRows; ++Y)
	{
        // ... draw logic ...
	}

    // --- Draw Debug Text (Design-time only) ---
	if (IsDesignTime() && bIsShowNameOnTopLeftCorner)
	{
        // ... draws the owner's name and grid dimensions ...
	}

	return CurrentLayerId;
}
```

#### `UOBGridInventoryWidget`
This is the heart of the system. It's a `UUserWidget` containing a `USizeBox`, `UOverlay`, `UGridPanel`, and our custom `UOBGridBackgroundWidget`.

*   **Responsibilities**:
    *   Managing item widgets (`AddItemWidget`, `RemoveItemWidget`).
    *   Finding free slots for new items (`FindFreeSlot`, `IsAreaClear`).
    *   Calculating and applying the correct scale to fit its container (`RecalculateScaleAndRefreshLayout`).
    *   Visually populating empty grid cells with "dummy" widgets for consistent styling.
    *   Responding to layout changes in real-time via `NativeTick`.

It uses a `TMap<TObjectPtr<UUserWidget>, FOBGridItemInfo> PlacedItemInfoMap` to keep track of every item widget and its position/size within the grid.

### New Features

#### 1. Automatic Aspect-Ratio Scaling
One of the most important features is the grid's ability to resize correctly. We want the grid to fill its parent container as much as possible without stretching the grid cells.

The solution is implemented in `RecalculateScaleAndRefreshLayout`. This function is called from `NativeTick` whenever the widget's allocated size changes.

1.  `CalculateCurrentScale` determines the ideal scale by comparing the widget's allocated size to the target size (e.g., `NumColumns * CellSize`). It calculates both horizontal and vertical scale factors and chooses the smaller of the two to prevent distortion.
2.  `UpdateSizeBoxOverride` then applies this calculated scale to the `USizeBox` that contains the grid. This effectively scales the entire inventory grid uniformly.

```c++
// In UOBGridInventoryWidget.cpp
void UOBGridInventoryWidget::RecalculateScaleAndRefreshLayout(const FGeometry& CurrentGeometry)
{
	// Calculate a new scale based on current geometry
	if (!CalculateCurrentScale(CurrentGeometry))
	{
		return;
	}

	// Apply new scale
	UpdateSizeBoxOverride();

	// Refresh grid layout
	if (ItemGridPanel)
	{
		ItemGridPanel->InvalidateLayoutAndVolatility();
	}
}
```

#### 2. Dynamic Dummy Cells
To give our grid a consistent look, we wanted every empty cell to have a background. Instead of leaving them blank, the `UOBGridInventoryWidget` intelligently fills unoccupied space with placeholder widgets.

The `UpdateDummyCells` method is the workhorse here. It's called whenever an item is added or removed.
1.  It first builds a `TSet` of all cells currently occupied by items.
2.  It then iterates through its tracked dummy widgets (`DummyCellWidgetsMap`) and removes any that are now on an occupied cell or have become invalid.
3.  Finally, it loops through every cell of the grid. If a cell is not occupied and doesn't already have a dummy widget, it creates and adds one.

This ensures the grid is always perfectly filled, providing a clean visual background for items.

#### 3. Robust Item Management
Adding and removing items is straightforward and safe. The `AddItemWidget` function will automatically find the next available slot that can accommodate the item's size (`ItemRows` and `ItemCols`).

```c++
// In UOBGridInventoryWidget.cpp
UUserWidget* UOBGridInventoryWidget::AddItemWidget(UObject* ItemDataSource, const int32 ItemRows, const int32 ItemCols)
{
	// ... validation ...

	// Find free slot in grid for item
	int32 FoundRow = -1;
	int32 FoundCol = -1;
	if (!FindFreeSlot(ItemRows, ItemCols, FoundRow, FoundCol))
	{
		return nullptr; // No space found
	}

	return AddItemWidgetInternal(ItemDataSource, ItemRows, ItemCols, FoundRow, FoundCol);
}
```
We also provide an `AddItemWidgetAt` function for placing an item at a specific coordinate. The core of this logic is the `IsAreaClear` function, which checks for collisions with existing items before placement.

### Challenges and Solutions

**Challenge**: Efficiently drawing a scalable grid.
*   **Solution**: Rather than using hundreds of UMG image widgets to form the grid lines (which is terrible for performance), we created the `UOBGridBackgroundWidget`. Its `NativePaint` override uses Slate draw calls (`FSlateDrawElement::MakeLines`), which is the correct and highly performant way to handle custom rendering for UMG widgets.

**Challenge**: Ensuring the grid is always responsive to resolution or container size changes.
*   **Solution**: We leverage the `NativeTick` function to monitor the widget's geometry. By storing the `LastKnownAllocatedSize` and comparing it against the current size each frame, we can detect layout changes and trigger the `RecalculateScaleAndRefreshLayout` logic only when needed. This is efficient and ensures the UI always looks correct.

### Next Steps

While this system is fully functional, there are always improvements to be made. Here's what's next on our list:
*   **Drag-and-Drop**: Implementing full drag-and-drop functionality for items within the grid and between different inventory widgets.
*   **Item Stacking**: Adding logic and visual representation for stackable items.
*   **Controller/Keyboard Navigation**: Enhancing the `NativeOnNavigation` override to allow seamless grid navigation with a gamepad or keyboard.
*   **Optimization**: While the current system is performant, we will investigate if the `UpdateDummyCells` logic can be further optimized for extremely large grids or rapid item changes.

We're really happy with this foundational system. It gives our design team the flexibility they need while ensuring great performance and a clean, responsive user experience.

Stay tuned for our next post!
```