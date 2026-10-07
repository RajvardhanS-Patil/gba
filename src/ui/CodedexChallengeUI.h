#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <functional>

struct ChallengeOption {
    std::string text;
    bool isCorrect;
    std::string explanation;
};

class CodedexChallengeUI {
public:
    CodedexChallengeUI();

    void startChallenge(
        const std::string& questTitle,
        const std::string& promptText,
        const std::vector<std::string>& codeSnippetLines,
        const std::vector<ChallengeOption>& options,
        std::function<void(bool success)> onComplete
    );

    void update(float dt);
    void draw() const;

    bool isActive() const { return active_; }

private:
    bool active_ = false;
    std::string questTitle_;
    std::string promptText_;
    std::vector<std::string> codeLines_;
    std::vector<ChallengeOption> options_;
    std::function<void(bool success)> onComplete_;

    int selectedIdx_ = 0;
    bool evaluated_ = false;
    bool answeredCorrect_ = false;
    float feedbackTimer_ = 0.0f;
    std::string feedbackMessage_;
};
