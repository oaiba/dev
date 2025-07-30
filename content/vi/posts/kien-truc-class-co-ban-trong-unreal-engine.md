+++
title = "Kiến Trúc Class Cơ Bản trong Unreal Engine"
date = "2025-07-30T10:13:42+07:00"
draft = false 
author = "Bob"
tags = ["hướng dẫn", "markdown", "hugo"]
categories = ["Kỹ Thuật"]
summary = "Làm quen với Kiến trúc Class cơ bản trong Unreal Engine"
+++

> Bài viết này giới thiệu các lớp cơ bản trong kiến trúc Unreal Engine – là yếu tố khác biệt chính so với Unity. Hiểu sâu kiến trúc giúp bạn xây dựng hệ thống liên lạc giữa các thành phần trong game một cách tối ưu hơn
Epic Games Developers.

* * *

## 1\. `Object`

*   Là lớp gốc (base class) cho mọi thứ sử dụng kiến trúc Unreal.

*   Không thể có `Component` (không đại diện đồ họa), cũng không `replicate` theo mặc định.

*   Dùng cho: `Manager`, `Container dữ liệu`, `Hệ thống logic không cần hiển thị`.

*   Tạo `Object` có 2 cách:

    *   **Instanced**: tạo lúc runtime, có thể gọi hàm và chỉnh thuộc tính.

    *   **Defaulted**: dùng giá trị mặc định, không tạo instance thật.


* * *

## 2\. `Actor`

*   Kế thừa từ `Object`, có thể gắn `Component` và hiển thị trong Level.

*   Có `RootComponent` bắt buộc.

*   Hỗ trợ `Replication`.

*   Hầu hết mọi thứ trong `Outliner` là `Actor`.

*   Dùng khi bạn cần đại diện vật lý trong Level hoặc một `manager` cần replicate.


* * *

## 3\. Các Component Cơ Bản

### 3.1 `ActorComponent`

*   Kế thừa từ `Object`.

*   Có liên kết đến `Actor chủ`.

*   Dùng để thêm chức năng modular (Inventory, Health, Weapon,...).

*   Ví dụ: `UHealthComponent`, `UInventoryComponent`.


### 3.2 `SceneComponent`

*   Kế thừa từ `ActorComponent`.

*   Có thông tin `Transform` (Location, Rotation, Scale).

*   Có thể `AttachToComponent` khác tạo thành cây.

*   **Không trực tiếp hiển thị** (render) nhưng vẫn tính toán transform mỗi frame nếu movable.


> ⚠️ **Performance Tip**: Nếu không cần gắn vào hệ thống transform, nên dùng `FVector` thay vì tạo `SceneComponent`.

#### Mobility Types

| Type | Mô tả |
| --- | --- |
| Static | Không đổi trong runtime. Tối ưu nhất, nên dùng mặc định. |
| Stationary | Chỉ áp dụng cho đèn (Light), một phần có thể thay đổi. |
| Movable | Thay đổi trong runtime. Cẩn trọng vì có thể gây lag. |

### 3.3 `RootComponent`

*   Mỗi `Actor` bắt buộc có một `RootComponent`.

*   Các `SceneComponent` khác sẽ gắn vào `Root`.


* * *

## 4\. `Pawn`

*   Là `Actor` có thể được điều khiển bởi người chơi hoặc AI.

*   Không tự di chuyển được nếu không có `Controller`.

*   Là base class cho nhân vật có thể điều khiển trong game.


* * *

## 5\. `Character`

*   Kế thừa từ `Pawn`, là một nhân vật humanoid sẵn sàng hoạt động.

*   Bao gồm sẵn:

    *   `CapsuleComponent` (collision)

    *   `CharacterMovementComponent`

    *   `SkeletalMeshComponent` (mesh)

*   Hỗ trợ nhảy, chạy, crouch, climb...

*   Tối ưu cho cả single player và multiplayer.


* * *

## 6\. `Controller`

*   Actor điều khiển `Pawn`.

*   Có 2 loại:

    *   `PlayerController`: điều khiển bởi người chơi.

    *   `AIController`: điều khiển bởi hệ thống AI.

*   Xử lý input, target, và các lệnh điều khiển.


* * *

## 7\. `PlayerCameraManager` & `CameraModifier`

### `PlayerCameraManager`

*   Kế thừa từ `Actor`.

*   Điều khiển camera cho người chơi.

*   Hỗ trợ hiệu ứng camera:

    *   `Camera Shake`

    *   `Zoom`

    *   `FOV Blend`

    *   `Lerp To Target`


* * *

## 8\. `HUD`

*   Quản lý giao diện người chơi (`UMG`, `Canvas`, `Debug UI`).

*   Mỗi `PlayerController` sẽ có một `HUD` đi kèm.

*   Có thể vẽ `Text`, `Line`, `Shapes` hoặc quản lý `Widget`.


* * *

## 9\. `Info`

*   Kế thừa từ `Actor`.

*   Dùng cho các manager/actor cần xuất hiện trong level và replicate nhưng không cần tương tác vật lý.

*   Ví dụ: `GameMode`, `GameState`, `PlayerState`.


* * *

## 10\. `GameModeBase` / `GameMode`

*   Chỉ tồn tại trên **Server**.

*   Định nghĩa logic gameplay:

    *   Loại `Pawn` mặc định.

    *   `PlayerController`, `HUD`, `PlayerState`.

*   Được chỉ định trong Project Settings hoặc trong Level.


* * *

## 11\. `GameStateBase` / `GameState`

*   Tồn tại ở cả server và client.

*   Lưu các trạng thái toàn cục như:

    *   `Thời gian game`

    *   `Số lượng người chơi`

    *   `Điểm`, `Team`, `GamePhase`

*   Tự động replicate dữ liệu quan trọng.


* * *

## 12\. `PlayerState`

*   Gắn với mỗi người chơi (player hoặc bot).

*   Lưu trữ các thông tin:

    *   `Tên`

    *   `Điểm`

    *   `Ping`

    *   `Level`, `Class`, `Skin`

*   Replicate tới các client để hiển thị đúng UI.


* * *

## 13\. `LocalPlayer`

*   Đại diện cho người chơi đang sử dụng local device (PC, Console).

*   Không xuất hiện rõ ràng trong game, nhưng giúp kết nối giữa Controller và Viewport.


* * *

## 14\. `GameInstance` & Subsystems

### `GameInstance`

*   Sinh ra khi khởi động game, tồn tại xuyên suốt nhiều Level.

*   Quản lý:

    *   Đăng nhập người chơi.

    *   Lưu trạng thái toàn cục.

    *   Chuyển cảnh.

    *   Truy cập Subsystem.


### Subsystem Types

| Type | Scope |
| --- | --- |
| EngineSubsystem | Tồn tại theo ứng dụng |
| GameInstanceSubsystem | Theo GameInstance |
| WorldSubsystem | Tồn tại theo mỗi World |
| LocalPlayerSubsystem | Tồn tại theo LocalPlayer |

* * *

## 15\. `World`, `Level` & `Level Blueprint`

*   **World**: đại diện cho môi trường game hiện tại.

*   **Level**: mỗi bản đồ con bên trong World.

*   **Level Blueprint**: chứa logic đặc trưng của Level, nhưng không khuyến khích dùng nhiều vì khó tái sử dụng.

*   Nên xử lý logic qua Actor + Event + Interface hoặc Tag System.


* * *

## 16\. `DataAsset`

*   Dùng để định nghĩa dữ liệu cấu hình (item, weapon, skill,...).

*   Không có UI, không hoạt động như `Actor`.

*   Dùng để lưu dữ liệu dùng lại được trong nhiều nơi.

*   Tạo từ class `UPrimaryDataAsset` hoặc `UDataAsset`.


* * *

## 17\. `Blueprint Function Library`

*   Tạo các hàm static có thể dùng trong mọi Blueprint.

*   Không cần tạo instance.

*   Dùng cho các hàm tiện ích (math, input helper, v.v).

*   Ưu tiên viết bằng C++ để tối ưu hơn Blueprint.


* * *

## 18\. Quy Trình Khởi Tạo Game (Basic Flow)

1.  **GameInstance** được tạo.

2.  **World** được khởi tạo khi load Level.

3.  **GameMode** sinh ra (Server Only).

4.  Spawn các class mặc định:

    *   `DefaultPawn`

    *   `PlayerController`

    *   `HUD`

5.  Các `Actor` trong Level được spawn.

6.  Bắt đầu Tick game.


> 📌 Lưu ý: `GameInstance` không tồn tại khi dùng Editor Preview.

* * *

## 19\. Pro Tip 💡

*   Đọc file `.h` của các class gốc như `AActor`, `UObject`, `APawn`, `UActorComponent` để hiểu cấu trúc thật sự.

*   Có thể tìm trong folder `Engine/Source/Runtime/` của Unreal Engine.

*   Học từ source code sẽ giúp bạn hiểu cách Epic tổ chức gameplay framework.


* * *

Nếu bạn thấy bài viết này hữu ích, hãy đánh dấu lại để dùng khi cần, hoặc chia sẻ cho người mới bắt đầu học Unreal nhé!

* * *
































