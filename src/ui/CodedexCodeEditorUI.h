#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <functional>

struct CodeTestCase {
    std::string name;
    std::string input;
    std::string expected;
    std::string got;
    bool passed = false;
};

struct ProblemSpec {
    std::string id;
    std::string title;
    std::string difficulty = "Easy"; // "Easy", "Medium", "Hard"
    std::string category = "Arrays"; // "Binary Search", "Two Pointers", etc.
    std::string description;
    std::vector<std::string> examples;     // Formatted example strings
    std::vector<std::string> constraints;  // E.g. {"1 <= n <= 10^5", "O(1) Auxiliary Space"}
    std::vector<std::string> templateBefore;
    std::string defaultEditableCode;
    std::vector<std::string> templateAfter;
    std::function<std::vector<CodeTestCase>(const std::string& code)> testRunner;
    std::function<void(bool success)> onComplete;
};

class CodedexCodeEditorUI {
public:
    CodedexCodeEditorUI();
    ~CodedexCodeEditorUI();

    void startEditor(const ProblemSpec& spec);

    // Backward-compatible overload
    void startEditor(
        const std::string& problemTitle,
        const std::string& promptText,
        const std::vector<std::string>& templateBefore,
        const std::string& defaultEditableCode,
        const std::vector<std::string>& templateAfter,
        std::function<std::vector<CodeTestCase>(const std::string& code)> testRunner,
        std::function<void(bool success)> onComplete
    );

    void update(float dt);
    void draw() const;           // GBA low-res (clean placeholder)
    void drawOverlay() const;    // 960x640 Native LeetCode / Codédex Screen Overlay

    bool isActive() const { return active_; }

private:
    void ensureFontsLoaded() const;
    void executeTests();
    void resetCode();

    bool active_ = false;
    ProblemSpec spec_;

    std::string editableCode_;
    std::string initialCode_;
    int cursorIndex_ = 0;
    float cursorBlink_ = 0.0f;
    int scrollOffset_ = 0;

    enum class TestStatus { IDLE, RUNNING, ACCEPTED, WRONG_ANSWER };
    TestStatus status_ = TestStatus::IDLE;
    std::vector<CodeTestCase> testResults_;
    int activeTestTab_ = 0; // 0: Case 1, 1: Case 2, 2: Case 3
    float runtimeMs_ = 0.0f;

    // Font cache
    mutable Font fontCode_{0};
    mutable Font fontCodeSmall_{0};
    mutable Font fontUI_{0};
    mutable Font fontUIBold_{0};
    mutable bool fontsLoaded_ = false;
};
