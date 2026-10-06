#include "LinkedList.h"
#include "raylib.h"
#include <string>

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;
extern const Color CYBER_DANGER;

LinkedList::~LinkedList() { reset(); }

void LinkedList::reset() {
    while (head_) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
    }
    size_ = 0;
    highlightIndex_ = -1;
}

void LinkedList::insert(int value) {
    if (!head_) { head_ = new Node(value); }
    else {
        Node* curr = head_;
        while (curr->next) curr = curr->next;
        curr->next = new Node(value);
    }
    size_++;
}

void LinkedList::insertFront(int value) {
    Node* n = new Node(value);
    n->next = head_;
    head_ = n;
    size_++;
}

void LinkedList::insertAt(int index, int value) {
    if (index == 0) { insertFront(value); return; }
    Node* curr = head_;
    for (int i = 0; i < index - 1 && curr; i++) curr = curr->next;
    if (curr) {
        Node* n = new Node(value);
        n->next = curr->next;
        curr->next = n;
        size_++;
    }
}

bool LinkedList::remove(int value) {
    if (!head_) return false;
    if (head_->data == value) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
        size_--;
        return true;
    }
    Node* curr = head_;
    while (curr->next && curr->next->data != value) curr = curr->next;
    if (curr->next) {
        Node* temp = curr->next;
        curr->next = temp->next;
        delete temp;
        size_--;
        return true;
    }
    return false;
}

bool LinkedList::search(int value) const {
    Node* curr = head_;
    while (curr) {
        if (curr->data == value) return true;
        curr = curr->next;
    }
    return false;
}

int LinkedList::get(int index) const {
    Node* curr = head_;
    for (int i = 0; i < index && curr; i++) curr = curr->next;
    return curr ? curr->data : -1;
}

void LinkedList::setHighlight(int index) { highlightIndex_ = index; }

std::string LinkedList::getName() const { return "Linked List"; }
std::string LinkedList::getComplexityInfo() const { return "Access: O(n) | Search: O(n) | Insert: O(1)* | Delete: O(1)*"; }
size_t LinkedList::size() const { return size_; }
bool LinkedList::empty() const { return size_ == 0; }

void LinkedList::visualize(float x, float y, float w, float h) const {
    float nodeW = 80;
    float nodeH = 50;
    float spacing = 40;
    float startX = x + 50;
    float startY = y + h / 2.0f - nodeH / 2.0f;

    Node* curr = head_;
    int i = 0;
    
    DrawText("HEAD", startX, startY - 25, 10, CYBER_DIM);
    
    while (curr) {
        float cx = startX + i * (nodeW + spacing);
        Color borderColor = (i == highlightIndex_) ? CYBER_CYAN : CYBER_DIM;
        
        if (i == highlightIndex_) {
            DrawRectangleRounded({cx-2, startY-2, nodeW+4, nodeH+4}, 0.2f, 8, {0, 229, 255, 100});
        }
        
        (void)w;
        DrawRectangleRounded({cx, startY, nodeW, nodeH}, 0.2f, 8, CYBER_PANEL);
        DrawRectangleRoundedLinesEx({cx, startY, nodeW, nodeH}, 0.2f, 8, 2.0f, borderColor);
        
        std::string valStr = std::to_string(curr->data);
        DrawText(valStr.c_str(), cx + nodeW/2 - MeasureText(valStr.c_str(), 20)/2, startY + nodeH/2 - 10, 20, CYBER_TEXT);
        
        if (curr->next) {
            float arrowX = cx + nodeW;
            float arrowY = startY + nodeH/2;
            DrawLineEx({arrowX, arrowY}, {arrowX + spacing, arrowY}, 2.0f, CYBER_DIM);
            DrawTriangle({arrowX + spacing, arrowY}, {arrowX + spacing - 10, arrowY - 5}, {arrowX + spacing - 10, arrowY + 5}, CYBER_DIM);
        } else {
            float arrowX = cx + nodeW;
            float arrowY = startY + nodeH/2;
            DrawLineEx({arrowX, arrowY}, {arrowX + spacing/2, arrowY}, 2.0f, CYBER_DIM);
            DrawText("NULL", arrowX + spacing/2 + 5, arrowY - 5, 10, CYBER_DANGER);
        }
        
        curr = curr->next;
        i++;
    }
}
