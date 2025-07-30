+++
title = "Chào Thế Giới: Hướng Dẫn Viết Bài Blog Markdown Đầu Tiên"
date = "2025-07-30T10:13:42+07:00"
draft = true 
author = "Bob Nguyen"
tags = ["hướng dẫn", "markdown", "hugo"]
categories = ["Kỹ Thuật"]
summary = "Một bài viết mẫu chi tiết, hướng dẫn bạn cách sử dụng các yếu tố Markdown phổ biến nhất để tạo ra một bài blog đẹp mắt và đầy đủ thông tin."
# Nhiều theme hỗ trợ ảnh bìa, bạn có thể thêm URL ảnh ở đây
# cover = "https://picsum.photos/1200/630" 
+++

Chào mừng đến với blog mới của tôi! Đây là bài viết đầu tiên, và tôi muốn nhân cơ hội này để trình bày một vài yếu tố định dạng phổ biến mà bạn có thể sử dụng khi viết bài bằng Markdown cho blog Hugo của mình.

## Các Yếu Tố Markdown Cơ Bản

Markdown được tạo ra để giúp việc viết nội dung cho web trở nên dễ dàng. Dưới đây là một vài ví dụ cơ bản:

* Bạn có thể làm cho chữ **in đậm** bằng cách dùng hai dấu sao.
* Hoặc làm cho chữ *in nghiêng* bằng một dấu sao.
* Tạo danh sách không có thứ tự:
    * Mục 1
    * Mục 2
    * Mục con
* Tạo danh sách có thứ tự:
    1.  Bước đầu tiên
    2.  Bước thứ hai
    3.  Bước cuối cùng

> Đây là một khối trích dẫn (blockquote). Rất hữu ích khi bạn muốn trích dẫn lời nói của ai đó hoặc nhấn mạnh một đoạn văn bản quan trọng.

---

## Hiển Thị Code (Code Block)

Là một lập trình viên, hiển thị các đoạn mã một cách rõ ràng là điều không thể thiếu. Markdown hỗ trợ làm điều này rất tốt.

**Source Ref (Trích dẫn nguồn):** Đoạn mã JavaScript dưới đây là một ví dụ về cách sử dụng `async/await` để lấy dữ liệu từ một API. Bạn có thể tham khảo thêm tài liệu về JavaScript tại [MDN Web Docs](https://developer.mozilla.org/en-US/docs/Web/JavaScript).

```javascript
// Đây là một comment (bình luận) trong code block.
async function fetchData(url) {
  try {
    // 1. Gửi yêu cầu đến URL bằng fetch
    const response = await fetch(url);

    // 2. Kiểm tra xem yêu cầu có thành công không (status code 200-299)
    if (!response.ok) {
      throw new Error(`Lỗi HTTP! Trạng thái: ${response.status}`);
    }

    // 3. Chuyển đổi phản hồi sang định dạng JSON
    const data = await response.json();
    
    // 4. In dữ liệu ra console và trả về
    console.log("Dữ liệu đã nhận:", data);
    return data;

  } catch (error) {
    // Xử lý nếu có lỗi xảy ra trong quá trình fetch
    console.error("Không thể fetch dữ liệu:", error);
  }
}
```
// Gọi hàm với một API JSON mẫu
fetchData('[https://jsonplaceholder.typicode.com/todos/1](https://jsonplaceholder.typicode.com/todos/1)');

## Chèn Hình Ảnh và GIF

Một hình ảnh đáng giá ngàn lời nói, và một ảnh GIF đôi khi còn hơn thế nữa!

### Image (Hình ảnh tĩnh)

Dưới đây là ví dụ chèn một hình ảnh từ trang web `picsum.photos`.

![Bàn làm việc của một lập trình viên với laptop và ly cà phê](https://picsum.photos/seed/hugo/800/400)
*Chú thích: Hình ảnh mẫu từ Picsum Photos.*

### GIF (Ảnh động)

Chèn GIF cũng tương tự như chèn ảnh. Chúng ta sẽ lấy một ảnh từ GIPHY.

![GIF một chú chó vui mừng ngồi trước máy tính](https://media.giphy.com/media/slOhiKAVFgwr6/giphy.gif)
*Chú thích: Thể hiện cảm xúc khi code chạy thành công!*