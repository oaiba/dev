https://vorixo.github.io/devtricks/stateful-events-multiplayer/#onreps  
https://wizardcell.com/unreal/multiplayer-tips-and-tricks/
https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine  
https://www.tomlooman.com/unreal-engine-naming-convention-guide/  
https://unrealdirective.com/resources/asset-naming-conventions  
https://unrealdirective.com/tips  
https://github.com/Allar/ue5-style-guide/tree/v2?tab=readme-ov-file

### 1. Yêu cầu tiên quyết: Xây dựng nền móng đúng đắn

#### **Sử dụng phiên bản Unreal Engine từ mã nguồn (Source Build)**

* **Tại sao lại làm như thế?**
    * **Khả năng biên dịch Server Target:** Phiên bản Engine tải từ Epic Games Launcher (Binary build) đã được biên dịch
      sẵn và không bao gồm các công cụ cần thiết để tạo ra một bản build hoàn toàn riêng biệt cho server (
      `Server Target`). Chỉ có phiên bản từ mã nguồn mới cho phép chúng ta biên dịch mục tiêu này, tạo ra một file thực
      thi `.exe` siêu nhẹ, không chứa mã nguồn đồ họa hay âm thanh không cần thiết.
    * **Gỡ lỗi (Debug) Engine:** Khi gặp phải các lỗi sâu bên trong Engine, việc có mã nguồn cho phép chúng ta debug
      từng dòng lệnh, hiểu rõ nguyên nhân và thậm chí tự sửa lỗi nếu cần.
    * **Tùy biến Engine:** chúng ta có toàn quyền tùy chỉnh và mở rộng các tính năng của Engine để phù hợp với nhu cầu
      đặc thù của dự án.
    * **Tiếp cận các tính năng thử nghiệm:** Một số tính năng mới nhất có thể chỉ tồn tại trên các nhánh nhất định của
      mã nguồn.

* **Tip & Trick cho Team:**
    * **Biên dịch một lần, dùng cho cả team:** Một người trong team (thường là lead hoặc build engineer) sẽ chịu trách
      nhiệm biên dịch Engine từ mã nguồn. Sau đó, toàn bộ thư mục Engine đã biên dịch có thể được nén lại và chia sẻ cho
      cả team thông qua một server nội bộ hoặc dịch vụ lưu trữ. Điều này giúp các thành viên khác không phải tốn thời
      gian biên dịch lại.
    * **Sử dụng Unreal Build System (UBT):** Tận dụng UBT để tự động hóa quy trình build Engine, giúp đảm bảo tính nhất
      quán giữa các lần build.

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

#### **Triết lý**

Hãy tưởng tượng một thành phố được quy hoạch một cách tuyệt đối theo ngành nghề và chức năng. Tất cả các ngân hàng nằm
ở "Quận Tài chính", tất cả các nhà máy nằm ở "Khu Công nghiệp", tất cả các trường đại học nằm ở "Làng Đại học". Nếu
chúng ta muốn tìm một thứ gì đó, chúng ta chỉ cần biết "nó là loại gì" để đi đến đúng khu vực.

Trong Unreal, triết lý này có nghĩa là **tất cả các asset cùng loại sẽ được nhóm lại với nhau, bất kể chúng thuộc về
tính năng hay nhân vật nào.**

#### **Cách làm (How-to)**

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

#### **Triết lý**

Hãy tưởng tượng một thành phố được xây dựng như một tập hợp các "khu đô thị" hoặc "công viên chủ đề" hoàn toàn tự chủ.
Mọi thứ chúng ta cần cho "Công viên Kỷ Jura" (khủng long, cây cối, xe jeep, hàng rào điện) đều nằm gọn bên trong bức
tường của công viên đó. Nếu chúng ta muốn di chuyển cả công viên sang một địa điểm mới, chúng ta chỉ cần bứng cả khu đất
đó đi.

Trong Unreal, triết lý này có nghĩa là **tất cả các asset liên quan đến một tính năng hoặc một thực thể cụ thể sẽ được
nhóm lại với nhau trong một thư mục duy nhất.**

#### **Cách làm (How-to)**

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

        * **Quy tắc #1: Phải có "Người gác cổng" (Strict Gatekeeping):** Không ai được phép tự ý thêm asset vào
          `Content/Shared`. Phải có một hoặc vài người (thường là Art Director, Technical Art Lead, hoặc Lead Engineer)
          chịu trách nhiệm phê duyệt mọi thứ được đưa vào đây.
        * **Quy tắc #2: "Quy tắc Ba lần" (The Rule of Three):** Một asset chỉ nên được xem xét để đưa vào `Shared` khi
          nó được yêu cầu sử dụng ở **ba hoặc nhiều nơi khác nhau và riêng biệt**. Nếu nó chỉ được dùng cho hai nhân
          vật, có thể tốt hơn là giữ nó ở một trong hai thư mục nhân vật đó và tham chiếu chéo.
        * **Quy tắc #3: `Shared` là "Chỉ đọc" với Hầu hết mọi người:** Đối với đa số thành viên trong team, các asset
          trong `Shared` nên được coi là "read-only". Họ chỉ nên tạo instance hoặc tham chiếu đến chúng. Việc
          chỉnh sửa một asset trong `Shared` phải là một quy trình có kiểm soát, có thể yêu cầu một buổi họp review
          ngắn.
        * **Quy tắc #4: Kiểm toán Định kỳ (Regular Audits):** Cứ mỗi một hoặc hai tháng, "người gác cổng" nên rà soát
          lại thư mục `Shared` để tìm và loại bỏ các asset không còn được sử dụng hoặc những asset được thêm vào không
          đúng quy cách.

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

### **2. Lựa chọn IDE: JetBrains Rider - Nâng cấp Trạm Tác chiến của chúng ta**

Việc lựa chọn Môi trường phát triển tích hợp (IDE) không đơn thuần là sở thích cá nhân. Đó là một quyết định chiến lược
ảnh hưởng trực tiếp đến năng suất, chất lượng code và tốc độ làm việc của cả team. Trong thế giới Unreal Engine, việc
chuyển từ Visual Studio sang **JetBrains Rider** không chỉ là thay đổi một công cụ, mà là nâng cấp từ một chiếc xe gia
đình đáng tin cậy lên một **siêu xe điện được trang bị AI**, được thiết kế để chinh phục những chặng đường phức tạp
nhất.

#### **Tích hợp sâu với Unreal Engine: IDE "Nói" ngôn ngữ của Game Dev**

Rider không chỉ đọc code C++; nó "hiểu" cách Unreal Engine suy nghĩ.

* **Phân tích Code thông minh:** Rider phân tích code của chúng ta trong thời gian thực và đưa ra các cảnh báo mà chỉ
  người làm Unreal mới hiểu. Nó sẽ là người đồng đội mẫn cán, nhắc nhở chúng ta khi: "chúng ta đã quên gọi
  `Super::BeginPlay()`", "`UPROPERTY` này thiếu `GENERATED_BODY()`", hay thậm chí là phát hiện các lỗi sai chính tả, ngữ
  pháp trong comment và tên biến, giúp code base của chúng ta luôn sạch sẽ và chuyên nghiệp.
* **Quản lý Dự án Tích hợp:** Thay vì phải mở Unreal Editor để tạo một Module hay Plugin mới, chúng ta có thể thực hiện
  việc đó ngay trong Rider. Nó sẽ tự động tạo ra toàn bộ cấu trúc file cần thiết (`.h`, `.cpp`, `Build.cs`), cập nhật
  file  `.uproject` và `.uplugin`. Đây là một sự tự động hóa quy trình giúp tiết kiệm thời gian và giảm thiểu sai sót do
  con người.

#### **Các tính năng "Thay đổi cuộc chơi": Tối ưu hóa từng cú click chuột**

* **Kết nối Blueprint:** "Cây cầu" ma thuật này cho phép chúng ta tìm kiếm một hàm C++ và xem nó được sử dụng ở đâu,
  không chỉ trong code mà còn ở **tất cả các file Blueprint**. Khi chúng ta đổi tên một `UFUNCTION`, Rider sẽ tự động
  cập nhật nó ở mọi nơi, giúp chúng ta tái cấu trúc code một cách tự tin mà không sợ làm hỏng logic của game designer.
* **Unreal Engine Naming C++ Convention:**
* **Tích hợp Git liền mạch:** Rider biến việc làm việc với Git từ một công việc dòng lệnh phức tạp thành một trải nghiệm
  trực quan.
    * chúng ta có thể xem so sánh khác biệt (diff) ngay trong trình soạn thảo, xem ai đã viết từng dòng code (
      `git blame`), và duyệt qua lịch sử commit dưới dạng biểu đồ.
    * Quan trọng nhất, chúng ta có thể **commit từng phần (partial commit)** của một file. Điều này khuyến khích việc
      tạo ra các commit nhỏ, nguyên tử (atomic commits), giúp lịch sử dự án trở nên cực kỳ rõ ràng và dễ theo dõi.
* **Trung tâm phát triển Android tất cả trong một:** Rider tích hợp gần như hoàn chỉnh các công cụ của Android Studio.
  chúng ta có thể mở **Logcat** để xem log từ thiết bị, mở **Terminal** để chạy lệnh ADB, và quản lý **Emulator** trực
  tiếp trong IDE. Nó loại bỏ hoàn toàn nhu cầu phải chạy Android Studio song song, tiết kiệm tài nguyên máy và giúp
  chúng ta tập trung vào một môi trường duy nhất.
* **AI Assistant & Local History: Trợ lý ảo và Cỗ máy thời gian**
    * **AI Assistant:** Đây là một lập trình viên cặp (pair programmer) ảo. Nó có thể giúp chúng ta viết code
      boilerplate, giải thích các đoạn code phức tạp, tìm lỗi, và đặc biệt là **tự động viết tài liệu (documentation)**
      cho các hàm của chúng ta theo chuẩn ngành, một công việc tốn nhiều thời gian nhưng cực kỳ quan trọng.
    * **Local History:** Đây là lớp lưới an toàn cuối cùng của chúng ta. Nó tự động lưu lại lịch sử thay đổi của tất cả
      các file, hoàn toàn độc lập với Git. Nếu chúng ta vô tình xóa mất một file hay một đoạn code quan trọng mà chưa
      commit, Local History cho phép chúng ta quay ngược thời gian và khôi phục lại nó một cách dễ dàng.

#### **Sức mạnh của Hệ sinh thái JetBrains: Khi các công cụ kết hợp thành một Dàn nhạc**

Giá trị thực sự của Rider được nhân lên gấp bội khi chúng ta tích hợp nó với các công cụ khác trong hệ sinh thái
JetBrains,tạo ra một quy trình làm việc chuyên nghiệp, khép kín.

* **DataGrip (Quản lý Database):** Rider có sẵn một trình quản lý database cơ bản. Khi chúng ta cần các công cụ mạnh mẽ
  hơn để làm việc với backend database của game (lưu trữ tài khoản, vật phẩm người chơi), DataGrip cung cấp một giao
  diện chuyên dụng. Vì cùng một hệ sinh thái, trải nghiệm người dùng và các phím tắt sẽ cực kỳ quen thuộc.
* **TeamCity (CI/CD):** Đây là máy chủ Tích hợp và Triển khai liên tục của JetBrains. chúng ta có thể cấu hình TeamCity
  để tự động theo dõi repository Git của chúng ta. Mỗi khi có commit mới, nó sẽ tự động build các bản Client và Server
  cho mọi nền tảng, chạy unit test, và triển khai lên server thử nghiệm.
* **YouTrack (Quản lý dự án & Lỗi):** Tích hợp YouTrack với Rider cho phép chúng ta xem thông tin ticket ngay trong IDE.
  Khi chúng ta làm việc trên một nhánh mới, chúng ta có thể liên kết nó với một ticket. Mỗi commit sẽ tự động được ghi
  nhận vào ticket đó, tạo ra một luồng thông tin minh bạch từ yêu cầu ban đầu của designer đến dòng code cuối cùng của
  developer.

### **Thiết lập Target cho Dedicated Server: "Phòng phẫu thuật" tách Client và Server**

Phần này mô tả một trong những bước kỹ thuật quan trọng nhất.

#### **Tip & Trick cho Team (Mở rộng)**

* **Sử dụng Preprocessor Directives một cách chiến lược:**
    * **Triển khai gợi ý của chúng ta:** Các chỉ thị tiền xử lý không chỉ giới hạn ở `#if UE_SERVER`. chúng ta có thể
      kết hợp chúng để tạo ra các logic cực kỳ linh hoạt:
        * `#if PLATFORM_WINDOWS && WITH_EDITOR`: Code chỉ chạy trong Editor trên Windows.
        * `#if UE_BUILD_SHIPPING`: Code sẽ bị loại bỏ trong bản build cuối cùng cho người chơi (dùng để loại bỏ các công
          cụ gian lận/debug).
        * `#if WITH_MYPLUGIN_NAME`: Code chỉ được biên dịch nếu một plugin cụ thể nào đó đang được bật.
    * Điều này giúp giữ cho code base của chúng ta duy nhất nhưng có thể thích ứng với mọi nền tảng và mọi cấu hình
      build, một yếu tố cực kỳ quan trọng để bảo trì dự án trong dài hạn.

### **Giai đoạn 2.5: Kiến trúc Gameplay Multiplayer Nâng cao - Xây dựng những cỗ máy Lego phức tạp**

Chúng ta đã nắm vững các viên gạch Replication, Roles, và RPCs. Bây giờ là lúc tìm hiểu các bản thiết kế nâng cao để xây
dựng những hệ thống phức tạp như hệ thống kỹ năng, trang bị, hay AI một cách hiệu quả và dễ mở rộng.

#### **1. Bộ Ba Quyền Lực: GameState, PlayerState và PlayerController**

Ngoài `Character`, Unreal cung cấp các lớp cốt lõi mà mọi lập trình viên multiplayer phải nắm vững.

* **`AGameStateBase` (GameState): Bảng điểm của trận đấu**
    * **Nó là gì?** Một Actor đặc biệt, tồn tại duy nhất trên server và được sao chép (replicate) đến tất cả client. Nó
      chứa trạng thái **chung** của toàn bộ trận đấu.
    * **Tại sao cần?** Đây là nơi để lưu trữ các thông tin mà mọi người chơi cần biết: thời gian còn lại của trận đấu,
      danh sách người chơi, đội nào đang thắng, trạng thái của các mục tiêu trong map...
    * **Tip & Trick:** Khi một client mới kết nối vào giữa trận, GameState là thứ đầu tiên họ nhận được để "bắt kịp"
      trạng thái hiện tại của game. Hãy đặt mọi thông tin công khai, toàn cục vào đây.

* **`APlayerState` (PlayerState): "Căn cước công dân" của người chơi**
    * **Nó là gì?** Một Actor đi kèm với mỗi người chơi, tồn tại trên server và được sao chép đến tất cả client. Nó chứa
      trạng thái **cá nhân** của một người chơi, tách biệt với nhân vật mà họ đang điều khiển.
    * **Tại sao cần?** Đây là nơi lưu trữ các thông tin bền vững của người chơi trong suốt phiên chơi, ngay cả khi họ
      chết và hồi sinh (nhân vật cũ bị hủy, nhân vật mới được tạo ra). Ví dụ: tên người chơi, điểm số, số mạng giết
      được, ping, đội...
    * **Tip & Trick:** Đừng bao giờ lưu điểm số hay tên người chơi trực tiếp trong lớp `Character`. Hãy lưu chúng trong
      `PlayerState`. `Character` chỉ nên chứa các thông tin tạm thời như máu, đạn.

* **`APlayerController` (PlayerController): Linh hồn của người chơi**
    * **Nó là gì?** Là đại diện cho người chơi con người trong game. Nó tồn tại trên server và client sở hữu nó. Nó nhận
      input và "chiếm hữu" (possess) một `Pawn` (như `Character`) để điều khiển.
    * **Tại sao cần?** Đây là kênh giao tiếp chính từ client lên server. Các RPCs yêu cầu hành động (bắn súng, dùng kỹ
      năng) thường được đặt ở đây hoặc trong `Character`. Các logic không liên quan đến hình thể nhân vật (như hiển thị
      UI, quản lý menu) nên nằm ở `PlayerController`.

#### **2. Gameplay Ability System (GAS): Bộ Lego Technic cho Kỹ năng và Thuộc tính**

* **Nó là gì?** Một framework cực kỳ mạnh mẽ và phức tạp do chính Epic Games phát triển và sử dụng trong các game như
  Fortnite, Paragon. Nó cung cấp sẵn một bộ khung hoàn chỉnh để tạo ra các kỹ năng, hiệu ứng (buff/debuff), và thuộc
  tính (máu, mana, sức mạnh...).
* **Tại sao nên dùng?**
    * **Tối ưu cho mạng:** GAS được thiết kế từ đầu cho game multiplayer, xử lý sẵn các vấn đề về dự đoán (prediction)
      trên client, giúp kỹ năng có cảm giác phản hồi ngay lập tức.
    * **Dễ mở rộng:** Thay vì hard-code các kỹ năng, chúng ta có thể tạo ra chúng dưới dạng các asset riêng biệt. Game
      designer có thể tự do tạo và chỉnh sửa kỹ năng mà không cần đụng đến code.
    * **Quản lý phức tạp:** Dễ dàng xử lý các tương tác phức tạp như: "Kỹ năng này gây 50 sát thương lửa, làm chậm 30%
      trong 5 giây, và hồi 10 mana cho người dùng nếu mục tiêu đang bị đóng băng."
* **Tip & Trick:** Học GAS là một khoản đầu tư lớn nhưng cực kỳ xứng đáng cho các game có hệ thống combat phức tạp (RPG,
  MOBA, Action). Hãy bắt đầu tìm hiểu nó sớm.

#### **3. Lag Compensation: Bắn trúng "bóng ma"**

* **Vấn đề là gì?** Khi chúng ta (Client A) bắn vào địch (Client B), chúng ta bắn vào vị trí chúng ta *nhìn thấy* trên
  màn hình. Nhưng do ping, vị trí đó là vị trí của Client B cách đây 50ms. Khi gói tin của chúng ta đến server, Client B
  đã di chuyển đến một vị trí khác. Server sẽ nói chúng ta bắn trượt. Cảm giác cực kỳ khó chịu.
* **Giải pháp (Lag Compensation):**
    * Khi Server nhận được yêu cầu bắn từ Client A, nó sẽ không kiểm tra ngay lập tức.
    * Nó sẽ "quay ngược thời gian" của thế giới game lại một khoảng bằng đúng ping của Client A.
    * Nó sẽ tạm thời di chuyển tất cả các nhân vật khác về vị trí của họ tại thời điểm đó.
    * *Bây giờ*, nó mới thực hiện việc kiểm tra bắn. Kết quả sẽ khớp với những gì Client A đã thấy.
    * Sau đó, nó trả thế giới game về lại thời điểm hiện tại.
* **Tip & Trick:**
    * May mắn là `CharacterMovementComponent` đã tự động xử lý việc này cho chuyển động.
    * Tuy nhiên, đối với việc bắn súng (line trace) hoặc các kỹ năng có mục tiêu, chúng ta phải **tự mình xây dựng hệ
      thống này**. Đây là một trong những kỹ thuật khó nhưng bắt buộc phải có cho các game hành động bắn súng. Hãy
      nghiên cứu sâu về nó.

---

### 3. Cấu hình Project Settings: Chỉ dẫn cho Server hoạt động

* **Làm như thế nào?**
    1. **Server Default Map:** Mở `Edit > Project Settings > Maps & Modes`. Tìm đến mục `Server Default Map` và chọn bản
       đồ mà chúng ta muốn server tự động tải khi nó khởi động. Thường đây sẽ là một bản đồ menu chính hoặc một "lobby
       map".
    2. **Packaging Maps:** Trong `Project Settings > Packaging`, cuộn xuống và nhấn vào mũi tên mở rộng ở dưới cùng để
       hiển thị các cài đặt nâng cao. Trong mục `List of Maps to Include in a Packaged Build`, thêm vào tất cả các bản
       đồ mà game của chúng ta sẽ sử dụng, bao gồm cả bản đồ default của server và tất cả các bản đồ gameplay khác.

* **Tại sao lại làm như thế?**
    * **Tự động hóa khởi động server:** Việc thiết lập `Server Default Map` cho phép chúng ta khởi động server mà không
      cần truyền thêm tham số dòng lệnh chỉ định bản đồ. Server sẽ tự biết phải tải bản đồ nào đầu tiên.
    * **Đảm bảo tài sản (assets) được đóng gói:** Quá trình đóng gói (packaging) của Unreal chỉ tự động "nấu" (cook) và
      đưa vào bản build các assets được tham chiếu từ một "root" nào đó (thường là các bản đồ). Nếu chúng ta không thêm
      các bản đồ vào danh sách này, các assets chỉ được sử dụng trong bản đồ đó có thể sẽ không được đóng gói, dẫn đến
      lỗi " Can't find file for package..." khi server cố gắng chuyển đến bản đồ đó.

* **Tip & Trick cho Team:**
    * **Tạo một bản đồ "dummy" cho server:** Đôi khi, chúng ta không muốn server khởi động ngay vào một bản đồ gameplay
      nặng nề. Hãy tạo một bản đồ trống, siêu nhẹ, đặt tên là `Entry` hoặc `ServerEntry` và đặt nó làm
      `Server Default Map`.
      Từ bản đồ này, server có thể thực hiện các tác vụ khởi tạo ban đầu trước khi chuyển sang bản đồ gameplay thực sự.
    * **Sử dụng Primary Asset Labels:** Đối với các dự án lớn, việc quản lý danh sách map thủ công có thể trở nên khó
      khăn. Hãy tìm hiểu về hệ thống "Primary Asset Labels" trong `Project Settings > Asset Manager`. Nó cho phép chúng
      ta tự động đưa các assets (bao gồm cả map) vào bản build dựa trên các quy tắc và nhãn mà chúng ta định nghĩa, thay
      vì phải thêm từng cái một vào danh sách.

### Phần 4: Git Submodule - Nghệ thuật lắp ghép "Lego" cho Dự án

Hãy tưởng tượng dự án Unreal Engine của chúng ta là một công trình Lego phức tạp. Phần mã nguồn C++ và các assets chính
củachúng ta là những viên gạch nền cơ bản. Các plugin và thư viện bên ngoài (ví dụ: plugin về AI, hệ thống UI nâng cao,
OnlineSubsystem...) chính là những bộ Lego chuyên biệt, độc đáo mà chúng ta muốn gắn vào công trình của mình.

**Git Submodule không sao chép** những bộ Lego đó vào thùng gạch của chúng ta. Thay vào đó, nó ghi lại một tờ giấy
nhớ: "Tạivị trí này, hãy lấy bộ Lego 'AwesomePlugin' phiên bản 1.2 (chính xác là mảnh ghép có mã số `abc1234`) và lắp nó
vào đây".

Phiên bản `1.2` hay mã số `abc1234` chính là **commit hash** của repository plugin đó.

Đây là nguyên tắc cốt lõi: **Repo chính chỉ lưu trữ một con trỏ (pointer) đến một commit cụ thể của submodule, chứ không
phải toàn bộ lịch sử của nó.**

---

#### 1. Nền tảng không thể thiếu: Git LFS và `.gitignore`

Trước khi lắp ghép, sàn nhà của chúng ta phải sạch sẽ.

* **Tại sao Git LFS là bắt buộc?**
    * Git được sinh ra để quản lý code (file text), nó sẽ lưu lại mọi thay đổi nhỏ nhất của file. Với các file nhị phân
      lớn như `.uasset`, `.fbx`, `.png`, chỉ cần thay đổi 1 pixel, Git sẽ coi đó là một file hoàn toàn mới và lưu trữ
      một bản sao đầy đủ. Điều này khiến repository phình to lên hàng chục, hàng trăm GB một cách chóng mặt, việc clone
      hay pull trở thành cực hình.
    * Git LFS giải quyết bằng cách: Thay vì lưu file lớn vào Git, nó lưu file đó lên một server riêng và chỉ để lại một
      file text "con trỏ" vài trăm byte trong repository. Khi chúng ta checkout, LFS sẽ tự động tải file lớn đó về.

* **Tại sao `.gitignore` lại quan trọng?**
    * Unreal Engine tự động sinh ra rất nhiều file và thư mục tạm thời trong quá trình làm việc (như `Intermediate`,
      `Saved`, `DerivedDataCache`). Các file này có dung lượng rất lớn, đặc thù cho từng máy và không nên được đưa vào
      quản lý phiên bản.
    * Một file `.gitignore` chuẩn sẽ yêu cầu Git "làm lơ" các thư mục này, giữ cho repository của chúng ta gọn nhẹ và
      chỉ chứa những gì thực sự cần thiết.

**Tip Trick cho Team:**

* **Thiết lập một lần:** Lead của team nên là người thiết lập file `.gitattributes` (cho LFS) và `.gitignore` chuẩn ngay
  từ đầu. Mọi thành viên chỉ việc clone và làm theo.
* **Kiểm tra trước khi commit:** Luôn chạy `git status` trước khi commit để đảm bảo không có file rác nào (như các file
  trong thư mục `Saved`) vô tình được thêm vào.

---

#### 2. Chiến lược sử dụng Submodule tối ưu

Đây là phần nâng cao, giúp team của chúng ta không bị rối khi dự án phình to.

##### Chiến lược 1: "Fork First" - Luôn làm chủ các dependency của chúng ta

Không bao giờ thêm submodule trực tiếp từ repository gốc của một plugin bên thứ ba. Hãy luôn **fork** nó về organisation
GitHub/GitLab của team chúng ta trước.

* **Làm như thế nào?**
    1. Vào trang GitHub của plugin, nhấn nút "Fork".
    2. Khi thêm submodule, hãy dùng URL của repository chúng ta vừa fork:
       `git submodule add https://github.com/MyTeam/AwesomePlugin_Fork.git Plugins/AwesomePlugin`

* **Tại sao lại làm như thế?**
    * **Toàn quyền kiểm soát:** Điều gì xảy ra nếu plugin gốc có lỗi nghiêm trọng ảnh hưởng đến game của chúng ta, nhưng
      tác giả lại không sửa? Với bản fork, team chúng ta có thể tự mình vào sửa lỗi, commit và push lên bản fork đó.
    * **An toàn khi cập nhật:** chúng ta có thể thoải mái kéo các cập nhật mới nhất từ repository gốc (`upstream`) về
      bản
      fork của mình, kiểm thử cẩn thận trước khi quyết định tích hợp vào dự án chính. chúng ta không bị ép phải cập nhật
      ngay lập tức.
    * **Tùy biến:** chúng ta có thể thêm các tính năng riêng cho plugin để phù hợp với game của mình mà không ảnh hưởng
      đến ai.

##### Chiến lược 2: "Plugin Meta-Repo" - Tạo một "Hộp dụng cụ" Plugin

Khi dự án sử dụng 5-10 plugin trở lên, việc quản lý từng cái một trong repo chính sẽ rất lộn xộn. Giải pháp là tạo ra
một repository trung gian.

* **Làm như thế nào?**
    1. Tạo một repository mới, đặt tên là `MyGame-Plugins`.
    2. Trong repository này, **hãy thêm tất cả các plugin chúng ta cần dưới dạng submodule của nó**.
       ```bash
       # Bên trong repo MyGame-Plugins
       git submodule add https://github.com/MyTeam/AwesomePlugin_Fork.git AwesomePlugin
       git submodule add https://github.com/MyTeam/AnotherPlugin_Fork.git AnotherPlugin
       git commit -m "Add required plugins"
       ```
    3. Bây giờ, trong repository game chính, chúng ta chỉ cần thêm **duy nhất một submodule**:
       `git submodule add https://github.com/MyTeam/MyGame-Plugins.git Plugins`

* **Tại sao lại làm như thế?**
    * **Gọn gàng:** Repository chính của game sẽ cực kỳ sạch sẽ, chỉ có một submodule `Plugins` thay vì một danh sách
      dài.
    * **Quản lý tập trung:** Một người (ví dụ Tech Lead) có thể chịu trách nhiệm quản lý repository `MyGame-Plugins`.
      Khi cần cập nhật một plugin nào đó, họ sẽ cập nhật trong "hộp dụng cụ" này, kiểm thử, rồi commit. Team dev chỉ cần
      cập nhật một submodule duy nhất là `Plugins`.
    * **Tái sử dụng:** Nếu công ty chúng ta có nhiều dự án game, "hộp dụng cụ" plugin này có thể được tái sử dụng cho
      tất cả các dự án.

##### Chiến lược 3: Cấu trúc Repo cho Dev và Artist - Monorepo là lựa chọn hàng đầu

* **Phân chia folder như thế nào?**
    * Cách tiếp cận hiệu quả nhất cho hầu hết các team làm game Unreal là **Monorepo** - tức là tất cả mọi thứ (code,
      art, sound...) đều nằm chung trong một repository lớn.
    * Sự phân chia nên nằm ở cấp độ thư mục, với quy tắc rõ ràng:
        * `Content/Art/Characters`: Chứa các model, texture, material của nhân vật.
        * `Content/Art/Environments`: Chứa các assets môi trường.
        * `Content/Blueprints/Characters`: Chứa các Blueprint liên quan đến nhân vật.
        * `Source/MyGame/Public`: Chứa các file header C++.
        * `Source/MyGame/Private`: Chứa các file cpp C++.

* **Tại sao Monorepo lại hiệu quả?**
    * **Atomic Commits (Commit nguyên tử):** Một tính năng mới thường đòi hỏi cả sự thay đổi về code (dev) và art (
      artist). Monorepo cho phép chúng ta thực hiện một commit duy nhất bao gồm cả hai thay đổi này. Điều này giúp lịch
      sử dự án cực kỳ dễ theo dõi. Khi cần revert một tính năng, chúng ta chỉ cần revert một commit là xong.
    * **Dễ dàng đồng bộ:** Mọi người luôn ở trên cùng một phiên bản của tất cả mọi thứ. Không có chuyện code chạy được
      nhưng art lại là phiên bản cũ và gây lỗi.
    * **Submodule chỉ dành cho bên thứ ba:** Trong mô hình này, submodule chỉ nên được sử dụng cho các code/thư
      viện/plugin từ bên ngoài, không phải để phân tách các phần nội bộ của dự án.

---

#### 3. Quy trình làm việc (Workflow) hàng ngày với Submodule

Đây là kim chỉ nam cho mọi thành viên trong team.

* **A. Lần đầu tiên lấy dự án về:**
  `git clone --recurse-submodules <URL_repo_chính>`
  *Lệnh này sẽ clone repo chính, sau đó tự động clone tất cả các submodule được đăng ký.*

* **B. Bắt đầu một ngày làm việc (lấy code mới nhất về):**
    1. `git pull`  *(Lấy code mới nhất của repo chính. Có thể con trỏ submodule đã bị thay đổi bởi người khác).*
    2. `git submodule update --recursive`  *(Đọc con trỏ mới và cập nhật code của submodule cho khớp).*
       *Đây là quy trình 2 bước bắt buộc. Nếu chỉ `git pull`, code submodule của chúng ta sẽ bị cũ và gây lỗi biên
       dịch.*

* **C. Cập nhật một plugin lên phiên bản mới (Dành cho Lead):**
    1. `cd Plugins/AwesomePlugin` *(Đi vào thư mục của submodule).*
    2. `git checkout main` *(Chuyển sang nhánh chính).*
    3. `git pull` *(Kéo code mới nhất của plugin về).*
    4. `cd ../..` *(Quay trở lại thư mục gốc của dự án chính).*
    5. `git status` *(chúng ta sẽ thấy dòng `modified: Plugins/AwesomePlugin (new commits)`).*
    6. `git add Plugins/AwesomePlugin`
    7. `git commit -m "Feat: Update AwesomePlugin to version 2.0 for new feature X"`
    8. `git push` *(Đẩy "tờ giấy nhớ" mới lên cho cả team).*
       *Sau khi Lead push, các thành viên khác sẽ thực hiện quy trình B để nhận bản cập nhật.*

* **D. Sửa lỗi/thêm tính năng tùy chỉnh vào plugin (Dành cho Dev):**
    1. `cd Plugins/AwesomePlugin` *(Đi vào thư mục của submodule đã fork).*
    2. *Thực hiện các thay đổi code...*
    3. `git add .`
    4. `git commit -m "Fix: Fixed a critical crash when..."`
    5. `git push` *(Push thay đổi lên repository fork của plugin).*
    6. *Làm theo các bước từ 4 đến 8 của quy trình C để cập nhật con trỏ trong repo chính.*

### Giai đoạn 2: Xây dựng và Phát triển - Lắp ráp các khối Lego Gameplay

Nếu Giai đoạn 1 là việc chuẩn bị "xưởng lắp ráp" và "bản thiết kế" (Project Setup, Git), thì Giai đoạn 2 chính là lúc
chúng ta thực sự lắp ráp các cỗ máy phức tạp từ những viên gạch Lego cơ bản. Mỗi tính năng, từ việc di chuyển, bắn súng,
đến nhặt đồ, đều là một cỗ máy nhỏ được lắp ráp từ 3 loại "gạch Lego" mạng cốt lõi: **Replication**, **Network Roles**,
và **RPCs**.

---

### 1. Lập trình Gameplay cho Multiplayer: Bộ gạch Lego nền tảng

Hãy tưởng tượng **Server là người Thợ Cả (Master Builder)**, người duy nhất có bản thiết kế gốc và quyền quyết định cuối
cùng. **Mỗi Client là một người Học Việc**, đang cố gắng xây một bản sao y hệt mô hình của Thợ Cả.

#### **A. Replication (Sao chép): "Thợ Cả thông báo sự thay đổi"**

Đây là loại gạch Lego cơ bản nhất, dùng để đồng bộ hóa **trạng thái** (state) của các vật thể.

* **Phép loại suy Lego:** Khi Thợ Cả (Server) thay đổi màu của một viên gạch trên mô hình gốc từ xanh sang đỏ, họ sẽ hô
  lên cho tất cả Học Việc (Clients): "Viên gạch ở vị trí XYZ giờ là màu đỏ!". Mọi người Học Việc nghe thấy và thay đổi
  viên gạch tương ứng trên bản sao của họ.
* **Tại sao nó quan trọng?**
    * Nó đảm bảo rằng thế giới game của mọi người chơi trông giống nhau. Máu của một nhân vật, số đạn trong băng, vị trí
      của một cánh cửa... tất cả đều là "trạng thái" cần được đồng bộ.
    * Đây là cách hiệu quả nhất để giữ cho dữ liệu nhất quán mà không cần gửi tin nhắn liên tục.

* **Làm như thế nào trong C++?**
    1. Trong file `.h` của Actor, chúng ta phải bật chế độ replication: `bReplicates = true;` trong Constructor.
    2. Đánh dấu các biến cần đồng bộ bằng `UPROPERTY(Replicated)` hoặc `UPROPERTY(ReplicatedUsing=FunctionName)`.
    3. Override hàm `GetLifetimeReplicatedProps`.

  ```cpp
  // MyCharacter.h
  #pragma once
  #include "GameFramework/Character.h"
  #include "MyCharacter.generated.h"

  UCLASS()
  class MYGAME_API AMyCharacter : public ACharacter
  {
      GENERATED_BODY()
  public:
      AMyCharacter();

      // Biến này sẽ được đồng bộ từ Server xuống các Client
      UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
      float CurrentHealth;

      UFUNCTION()
      void OnRep_CurrentHealth(); // Hàm này sẽ được gọi trên Client mỗi khi CurrentHealth thay đổi

  protected:
      virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
  };

  // MyCharacter.cpp
  #include "Net/UnrealNetwork.h"

  AMyCharacter::AMyCharacter()
  {
      bReplicates = true; // Bật chức năng "Lego" cho Actor này
  }

  void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
  {
      Super::GetLifetimeReplicatedProps(OutLifetimeProps);
      // Đăng ký biến CurrentHealth vào hệ thống replication
      DOREPLIFETIME(AMyCharacter, CurrentHealth);
  }

  void AMyCharacter::OnRep_CurrentHealth()
  {
      // Đây là nơi hoàn hảo để cập nhật thanh máu trên UI của Client
      // Vì nó chỉ chạy trên Client khi máu thực sự thay đổi.
  }
  ```

* **OOP & Tip Trick:**
    * **Nguyên tắc Đóng gói (Encapsulation):** Không bao giờ để Client trực tiếp thay đổi các biến Replicated. Hãy làm
      cho `CurrentHealth` thành `private` hoặc `protected` và tạo ra một hàm public như `TakeDamage(float Damage)` để xử
      lý. Việc thay đổi máu thực sự chỉ nên xảy ra trên Server.
    * **`ReplicatedUsing` là người chúng ta tốt nhất của chúng ta:** Dùng nó để kích hoạt các hiệu ứng hình ảnh/âm thanh
      trên
      Client (như cập nhật UI, chạy hiệu ứng máu bắn ra) ngay khi một biến được cập nhật. Điều này giúp tách biệt logic
      gameplay (trên server) và logic hiển thị (trên client).

#### **B. Network Roles (Vai trò mạng): "Ai là Thợ Cả?"**

Hệ thống này xác định quyền hạn của mỗi máy tính đối với một Actor cụ thể.

* **Phép loại suy Lego:**
    * **Authority (Thợ Cả):** Máy tính có bản thiết kế gốc của một Actor (luôn là Server cho các Actor trong thế giới
      game). Chỉ nó mới có quyền thay đổi Actor đó.
    * **Autonomous Proxy (Học Việc tự chủ):** Client đang điều khiển trực tiếp Actor đó (ví dụ: nhân vật của chính chúng
      ta).
      Client này có thể gửi **yêu cầu** cho Thợ Cả nhanh hơn.
    * **Simulated Proxy (Học Việc quan sát):** Client chỉ đang nhìn thấy một Actor do người khác điều khiển. Họ chỉ nhận
      thông tin cập nhật từ Thợ Cả.

* **Tại sao nó quan trọng?**
    * **Chống gian lận (Anti-Cheat):** Đây là nền tảng của mô hình Server-Authoritative. Một Client không thể tự quyết
      định "OK, máu của tôi bây giờ là 1000" vì nó không có `Authority`. Mọi quyết định quan trọng về gameplay (trừ máu,
      tính sát thương, cho vật phẩm) đều phải được kiểm tra qua `HasAuthority()`.
    * **Tối ưu hóa:** Logic AI, các tính toán phức tạp chỉ cần chạy trên Server (`Authority`), không cần chạy trên
      Client.

* **Làm như thế nào?**
    * Sử dụng `if (HasAuthority())` hoặc `if (GetLocalRole() == ROLE_Authority)` để bao bọc các đoạn code chỉ Server mới
      được phép thực thi.

  ```cpp
  void AMyCharacter::TakeDamage(float DamageAmount)
  {
      // Chỉ Thợ Cả (Server) mới có quyền trừ máu
      if (HasAuthority())
      {
          CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
          // Server thay đổi giá trị. Sau đó, hệ thống Replication sẽ tự động
          // gửi giá trị CurrentHealth mới này xuống cho tất cả các Client.
      }
  }
  ```

* **OOP & Tip Trick:**
    * **Authority Pattern:** Đây là một Design Pattern cơ bản trong lập trình mạng. Hãy tập thói quen luôn tự hỏi: "Đoạn
      code này có thay đổi trạng thái của game không? Nếu có, nó có đang được bọc trong `if (HasAuthority())` không?".
    * Trong Blueprint, chúng ta có thể dùng node `Switch Has Authority`.

#### **C. Remote Procedure Calls (RPCs): "Kênh giao tiếp giữa Thợ Cả và Học Việc"**

RPCs là các "mệnh lệnh" hoặc "yêu cầu" dưới dạng hàm được gọi qua mạng. Chúng dùng cho các **sự kiện** (events), không
phải trạng thái.

* **Phép loại suy Lego:**
    * **Server RPC (Yêu cầu lên Thợ Cả):** Người Học Việc (Client) nói với Thợ Cả (Server): "Thưa ngài, tôi muốn bắn
      khẩu súng này!".
    * **Client RPC (Mệnh lệnh cho một Học Việc):** Thợ Cả nói riêng với người Học Việc vừa yêu cầu: "OK, khẩu súng của
      cậu bị giật mạnh, hãy hiển thị hiệu ứng đó trên màn hình của cậu đi!".
    * **Multicast RPC (Thông báo cho tất cả):** Thợ Cả hô to cho tất cả mọi người: "Mọi người chú ý! Vừa có một tiếng
      súng nổ ở vị trí XYZ, hãy chạy hiệu ứng và âm thanh!".

* **Tại sao nó quan trọng?**
    * Đây là cách duy nhất để Client có thể yêu cầu Server thực hiện một hành động. Client không thể tự mình bắn súng,
      họ phải "xin phép" Server.
    * Dùng để kích hoạt các hiệu ứng tức thời, không đáng tin cậy (unreliable) như tiếng bước chân, hoặc các sự kiện
      quan trọng, đáng tin cậy (reliable) như sử dụng một kỹ năng.

* **Làm như thế nào?**
    * Sử dụng các macro `UFUNCTION` đặc biệt trong file `.h`.

  ```cpp
  // MyCharacter.h

  // Client gọi hàm này, nhưng nó sẽ thực thi trên Server
  UFUNCTION(Server, Reliable, WithValidation)
  void Server_FireWeapon();

  // Server gọi hàm này, nó sẽ thực thi trên tất cả các Client
  UFUNCTION(NetMulticast, Unreliable)
  void Multicast_PlayFireFX();

  // MyCharacter.cpp

  // Hàm kiểm tra xem yêu cầu từ Client có hợp lệ không
  bool AMyCharacter::Server_FireWeapon_Validate() { return true; /* Thêm logic kiểm tra chống spam ở đây */ }

  // Logic thực thi trên Server sau khi Client yêu cầu
  void AMyCharacter::Server_FireWeapon_Implementation()
  {
      // Kiểm tra xem nhân vật có đủ đạn, có đang bị khống chế không...
      // -> Logic gameplay cốt lõi chỉ chạy trên Server
      if (CanFire())
      {
          // Bắn một tia đạn (line trace)
          // Trừ đạn
          // Gây sát thương nếu trúng đích
          //...

          // Sau khi xử lý xong, thông báo cho tất cả mọi người
          Multicast_PlayFireFX();
      }
  }

  void AMyCharacter::Multicast_PlayFireFX_Implementation()
  {
      // Chạy hiệu ứng tia lửa đầu nòng, âm thanh súng nổ...
      // Đoạn code này sẽ chạy trên Server VÀ tất cả các Client
  }

  void AMyCharacter::Fire() // Hàm này được gọi từ input của người chơi
  {
      // Người chơi (Client) không trực tiếp bắn.
      // Họ gửi yêu cầu lên Server.
      Server_FireWeapon();
  }
  ```
* **OOP & Tip Trick:**
    * **Command Pattern:** RPCs, đặc biệt là Server RPC, hoạt động rất giống Command Pattern. Client đóng gói một "
      lệnh" (bắn súng, dùng item) và gửi nó đi, Server là người nhận và thực thi lệnh đó.
    * **`Reliable` vs `Unreliable`:** Chỉ sử dụng `Reliable` cho những gì **bắt buộc** phải đến nơi (bắn súng, dùng kỹ
      năng). Sử dụng `Unreliable` cho các hiệu ứng phụ, không quan trọng (tiếng bước chân, tia lửa...). Gửi quá nhiều
      RPC `Reliable` có thể làm nghẽn mạng.

---

### 2. Các Plugin cần thiết: Những bộ Lego Technic chuyên dụng

Đây là những "bộ Lego" làm sẵn, cực kỳ phức tạp mà chúng ta không muốn tự mình xây dựng từ đầu.

* **Online Subsystem (OSS):**
    * **Nó là gì?** Một lớp trừu tượng (Interface) của Unreal Engine, hoạt động như một "đầu cắm Lego đa năng". chúng ta
      chỉ
      cần lập trình với đầu cắm này, nó sẽ tự động tương thích với các "ổ cắm" khác nhau như Steam, Epic Online
      Services, Xbox Live...
    * **Tại sao cần?** Để chúng ta không phải viết code riêng cho từng nền tảng. Viết code tạo phòng chờ (session) một
      lần,
      chạy được cả trên Steam và Epic Games Store.
    * **Lưu ý:** Việc cấu hình OSS trong file `DefaultEngine.ini` khá phức tạp và đặc thù cho từng nền tảng. Hãy đọc kỹ
      tài liệu của Epic và nền tảng chúng ta chọn.

* **Advanced Sessions Plugin:**
    * **Nó là gì?** Một plugin miễn phí giúp việc sử dụng OSS trở nên đơn giản hơn rất nhiều bằng cách cung cấp các node
      Blueprint tiện lợi.
    * **Tại sao cần?** Nó là một "bộ chuyển đổi" giúp việc cắm các "đầu cắm Lego" trở nên dễ dàng hơn, đặc biệt nếu
      chúng ta
      muốn các game designer có thể tự tạo logic phòng chờ bằng Blueprint.

* **Voice Chat / Anti-Cheat:**
    * Đây là những "bộ Lego" chuyên dụng khác. Việc tích hợp chúng thường đòi hỏi làm việc với SDK riêng của nhà cung
      cấp (Vivox, EAC...). Hãy coi chúng như những submodule riêng biệt trong dự án.

---

### 3. Biên dịch (Build) và Đóng gói (Package): Tạo ra hai bộ Lego để bán

Sau khi lắp ráp xong mô hình, chúng ta cần tạo ra hai phiên bản "hộp sản phẩm" khác nhau.

* **Biên dịch Server:**
    * **Nó là gì?** Là quá trình tạo ra một file `.exe` chỉ chứa logic gameplay, không có đồ họa, âm thanh.
    * **Tại sao?** Vì server không cần render thế giới game, nó chỉ cần tính toán. File `.exe` này siêu nhẹ, giúp chúng
      ta
      chạy nhiều phòng game trên một máy chủ vật lý để tiết kiệm chi phí.
    * **Làm như thế nào?** Trong Visual Studio, ở thanh dropdown Solution Configurations, thay vì chọn
      `Development Editor`, hãy chọn `Development Server` và nhấn Build.

* **Đóng gói (Packaging):**
    * **Nó là gì?** Là quá trình "nấu" (cook) tất cả các assets của chúng ta (model, texture, map...) thành định dạng
      tối ưu
      và đóng gói chúng cùng với file `.exe` để tạo ra một sản phẩm hoàn chỉnh có thể phân phối.
    * **Sự khác biệt cốt lõi:**
        * **Đóng gói Client:** `File > Package Project > Windows`. Đây là phiên bản đầy đủ cho người chơi, chứa cả đồ
          họa và gameplay.
        * **Đóng gói Server:** `File > Package Project > Build Target > [TênDựÁn] Server`. Thao tác này sẽ tạo ra một
          thư mục riêng (ví dụ `WindowsServer`) chỉ chứa những gì server cần. Nó sẽ không có các texture 4K hay các file
          âm thanh nặng nề.

* **Tip & Trick cho Team:**
    * **Tự động hóa:** Viết một file script `.bat` đơn giản sử dụng `RunUAT.bat` của Unreal để tự động hóa toàn bộ quá
      trình đóng gói cho cả Client và Server. Điều này đảm bảo tính nhất quán và tiết kiệm thời gian, đặc biệt khi chúng
      ta
      cần build thường xuyên. Mỗi lần cần build mới, chỉ cần chạy một file batch là xong.

Tuyệt vời! Chúng ta sẽ đi sâu vào hai giai đoạn cuối cùng, tập trung vào việc biến sản phẩm trong môi trường phát triển
thành một hệ thống hoạt động thực tế, ổn định và có thể mở rộng. Đây là lúc công trình Lego của chúng ta được kiểm tra
độ bền
và đưa ra thế giới.

---

### Giai đoạn 3: Kiểm thử và Tối ưu hóa - Thử nghiệm và Gia cố công trình Lego

Đây là giai đoạn chúng ta "lắc mạnh" công trình Lego của mình để xem có mảnh nào rơi ra không, và tìm cách gia cố những
điểm
yếu trước khi giao cho người khác chơi.

#### 1. Chạy và kiểm thử Dedicated Server cục bộ: Sân chơi thử nghiệm tại nhà

Trước khi đưa server lên mây, việc kiểm thử tại chính máy của chúng ta là bước nhanh nhất và hiệu quả nhất để tìm ra các
lỗi
gameplay.

* **Làm như thế nào?**
    1. **Tạo file Batch (.bat) để tự động hóa:** Thay vì tạo shortcut và chỉnh tay, hãy tạo các file `.bat`. Đây là
       một "Tip & Trick" cực kỳ tiết kiệm thời gian cho cả team.
        * **`StartServer.bat`:**
          ```batch
          @echo off
          echo Starting Server...
          start Binaries\Win64\[TênDựÁn]Server.exe /Game/Maps/TenMap_Gameplay?listen -log
          exit
          ```
            * `?listen`: Báo cho server biết nó cần mở cổng và lắng nghe kết nối từ client.
            * `-log`: Mở một cửa sổ console riêng để chúng ta có thể xem log của server theo thời gian thực.
        * **`StartClient.bat`:**
          ```batch
          @echo off
          echo Starting Client...
          start Binaries\Win64\[TênDựÁn].exe 127.0.0.1 -game
          exit
          ```
            * `127.0.0.1`: Địa chỉ IP "localhost", tức là kết nối đến server đang chạy trên cùng máy.
            * `-game`: Chạy game ở chế độ cửa sổ độc lập thay vì mở editor.
    2. **Kiểm thử trong Editor:** Để debug nhanh hơn, hãy vào `Play > Advanced Settings...`. Tại đây, chúng ta có thể
       thiết
       lập:
        * `Number of Players`: Đặt là 2 hoặc hơn.
        * `Multiplayer Options > Run Dedicated Server`: Đánh dấu tick vào đây.
        * Khi chúng ta nhấn Play, Unreal Editor sẽ tự động khởi chạy một server và nhiều client, cho phép chúng ta đặt
          breakpoint
          và debug trực tiếp.

* **Tại sao lại làm như thế?**
    * **Vòng lặp nhanh (Fast Iteration):** Việc chạy và kiểm thử cục bộ cho phép chúng ta tìm và sửa lỗi logic
      gameplay (
      replication, RPCs...) trong vài phút, thay vì mất hàng giờ để build, tải lên server đám mây rồi mới phát hiện lỗi.
    * **Tách biệt vấn đề:** Nếu game chạy đúng ở local nhưng lỗi khi lên server, chúng ta biết vấn đề nằm ở môi trường
      mạng
      hoặc cấu hình máy chủ, chứ không phải ở code gameplay.

* **Tip & Trick cần thiết:**
    * **Giả lập mạng kém:** Mạng ở local luôn hoàn hảo. Để kiểm thử game trong điều kiện thực tế, hãy sử dụng các lệnh
      console trong client để giả lập lag và mất gói tin:
        * `Net PktLag=[miligiây]`: Ví dụ `Net PktLag=100` để giả lập ping 100ms.
        * `Net PktLoss=[phần trăm]`: Ví dụ `Net PktLoss=5` để giả lập mất 5% gói tin.
        * Đây là bước **bắt buộc** để tìm ra các lỗi liên quan đến dự đoán chuyển động (movement prediction) và các sự
          kiện không đáng tin cậy (unreliable RPCs).
    * **Sử dụng nhiều file `.bat`:** Tạo nhiều file `StartClient_Player2.bat`, `StartClient_Player3.bat`... để nhanh
      chóng mở nhiều client cho một lần test.

#### 2. Tối ưu hóa hiệu năng: Đảm bảo công trình Lego không bị sập khi có nhiều người chơi

* **Network Relevancy and Culling: "Chỉ gửi những viên Lego mà người chơi nhìn thấy"**
    * **Nó là gì?** Theo mặc định, Unreal sẽ không gửi cập nhật về một Actor cho một người chơi nếu Actor đó ở quá xa.
      Đây là "Distance-based culling". Tuy nhiên, chúng ta có thể kiểm soát nó một cách tinh vi hơn.
    * **Làm như thế nào?** Override hàm `bool AActor::IsNetRelevantFor(const FRealViewer& RealViewer)` trong Actor của
      chúng ta.
    * **Tại sao nó quan trọng?** Tưởng tượng một game battle royale 100 người. Nếu server gửi thông tin của tất cả 99
      người chơi khác đến máy chúng ta, băng thông sẽ tắc nghẽn ngay lập tức. Bằng cách chỉ gửi thông tin của 5-10 người
      chơi
      gần nhất, chúng ta đã giảm tải mạng xuống hơn 90%.
    * **Tip & Trick:**
        * **Ví dụ thực tế:** Một đơn vị tàng hình (invisible). Hàm `IsNetRelevantFor` của nó nên được viết để trả về
          `false` đối với những người chơi thuộc đội đối phương (trừ khi họ có item phát hiện tàng hình).
        * **Tần suất cập nhật:** Tinh chỉnh biến `NetUpdateFrequency` của Actor. Các Actor di chuyển nhanh (như nhân
          vật) cần tần suất cao (ví dụ: 100 lần/giây), nhưng các Actor tĩnh hoặc ít thay đổi (như một cánh cửa) chỉ cần
          tần suất rất thấp (ví dụ: 2 lần/giây).

* **Server-side Optimization: "Thợ Cả không cần quan tâm đến màu sắc"**
    * **Nó là gì?** Server chỉ quan tâm đến logic: Vị trí ở đâu? Máu bao nhiêu? Có bắn trúng không? Nó không cần biết
      hiệu ứng cháy nổ trông đẹp thế nào, hay âm thanh nghe ra sao.
    * **Làm như thế nào?** Sử dụng preprocessor directive một cách triệt để.
      ```cpp
      void AMyProjectile::OnHit(...)
      {
          // Logic gây sát thương luôn chạy
          ApplyDamage(...);

          // Logic hiệu ứng chỉ chạy trên các máy có thể "thấy" được
          #if !UE_SERVER
              UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation());
              UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
          #endif
      }
      ```
    * **Tại sao?** Mỗi chu kỳ CPU được tiết kiệm trên server có nghĩa là server có thể xử lý nhiều người chơi hơn, hoặc
      chạy các thuật toán AI phức tạp hơn. Việc loại bỏ các tác vụ đồ họa/âm thanh là cách tiết kiệm tài nguyên hiệu quả
      nhất.

* **Profiling: "Dùng kính lúp soi từng viên gạch"**
    * **Nó là gì?** Là quá trình đo lường để tìm ra chính xác phần nào của game đang gây chậm.
    * **Làm như thế nào?**
        * **Unreal Insights:** Đây là công cụ mạnh nhất. chúng ta chạy nó song song với game, nó sẽ ghi lại mọi thứ, từ
          thời
          gian thực thi của từng hàm C++, dữ liệu mạng được gửi đi, đến việc tải assets.
        * **Lệnh Console:**
            * `Stat Unit`: Hiển thị thời gian xử lý của Frame, Game Thread, Render Thread, GPU. Trên server, Game Thread
              là quan trọng nhất.
            * `Stat Net`: Hiển thị chi tiết thông tin mạng đang được gửi/nhận.
    * **Tại sao?** "Phỏng đoán là nguồn gốc của mọi sai lầm trong tối ưu hóa". Đừng đoán xem cái gì đang chậm, hãy đo
      lường nó. chúng ta có thể ngạc nhiên khi thấy một hàm tưởng chừng vô hại lại đang chiếm 30% tài nguyên CPU của
      server.

---

### Giai đoạn 4: Triển khai và Vận hành - Đóng hộp và vận chuyển Lego đi khắp thế giới

Đây là giai đoạn biến game của chúng ta từ một sản phẩm chạy trên máy cá nhân thành một dịch vụ toàn cầu.

#### 1. Chuẩn bị cho việc triển khai: Xây dựng dây chuyền sản xuất

* **Cross-Compilation (Biên dịch chéo cho Linux):**
    * **Nó là gì?** Sử dụng Visual Studio trên Windows để tạo ra một file thực thi có thể chạy trên Linux.
    * **Làm như thế nào?**
        1. Mở Epic Games Launcher, tìm đến phiên bản Engine của chúng ta, chọn `Options`.
        2. Trong `Target Platforms`, tick vào `Linux`. Launcher sẽ tải về bộ công cụ (toolchain) cần thiết.
        3. Sau khi cài xong, chúng ta chỉ cần mở Visual Studio, chọn cấu hình `Development Server` và nền tảng `Linux`
           để
           biên dịch.
    * **Tại sao?** Chi phí thuê máy chủ Windows thường cao hơn đáng kể so với máy chủ Linux do chi phí bản quyền. Linux
      cũng được coi là ổn định và hiệu quả hơn cho các tác vụ máy chủ. Hầu hết các nhà cung cấp dịch vụ game server đều
      chạy trên Linux.

* **Containerization (Đóng gói bằng Docker): "Đóng gói Lego vào hộp tiêu chuẩn"**
    * **Nó là gì?** Docker gói ứng dụng server của chúng ta và tất cả những gì nó cần để chạy (thư viện, file cấu
      hình...)
      vào một "container" duy nhất. Container này giống như một máy ảo siêu nhẹ.
    * **Làm như thế nào?** Tạo một file tên là `Dockerfile` trong thư mục dự án của chúng ta.
      ```dockerfile
      # Sử dụng một ảnh Linux cơ bản
      FROM ubuntu:20.04

      # Đặt thư mục làm việc bên trong container
      WORKDIR /app

      # Sao chép thư mục server đã được đóng gói từ máy của chúng ta vào container
      COPY ./LinuxServer ./

      # Mở cổng 7777 cho kết nối game (giao thức UDP)
      EXPOSE 7777/udp

      # Lệnh sẽ được chạy khi container khởi động
      # Thêm cờ -nohomedir để tránh lỗi ghi log vào thư mục không tồn tại
      CMD ["./MyGame/Binaries/Linux/MyGameServer-Linux-Shipping", "MyMap?listen", "-log", "-nohomedir"]
      ```
    * **Tại sao?**
        * **Tính nhất quán:** Container đảm bảo server của chúng ta sẽ chạy y hệt trên máy của dev, trên server thử
          nghiệm,
          và trên server sản phẩm. Loại bỏ hoàn toàn lỗi "Ủa, trên máy em chạy được mà!".
        * **Khả năng mở rộng:** Các công cụ quản lý đám mây như Kubernetes và Agones được thiết kế để làm việc với
          container. Chúng có thể tự động tạo ra, khởi động, và xóa đi hàng trăm container server của chúng ta dựa trên
          nhu
          cầu của người chơi.

#### 2. Triển khai lên các nền tảng đám mây: Chọn nhà kho và hệ thống vận chuyển

* **Các lựa chọn:**
    * **Cơ bản (IaaS - Infrastructure as a Service):** Thuê máy ảo (VM) trên AWS EC2, Google Cloud Compute Engine, Azure
      VMs. chúng ta sẽ phải tự cài đặt mọi thứ, tự quản lý việc khởi động server, cập nhật phiên bản. Phù hợp để bắt đầu
      và
      học hỏi.
    * **Nâng cao (PaaS - Platform as a Service / Fleet Management):** Sử dụng các dịch vụ chuyên dụng như **AWS Gamelift
      ** hoặc **Google Cloud Agones** (trên nền Kubernetes).
* **Tại sao nên dùng Fleet Management?**
    * chúng ta chỉ cần đưa cho chúng container server của mình.
    * Chúng sẽ tự động:
        * **Tự động co giãn (Autoscaling):** Khi có nhiều người chơi, chúng tự tạo thêm server. Khi người chơi nghỉ,
          chúng tự động xóa bớt để tiết kiệm tiền.
        * **Phân phối toàn cầu:** Đặt server ở gần người chơi (ví dụ: người chơi ở châu Âu sẽ kết nối tới server ở
          Frankfurt) để giảm ping.
        * **Kiểm tra sức khỏe (Health Checks):** Tự động khởi động lại các server bị treo.
* **Tip & Trick:** Đừng cố tự xây dựng một hệ thống quản lý server (fleet management). Đó là một công việc cực kỳ phức
  tạp. Hãy tận dụng các giải pháp đã có sẵn như Agones (mã nguồn mở) hoặc Gamelift.

#### 3. Tích hợp và Deploy lên Store (Ví dụ: Steam): Đưa sản phẩm lên kệ

* **Cấu hình App và Depot: "Hai hộp sản phẩm khác nhau"**
    * chúng ta cần liên hệ với Valve để yêu cầu tạo **hai App ID**:
        1. **App ID chính (Type: Game):** Đây là game mà người chơi sẽ mua.
        2. **App ID phụ (Type: Tool):** Đây là "Dedicated Server" của chúng ta, nó sẽ miễn phí.
    * Trong mỗi App, chúng ta tạo "Depot" (kho chứa file):
        * Depot của game sẽ chứa các bản build cho Windows, Mac, Linux của **client**.
        * Depot của server sẽ chỉ chứa bản build **Linux server**.
    * **Tại sao?** Điều này cho phép người chơi chỉ tải về client (khoảng 20GB), và những người muốn host server (hoặc
      chính chúng ta) chỉ cần tải về server (khoảng 500MB).

* **SteamPipe: "Cỗ máy đóng gói và dán nhãn"**
    * Nó là một công cụ dòng lệnh để tải các file build của chúng ta lên các depot tương ứng.
    * chúng ta sẽ tạo các file kịch bản `.vdf` để chỉ định "thư mục build client" sẽ được tải lên "depot client", và "
      thư mục
      build server" sẽ được tải lên "depot server".
    * **Tip & Trick (Quan trọng):** Sử dụng các nhánh (branch) khác nhau trong Steamworks.
        * **`development` branch:** Tải các bản build hàng ngày lên đây để team nội bộ và QA kiểm thử.
        * **`public` branch (mặc định):** Chỉ khi bản build ở nhánh `development` đã ổn định, chúng ta mới "promote" nó
          sang
          nhánh này cho tất cả người chơi. Đây là quy trình phát hành an toàn, tránh việc đẩy một bản build lỗi ra cho
          toàn bộ cộng đồng.

* **Kịch bản cài đặt (Install Script):**
    * Trong cấu hình depot của server trên Steamworks, chúng ta cần thêm một kịch bản cài đặt.
    * Nó sẽ chạy khi ai đó tải server của chúng ta qua SteamCMD, đảm bảo các thư viện cần thiết trên Linux được cài đặt.

---

### **Giai đoạn 5: Vận hành Chuyên nghiệp - Dây chuyền lắp ráp tự động và Trung tâm điều khiển**

Ra mắt game mới chỉ là sự khởi đầu. Để vận hành một game online thành công, chúng ta cần các quy trình tự động hóa và
các
công cụ giám sát mạnh mẽ.

#### **1. CI/CD (Continuous Integration/Continuous Deployment): Dây chuyền lắp ráp tự động**

* **Nó là gì?** CI/CD là một phương pháp mà mỗi khi một lập trình viên đẩy code lên một nhánh nhất định (ví dụ:
  `develop`), một hệ thống tự động sẽ:
    1. **CI (Tích hợp liên tục):** Lấy code mới về, biên dịch game, chạy các bài test tự động để đảm bảo không có lỗi
       nào mới được thêm vào.
    2. **CD (Triển khai liên tục):** Nếu CI thành công, nó sẽ tự động đóng gói (package) bản build Client và Server, sau
       đó tải chúng lên Steam (vào nhánh `development`) và triển khai container server mới lên môi trường thử nghiệm (
       staging environment) trên đám mây.
* **Tại sao nó là chân lý?**
    * **Loại bỏ sai sót con người:** Không còn cảnh "quên build server" hay "đóng gói nhầm phiên bản". Mọi thứ đều tự
      động và nhất quán.
    * **Tăng tốc độ phát triển:** Team có thể có bản build mới để test mỗi ngày, thậm chí mỗi giờ, thay vì chờ đợi một
      người build thủ công.
    * **An toàn:** Các bài test tự động hoạt động như một lớp bảo vệ, ngăn chặn lỗi nghiêm trọng được đưa vào game.
* **Làm như thế nào?**
    * Sử dụng các dịch vụ như **GitHub Actions** (tích hợp sẵn với GitHub), **Jenkins**, hoặc **GitLab CI/CD**.
    * chúng ta sẽ viết các file kịch bản (ví dụ `.yml` cho GitHub Actions) để định nghĩa các bước trên.

#### **2. Backend Services và Matchmaking: Bộ não đằng sau cuộc vui**

* **Game Server vs. Backend Server:**
    * **Game Server (Dedicated Server):** Xử lý logic game thời gian thực (real-time). Vòng đời của nó ngắn, chỉ tồn tại
      trong một trận đấu.
    * **Backend Services:** Một tập hợp các dịch vụ chạy 24/7, không xử lý real-time. Chúng là nơi lưu trữ dữ liệu bền
      vững và xử lý các logic meta-game.
* **Các dịch vụ Backend cốt lõi:**
    * **Identity/Account Service:** Quản lý tài khoản, đăng nhập, liên kết với Steam/Epic.
    * **Player Data Storage:** Lưu trữ kho đồ, tiền tệ, tiến trình của người chơi trong một cơ sở dữ liệu (Database).
    * **Matchmaking Service:** Đây là dịch vụ "mai mối". Người chơi gửi yêu cầu "tìm trận" đến đây. Matchmaker sẽ dựa
      trên các quy tắc (ping, rank/skill, chế độ chơi) để tập hợp một nhóm người chơi và ra lệnh cho hệ thống Fleet
      Management (như Agones) khởi động một Game Server mới cho họ.
* **Tip & Trick:**
    * Đừng cố tự xây dựng tất cả từ đầu. Hãy xem xét các giải pháp "Backend-as-a-Service" (BaaS) như **AccelByte**, *
      *Pragma Platform**, hoặc **Amazon GameSparks**. Chúng cung cấp sẵn các dịch vụ trên.
    * Đối với matchmaking, **Open Match** của Google là một framework mã nguồn mở mạnh mẽ.

#### **3. Ghi log, Giám sát và Vận hành trực tiếp (Live Ops): Trung tâm điều khiển**

* **Structured Logging (Ghi log có cấu trúc):**
    * Đừng chỉ ghi log dưới dạng text thông thường: `Player John connected.`.
    * Hãy ghi log dưới dạng JSON:
      `{"timestamp": "...", "event": "PlayerConnect", "playerId": "12345", "playerName": "John", "serverIp": "..."}`.
    * **Tại sao?** Log có cấu trúc có thể được tự động thu thập và đưa vào các hệ thống phân tích. chúng ta có thể dễ
      dàng
      truy vấn: "Cho tôi xem tất cả các sự kiện `PlayerConnect` trong 24 giờ qua" hoặc "Vẽ biểu đồ số lượng người chơi
      trên từng server".

* **Monitoring & Alerting (Giám sát & Cảnh báo):**
    * Sử dụng các công cụ như **Prometheus** (thu thập số liệu), **Grafana** (vẽ biểu đồ), và **Alertmanager**.
    * Thiết lập các **Dashboard** để theo dõi sức khỏe của toàn bộ hệ thống theo thời gian thực:
        * Số lượng người chơi đang online (CCU).
        * Số lượng server đang hoạt động.
        * CPU/RAM trung bình của các server.
        * Thời gian tìm trận trung bình.
    * Thiết lập các **Cảnh báo (Alerts)** tự động gửi tin nhắn vào Slack/Email cho team khi có sự cố: "Cảnh báo: Server
      X có CPU đạt 95% trong 5 phút!" hoặc "Số lượng server bị crash tăng đột biến!".

* **Live Ops:**
    * Đây là tư duy vận hành game như một dịch vụ. Team của chúng ta có thể:
        * Theo dõi các chỉ số để hiểu hành vi người chơi.
        * Phát hiện và xử lý sự cố nhanh chóng nhờ hệ thống giám sát.
        * Bật/tắt các sự kiện trong game, thay đổi các thông số (ví dụ: sát thương của một khẩu súng) mà không cần
          deploy bản build mới, thông qua một hệ thống quản lý từ xa.

Chắc chắn rồi! Để biến tài liệu này thành một cẩm nang "từ A đến Á" thực thụ, việc bổ sung các "Tip & Trick" chuyên sâu,
được đúc kết từ kinh nghiệm thực chiến của các nhà phát triển đi trước, là vô cùng cần thiết.

Dưới đây là một danh sách các mẹo và thủ thuật quan trọng, được sắp xếp theo từng giai đoạn, mà chúng ta nên thêm vào
tài
liệu của mình.

---

### **Các Tip & Trick Nâng cao cho Team Chuyên nghiệp**

Những kỹ thuật này là sự khác biệt giữa một game multiplayer "chạy được" và một game multiplayer "chạy tốt" - ổn định,
dễ bảo trì, và có khả năng mở rộng.

#### **I. Mẹo về Kiến trúc & Thiết kế (Giai đoạn 1 & 2)**

##### **1. "Gầy" Character, "Béo" Component (Lean Character, Fat Components)**

* **Vấn đề:** Rất nhiều người mới bắt đầu có xu hướng nhồi nhét tất cả logic vào lớp `ACharacter`: logic túi đồ, hệ
  thống kỹ năng, quản lý máu, năng lượng... Điều này tạo ra một "God Class" khổng lồ, khó đọc, khó bảo trì và không thể
  tái sử dụng.
* **Giải pháp:** Áp dụng triệt để kiến trúc Component.
    * Tạo các `UActorComponent` riêng biệt cho từng hệ thống: `UInventoryComponent`, `UHealthComponent`,
      `UAbilityComponent`...
    * Trong `ACharacter`, chỉ cần thêm các component này vào. `ACharacter` chỉ nên chịu trách nhiệm về những gì cốt lõi
      của một nhân vật: di chuyển và hoạt hình.
* **Tại sao lại là "Trick" hay?**
    * **Tính Module:** chúng ta có thể dễ dàng thêm hoặc bớt `UInventoryComponent` cho bất kỳ Actor nào (ví dụ: một cái
      rương).
    * **Dễ Test:** chúng ta có thể kiểm thử `UHealthComponent` một cách độc lập mà không cần tạo ra cả một `ACharacter`
      hoàn
      chỉnh.
    * **Làm việc nhóm:** Team A làm `InventoryComponent`, team B làm `AbilityComponent` mà không sợ đụng code của nhau.

##### **2. Sử dụng Interfaces cho Tương tác (Interfaces for Interaction)**

* **Vấn đề:** Làm thế nào để nhân vật có thể tương tác với nhiều loại vật thể khác nhau (cửa, item, NPC) mà không cần
  phải kiểm tra kiểu (`cast`) liên tục?
* **Giải pháp:** Tạo một C++ Interface, ví dụ `UInteractableInterface`.
    * Interface này định nghĩa các hàm như `OnInteract(AController* Interactor)` và `GetInteractionText()`.
    * Bất kỳ Actor nào (cửa, item...) muốn có thể tương tác được chỉ cần implement Interface này.
    * Khi nhân vật nhìn vào một vật thể, họ chỉ cần kiểm tra: `if (LookAtActor->Implements<UInteractableInterface>())`,
      sau đó gọi hàm `IInteractableInterface::Execute_OnInteract(LookAtActor, GetController());` mà không cần quan tâm
      Actor đó là loại gì.
* **Tại sao lại là "Trick" hay?** Nó giúp **tách biệt hoàn toàn (decoupling)** logic tương tác khỏi các lớp cụ thể, giúp
  code của chúng ta sạch sẽ, linh hoạt và cực kỳ dễ mở rộng.

##### **3. Data-Driven Design với Data Assets và Curve Tables**

* **Vấn đề:** Các thông số game (sát thương súng, máu của quái vật, lượng kinh nghiệm cần để lên cấp) bị hard-code trong
  C++ hoặc trong từng Blueprint riêng lẻ, khiến việc cân bằng game trở thành một cơn ác mộng.
* **Giải pháp:**
    * **`UDataAsset`:** Tạo một lớp C++ kế thừa từ `UDataAsset`, ví dụ `UWeaponDataAsset`. Lớp này chứa tất cả các thuộc
      tính của một vũ khí (sát thương, tốc độ bắn, số đạn...). Sau đó, trong Editor, các designer có thể tạo ra vô số
      asset `DA_AK47`, `DA_Pistol`... từ lớp này. `ABaseWeapon` của chúng ta chỉ cần một con trỏ `UPROPERTY` đến
      `UWeaponDataAsset` là có thể lấy toàn bộ thông tin.
    * **`UCurveTable`:** Sử dụng Curve Table để định nghĩa sự phát triển của các chỉ số theo cấp độ. Ví dụ, tạo một
      Curve Table cho "HealthPerLevel". Cấp 1 có 100 máu, cấp 2 có 110 máu... Code C++ chỉ cần đọc giá trị từ bảng này
      dựa trên cấp độ hiện tại.
* **Tại sao lại là "Trick" hay?** Nó chuyển quyền cân bằng game từ lập trình viên sang cho game designer. Họ có thể tinh
  chỉnh hàng trăm vật phẩm mà không cần một dòng code nào, tăng tốc độ phát triển và thử nghiệm lên gấp nhiều lần.

#### **II. Mẹo về Workflow & Debugging (Giai đoạn 2 & 3)**

##### **1. Bậc thầy Debug Mạng (Network Debugging Mastery)**

* Việc debug mạng không chỉ là `print string`. Hãy sử dụng các công cụ chuyên dụng:
    * **Các lệnh Console cốt lõi:**
        * `showdebug abilitysystem`: Hiển thị thông tin debug của GAS ngay trên màn hình.
        * `showdebug animation`: Hiển thị trạng thái của Animation Blueprint.
        * `Net PktLag=[ms], Net PktLoss=[%], Net PktOrder=[0|1]`: Các lệnh "thần thánh" để giả lập môi trường mạng xấu
          ngay trong Editor.
    * **Network Profiler:** Mở `Unreal Insights` (`Shift+Alt+I`) và chạy game. Tab "Networking" sẽ cho chúng ta thấy
      chính
      xác từng packet được gửi đi, chứa thông tin gì, thuộc tính nào được replicate, RPC nào được gọi, và chúng tốn bao
      nhiêu băng thông. Đây là công cụ tối thượng để tìm ra nguyên nhân gây lag.

##### **2. Tận dụng Gameplay Debugger**

* **Vấn đề:** Làm sao để xem trạng thái của một Actor (đặc biệt là AI) một cách nhanh chóng mà không cần đặt breakpoint?
* **Giải pháp:** Bật Gameplay Debugger (`Project Settings > Gameplay Debugger > Bật `). Trong game, nhấn phím
  `Apostrophe (')`. Một giao diện sẽ hiện ra, cung cấp vô số thông tin về Actor chúng ta đang nhìn vào: trạng thái
  Behavior
  Tree của AI, các Gameplay Tag đang active, thông tin của Ability System...
* **Tại sao lại là "Trick" hay?** Nó cho phép chúng ta debug "sống" (live) mà không cần dừng game, cực kỳ hữu ích khi
  debug
  các hệ thống phức tạp như AI và GAS.

##### **3. Hot Reload và Live Coding**

* Đừng tắt Editor mỗi khi chúng ta thay đổi một dòng code C++.
    * **Hot Reload (Cũ):** Nhấn `Ctrl+Alt+F11` trong Editor. Nó sẽ biên dịch lại code và "vá" vào phiên Editor đang
      chạy.
    * **Live Coding (Mới & Tốt hơn):** Nhấn `Ctrl+Alt+F11`. Live Coding là một hệ thống mới hơn, ổn định và nhanh hơn
      Hot Reload. Hãy đảm bảo nó được bật trong `Editor Preferences > Live Coding`.
* **Tại sao lại là "Trick" hay?** Nó giảm thời gian chờ đợi biên dịch từ vài phút xuống còn vài giây, giúp chúng ta duy
  trì
  dòng chảy công việc và tăng năng suất lập trình C++ lên đáng kể.

#### **III. Mẹo về Tối ưu hóa Mạng (Giai đoạn 3 & 4)**

##### **1. `AActor::NetDormancy` - "Ru ngủ" các Actor**

* **Vấn đề:** Có những Actor ít khi thay đổi (ví dụ: một item nằm trên đất). Việc server liên tục kiểm tra và gửi cập
  nhật (dù không có gì thay đổi) cho chúng là một sự lãng phí.
* **Giải pháp:** Sử dụng `NetDormancy`. chúng ta có thể đặt trạng thái của một Actor thành "ngủ đông". Khi ở trạng thái
  này,
  server sẽ hoàn toàn không gửi bất kỳ cập nhật nào về nó nữa cho đến khi có một sự kiện "đánh thức" nó dậy (ví dụ: một
  người chơi đi lại gần).
* **Tại sao lại là "Trick" hay?** Trong một map có hàng ngàn vật thể, việc "ru ngủ" 90% trong số chúng có thể giảm tải
  băng thông của server một cách đáng kinh ngạc.

##### **2. Nén dữ liệu trong RPCs (RPC Data Quantization)**

* **Vấn đề:** Gửi các RPC với nhiều tham số `FVector` hoặc `FRotator` (mỗi `float` là 4 byte) có thể rất tốn băng thông.
* **Giải pháp:** Nén dữ liệu (Quantization). Thay vì gửi một giá trị `float` đầy đủ, hãy gửi một phiên bản nén của nó.
    * **Vị trí:** chúng ta có thực sự cần độ chính xác đến 6 chữ số thập phân? Có thể chúng ta chỉ cần gửi 3 số nguyên
      `int16` và
      tính toán lại ở phía nhận.
    * **Hướng (Rotation):** Một `FRotator` tốn 12 byte. Nhưng chúng ta có thể nén nó xuống chỉ còn 2-4 byte bằng cách
      chỉ gửi
      1 hoặc 2 thành phần và tính toán lại phần còn lại, hoặc sử dụng các kỹ thuật nén Quaternion.
* **Tại sao lại là "Trick" hay?** Đây là một kỹ thuật tối ưu hóa nâng cao, giúp giảm kích thước của các RPC quan trọng (
  như bắn súng), cho phép server xử lý nhiều hành động hơn mỗi giây.

##### **3. `FNetSerialize` và `Fast TArray Replication`**

* **Vấn đề:** Làm sao để replicate một `TArray` chứa các `UObject*` hoặc các `struct` phức tạp một cách hiệu quả?
* **Giải pháp:**
    * **`Fast TArray Replication`:** Đối với `TArray` chứa các `struct`, hãy sử dụng
      `TArray<FMyStruct, TFastArrayAllocator<FMyStruct>>`. Bằng cách implement `FNetDeltaSerialize` cho `struct` của
      chúng ta, hệ thống replication sẽ chỉ gửi những phần tử đã thay đổi trong mảng, thay vì gửi lại toàn bộ mảng mỗi
      khi có
      thay đổi.
    * **`FNetSerialize`:** Đối với các `struct` phức tạp, hãy implement hàm `NetSerialize` cho chúng. Hàm này cho chúng
      ta
      toàn quyền kiểm soát việc đọc/ghi dữ liệu vào luồng packet, cho phép chúng ta áp dụng các kỹ thuật nén tùy chỉnh.
* **Tại sao lại là "Trick" hay?** Đây là cấp độ kiểm soát cao nhất đối với việc tuần tự hóa dữ liệu mạng, cần thiết cho
  các hệ thống như túi đồ hoặc danh sách hiệu ứng trạng thái.