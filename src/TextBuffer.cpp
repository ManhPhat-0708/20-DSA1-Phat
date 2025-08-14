#include "TextBuffer.h"

// ==================== HistoryManager ====================
TextBuffer::HistoryManager::HistoryManager() : currentIndex(0) {}

TextBuffer::HistoryManager::~HistoryManager() {}

// TODO

void TextBuffer::HistoryManager::addAction(const Action &a){
bool edit = (a.actionName == "insert" || a.actionName == "delete" || a.actionName == "remove");
if(edit){
    //xóa các hành động phía sau currentIndex nếu đã undo trước đó
    while(actions.size() > currentIndex){
        actions.deleteAt(currentIndex);
    }

// chèn action mới vào cuối lịch sử đang có
actions.insertAtTail(a);
currentIndex = actions.size();
} else {
    actions.insertAt(currentIndex, a);
    ++currentIndex;
}
}

bool TextBuffer::HistoryManager::canUndo() const{
    return currentIndex > 0;
}

bool TextBuffer::HistoryManager::canRedo() const {
    return currentIndex < actions.size();
}

void TextBuffer::HistoryManager::printHistory() const{
    cout<<"[";
    for (int i = 0; i < currentIndex; i++)
    {
        cout<<actions.get(i);
        if(i + 1 < currentIndex) cout<<", ";
    }
    cout<<"]";
}

TextBuffer::HistoryManager::Action
TextBuffer::HistoryManager::getLastAct() const {
    if (currentIndex == 0) throw std::out_of_range("No action to undo");
    return actions.get(currentIndex - 1); // trả về theo giá trị
}


TextBuffer::HistoryManager::Action
TextBuffer::HistoryManager::getCurrentAction() const {
    if (currentIndex >= actions.size()) throw std::out_of_range("No action to redo");
    return actions.get(currentIndex); // trả về theo giá trị
}


void TextBuffer::HistoryManager::moveBack() {
    if (currentIndex > 0) --currentIndex;
}

void TextBuffer::HistoryManager::moveForward() {
    if (currentIndex < actions.size()) ++currentIndex;
}


// ================== TextBuffer ==================
TextBuffer::TextBuffer() : cursorPos(0)
{
    history = new HistoryManager();
}

// TODO
TextBuffer::~TextBuffer(){
    delete history;
}

void TextBuffer::insert(char c){
    buffer.insertAt(cursorPos, c); // thêm ký tự vào buffer

    //thêm action vô history
    HistoryManager::Action act{"insert", cursorPos, cursorPos + 1, c};
    history->addAction(act);
    cursorPos++;
}

void TextBuffer::deleteChar(){
    if(cursorPos == 0){
        throw std::out_of_range("Index is invalid!");
    }
    //lấy ký tự bên trái con trỏ
    char removed = buffer.get(cursorPos - 1);

    //xóa ký tự đó đi
    buffer.deleteAt(cursorPos - 1);

    //ghi lại vô history
    HistoryManager::Action a{"delete", cursorPos, cursorPos - 1, removed};
    history->addAction(a);
    //dịch con trỏ sang trái
    --cursorPos;
}

void TextBuffer::moveCursorLeft(){
    if(cursorPos == 0){
        throw cursor_error("Cursor error!");
    }
    HistoryManager::Action a{"move", cursorPos, cursorPos - 1, 'L'};
    cursorPos--;
    history->addAction(a);
}

void TextBuffer::moveCursorRight() {
    if (cursorPos == buffer.size()) {
        throw cursor_error("Cursor error!");
    }
    int before = cursorPos;
    ++cursorPos;

    HistoryManager::Action step{"move", before, cursorPos, 'R'};
    history->addAction(step);
}

void TextBuffer::moveCursorTo(int index) {
    if (index < 0 || index > buffer.size()) {
        throw std::out_of_range("Index is invalid!");
    }
    int from = cursorPos;
    cursorPos = index;

    HistoryManager::Action jump{"move", from, cursorPos, 'J'};
    history->addAction(jump);
}

string TextBuffer::getContent() const {
    string s = "";
    for (int i = 0; i < buffer.size(); ++i) {
        s = s + buffer.get(i);
    }
    return s;
}

int TextBuffer::getCursorPos() const {
    return cursorPos;
}

int TextBuffer::findFirstOccurrence(char c) const {
    for (int i = 0; i < buffer.size(); ++i) {
        if (buffer.get(i) == c) {
            return i;
        }
    }
    return -1; // không tìm thấy
}

int* TextBuffer::findAllOccurrences(char c, int &count) const {
    count = 0;
    // Đếm số lần xuất hiện
    for (int i = 0; i < buffer.size(); ++i) {
        if (buffer.get(i) == c) {
            ++count;
        }
    }
    if (count == 0) return nullptr;

    int* positions = new int[count];
    int idx = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        if (buffer.get(i) == c) {
            positions[idx++] = i;
        }
    }
    return positions; // nhớ delete[] ở nơi gọi
}

void TextBuffer::sortAscending() {
    int n = buffer.size();
    if (n <= 1) { 
        cursorPos = 0; 
        return; 
    }

    // Insertion sort dựa trên compareAlphabet
    for (int i = 1; i < n; ++i) {
        char key = buffer.get(i);

        // Tìm vị trí chèn đầu tiên mà key < buffer[pos]
        int pos = 0;
        while (pos < i && !compareAlphabet(key, buffer.get(pos))) {
            ++pos;
        }

        // Nếu key cần dịch sang trái, xóa ở i rồi chèn vào pos
        if (pos < i) {
            buffer.deleteAt(i);      // sau bước này, các index < i giữ nguyên
            buffer.insertAt(pos, key);
        }
    }

    cursorPos = 0; // Sau sort xong, con trỏ về đầu
}


void TextBuffer::deleteAllOccurrences(char c) {
    int count = 0;
    for(int i = 0; i < buffer.size();++i) if(buffer.get(i) == c) count++;
    if(count == 0) return;

    for (int i = buffer.size() - 1; i >= 0; --i) {
        if (buffer.get(i) == c) {
            buffer.deleteAt(i);
            if (cursorPos > i) {
                --cursorPos;
            } else if (cursorPos == i && cursorPos > 0) {
                --cursorPos;
            }
        }
    }
    cursorPos = 0; // sau khi có xóa, con trỏ bay về đầu
}


void TextBuffer::undo(){
    if(!history->canUndo()) return;

    //lấy action vừa áp dụng gần nhất trong history
    auto a = history->getLastAct();
    history->moveBack();

    if (a.actionName == "insert") {
        // Đảo insert: xoá lại ký tự vừa chèn
        buffer.deleteAt(a.cursorAfter - 1);   // hoặc a.cursorBefore (vì after = before + 1)
        cursorPos = a.cursorBefore;

    } else if (a.actionName == "delete" || a.actionName == "remove") {
        // Đảo delete: chèn lại ký tự đã xoá ở vị trí bên trái con trỏ lúc xoá
        buffer.insertAt(a.cursorBefore - 1, a.data);
        cursorPos = a.cursorBefore;

    } else if (a.actionName == "move") {
        // Đảo move: trả con trỏ về vị trí cũ
        cursorPos = a.cursorBefore;
    }
}

void TextBuffer::redo() {
    if (!history->canRedo()) return;

    auto a = history->getCurrentAction(); // action ở currentIndex
    history->moveForward();         // tiến "con trỏ history" trước khi áp dụng

    if (a.actionName == "insert") {
        // Làm lại insert: chèn ký tự ở vị trí cũ
        buffer.insertAt(a.cursorBefore, a.data);
        cursorPos = a.cursorAfter;

    } else if (a.actionName == "delete" || a.actionName == "remove") {
        // Làm lại delete: xóa ký tự ở vị trí bên trái cursorBefore
        buffer.deleteAt(a.cursorBefore - 1);
        cursorPos = a.cursorAfter;

    } else if (a.actionName == "move") {
        // Làm lại move: di chuyển con trỏ
        cursorPos = a.cursorAfter;
    }
}


void TextBuffer::printHistory() const
{
    history->printHistory();
}