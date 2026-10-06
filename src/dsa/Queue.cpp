#include "Queue.h"
#include "raylib.h"
#include <string>

extern const Color CYBER_CYAN;
extern const Color CYBER_PANEL;
extern const Color CYBER_TEXT;
extern const Color CYBER_DIM;

void Queue::insert(int value) { enqueue(value); }

void Queue::enqueue(int value) {
    data_.push_back(value);
    animProgress_ = 0.0f;
    enqueueing_ = true;
    dequeueing_ = false;
    animatingValue_ = value;
}

int Queue::dequeue() {
    if (!empty()) {
        int val = data_.front();
        data_.erase(data_.begin());
        animProgress_ = 0.0f;
        dequeueing_ = true;
        enqueueing_ = false;
        animatingValue_ = val;
        return val;
    }
    return -1;
}

int Queue::front() const { return empty() ? -1 : data_.front(); }

bool Queue::remove(int value) {
    if (!empty() && data_.front() == value) { dequeue(); return true; }
    return false;
}

bool Queue::search(int value) const {
    for (int v : data_) if (v == value) return true;
    return false;
}

std::string Queue::getName() const { return "Queue"; }
std::string Queue::getComplexityInfo() const { return "Enqueue: O(1) | Dequeue: O(1) | Front: O(1)"; }
size_t Queue::size() const { return data_.size(); }
bool Queue::empty() const { return data_.empty(); }
void Queue::reset() { data_.clear(); enqueueing_ = dequeueing_ = false; animProgress_ = 1.0f; }

void Queue::updateAnim(float dt) {
    if (animProgress_ < 1.0f) {
        animProgress_ += dt * 5.0f;
        if (animProgress_ > 1.0f) {
            animProgress_ = 1.0f;
            enqueueing_ = dequeueing_ = false;
        }
    }
}

void Queue::visualize(float x, float y, float w, float h) const {
    float elemW = 60;
    float elemH = 60;
    float spacing = 10;
    float startX = x + 50;
    float startY = y + h/2 - elemH/2;

    (void)w;
    DrawText("FRONT", startX, startY - 20, 10, CYBER_CYAN);
    if (!empty()) DrawText("REAR", startX + (data_.size()-1)*(elemW+spacing), startY - 20, 10, CYBER_DIM);

    for (size_t i = 0; i < data_.size(); i++) {
        float ex = startX + i * (elemW + spacing);
        
        if (i == data_.size() - 1 && enqueueing_) {
            ex += (1.0f - animProgress_) * 50.0f;
        }

        Color borderColor = (i == 0) ? CYBER_CYAN : CYBER_DIM;
        
        if (i == 0 && !enqueueing_ && !dequeueing_) {
            DrawRectangleRounded({ex-2, startY-2, elemW+4, elemH+4}, 0.2f, 8, {0, 229, 255, 100});
        }

        DrawRectangleRounded({ex, startY, elemW, elemH}, 0.2f, 8, CYBER_PANEL);
        DrawRectangleRoundedLinesEx({ex, startY, elemW, elemH}, 0.2f, 8, 2.0f, borderColor);
        
        std::string valStr = std::to_string(data_[i]);
        DrawText(valStr.c_str(), static_cast<int>(ex + elemW/2 - MeasureText(valStr.c_str(), 20)/2), static_cast<int>(startY + elemH/2 - 10), 20, CYBER_TEXT);
    }

    if (dequeueing_) {
        float ex = startX - 50.0f * animProgress_;
        Color c = CYBER_PANEL; c.a = static_cast<unsigned char>(255 * (1.0f - animProgress_));
        Color tc = CYBER_TEXT; tc.a = c.a;
        Color bc = CYBER_CYAN; bc.a = c.a;
        
        DrawRectangleRounded({ex, startY, elemW, elemH}, 0.2f, 8, c);
        DrawRectangleRoundedLinesEx({ex, startY, elemW, elemH}, 0.2f, 8, 2.0f, bc);
        std::string valStr = std::to_string(animatingValue_);
        DrawText(valStr.c_str(), static_cast<int>(ex + elemW/2 - MeasureText(valStr.c_str(), 20)/2), static_cast<int>(startY + elemH/2 - 10), 20, tc);
    }
}
