+++
title = "DevLog: #3 Building Advanced 'Twin-Stick' Movement in C++"
date = "2025-08-03T10:19:42+07:00"
draft = false 
author = "Bob"
tags = ["DevLog", "Tutorial"]
categories = ["Tech"]
summary = "Building Advanced 'Twin-Stick' Movement in C++"
showtoc = true
comments = true
cover.image = 'https://github.com/oaiba/dev-blog/blob/main/static/downloads/devlog-2/dynamic-camera-parameter.gif?raw=true'
cover.caption = 'Building Advanced "Twin-Stick" Movement in C++'
cover.alt = 'this is alter cover'
cover.responsiveImages = true
cover.hidden = false
weight = 4
+++

### 🎯 The Problem & The Goal
*   **The Problem:** After setting up a flexible top-down camera system in the previous two articles, we face a classic challenge: character movement. Unreal's default movement system (`bOrientRotationToMovement`) often couples the movement direction with the character's facing direction. This is great for third-person adventure games but is a major limitation for top-down games, twin-stick shooters, or MOBAs, where the player needs to **run in one direction and aim or look in another**.
*   **The Goal:** In this article, we will build a robust and flexible omnidirectional movement system in C++. This system allows the character to move relative to the camera's perspective and to rotate their body to look or aim independently. Most importantly, we will create a seamless state-switching mechanism between two modes: "normal movement" (facing the run direction) and "aiming" (facing a specific direction).
* * *