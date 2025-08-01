+++
title = "DevLog: #2 Set Camera Parameter in Runtime"
date = "2025-07-31T14:19:42+07:00"
draft = false 
author = "Bob"
tags = ["DevLog", "Tutorial"]
categories = ["Tech"]
summary = "Set Camera Parameter in Runtime"
showtoc = true
comments = true
cover.image = 'https://d1iv7db44yhgxn.cloudfront.net/documentation/images/e32bed42-dfd6-4419-90d7-118008fb9bed/gameplay-cameras-overview-1.png'
cover.caption = 'Camera Parameter in Runtime'
cover.alt = 'this is alter cover'
cover.responsiveImages = true
cover.hidden = false
weight = 3
+++

### 🎯 The Problem & The Goal

* **The Problem:** In our previous articles, we created a custom C++ `Camera Node` with properties like
  `DistanceFromTarget`. However, these values are statically set within the `Camera Stack` asset. This creates a major
  problem: How can our game logic (e.g., the `Player Character` Blueprint) change these values dynamically at runtime?
  For instance, how can we allow the player to use the mouse wheel to zoom the camera in and out? Getting a direct
  reference to a `Camera Node` is complex and goes against the modular design of the framework.
* **The Goal:** This article will guide you through a modern and effective workflow to solve this exact problem. We will
  use a `Camera Variable Collection` to create a data "bridge," allowing us to **control Camera Node properties from any
  Blueprint** in a safe, performant, and completely decoupled manner.

* * *

### 📚 Context & Theory

#### Key Concepts

To understand the solution, we need to be clear on the role of each component in
the [Gameplay Camera System](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-camera-system-overview):

![The Gameplay Camera System](https://d1iv7db44yhgxn.cloudfront.net/documentation/images/e32bed42-dfd6-4419-90d7-118008fb9bed/gameplay-cameras-overview-1.png)


> **In short:** Instead of the Blueprint trying to "talk" directly to the Camera Node, the Blueprint will change a value
> in the `Camera Variable Collection`, and the Camera Node will automatically update itself based on that value.

* * *

### 🚀 The Implementation Journey

We will implement a mouse-wheel zoom feature for the `Custom Camera Node Top Down` we created in the previous article.

#### Step 1: Create a `Camera Variable Collection`

This will be our shared data "depot."

* We create a collection then possible to modify properties inside the Camera asset. In the **Content Browser**,
  right-click -> **Gameplay** -> **Camera Variable Collection**. Name it something like `CVC_TopDown`.
  ![Create Camera Variable Collection](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/create-camera-variable-collection.png?raw=true)

#### Step 2: Add the `Distance` Variable to the Collection

* Create a `Float` variable inside the collection to store the current camera distance.
* Open the `CVC_TopDown` asset. Click the **Add (+)** button and select **Float**. Name the new
  variable `Distance` and set its **Default Value** (e.g., `500.0`).
  ![Create Float Camera Variable](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/create-float-camera-value.png?raw=true)
  ![Set Default Value](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/set-default-value.png?raw=true)

#### Step 3: Link the `Camera Node` to the `Variable Collection`

* We need to "tell" the `DistanceFromTarget` property on our Camera Node: "From now on, don't use your
  static value. Instead, get your value from the `Distance` variable inside `CVC_TopDown`."
  ![Link Camera Variable Collection](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/link-camera-variable-collection-to-camera-node.png?raw=true)

1. Open your `Camera Stack` (e.g., `CS_TopDownCamera`).

2. Select the `Custom Top-Down Boom Arm` node.

3. In the Details Panel, find the `Distance From Target` property. Click the **database icon** next to it.

4. Select `CVC_TopDown` from the list.

5. In the `Variable` field, choose the `Distance` variable we created.

   > **💡 Pro-Tip:** The database icon will now be illuminated, indicating that this property is being driven by an
   external variable. This is a very helpful visual cue.

#### Step 4: Implement the Control Logic in the Player Character Blueprint

* Now it's time to write the logic that changes the value in the collection. We will capture the mouse wheel event, read
  the current `Distance` value from the collection, add/subtract from it, and then write the new value back into the
  collection. Open your `Character` Blueprint.

1. Add the `Input Axis Mouse Wheel` event. then add branch to only execute when we have input value.
  ![Debug Wheel](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/debug-wheel-to-test.png?raw=true)

2. Get a reference to the `Gameplay Camera Component`. then get `Get Shared Camera Data`

3. To calculate the new value, we need the old one. Drag off a `Get Float Variable` node to **read** the data.

4. Drag off a `Set Float Variable` node. This is the node used to **write** data to the collection.
  ![Set Float Variable](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/set-float-variable-camera-variable-parameter.png?raw=true)

5. Connect the logic as shown below. We multiply the `Axis Value ` by a `Distance Gap` (e.g., 100) and add it to the current
   value.
  ![Detail](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/detail-blueprint-to-test.png?raw=true)


* * *

### 📊 Visual Results & Analysis

* **Visual Demo:** The result is a smooth, intuitive zoom feature. As you scroll the mouse wheel, the camera seamlessly
  moves in and out, constrained within the `Min` and `Max` limits we defined.
  ![Sample Output](https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/dynamic-camera-parameter.gif?raw=true)

* **Result Analysis:**
* **Functional:** The player now has the expected, direct control over the camera distance, enhancing the game
  experience.
* **Technical:** This solution is highly performant. The read/write operations on a `Variable Collection` are optimized.
  More importantly, it's completely asynchronous (fire-and-forget). The Blueprint simply "posts" a new value without
  needing to know or care what the `Camera Stack` is doing.
* **Workflow:** This is a massive win! The camera control logic (player input) is self-contained in the
  `Player Character`. The camera design (nodes, effects, etc.) is self-contained in the `Camera Stack`. The two systems
  are **completely decoupled** via the `Variable Collection`. A designer can now change the entire `Camera Stack`
  without breaking the programmer's logic, and vice versa.

* * *

### ✨ Conclusion & Key Takeaways

Here's what we learned today:

* **Lesson 1:** The `Camera Variable Collection` is the standard, powerful tool for creating a data bridge for dynamic,
  runtime control of the camera system.
* **Lesson 2:** How to link a property within a `Camera Node` to a collection variable via the Details Panel.
* **Lesson 3:** How to use the `Set Float Variable` and `Get Float Variable` nodes from the `Gameplay Camera Component`
  in Blueprint to interact with the collection safely and efficiently.

### 💡 Challenge & Next Steps

* **A Challenge for You:** Using this exact technique, try to implement logic that temporarily increases the camera's
  `Stiffness` value whenever the player presses a "Sprint" button, making the camera follow more tightly and
  aggressively.
* **Next Steps:** In our next article, we will explore how to create more complex `Camera Nodes` that can activate or
  deactivate themselves based on in-game events.