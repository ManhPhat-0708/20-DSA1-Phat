
# TUẦN 3 - DSA - HK251 - HIỆN THỰC BTL1 HK243


## Biên dịch

### **Lệnh biên dịch bình thường**
```bash
g++ -std=c++17 -o main main.cpp -I. -Isrc -Ihelpers src/*.cpp tests/*.cpp
```
- `-std=c++17` — Sử dụng tiêu chuẩn C++17.
- `-o main` — Tạo file thực thi `main`.
- `-I. -Isrc -Ihelpers` — Thêm đường dẫn include hiện tại, `src/`, `helpers/`.
- `src/*.cpp` — Tất cả file `.cpp` trong thư mục `src`.
- `tests/*.cpp` — Tất cả file `.cpp` trong thư mục `tests`.

---

### **Lệnh biên dịch với AddressSanitizer (phát hiện lỗi bộ nhớ)**
```bash
g++ -std=c++17 -fsanitize=address -fno-omit-frame-pointer -g -O0 -Wall -Wextra -o main  main.cpp -I. -Isrc -Ihepler src/*.cpp tests/*.cpp
```
**Giải thích thêm:**
- `-fsanitize=address` — Bật AddressSanitizer để phát hiện lỗi như tràn bộ nhớ, use-after-free...
- `-fno-omit-frame-pointer` — Giữ thông tin stack trace để debug dễ hơn.
- `-g` — Thêm thông tin debug.
- `-O0` — Tắt tối ưu hóa để debug chính xác.
- `-Wall -Wextra` — Bật tất cả cảnh báo quan trọng.

## HistoryManager – Quản lý lịch sử thao tác (Undo/Redo) trong `TextBuffer`

### 1. Mục đích
`HistoryManager` là lớp con của `TextBuffer`, dùng để **lưu trữ và quản lý lịch sử các hành động chỉnh sửa văn bản**, hỗ trợ các thao tác **Undo** và **Redo**.

### 2. Cấu trúc `Action`
```cpp
struct Action {
    string actionName;   // Tên hành động (ví dụ: "insert", "remove")
    int cursorBefore;    // Vị trí con trỏ trước khi thực hiện hành động
    int cursorAfter;     // Vị trí con trỏ sau khi thực hiện hành động
    char data;           // Ký tự liên quan đến hành động

    bool operator==(const Action &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Action &a);
};
```
**Chức năng đặc biệt**:
- `operator==`: So sánh hai hành động.
- `operator<<`: In hành động ra dạng `(actionName, cursorBefore, cursorAfter, data)`.

### 3. Thuộc tính chính
- `DoublyLinkedList<Action> actions` – Danh sách liên kết kép lưu các hành động.
- `int currentIndex` – Chỉ số hành động hiện tại trong lịch sử (dùng để quản lý undo/redo).

### 4. Các phương thức
| Phương thức | Chức năng |
|-------------|-----------|
| `HistoryManager()` | Khởi tạo `currentIndex = 0`. |
| `~HistoryManager()` | Destructor mặc định. |
| `void addAction(const Action &a)` | Thêm hành động mới. Nếu là `"insert"` hoặc `"remove"` thì xóa các hành động sau `currentIndex` để giữ lịch sử nhất quán. |
| `bool canUndo() const` | Trả về `true` nếu có thể **Undo**. |
| `bool canRedo() const` | Trả về `true` nếu có thể **Redo**. |
| `void printHistory() const` | In ra các hành động từ đầu đến `currentIndex`, dùng `operator<<` của `Action`. |

### 5. Ví dụ sử dụng
```cpp
TextBuffer::HistoryManager history;

TextBuffer::HistoryManager::Action a{"insert", 0, 1, 'A'};
history.addAction(a);

TextBuffer::HistoryManager::Action b{"remove", 1, 0, 'B'};
history.addAction(b);

history.printHistory();
// Output: [(insert, 0, 1, A), (remove, 1, 0, B)]

if (history.canUndo()) {
    std::cout << "Có thể undo\n";
}
if (!history.canRedo()) {
    std::cout << "Không thể redo\n";
}
```

### 6. Ghi chú
- `currentIndex` đóng vai trò **con trỏ thời gian** trong lịch sử thao tác:
  - **Undo**: Giảm `currentIndex`.
  - **Redo**: Tăng `currentIndex`.
- Khi thêm/xóa hành động mới sau khi đã undo, toàn bộ hành động phía sau `currentIndex` sẽ bị xóa để tránh xung đột lịch sử.



## TextBuffer – Bộ đệm văn bản với hỗ trợ Undo/Redo

### 1. Mục đích
`TextBuffer` là lớp chính quản lý **nội dung văn bản** và **vị trí con trỏ**, đồng thời tích hợp **`HistoryManager`** để hỗ trợ **Undo** và **Redo**.  
Lớp này cung cấp các thao tác chỉnh sửa như **chèn**, **xóa**, **di chuyển con trỏ**, **tìm kiếm**, và **sắp xếp**.

---

### 2. Cấu trúc dữ liệu chính
```cpp
class TextBuffer {
private:
    DoublyLinkedList<char> buffer;   // Lưu trữ các ký tự của văn bản
    int cursorPos;                   // Vị trí con trỏ hiện tại (0-based)
    HistoryManager *history;         // Quản lý lịch sử thao tác
};
```

---

### 3. Thuộc tính
- `buffer` — Danh sách liên kết kép lưu từng ký tự.
- `cursorPos` — Vị trí con trỏ, có thể nằm từ 0 (đầu) đến `buffer.size()` (cuối).
- `history` — Con trỏ tới `HistoryManager` để lưu các thao tác Undo/Redo.

---

### 4. Các phương thức

| Phương thức | Chức năng |
|-------------|-----------|
| **Khởi tạo/Hủy** |
| `TextBuffer()` | Khởi tạo buffer rỗng, con trỏ ở vị trí 0. |
| `~TextBuffer()` | Giải phóng bộ nhớ `history`. |
| **Chèn/Xóa** |
| `void insert(char c)` | Chèn ký tự `c` tại vị trí con trỏ, lưu vào `history`, con trỏ dịch sang phải. |
| `void deleteChar()` | Xóa ký tự bên trái con trỏ, lưu vào `history`, con trỏ dịch sang trái. Ném lỗi nếu con trỏ ở đầu. |
| **Di chuyển con trỏ** |
| `void moveCursorLeft()` | Dịch con trỏ sang trái 1. Ném `cursor_error` nếu ở đầu. |
| `void moveCursorRight()` | Dịch con trỏ sang phải 1. Ném `cursor_error` nếu ở cuối. |
| `void moveCursorTo(int index)` | Di chuyển con trỏ đến `index`. Ném `std::out_of_range` nếu không hợp lệ. |
| **Undo/Redo** |
| `void undo()` | Hoàn tác hành động gần nhất. |
| `void redo()` | Làm lại hành động vừa hoàn tác. |
| **Tiện ích** |
| `string getContent() const` | Trả về nội dung hiện tại. |
| `int getCursorPos() const` | Trả về vị trí con trỏ. |
| `int findFirstOccurrence(char c) const` | Tìm vị trí đầu tiên của ký tự `c`. |
| `int* findAllOccurrences(char c, int &count) const` | Tìm tất cả vị trí ký tự `c`. |
| `void sortAscending()` | Sắp xếp các ký tự tăng dần, con trỏ về đầu. |
| `void deleteAllOccurrences(char c)` | Xóa toàn bộ ký tự `c`. |
| `void printHistory() const` | In lịch sử thao tác (qua `HistoryManager`). |

---

### 5. Ví dụ sử dụng
```cpp
TextBuffer buf;

buf.insert('A');
buf.insert('B');
buf.insert('C');
buf.moveCursorLeft(); // Con trỏ trước C

buf.undo(); // Hoàn tác di chuyển con trỏ
buf.redo(); // Làm lại di chuyển con trỏ

std::cout << buf.getContent(); // "ABC"
```

---

### 6. Ghi chú
- Mỗi thao tác chỉnh sửa sẽ tạo một `Action` trong `HistoryManager` để phục vụ Undo/Redo.
- Các hành động `"insert"`, `"delete"`, `"move"` đều được lưu kèm thông tin `cursorBefore`, `cursorAfter` và `data` (ký tự liên quan).
- **Undo** khôi phục trạng thái trước hành động.
- **Redo** thực hiện lại hành động vừa hoàn tác.