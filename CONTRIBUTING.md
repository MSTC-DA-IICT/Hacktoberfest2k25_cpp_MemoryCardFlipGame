# Contributing to Memory Card Flip Game 🤝

First off, thanks for taking the time to contribute! ❤️ 

This project is part of **Hacktoberfest 2025** and we're excited to welcome contributors of all skill levels. Whether you're fixing bugs, adding features, improving documentation, or helping with testing, every contribution matters!

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Development Setup](#development-setup)
- [Coding Guidelines](#coding-guidelines)
- [Pull Request Process](#pull-request-process)
- [Issue Guidelines](#issue-guidelines)
- [Hacktoberfest Participation](#hacktoberfest-participation)
- [Community](#community)

## 🤝 Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to [microsoftclub@daiict.ac.in](mailto:microsoftclub@daiict.ac.in).

## 🚀 Getting Started

### Before You Begin

1. **Star** ⭐ and **Fork** 🍴 this repository
2. **Read** this contributing guide completely
3. **Check** existing [Issues](https://github.com/MSTC-DA-IICT/memory-card-game/issues) and [Pull Requests](https://github.com/MSTC-DA-IICT/memory-card-game/pulls)
4. **Set up** your development environment (see [Development Setup](#development-setup))

### What Can You Contribute?

We welcome contributions in many forms:

- 🐛 **Bug Fixes**: Fix gameplay bugs, memory leaks, or visual glitches
- ✨ **New Features**: Add game modes, power-ups, themes, or difficulty levels
- 🎨 **UI/UX Improvements**: Enhance graphics, animations, or user interface
- 🔊 **Audio**: Add sound effects, background music, or audio settings
- 📖 **Documentation**: Improve README, add code comments, or create tutorials
- 🧪 **Testing**: Write unit tests, integration tests, or find bugs
- 🔧 **Performance**: Optimize code, reduce memory usage, or improve fps
- 🌐 **Accessibility**: Make the game more accessible to all users

## 💻 Development Setup

### Prerequisites

- **C++17** or later compiler (GCC, Clang, or MSVC)
- **CMake 3.16** or higher
- **Git** for version control
- **Code Editor** (VS Code, CLion, Visual Studio, etc.)

### Step-by-Step Setup

1. **Fork and Clone**:
   ```bash
   # Fork this repo on GitHub, then clone your fork
   git clone https://github.com/YOUR_USERNAME/memory-card-game.git
   cd memory-card-game
   
   # Add upstream remote
   git remote add upstream https://github.com/MSTC-DA-IICT/memory-card-game.git
   ```

2. **Build the Project**:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   make -j4
   ```

3. **Run the Game**:
   ```bash
   ./memory_game
   ```

4. **Run Tests** (if available):
   ```bash
   make test
   # or
   ctest --output-on-failure
   ```

### Recommended Tools

- **VS Code Extensions**:
  - C/C++ Extension Pack
  - CMake Tools
  - GitLens
  - Better Comments

- **Development Tools**:
  - Valgrind (Linux) for memory debugging
  - AddressSanitizer for debugging
  - clang-format for code formatting

## 🛠️ How to Contribute

### Finding Issues to Work On

1. **Good First Issues**: Look for [`good-first-issue`](https://github.com/MSTC-DA-IICT/memory-card-game/labels/good-first-issue) label
2. **Hacktoberfest Issues**: Check [`hacktoberfest`](https://github.com/MSTC-DA-IICT/memory-card-game/labels/hacktoberfest) label
3. **Help Wanted**: Browse [`help-wanted`](https://github.com/MSTC-DA-IICT/memory-card-game/labels/help-wanted) issues
4. **Create Your Own**: Found a bug or have an idea? Create a new issue!

### Before Starting Work

1. **Comment on the issue** to let others know you're working on it
2. **Wait for confirmation** from maintainers before starting (especially for large features)
3. **Ask questions** if anything is unclear

### Making Changes

1. **Create a new branch** from `main`:
   ```bash
   git checkout main
   git pull upstream main
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following our [coding guidelines](#coding-guidelines)

3. **Test your changes**:
   - Build the project successfully
   - Run the game and test your feature
   - Run existing tests if available
   - Add new tests for your changes

4. **Commit your changes**:
   ```bash
   git add .
   git commit -m "feat: add amazing new feature
   
   - Detailed description of what was added
   - Why this change was needed
   - Any breaking changes or migration notes"
   ```

## 📝 Coding Guidelines

### C++ Style Guide

- **Indentation**: Use **4 spaces** (no tabs)
- **Line Length**: Maximum **100 characters** per line
- **Naming Conventions**:
  - Variables and functions: `camelCase`
  - Classes and structs: `PascalCase`
  - Constants: `UPPER_SNAKE_CASE`
  - Private members: prefix with `m_` (e.g., `m_cardTexture`)

### Code Organization

```cpp
// File: Card.h
#pragma once

#include <raylib.h>
#include <string>

class Card {
public:
    Card(int id, const std::string& texturePath);
    ~Card();
    
    void draw(Vector2 position) const;
    void flip();
    
    bool isFlipped() const { return m_isFlipped; }
    int getId() const { return m_id; }

private:
    int m_id;
    bool m_isFlipped;
    Texture2D m_texture;
    
    void loadTexture(const std::string& path);
};
```

### Documentation

- **Header Comments**: Every class and function should have brief documentation
- **Complex Logic**: Add inline comments explaining non-obvious code
- **TODO Comments**: Use `// TODO: description` for future improvements

```cpp
/**
 * @brief Represents a memory game card with flip animation
 * 
 * The Card class handles individual card state, rendering, and animations.
 * Each card has a unique ID and texture that represents its symbol.
 */
class Card {
    // Implementation...
};
```

### Error Handling

```cpp
// Good: Check for errors and handle them appropriately
Texture2D Card::loadTexture(const std::string& path) {
    Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load texture: %s", path.c_str());
        return GetTextureDefault(); // Fallback texture
    }
    return texture;
}
```

## 🔄 Pull Request Process

### Before Submitting

1. **Update your branch** with the latest changes:
   ```bash
   git checkout main
   git pull upstream main
   git checkout your-feature-branch
   git merge main
   ```

2. **Test thoroughly**:
   - Your changes work as expected
   - No existing functionality is broken
   - Game runs without crashes
   - No memory leaks (use tools like Valgrind)

3. **Clean up your commits** (optional but appreciated):
   ```bash
   git rebase -i main  # Squash related commits
   ```

### Pull Request Template

When creating a PR, please fill out this information:

```markdown
## 📝 Description
Brief description of what this PR does.

## 🔗 Related Issue
Closes #[issue number]

## 🧪 Testing
- [ ] Built successfully on my machine
- [ ] Tested the new feature/fix manually
- [ ] Added/updated unit tests (if applicable)
- [ ] No memory leaks detected

## 📸 Screenshots/GIFs (if applicable)
Show the visual changes or new features.

## ⚠️ Breaking Changes
List any breaking changes and migration steps.

## 📋 Checklist
- [ ] Code follows the style guide
- [ ] Self-review completed
- [ ] Documentation updated (if needed)
- [ ] Tests added/updated
```

### Review Process

1. **Automated checks** must pass (build, tests, style checks)
2. **Code review** by at least one maintainer
3. **Address feedback** promptly and professionally
4. **Final approval** and merge by maintainers

## 📋 Issue Guidelines

### Reporting Bugs

Use the **Bug Report** template and include:

- **OS and Version**: Windows 10, Ubuntu 20.04, macOS Big Sur, etc.
- **Compiler**: GCC 9.3, Clang 12, MSVC 2019, etc.
- **raylib Version**: Check in CMakeLists.txt
- **Steps to Reproduce**: Exact steps that cause the bug
- **Expected vs Actual**: What should happen vs what actually happens
- **Screenshots/Videos**: Visual proof helps a lot!

### Requesting Features

Use the **Feature Request** template and include:

- **Problem Description**: What problem does this solve?
- **Proposed Solution**: How would you implement this?
- **Alternatives Considered**: Other ways to solve the problem
- **Additional Context**: Mockups, examples from other games, etc.

## 🎃 Hacktoberfest Participation

### Eligibility

- Repository must have the `hacktoberfest` topic
- PRs must be submitted between **October 1-31, 2025**
- PRs must be **merged**, **approved**, or labeled `hacktoberfest-accepted`
- PRs marked as `spam` or `invalid` will not count

### Quality Standards

We maintain high standards for Hacktoberfest contributions:

✅ **Good PRs**:
- Solve real problems or add meaningful features
- Include proper testing and documentation
- Follow our coding guidelines
- Have clear commit messages

❌ **PRs we'll reject**:
- Trivial changes (like fixing typos in README)
- Generated or copied code without understanding
- Changes that break existing functionality
- PRs without proper testing

### Getting Help

- 💬 [GitHub Discussions](https://github.com/MSTC-DA-IICT/memory-card-game/discussions)
- 📧 Email us: [microsoftclub@daiict.ac.in](mailto:microsoftclub@daiict.ac.in)
- 🐛 Open an issue for bugs or questions

## 👥 Community

### MSTC DA-IICT

This project is maintained by the **Microsoft Student Technical Club** at **DA-IICT**:

- 🌐 Website: [mstc.daiict.ac.in](http://mstc.daiict.ac.in)
- 📧 Email: microsoftclub@daiict.ac.in
- 💼 LinkedIn: [MSTC DA-IICT](https://www.linkedin.com/company/mstc-da-iict/)

### Recognition

All contributors will be:
- ✨ Listed in our **Contributors** section
- 🎉 Mentioned in **release notes** for their contributions
- 🏆 Featured on our **social media** (with permission)
- 📧 Added to our **contributor mailing list** for project updates

## 📚 Additional Resources

### Learning C++ and Game Development
- [C++ Reference](https://en.cppreference.com/)
- [raylib Examples](https://www.raylib.com/examples.html)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)

### Git and GitHub
- [Git Handbook](https://guides.github.com/introduction/git-handbook/)
- [GitHub Flow](https://guides.github.com/introduction/flow/)
- [Writing Good Commit Messages](https://chris.beams.io/posts/git-commit/)

### Open Source Contribution
- [First Contributions](https://github.com/firstcontributions/first-contributions)
- [How to Contribute to Open Source](https://opensource.guide/how-to-contribute/)

---

## 🙏 Thank You!

Thank you for contributing to the Memory Card Flip Game! Your efforts help make this project better for everyone. Every bug fix, feature addition, and documentation improvement makes a difference.

**Happy Contributing! 🎉**

---

*Made with ❤️ by [MSTC DA-IICT](https://github.com/MSTC-DA-IICT) for Hacktoberfest 2025*