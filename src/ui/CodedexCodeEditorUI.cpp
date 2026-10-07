#include "CodedexCodeEditorUI.h"
#include "core/GBAEngine.h"
#include "audio/SFXManager.h"
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <algorithm>

// ─── SYNTAX HIGHLIGHTING TOKENIZER ───────────────────────────────────────────
struct SyntaxToken {
    std::string text;
    Color color;
};

static bool isKeyword(const std::string& word) {
    static const std::unordered_set<std::string> kw = {
        "int", "void", "return", "if", "else", "while", "for", "bool", "class",
        "struct", "public", "private", "true", "false", "nullptr", "auto", "const",
        "unsigned", "char", "float", "double", "sizeof"
    };
    return kw.find(word) != kw.end();
}

static bool isType(const std::string& word) {
    static const std::unordered_set<std::string> tp = {
        "vector", "string", "stack", "queue", "unordered_map", "unordered_set",
        "map", "set", "Node", "ListNode", "TreeNode", "size_t", "pair"
    };
    return tp.find(word) != tp.end();
}

static std::vector<SyntaxToken> tokenizeLine(const std::string& line) {
    std::vector<SyntaxToken> tokens;
    size_t i = 0;
    size_t n = line.length();

    while (i < n) {
        // Comment
        if (i + 1 < n && line[i] == '/' && line[i + 1] == '/') {
            tokens.push_back({line.substr(i), Color{106, 153, 85, 255}});
            break;
        }

        // Whitespace
        if (isspace(line[i])) {
            size_t start = i;
            while (i < n && isspace(line[i])) i++;
            tokens.push_back({line.substr(start, i - start), WHITE});
            continue;
        }

        // String literal
        if (line[i] == '"' || line[i] == '\'') {
            char quote = line[i];
            size_t start = i++;
            while (i < n && line[i] != quote) {
                if (line[i] == '\\' && i + 1 < n) i++;
                i++;
            }
            if (i < n) i++;
            tokens.push_back({line.substr(start, i - start), Color{206, 145, 120, 255}});
            continue;
        }

        // Number
        if (isdigit(line[i]) || (line[i] == '-' && i + 1 < n && isdigit(line[i + 1]))) {
            size_t start = i++;
            while (i < n && (isdigit(line[i]) || line[i] == '.')) i++;
            tokens.push_back({line.substr(start, i - start), Color{181, 206, 168, 255}});
            continue;
        }

        // Word (identifier or keyword)
        if (isalpha(line[i]) || line[i] == '_') {
            size_t start = i++;
            while (i < n && (isalnum(line[i]) || line[i] == '_')) i++;
            std::string word = line.substr(start, i - start);
            Color col = Color{156, 220, 254, 255}; // default identifier
            if (isKeyword(word)) col = Color{86, 156, 214, 255};
            else if (isType(word)) col = Color{78, 201, 176, 255};
            tokens.push_back({word, col});
            continue;
        }

        // Operators / punctuation
        char op = line[i++];
        tokens.push_back({std::string(1, op), Color{212, 212, 212, 255}});
    }
    return tokens;
}

// ─── WORD WRAPPING HELPER ───────────────────────────────────────────────────
static std::vector<std::string> wrapText(Font font, const std::string& text, float maxWidth, float fontSize, float spacing) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string paragraph;
    while (std::getline(stream, paragraph)) {
        if (paragraph.empty()) {
            lines.push_back("");
            continue;
        }
        std::istringstream words(paragraph);
        std::string word;
        std::string currentLine = "";
        while (words >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            float w = MeasureTextEx(font, testLine.c_str(), fontSize, spacing).x;
            if (w > maxWidth && !currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }
        if (!currentLine.empty()) {
            lines.push_back(currentLine);
        }
    }
    return lines;
}

// ─── CONSTRUCTOR & DESTRUCTOR ───────────────────────────────────────────────
CodedexCodeEditorUI::CodedexCodeEditorUI() = default;

CodedexCodeEditorUI::~CodedexCodeEditorUI() {
    if (fontsLoaded_) {
        if (fontCode_.texture.id > 0) UnloadFont(fontCode_);
        if (fontCodeSmall_.texture.id > 0) UnloadFont(fontCodeSmall_);
        if (fontUI_.texture.id > 0) UnloadFont(fontUI_);
        if (fontUIBold_.texture.id > 0) UnloadFont(fontUIBold_);
        fontsLoaded_ = false;
    }
}

void CodedexCodeEditorUI::ensureFontsLoaded() const {
    if (fontsLoaded_) return;

    if (FileExists("assets/fonts/code.ttf")) {
        fontCode_ = LoadFontEx("assets/fonts/code.ttf", 16, nullptr, 0);
        SetTextureFilter(fontCode_.texture, TEXTURE_FILTER_BILINEAR);
        fontCodeSmall_ = LoadFontEx("assets/fonts/code.ttf", 13, nullptr, 0);
        SetTextureFilter(fontCodeSmall_.texture, TEXTURE_FILTER_BILINEAR);
    } else {
        fontCode_ = GetFontDefault();
        fontCodeSmall_ = GetFontDefault();
    }

    if (FileExists("assets/fonts/ui.ttf")) {
        fontUI_ = LoadFontEx("assets/fonts/ui.ttf", 15, nullptr, 0);
        SetTextureFilter(fontUI_.texture, TEXTURE_FILTER_BILINEAR);
        fontUIBold_ = LoadFontEx("assets/fonts/ui.ttf", 18, nullptr, 0);
        SetTextureFilter(fontUIBold_.texture, TEXTURE_FILTER_BILINEAR);
    } else {
        fontUI_ = fontCode_;
        fontUIBold_ = fontCode_;
    }

    fontsLoaded_ = true;
}

void CodedexCodeEditorUI::startEditor(const ProblemSpec& spec) {
    spec_ = spec;
    editableCode_ = spec.defaultEditableCode;
    initialCode_ = spec.defaultEditableCode;
    cursorIndex_ = static_cast<int>(editableCode_.length());
    cursorBlink_ = 0.0f;
    status_ = TestStatus::IDLE;
    testResults_.clear();
    activeTestTab_ = 0;
    active_ = true;

    SFXManager::instance().play("scan");
}

void CodedexCodeEditorUI::startEditor(
    const std::string& problemTitle,
    const std::string& promptText,
    const std::vector<std::string>& templateBefore,
    const std::string& defaultEditableCode,
    const std::vector<std::string>& templateAfter,
    std::function<std::vector<CodeTestCase>(const std::string& code)> testRunner,
    std::function<void(bool success)> onComplete
) {
    ProblemSpec spec;
    spec.title = problemTitle;
    spec.description = promptText;
    spec.templateBefore = templateBefore;
    spec.defaultEditableCode = defaultEditableCode;
    spec.templateAfter = templateAfter;
    spec.testRunner = testRunner;
    spec.onComplete = onComplete;
    startEditor(spec);
}

void CodedexCodeEditorUI::resetCode() {
    editableCode_ = initialCode_;
    cursorIndex_ = static_cast<int>(editableCode_.length());
    status_ = TestStatus::IDLE;
    testResults_.clear();
    SFXManager::instance().play("click");
}

void CodedexCodeEditorUI::executeTests() {
    if (!spec_.testRunner) return;

    testResults_ = spec_.testRunner(editableCode_);
    bool allPassed = !testResults_.empty();
    int firstFailedTab = -1;

    for (size_t i = 0; i < testResults_.size(); ++i) {
        if (!testResults_[i].passed) {
            allPassed = false;
            if (firstFailedTab == -1) firstFailedTab = static_cast<int>(i);
        }
    }

    if (allPassed) {
        status_ = TestStatus::ACCEPTED;
        SFXManager::instance().play("success");
    } else {
        status_ = TestStatus::WRONG_ANSWER;
        if (firstFailedTab >= 0) activeTestTab_ = firstFailedTab;
        SFXManager::instance().play("hit");
    }
}

void CodedexCodeEditorUI::update(float dt) {
    if (!active_) return;

    cursorBlink_ += dt;
    if (cursorBlink_ >= 1.0f) cursorBlink_ = 0.0f;

    Vector2 mouse = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // ─── MOUSE BUTTON INTERACTIONS ───
    // 1. Close Button (x: 850, y: 18, w: 86, h: 28)
    Rectangle closeBtn = {850.0f, 18.0f, 86.0f, 28.0f};
    if (CheckCollisionPointRec(mouse, closeBtn) && mouseClicked) {
        active_ = false;
        if (spec_.onComplete) spec_.onComplete(false);
        SFXManager::instance().play("click");
        return;
    }

    // 2. Reset Button (x: 770, y: 72, w: 64, h: 22)
    Rectangle resetBtn = {770.0f, 72.0f, 64.0f, 22.0f};
    if (CheckCollisionPointRec(mouse, resetBtn) && mouseClicked) {
        resetCode();
        return;
    }

    // 3. Run Tests Button (x: 770, y: 350, w: 154, h: 30)
    Rectangle runBtn = {770.0f, 350.0f, 154.0f, 30.0f};
    if (CheckCollisionPointRec(mouse, runBtn) && mouseClicked) {
        executeTests();
        return;
    }

    // 4. Test Case Tabs (x: 470 + i * 84, y: 400, w: 78, h: 24)
    for (int i = 0; i < 3; ++i) {
        Rectangle tabBtn = {470.0f + i * 84.0f, 400.0f, 78.0f, 24.0f};
        if (CheckCollisionPointRec(mouse, tabBtn) && mouseClicked) {
            activeTestTab_ = i;
            SFXManager::instance().play("hover");
        }
    }

    // 5. Submit Button (When Accepted) (x: 670, y: 574, w: 254, h: 34)
    if (status_ == TestStatus::ACCEPTED) {
        Rectangle submitBtn = {670.0f, 574.0f, 254.0f, 34.0f};
        if (CheckCollisionPointRec(mouse, submitBtn) && mouseClicked) {
            active_ = false;
            if (spec_.onComplete) spec_.onComplete(true);
            return;
        }
    }

    // ─── KEYBOARD SHORTCUTS ───
    if (IsKeyPressed(KEY_ONE)) activeTestTab_ = 0;
    if (IsKeyPressed(KEY_TWO)) activeTestTab_ = 1;
    if (IsKeyPressed(KEY_THREE)) activeTestTab_ = 2;

    if (status_ == TestStatus::ACCEPTED) {
        if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            active_ = false;
            if (spec_.onComplete) spec_.onComplete(true);
            return;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE) || (IsKeyPressed(KEY_X) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))) {
        active_ = false;
        if (spec_.onComplete) spec_.onComplete(false);
        SFXManager::instance().play("click");
        return;
    }

    if (IsKeyPressed(KEY_F5) || (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))) {
        executeTests();
        return;
    }

    // Text typing
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126) {
            editableCode_.insert(cursorIndex_, 1, static_cast<char>(key));
            cursorIndex_++;
            status_ = TestStatus::IDLE;
        }
        key = GetCharPressed();
    }

    // Backspace
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
        if (cursorIndex_ > 0) {
            editableCode_.erase(cursorIndex_ - 1, 1);
            cursorIndex_--;
            status_ = TestStatus::IDLE;
        }
    }

    // Delete
    if (IsKeyPressed(KEY_DELETE)) {
        if (cursorIndex_ < static_cast<int>(editableCode_.length())) {
            editableCode_.erase(cursorIndex_, 1);
            status_ = TestStatus::IDLE;
        }
    }

    // Arrow navigation
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
        if (cursorIndex_ > 0) cursorIndex_--;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
        if (cursorIndex_ < static_cast<int>(editableCode_.length())) cursorIndex_++;
    }
    if (IsKeyPressed(KEY_HOME)) cursorIndex_ = 0;
    if (IsKeyPressed(KEY_END)) cursorIndex_ = static_cast<int>(editableCode_.length());

    // Enter to execute tests if idle
    if (IsKeyPressed(KEY_ENTER) && status_ != TestStatus::ACCEPTED) {
        executeTests();
    }
}

void CodedexCodeEditorUI::draw() const {
    // High-res overlay handles full rendering!
}

void CodedexCodeEditorUI::drawOverlay() const {
    if (!active_) return;
    ensureFontsLoaded();

    Vector2 mouse = GetMousePosition();

    // 1. Semi-transparent backdrop blur/dim
    DrawRectangle(0, 0, 960, 640, Color{10, 13, 22, 246});

    // 2. Main Window Container (932 x 616)
    Rectangle mainBox = {14.0f, 12.0f, 932.0f, 616.0f};
    DrawRectangleRounded(mainBox, 0.02f, 8, Color{18, 22, 33, 255});
    DrawRectangleRoundedLinesEx(mainBox, 0.02f, 8, 1.5f, Color{42, 54, 76, 255});

    // 3. Header Bar (Height 46)
    Rectangle headerBar = {14.0f, 12.0f, 932.0f, 46.0f};
    DrawRectangleRounded(headerBar, 0.02f, 8, Color{24, 30, 46, 255});
    DrawLine(14, 58, 946, 58, Color{42, 54, 76, 255});

    // Logo & Brand
    DrawTextEx(fontUIBold_, "CODÉDEX", Vector2{28, 22}, 18, 1.0f, Color{0, 220, 160, 255});
    DrawTextEx(fontUI_, "// LEETCODE ARENA", Vector2{122, 24}, 15, 1.0f, Color{130, 155, 195, 255});

    // Difficulty Badge
    std::string diff = spec_.difficulty.empty() ? "Easy" : spec_.difficulty;
    Color diffColor = (diff == "Medium") ? Color{255, 192, 30, 255} : Color{0, 184, 163, 255};
    Color diffBg = (diff == "Medium") ? Color{255, 192, 30, 40} : Color{0, 184, 163, 40};
    DrawRectangleRounded(Rectangle{290, 22, 54, 20}, 0.3f, 4, diffBg);
    DrawRectangleRoundedLinesEx(Rectangle{290, 22, 54, 20}, 0.3f, 4, 1.0f, diffColor);
    DrawTextEx(fontUI_, diff.c_str(), Vector2{300, 24}, 13, 1.0f, diffColor);

    // Title
    DrawTextEx(fontUIBold_, spec_.title.c_str(), Vector2{354, 22}, 18, 1.0f, Color{240, 245, 252, 255});

    // Exit Button
    Rectangle closeBtn = {850.0f, 18.0f, 86.0f, 28.0f};
    bool closeHover = CheckCollisionPointRec(mouse, closeBtn);
    DrawRectangleRounded(closeBtn, 0.2f, 4, closeHover ? Color{65, 30, 36, 255} : Color{32, 40, 58, 255});
    DrawRectangleRoundedLinesEx(closeBtn, 0.2f, 4, 1.0f, closeHover ? Color{220, 80, 80, 255} : Color{50, 65, 92, 255});
    DrawTextEx(fontUI_, "✕ Close", Vector2{864, 23}, 14, 1.0f, closeHover ? Color{255, 140, 140, 255} : Color{180, 195, 220, 255});

    // ─── LEFT PANEL: PROBLEM SPEC & CONSTRAINTS (x: 26, y: 68, w: 424, h: 546) ───
    Rectangle leftPanel = {26.0f, 68.0f, 424.0f, 546.0f};
    DrawRectangleRounded(leftPanel, 0.02f, 6, Color{22, 27, 40, 255});
    DrawRectangleRoundedLinesEx(leftPanel, 0.02f, 6, 1.2f, Color{36, 46, 68, 255});

    // Left Tab Bar
    DrawRectangle(26, 68, 424, 32, Color{28, 35, 52, 255});
    DrawLine(26, 100, 450, 100, Color{36, 46, 68, 255});
    DrawTextEx(fontUIBold_, "Problem Description", Vector2{40, 75}, 15, 1.0f, Color{230, 240, 255, 255});
    DrawRectangle(38, 98, 126, 2, Color{0, 180, 255, 255}); // Active cyan indicator

    // Problem Description (Word-wrapped to fit perfectly within box!)
    float descY = 112.0f;
    std::vector<std::string> descLines = wrapText(fontUI_, spec_.description, 394.0f, 14.0f, 1.0f);
    for (const auto& line : descLines) {
        DrawTextEx(fontUI_, line.c_str(), Vector2{40, descY}, 14, 1.0f, Color{195, 212, 235, 255});
        descY += 20.0f;
    }

    descY += 10.0f;

    // Examples Section
    if (!spec_.examples.empty()) {
        DrawTextEx(fontUIBold_, "Examples:", Vector2{40, descY}, 15, 1.0f, Color{245, 205, 95, 255});
        descY += 22.0f;

        for (size_t exIdx = 0; exIdx < spec_.examples.size(); ++exIdx) {
            std::vector<std::string> exLines = wrapText(fontCodeSmall_, spec_.examples[exIdx], 380.0f, 13.0f, 1.0f);
            float cardHeight = exLines.size() * 18.0f + 14.0f;
            Rectangle exCard = {38.0f, descY, 400.0f, cardHeight};
            DrawRectangleRounded(exCard, 0.04f, 4, Color{14, 18, 28, 255});
            DrawRectangleRoundedLinesEx(exCard, 0.04f, 4, 1.0f, Color{32, 42, 62, 255});

            float cy = descY + 7.0f;
            for (const auto& eline : exLines) {
                DrawTextEx(fontCodeSmall_, eline.c_str(), Vector2{48, cy}, 13, 1.0f, Color{175, 200, 230, 255});
                cy += 18.0f;
            }
            descY += cardHeight + 8.0f;
        }
    }

    descY += 6.0f;

    // Constraints Section
    if (!spec_.constraints.empty()) {
        DrawTextEx(fontUIBold_, "Constraints & Complexities:", Vector2{40, descY}, 14, 1.0f, Color{140, 170, 210, 255});
        descY += 20.0f;
        for (const auto& c : spec_.constraints) {
            DrawCircle(46, static_cast<int>(descY + 6), 2.5f, Color{0, 180, 255, 200});
            DrawTextEx(fontCodeSmall_, c.c_str(), Vector2{56, descY}, 13, 1.0f, Color{165, 185, 215, 255});
            descY += 19.0f;
        }
    }

    // ─── RIGHT PANEL - TOP: C++ CODE EDITOR (x: 460, y: 68, w: 476, h: 320) ───
    Rectangle editorPanel = {460.0f, 68.0f, 476.0f, 320.0f};
    DrawRectangleRounded(editorPanel, 0.02f, 6, Color{15, 18, 28, 255});
    DrawRectangleRoundedLinesEx(editorPanel, 0.02f, 6, 1.2f, Color{36, 46, 68, 255});

    // Editor Tab Bar
    DrawRectangle(460, 68, 476, 32, Color{20, 25, 38, 255});
    DrawLine(460, 100, 936, 100, Color{36, 46, 68, 255});

    // Tab C++ Solution
    DrawRectangleRounded(Rectangle{470, 71, 114, 25}, 0.15f, 4, Color{26, 34, 50, 255});
    DrawTextEx(fontUIBold_, "Solution.cpp", Vector2{482, 75}, 14, 1.0f, Color{230, 240, 255, 255});
    DrawTextEx(fontCodeSmall_, "C++20", Vector2{600, 76}, 12, 1.0f, Color{120, 145, 180, 255});

    // Reset Button
    Rectangle resetBtn = {770.0f, 72.0f, 64.0f, 22.0f};
    bool resetHover = CheckCollisionPointRec(mouse, resetBtn);
    DrawRectangleRounded(resetBtn, 0.2f, 4, resetHover ? Color{45, 55, 80, 255} : Color{28, 35, 52, 255});
    DrawTextEx(fontUI_, "⟲ Reset", Vector2{778, 75}, 13, 1.0f, resetHover ? GBA_GOLD : Color{160, 180, 210, 255});

    // Code Surface
    float codeY = 108.0f;
    int lineNum = 1;

    // Gutter background
    DrawRectangle(460, 101, 38, 242, Color{18, 22, 34, 255});
    DrawLine(498, 101, 498, 343, Color{32, 40, 58, 255});

    auto drawSyntaxLine = [this](const std::string& line, float x, float y) {
        auto tokens = tokenizeLine(line);
        float tx = x;
        for (const auto& tok : tokens) {
            DrawTextEx(fontCode_, tok.text.c_str(), Vector2{tx, y}, 15, 1.0f, tok.color);
            tx += MeasureTextEx(fontCode_, tok.text.c_str(), 15, 1.0f).x;
        }
    };

    // 1. Template Before lines
    for (const auto& line : spec_.templateBefore) {
        DrawTextEx(fontCodeSmall_, TextFormat("%02d", lineNum++), Vector2{470, codeY + 1}, 12, 1.0f, Color{80, 100, 130, 255});
        drawSyntaxLine(line, 508.0f, codeY);
        codeY += 22.0f;
    }

    // 2. Editable Line (Highlighted row)
    DrawRectangle(499, static_cast<int>(codeY - 2), 436, 24, Color{28, 42, 68, 255});
    DrawTextEx(fontCodeSmall_, TextFormat("%02d", lineNum++), Vector2{470, codeY + 1}, 12, 1.0f, GBA_GOLD);
    drawSyntaxLine(editableCode_, 508.0f, codeY);

    // Blinking gold cursor
    if (cursorBlink_ < 0.5f) {
        std::string sub = editableCode_.substr(0, cursorIndex_);
        float cOff = MeasureTextEx(fontCode_, sub.c_str(), 15, 1.0f).x;
        DrawRectangle(static_cast<int>(508.0f + cOff), static_cast<int>(codeY - 1), 2, 20, GBA_GOLD);
    }
    codeY += 24.0f;

    // 3. Template After lines
    for (const auto& line : spec_.templateAfter) {
        DrawTextEx(fontCodeSmall_, TextFormat("%02d", lineNum++), Vector2{470, codeY + 1}, 12, 1.0f, Color{80, 100, 130, 255});
        drawSyntaxLine(line, 508.0f, codeY);
        codeY += 22.0f;
    }

    // Editor Action Bar (Run Button)
    DrawRectangle(460, 344, 476, 44, Color{18, 22, 34, 255});
    DrawLine(460, 344, 936, 344, Color{32, 40, 58, 255});
    DrawTextEx(fontUI_, "Type solution directly. Use [Left/Right] arrows to move cursor.", Vector2{472, 356}, 13, 1.0f, Color{130, 150, 180, 255});

    Rectangle runBtn = {770.0f, 350.0f, 154.0f, 32.0f};
    bool runHover = CheckCollisionPointRec(mouse, runBtn);
    DrawRectangleRounded(runBtn, 0.2f, 4, runHover ? Color{55, 215, 108, 255} : Color{44, 187, 93, 255});
    DrawTextEx(fontUIBold_, "▷ RUN TESTS [R]", Vector2{786, 356}, 14, 1.0f, WHITE);

    // ─── RIGHT PANEL - BOTTOM: TESTCASE RESULTS CONSOLE (x: 460, y: 396, w: 476, h: 218) ───
    Rectangle consolePanel = {460.0f, 396.0f, 476.0f, 218.0f};
    DrawRectangleRounded(consolePanel, 0.02f, 6, Color{13, 16, 25, 255});
    DrawRectangleRoundedLinesEx(consolePanel, 0.02f, 6, 1.2f, Color{36, 46, 68, 255});

    // Console Tab Bar
    DrawRectangle(460, 396, 476, 32, Color{18, 22, 34, 255});
    DrawLine(460, 428, 936, 428, Color{36, 46, 68, 255});

    for (int tIdx = 0; tIdx < 3; ++tIdx) {
        Rectangle tabBtn = {470.0f + tIdx * 84.0f, 400.0f, 78.0f, 24.0f};
        bool isAct = (activeTestTab_ == tIdx);
        bool tabHover = CheckCollisionPointRec(mouse, tabBtn);

        DrawRectangleRounded(tabBtn, 0.2f, 4, isAct ? Color{28, 38, 56, 255} : (tabHover ? Color{24, 30, 44, 255} : Color{16, 20, 30, 255}));
        if (isAct) {
            DrawRectangleRoundedLinesEx(tabBtn, 0.2f, 4, 1.0f, Color{0, 180, 255, 255});
        }

        // Status dot
        Color dotCol = Color{100, 115, 140, 255};
        if (tIdx < static_cast<int>(testResults_.size())) {
            dotCol = testResults_[tIdx].passed ? Color{0, 220, 120, 255} : Color{255, 80, 80, 255};
        }
        DrawCircle(static_cast<int>(tabBtn.x + 12), static_cast<int>(tabBtn.y + 12), 3.0f, dotCol);
        DrawTextEx(fontUI_, TextFormat("Case %d", tIdx + 1), Vector2{tabBtn.x + 22, tabBtn.y + 5}, 13, 1.0f, isAct ? WHITE : Color{160, 180, 205, 255});
    }

    // Console Body Content
    float conY = 438.0f;

    if (status_ == TestStatus::IDLE) {
        DrawTextEx(fontUIBold_, "Test Suite Ready", Vector2{474, conY}, 15, 1.0f, Color{160, 190, 230, 255});
        conY += 24.0f;
        DrawTextEx(fontUI_, "Ready to execute 3 test cases against your C++ algorithm.", Vector2{474, conY}, 13, 1.0f, Color{140, 165, 195, 255});
        conY += 22.0f;
        DrawTextEx(fontUI_, "Press [R] or click [▷ RUN TESTS] to compile and run.", Vector2{474, conY}, 14, 1.0f, GBA_GOLD);
    } else if (status_ == TestStatus::ACCEPTED) {
        // Green Accepted Banner
        Rectangle accBanner = {470.0f, conY, 456.0f, 30.0f};
        DrawRectangleRounded(accBanner, 0.15f, 4, Color{20, 65, 38, 255});
        DrawRectangleRoundedLinesEx(accBanner, 0.15f, 4, 1.0f, Color{45, 190, 95, 255});
        DrawTextEx(fontUIBold_, "✔ ACCEPTED (All 3 Test Cases Passed)", Vector2{484, conY + 6}, 15, 1.0f, Color{100, 255, 150, 255});
        DrawTextEx(fontCodeSmall_, "Runtime: 0 ms | Memory: 4.2 MB", Vector2{720, conY + 8}, 12, 1.0f, Color{160, 235, 180, 255});
        conY += 38.0f;

        // Active Case Inspection
        if (activeTestTab_ < static_cast<int>(testResults_.size())) {
            const auto& tc = testResults_[activeTestTab_];
            DrawTextEx(fontCodeSmall_, TextFormat("Input:    %s", tc.input.c_str()), Vector2{474, conY}, 13, 1.0f, Color{180, 210, 240, 255});
            conY += 20.0f;
            DrawTextEx(fontCodeSmall_, TextFormat("Expected: %s", tc.expected.c_str()), Vector2{474, conY}, 13, 1.0f, Color{120, 255, 150, 255});
            conY += 20.0f;
            DrawTextEx(fontCodeSmall_, TextFormat("Output:   %s [PASS ✔]", tc.got.c_str()), Vector2{474, conY}, 13, 1.0f, Color{120, 255, 150, 255});
            conY += 22.0f;
        }

        // Glowing Submit Button
        Rectangle submitBtn = {670.0f, 574.0f, 254.0f, 34.0f};
        bool subHover = CheckCollisionPointRec(mouse, submitBtn);
        DrawRectangleRounded(submitBtn, 0.2f, 4, subHover ? Color{55, 215, 108, 255} : Color{44, 187, 93, 255});
        DrawTextEx(fontUIBold_, "✔ SUBMIT & PROCEED [Enter]", Vector2{684, 582}, 14, 1.0f, WHITE);
    } else if (status_ == TestStatus::WRONG_ANSWER) {
        // Red Wrong Answer Banner
        Rectangle waBanner = {470.0f, conY, 456.0f, 30.0f};
        DrawRectangleRounded(waBanner, 0.15f, 4, Color{70, 25, 25, 255});
        DrawRectangleRoundedLinesEx(waBanner, 0.15f, 4, 1.0f, Color{220, 60, 60, 255});
        DrawTextEx(fontUIBold_, TextFormat("✖ WRONG ANSWER on Case %d", activeTestTab_ + 1), Vector2{484, conY + 6}, 15, 1.0f, Color{255, 120, 120, 255});
        conY += 38.0f;

        // Inspection diff
        if (activeTestTab_ < static_cast<int>(testResults_.size())) {
            const auto& tc = testResults_[activeTestTab_];
            DrawTextEx(fontCodeSmall_, TextFormat("Input:    %s", tc.input.c_str()), Vector2{474, conY}, 13, 1.0f, Color{240, 200, 200, 255});
            conY += 20.0f;
            DrawTextEx(fontCodeSmall_, TextFormat("Expected: %s", tc.expected.c_str()), Vector2{474, conY}, 13, 1.0f, Color{120, 255, 150, 255});
            conY += 20.0f;
            DrawTextEx(fontCodeSmall_, TextFormat("Output:   %s [FAIL ✖]", tc.got.c_str()), Vector2{474, conY}, 13, 1.0f, Color{255, 100, 100, 255});
            conY += 22.0f;
        }

        DrawTextEx(fontUI_, "Fix your logic above and click [▷ RUN TESTS] or press [R].", Vector2{474, 584}, 13, 1.0f, Color{255, 210, 130, 255});
    }
}
