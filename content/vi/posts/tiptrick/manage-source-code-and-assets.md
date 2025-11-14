+++
title = "Series: Phát triển và Vận hành Unreal Engine Multiplayer C++: Quản lý source code và assets"
date = "2025-11-14T10:14:42+07:00"
draft = false
author = "Bob"
tags = ["hướng dẫn", "markdown", "hugo"]
categories = ["Kỹ Thuật"]
summary = "Cung cấp một lộ trình đào tạo toàn diện, có hệ thống cho các lập trình viên C++ về việc thiết kế, xây dựng, triển khai và vận hành các game multiplayer sử dụng kiến trúc dedicated server trên Unreal Engine"
# Nhiều theme hỗ trợ ảnh bìa, bạn có thể thêm URL ảnh ở đây
# cover = "https://picsum.photos/1200/630" 
+++

---

### **Dự án C++: Xây dựng một "Siêu đô thị" có Quy hoạch và Luật lệ Rõ ràng**

Việc lựa chọn C++ và cấu trúc dự án không chỉ là xây dựng một "bộ khung xương"; nó là việc **thiết lập bản quy hoạch
tổng thể và bộ luật xây dựng** cho toàn bộ "siêu đô thị" game của chúng ta. Một bản quy hoạch tốt kết hợp với luật lệ
nghiêm ngặt sẽ đảm bảo thành phố có thể phát triển bền vững, dễ dàng nâng cấp, và không bao giờ chìm trong hỗn loạn.

"Allar's UE5 Style Guide" chính là bộ luật xây dựng đó.

---

### **Hai Triết lý Quy hoạch "Siêu đô thị" Unreal**

Khi bắt đầu một dự án, chúng ta đứng trước một ngã rẽ quan trọng về cách tổ chức hàng ngàn, thậm chí hàng triệu file
assets và code. Có hai trường phái tư duy chính, hai "bản quy hoạch tổng thể" khác biệt hoàn toàn.

---

### **Phương pháp 1: Quy hoạch theo Chức năng (Functional-First Approach)**

Hãy tưởng tượng một thành phố được quy hoạch một cách tuyệt đối theo ngành nghề và chức năng. Tất cả các ngân hàng nằm
ở "Quận Tài chính", tất cả các nhà máy nằm ở "Khu Công nghiệp", tất cả các trường đại học nằm ở "Làng Đại học". Nếu
chúng ta muốn tìm một thứ gì đó, chúng ta chỉ cần biết "nó là loại gì" để đi đến đúng khu vực.

Trong Unreal, triết lý này có nghĩa là **tất cả các asset cùng loại sẽ được nhóm lại với nhau, bất kể chúng thuộc về
tính năng hay nhân vật nào.**

Cấu trúc thư mục `Content` sẽ được tổ chức theo loại asset:

```
Content/
├── Blueprints/
│   ├── Pawns/
│   │   ├── BP_PlayerCharacter.uasset
│   │   └── BP_Enemy_Grunt.uasset
│   ├── Actors/
│   │   ├── BP_Door.uasset
│   │   └── BP_HealthPack.uasset
│   └── Weapons/
│       └── BP_AK47.uasset
│
├── Maps/
│   ├── MainMenu.umap
│   └── Jungle.umap
│
├── Materials/
│   ├── M_PlayerCharacter.uasset
│   ├── M_Enemy_Grunt.uasset
│   └── M_AK47.uasset
│
├── Meshes/
│   ├── StaticMeshes/
│   │   └── SM_HealthPack.uasset
│   └── SkeletalMeshes/
│       ├── SK_PlayerCharacter.uasset
│       ├── SK_Enemy_Grunt.uasset
│       └── SK_AK47.uasset
│
├── Textures/
│   ├── T_PlayerCharacter_D.uasset
│   ├── T_Enemy_Grunt_D.uasset
│   └── T_AK47_D.uasset
│
└── ... (Sounds, Particles, etc.)
```

#### **Ưu điểm (Pros)**

1. **Dễ tìm theo loại:** Nếu chúng ta cần tìm *bất kỳ* Material nào trong dự án, chúng ta biết chính xác phải vào thư
   mục `Content/Materials`. Điều này rất hữu ích cho các Technical Artist khi họ muốn kiểm tra và tối ưu hóa tất cả các
   material trong game.
2. **Thúc đẩy Tái sử dụng:** Khi tất cả các texture được đặt chung một nơi, artist sẽ có xu hướng tìm kiếm một texture
   kim loại đã có sẵn trước khi tạo ra một cái mới, giúp giảm sự trùng lặp và tiết kiệm dung lượng.
3. **Rõ ràng cho Lập trình viên (Ban đầu):** Cấu trúc này ban đầu có vẻ rất logic và sạch sẽ, đặc biệt đối với những
   người có tư duy lập trình, vì nó phân tách rõ ràng các "lớp" khác nhau của một thực thể (model, material, logic).

#### **Nhược điểm (Cons)**

1. **Cơn ác mộng cho việc Di chuyển & Xóa bỏ:** Đây là nhược điểm lớn nhất. Để xóa hoàn toàn nhân vật "Enemy Grunt",
   chúng ta phải đi lùng sục trong ít nhất 5 thư mục khác nhau (`Blueprints`, `Materials`, `Meshes`, `Textures`,
   `Animations`) để xóa từng file một. Nguy cơ để lại các file "mồ côi" (orphan files) làm rác dự án là cực kỳ cao.
2. **Workflow rời rạc cho Artist & Designer:** Để lắp ráp hoàn chỉnh một nhân vật, một artist phải liên tục nhảy giữa
   nhiều thư mục khác nhau để gán mesh, material, texture... Điều này làm gián đoạn dòng chảy công việc và tốn thời
   gian.
3. **Không thân thiện với DLC và Đóng gói (Chunking):** Khi chúng ta muốn đóng gói một bản DLC chỉ chứa map "Desert" và
   các asset liên quan, làm thế nào chúng ta có thể chỉ định cho Unreal Build Tool biết file nào thuộc về map đó? Gần
   như là không thể một cách tự động. Chúng ta sẽ phải tạo danh sách thủ công, một công việc dễ sai sót và không có khả
   năng mở rộng.

---

### **Phương pháp 2: Quy hoạch theo Nội dung (Content-First Approach)**

Hãy tưởng tượng một thành phố được xây dựng như một tập hợp các "khu đô thị" hoặc "công viên chủ đề" hoàn toàn tự chủ.
Mọi thứ chúng ta cần cho "Công viên Kỷ Jura" (khủng long, cây cối, xe jeep, hàng rào điện) đều nằm gọn bên trong bức
tường của công viên đó. Nếu chúng ta muốn di chuyển cả công viên sang một địa điểm mới, chúng ta chỉ cần bứng cả khu đất
đó đi.

Trong Unreal, triết lý này có nghĩa là **tất cả các asset liên quan đến một tính năng hoặc một thực thể cụ thể sẽ được
nhóm lại với nhau trong một thư mục duy nhất.**

Cấu trúc thư mục `Content` sẽ được tổ chức theo tính năng/thực thể:

```
Content/
├── Characters/
│   ├── Player/
│   │   ├── BP_PlayerCharacter.uasset
│   │   ├── Meshes/
│   │   │   └── SK_PlayerCharacter.uasset
│   │   ├── Materials/
│   │   │   └── M_PlayerCharacter.uasset
│   │   └── Textures/
│   │       └── T_PlayerCharacter_D.uasset
│   └── Enemies/
│       └── Grunt/
│           ├── BP_Enemy_Grunt.uasset
│           ├── Meshes/
│           │   └── SK_Enemy_Grunt.uasset
│           └── ...
│
├── Weapons/
│   └── Rifles/
│       └── AK47/
│           ├── BP_AK47.uasset
│           ├── Meshes/
│           │   └── SK_AK47.uasset
│           └── ...
│
├── Props/
│   └── HealthPack/
│       ├── BP_HealthPack.uasset
│       ├── Meshes/
│       │   └── SM_HealthPack.uasset
│       └── ...
│
└── Maps/
    ├── MainMenu/
    │   └── Map_MainMenu.umap
    └── Jungle/
        ├── Map_Jungle.umap
        ├── Meshes/         # Các mesh chỉ dùng trong map Jungle
        └── Materials/      # Các material chỉ dùng trong map Jungle
```

#### **Ưu điểm (Pros)**

1. **Tính tự chứa và Di động (Self-Contained & Portable):** Đây là ưu điểm tuyệt đối. Chúng ta có thể **di chuyển toàn
   bộ nhân vật Player sang một dự án khác chỉ bằng cách sao chép thư mục `Content/Characters/Player`**. Mọi thứ sẽ hoạt
   động vì tất cả các tham chiếu (references) đều nằm trong đó.
2. **Thiên đường cho DLC và Đóng gói (Chunking):** Đây là cách duy nhất để quản lý các gói nội dung một cách hiệu quả.
   Chúng ta có thể dễ dàng nói với Unreal: "Hãy đóng gói toàn bộ thư mục `Content/Maps/Jungle` thành Chunk 10." Khi
   người chơi muốn tải map này, game chỉ cần tải về file .pak của Chunk 10.
3. **Workflow liền mạch cho Artist & Designer:** Khi làm việc trên nhân vật Player, mọi file họ cần đều nằm ngay trong
   một thư mục. Họ có thể dễ dàng duyệt qua, chỉnh sửa và liên kết các asset với nhau mà không cần rời khỏi "khu vực làm
   việc" của mình.

#### **Nhược điểm (Cons)**

1. **Vấn đề nan giải về Asset dùng chung (The Shared Asset Dilemma):** Đây là điểm yếu chí mạng. Một Material Master,
   một texture kim loại trầy xước, một bộ animation di chuyển cơ bản... được dùng cho TẤT CẢ các nhân vật sẽ được đặt ở
   đâu? Điều này thường dẫn đến sự ra đời của một thư mục `Content/Shared`, và nếu không được quản lý chặt chẽ, nó có
   thể trở thành một "bãi rác" chứa mọi thứ, làm phá vỡ triết lý ban đầu.
2. **Nguy cơ Trùng lặp cao:** Vì sự tiện lợi, một artist có thể sẽ sao chép một texture từ thư mục `Shared` vào thư mục
   nhân vật của họ thay vì tạo một Material Instance. Điều này dẫn đến việc asset bị trùng lặp, làm tăng đáng kể kích
   thước của game.
3. **Có thể gây xung đột cho Lập trình viên:** Nếu các lớp C++ cụ thể cho từng tính năng cũng được đặt trong các thư mục
   nội dung này, nó có thể làm xáo trộn cấu trúc `Source` và gây khó khăn cho việc quản lý code.

#### **I. Triết lý cốt lõi: Quy hoạch Thư mục là Luật!**

Trước khi đi vào chi tiết, nguyên tắc tối cao mà Allar's Style Guide đặt ra là: **Sự nhất quán và có tổ chức trong cấu
trúc thư mục và quy tắc đặt tên là không thể thương lượng.**

* **Tại sao?**
    * **Khả năng dự đoán:** Bất kỳ thành viên nào trong team, dù mới hay cũ, đều có thể tìm thấy một asset họ cần một
      cách nhanh chóng mà không cần phải hỏi ai. Họ biết rằng một Material sẽ nằm trong thư mục `Materials` và có tiền
      tố `M_`.
    * **Hiệu quả tìm kiếm:** Content Browser của Unreal trở thành một công cụ cực kỳ mạnh mẽ khi chúng ta có thể lọc
      chính xác theo tiền tố (`BP_`, `T_`, `M_`...).
    * **Tránh sự hỗn loạn:** Nó ngăn chặn việc các asset tạm thời, thử nghiệm, hoặc trùng lặp làm ô nhiễm dự án chính.

#### **II. Cấu trúc Lai Tối ưu - Phiên bản "Allar's Certified"**

Chúng ta sẽ áp dụng mô hình "Hybrid City" nhưng với các quy tắc và tên thư mục cụ thể từ Allar's Style Guide.

##### **A. Hạ tầng Ngầm (Code - `Source/`) - Nền móng Kỹ thuật**

Phần này vẫn tuân thủ nguyên tắc **Quy hoạch theo Chức năng**. Đây là nền móng C++ vững chắc, ít khi bị thay đổi bởi
artist hay designer.

* `Source/MyGameCore`: Các hệ thống toàn cục, interfaces, các lớp helper.
* `Source/MyGameFramework`: Các lớp C++ cơ sở cho gameplay (`CharacterBase`, `WeaponBase`, `GameStateBase`...). Đây là
  cầu nối sang `Content/Core`.
* `Source/GameFeatures/`: Mỗi hệ thống gameplay lớn nên là một module riêng.
    * `Source/GameFeatures/Inventory`
    * `Source/GameFeatures/AbilitySystem`
    * `Source/GameFeatures/AI`

##### **B. Bề nổi (Assets - `Content/`) - Các Khu phố và Tòa nhà**

Đây là nơi chúng ta áp dụng triệt để các quy tắc của Allar, kết hợp giữa **Quy hoạch theo Chức năng** và **Quy hoạch
theo Nội dung**.

* `Content/Core/`: **"Trung tâm Hành chính & Hạ tầng Chung"**
    * Đây là nơi chứa các asset nền tảng, được sử dụng trên toàn bộ dự án. Nó tương ứng trực tiếp với
      `Source/MyGameFramework`.
    * `Core/Blueprints/`: Chứa các Blueprint cơ sở kế thừa trực tiếp từ C++ (`BP_CharacterBase`, `BP_WeaponBase`). Các
      designer sẽ tạo các Blueprint con từ những lớp này.
    * `Core/Data/`: Chứa các Data Table và Data Asset định nghĩa các quy tắc game cơ bản.
    * `Core/Materials/`: Chứa các Master Material, Material Function mà tất cả các asset khác sẽ tạo instance từ đó.
    * `Core/Input/`: Chứa các Input Action và Input Mapping Context của hệ thống Enhanced Input.

* `Content/Shared/`: Sẽ được đặt ở cấp cao nhất của Content, và bản thân nó cũng cần có một cấu trúc bên trong rõ
  ràng, thường là theo chức năng:
    * **Quy tắc đóng gói:** Khi thiết lập các quy tắc đóng gói (Packaging Rules) và Primary Asset Labels, toàn bộ thư
      mục `Content/Shared` sẽ được gán cho **Chunk 0**. Điều này đảm bảo rằng khi người chơi tải game lần đầu, họ sẽ có
      tất cả các asset nền tảng cần thiết để mọi gói nội dung khác (DLC, map mới) có thể hoạt động chính xác khi được
      tải về sau này.
    * **Để giảm thiểu rủi ro, team phải tuân thủ các quy tắc nghiêm ngặt:**

        * **Quy tắc #1: Phải có "Strict Gatekeeping":** Không ai được phép tự ý thêm asset vào
          `Content/Shared`. Phải có một hoặc vài người (thường là Art Director, Technical Art Lead, hoặc Lead Engineer)
          chịu trách nhiệm phê duyệt mọi thứ được đưa vào đây.
        * **Quy tắc #2: "The Rule of Three":** Một asset chỉ nên được xem xét để đưa vào `Shared` khi
          nó được yêu cầu sử dụng ở **ba hoặc nhiều nơi khác nhau và riêng biệt**. Nếu nó chỉ được dùng cho hai nhân
          vật, có thể tốt hơn là giữ nó ở một trong hai thư mục nhân vật đó và tham chiếu chéo.
        * **Quy tắc #3: `Shared` là "Chỉ đọc" với Hầu hết mọi người:** Đối với đa số thành viên trong team, các asset
          trong `Shared` nên được coi là "read-only". Họ chỉ nên tạo instance hoặc tham chiếu đến chúng. Việc
          chỉnh sửa một asset trong `Shared` phải là một quy trình có kiểm soát, có thể yêu cầu một buổi họp review
          ngắn.
        * **Quy tắc #4: Regular Audits:** Cứ mỗi một hoặc hai tháng, Senior nên rà soát lại thư mục `Shared` để tìm và
          loại bỏ các asset không còn được sử dụng hoặc những asset được thêm vào không đúng quy cách.

* `Content/Characters/`, `Content/Weapons/`, `Content/Environments/`, `Content/Props/`: **"Các Khu Dân cư và Công trình
  Chuyên dụng"**
    * Đây là phần **Quy hoạch theo Nội dung**. Mỗi thư mục này chứa tất cả mọi thứ liên quan đến một thực thể cụ thể.
    * *Ví dụ với `Content/Characters/CyberNinja/`*:
        * `BP_CyberNinja` (kế thừa từ `Core/Blueprints/BP_CharacterBase`)
        * `Meshes/SK_CyberNinja`
        * `Textures/T_CyberNinja_D`, `T_CyberNinja_N`
        * `Materials/MI_CyberNinja` (một instance của `Core/Materials/M_Character_Master`)
        * `Animations/A_CyberNinja_Attack`
    * **Lợi ích cho Pak/Chunk:** Cấu trúc tự chứa này làm cho việc đóng gói trở nên cực kỳ đơn giản. chúng ta có thể tạo
      một chunk ID cho toàn bộ thư mục `Characters/CyberNinja`. Nếu nhân vật này là một DLC, người chơi có thể tải về
      chỉ gói đó mà không ảnh hưởng đến phần còn lại của game. Tương tự với `Maps/Jungle/`.

* `Content/Developers/`: **"Khu vực Thử nghiệm & Sandbox Cá nhân"**
    * Đây là một trong những quy tắc thiên tài nhất của Allar. Mỗi thành viên trong team sẽ có một thư mục con riêng
      trong này (ví dụ: `Developers/JohnDoe/`).
    * **Mục đích:** Đây là nơi an toàn để mỗi người thử nghiệm, tạo các asset tạm thời, "chọc ngoáy" mà **không làm ô
      nhiễm** dự án chính.
    * **Quy tắc tối cao:** Thư mục `Content/Developers/` phải được thêm vào file `.gitignore` (hoặc các file ignore của
      Perforce). **Nội dung trong này không bao giờ được commit lên version control.**

##### **C. Sơ đồ Quy hoạch Tổng thể**

Đây là hình dung về cấu trúc lai cuối cùng của chúng ta:

```
├── Source/
│   ├── MyGameCore/
│   └── GameFeatures/
│       └── Inventory/
│
└── Content/
    ├── Core/
    │   ├── Blueprints/
    │   └── Materials/
    │
    ├── Characters/
    │   └── CyberNinja/
    │       ├── BP_CyberNinja.uasset
    │       ├── Meshes/
    │       └── Textures/
    │
    ├── Maps/
    │   └── Jungle/
    │       ├── Map_Jungle.umap
    │       ├── Meshes/
    │       └── Materials/
    │
    ├── GameFeatures/
    │   └── Inventory/
    │       ├── Data/
    │       └── UI/
    │
    ├── Developers/
    │   ├── JohnDoe/    (NEVER COMMIT)
    │   └── JaneSmith/  (NEVER COMMIT)
    │
    └── _External/      # Chứa các asset từ bên ngoài (ví dụ: Megascans)
    │
    ├── _Shared/
    
```

#### **III. Ưu điểm Vượt trội của Mô hình Quy hoạch Lai này**

* **Tối ưu cho mọi vai trò:**
    * **Lập trình viên** có một cấu trúc code module hóa, sạch sẽ trong `Source/`.
    * **Artist/Designer** có một cấu trúc nội dung trực quan, tự chứa trong `Content/`, cho phép họ làm việc độc lập.
    * **Technical Director** có thể dễ dàng quản lý và tự động hóa quy trình đóng gói (packaging) dựa trên cấu trúc này.
* **Sẵn sàng cho Tương lai (Future-Proof):**
    * **DLC & Cập nhật:** Việc thêm một gói nội dung mới (nhân vật, map) chỉ đơn giản là tạo một thư mục mới theo đúng
      quy chuẩn mà không cần tái cấu trúc lại toàn bộ dự án.
    * **Tối ưu hóa Phân phối:** Cấu trúc này là nền tảng để chúng ta có thể phân phối game dưới dạng các "chunk" nhỏ.
      Người chơi có thể tải phần chơi chính trước, và tải các gói ngôn ngữ, các map độ phân giải cao, hoặc các phần chơi
      khác sau.
* **Vận hành và Hỗ trợ đa nền tảng:**
    * Khi một lỗi xảy ra chỉ trên Android liên quan đến nhân vật CyberNinja, chúng ta biết chính xác cần phải kiểm tra
      các file trong `Content/Characters/CyberNinja` và các lớp C++ cơ sở mà nó kế thừa.
    * Việc quản lý các phiên bản (develop, staging, production) trở nên dễ dàng hơn vì cấu trúc nhất quán giúp các công
      cụ CI/CD (như TeamCity) xác định và đóng gói các thay đổi một cách đáng tin cậy.

---

### **Khả năng Mở rộng, Bảo trì và làm việc với Version Control**

Đây là lợi ích chiến lược lớn nhất khi chọn C++, quyết định khả năng "sống sót" và phát triển của dự án trong dài hạn.

#### **Kiến trúc linh hoạt: Kết hợp Kế thừa (OOP) và Tập hợp (Composition)**

C++ cho phép chúng ta tận dụng triệt để các nguyên tắc OOP. Ví dụ, chúng ta có thể tạo một lớp `ABaseWeapon` định nghĩa
các hành vi chung (bắn, nạp đạn). Sau đó, các lớp con `ARifle`, `APistol` có thể kế thừa và chuyên biệt hóa các hành vi
đó.

Tuy nhiên, một kiến trúc hiện đại và linh hoạt hơn là kết hợp Kế thừa với **Composition (Kiến trúc tập hợp các thành
phần)**.

* **Triển khai gợi ý của chúng ta:** Thay vì một `ARifle` kế thừa mọi thứ từ `ABaseWeapon`, chúng ta tạo ra một lớp
  `AWeapon` cơ bản. Các chức năng cụ thể như "bắn đạn ghém" (`ShotgunFireComponent`), "ngắm bắn qua ống ngắm" (
  `ScopeComponent`), hay "bắn tia laser" (`LaserBeamComponent`) sẽ được xây dựng dưới dạng các **Actor Component**.
* **Tại sao đây là phương pháp vượt trội?**
    * **"Cắm và Chạy" (Plug-and-Play):** chúng ta có thể tạo ra một khẩu súng hoàn toàn mới trong Blueprint chỉ bằng
      cách lắp ghép các Component này lại với nhau, giống như lắp ráp các module Lego. Một khẩu súng trường tấn công có
      thể được tạo từ `AWeapon` + `AutomaticFireComponent` + `ScopeComponent`.
    * **Tái sử dụng tối đa:** `ScopeComponent` không chỉ dùng cho súng, nó có thể được gắn vào cả ống nhòm hoặc các công
      cụ khác.
    * **Tránh "Địa ngục Kế thừa":** chúng ta sẽ không bị rơi vào tình huống tạo ra một cây kế thừa phức tạp, đa tầng,
      khó quản lý chỉ để chia sẻ một vài chức năng nhỏ.

#### **Cấu trúc Module và Version Control (Git): Xây dựng một "Thành phố" có quy hoạch**

Việc chia dự án thành các module (`MyGameCore`, `MyGameInventory`, `MyGameAI`...) giống như việc quy hoạch một thành phố
thành các quận chuyên biệt.

* **Giảm thiểu xung đột (Merge Conflicts):** Khi các team làm việc trong các "quận" (module) riêng của mình, khả năng
  họ "xây dựng" chồng chéo lên nhau và gây ra xung đột trên Git sẽ giảm đi đáng kể. Đây là yếu tố sống còn để duy trì
  hiệu suất làm việc của một team lớn.
* **Tăng tốc độ biên dịch:** Unreal Build Tool có thể biên dịch các module song song. Một dự án được module hóa tốt sẽ
  có thời gian biên dịch nhanh hơn. chúng ta cũng có thể thiết lập để chỉ biên dịch lại những module có thay đổi.
* **Sở hữu Code rõ ràng (Code Ownership):** Mỗi module có thể có một "chủ sở hữu" hoặc team chịu trách nhiệm chính. Khi
  có lỗi phát sinh trong hệ thống túi đồ, mọi người đều biết cần phải liên hệ với team `Inventory`. Điều này tạo ra sự
  rõ ràng và trách nhiệm trong tổ chức.

---

### **Tip & Trick cho Team: Tối ưu hóa quy trình làm việc**

#### **1. Hybrid Approach: "C++ xây Siêu xa lộ, Blueprint xây Nhà dân"**

Đây là triết lý làm việc cốt lõi giúp cân bằng giữa hiệu năng và tốc độ phát triển.

* **Vai trò của Lập trình viên C++:**
    * Xây dựng các hệ thống nền tảng vững chắc và các "đường cao tốc" truyền dữ liệu hiệu năng cao.
    * Không chỉ "phơi bày" các hàm (`UFUNCTION`), các biến (`UPROPERTY`) với các meta, property, họ còn định nghĩa các "
      kênh sự kiện" bằng delegate (
      `UPROPERTY(BlueprintAssignable)`) để Blueprint có thể "lắng nghe" và phản ứng lại các sự kiện từ C++ (ví dụ:
      `OnHealthChanged`, `OnItemAddedToInventory`).
    * Họ tạo ra các "hợp đồng" dữ liệu thông qua `UDataAsset` và `UDataTable`.

* **Triển khai gợi ý của chúng ta (Tách biệt Dữ liệu và Logic):**
    * Lập trình viên C++ sẽ tạo ra các lớp cơ sở như `UItemData` (kế thừa từ `UDataAsset`).
    * Game Designer, thay vì chỉnh sửa `BP_HealthPotion`, giờ đây sẽ tạo và chỉnh sửa một asset dữ liệu là
      `DA_HealthPotion`. Asset này chứa mọi thông số: tên, icon, số lượng máu hồi phục, hiệu ứng âm thanh khi sử dụng.
    * Lớp `AItemActor` (trong C++) hoặc `BP_ItemActor` (trong Blueprint) sẽ chỉ có một biến duy nhất là `ItemData` (một
      con trỏ tới `UItemData`). Toàn bộ hành vi và hiển thị của nó sẽ được điều khiển bởi dữ liệu trong asset đó.
    * **Kết quả:** Dev và Designer có thể làm việc song song. Designer có thể tạo ra hàng trăm item mà không cần sự can
      thiệp của dev, và dev có thể tối ưu hóa logic xử lý item mà không sợ làm hỏng dữ liệu của Designer. **Xung đột
      trên version control gần như bằng không.**

---
