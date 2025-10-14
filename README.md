# Memory Card Flip Game 🃏

[![Hacktoberfest](https://img.shields.io/badge/Hacktoberfest-2025-blueviolet)](https://hacktoberfest.digitalocean.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/MSTC-DA-IICT/memory-card-game)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![raylib](https://img.shields.io/badge/raylib-5.0-red)](https://www.raylib.com/)

A classic memory card flip game built with **C++** and **raylib** graphics library. Match pairs of cards to win! Perfect for learning game development and contributing to open source during **Hacktoberfest 2025**.

## 🎮 Game Features

- 🔀 **Randomized Card Layout**: Cards are shuffled each game
- ⏱️ **Timer**: Track your completion time
- 📊 **Score System**: Points based on matches and time
- 🎨 **Beautiful Graphics**: Clean, modern UI with smooth animations
- 🔊 **Sound Effects**: Audio feedback for card flips and matches
- 📱 **Multiple Difficulty Levels**: 4x4, 6x6, and 8x8 grids
- 🏆 **High Score Tracking**: Save your best times locally

## 🚀 Quick Start

### Prerequisites

- **C++17** or later
- **CMake 3.16** or later
- **raylib 5.0** (automatically downloaded by CMake)

### Installation & Running

1. **Clone the repository**:
   ```bash
   git clone https://github.com/MSTC-DA-IICT/memory-card-game.git
   cd memory-card-game
   ```

2. **Build the project**:
   ```bash
   mkdir build && cd build
   cmake ..
   make -j4
   ```

3. **Run the game**:
   ```bash
   ./memory_game
   ```

### Alternative Build Methods

#### Using Visual Studio (Windows)
1. Open the project folder in Visual Studio 2022
2. Select `CMakeLists.txt` as the CMake project
3. Build and run with **F5**

#### Using VS Code
1. Install the **C/C++** and **CMake Tools** extensions
2. Open the project folder
3. Select a kit when prompted
4. Press **F7** to build and **F5** to debug

## 🎯 How to Play

1. **Click on any card** to flip it and reveal the symbol
2. **Click on another card** to find its matching pair
3. **Match all pairs** to complete the level!
4. **Try to complete** in the shortest time with fewest moves

### Game Rules
- Only **2 cards** can be flipped at once
- If cards **match**, they stay revealed
- If cards **don't match**, they flip back after 1 second
- Game ends when **all pairs** are found

## 🛠️ Project Structure

```
memory-card-game/
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp           # Game logic
│   ├── Card.cpp           # Card implementation
│   ├── GameBoard.cpp      # Board management
│   └── Utils.cpp          # Utility functions
├── include/               # Header files
│   ├── Game.h
│   ├── Card.h
│   ├── GameBoard.h
│   └── Utils.h
├── assets/                # Game assets
│   ├── textures/          # Card textures and sprites
│   ├── sounds/            # Audio files
│   └── fonts/             # Custom fonts
├── tests/                 # Unit tests
├── docs/                  # Documentation
├── CMakeLists.txt         # Build configuration
├── README.md              # This file
├── CONTRIBUTING.md        # Contribution guidelines
└── LICENSE                # MIT License
```

## 🔧 Development Setup

### Adding New Features

1. **Fork** this repository
2. **Create** a feature branch: `git checkout -b feature/amazing-feature`
3. **Make** your changes in the appropriate files
4. **Test** your changes: `make test`
5. **Commit** your changes: `git commit -m 'Add amazing feature'`
6. **Push** to your fork: `git push origin feature/amazing-feature`
7. **Open** a Pull Request

### Code Style Guidelines

- Use **4 spaces** for indentation
- Follow **camelCase** for variables and functions
- Use **PascalCase** for classes
- Add **comments** for complex logic
- Include **unit tests** for new features

## 🤝 Contributing to Hacktoberfest

This project is participating in **Hacktoberfest 2025**! We welcome contributions from developers of all skill levels.

### Good First Issues 🌟

Look for issues labeled with:
- `good-first-issue` - Perfect for beginners
- `hacktoberfest` - Hacktoberfest-specific issues
- `help-wanted` - We need your expertise!

### Contribution Areas

- 🐛 **Bug Fixes**: Fix gameplay bugs or visual glitches
- ✨ **New Features**: Add power-ups, themes, or game modes
- 🎨 **Graphics**: Improve UI/UX or add new card designs
- 🔊 **Audio**: Add sound effects or background music
- 📖 **Documentation**: Improve README, add tutorials
- 🧪 **Testing**: Write unit tests or integration tests
- 🔧 **Performance**: Optimize game performance

### Getting Help

- 💬 **Discussions**: Use [GitHub Discussions](https://github.com/MSTC-DA-IICT/memory-card-game/discussions)
- 🐛 **Bug Reports**: Open an [Issue](https://github.com/MSTC-DA-IICT/memory-card-game/issues)
- 📧 **Email**: Contact us at `microsoftclub@daiict.ac.in`

Read our full [Contributing Guidelines](CONTRIBUTING.md) for detailed information.

## 🏫 About MSTC DA-IICT

This project is maintained by the **Microsoft Student Technical Club (MSTC)** at **Dhirubhai Ambani Institute of Information and Communication Technology (DA-IICT)**.

- 🌐 **Website**: [mstc.daiict.ac.in](http://mstc.daiict.ac.in)
- 💼 **LinkedIn**: [MSTC DA-IICT](https://www.linkedin.com/company/mstc-da-iict/)
- 📧 **Email**: microsoftclub@daiict.ac.in

### Our Mission
We aim to foster **open-source development**, provide **learning opportunities**, and build a **collaborative community** of tech enthusiasts.

## 📚 Learning Resources

### Game Development with raylib
- [raylib Official Website](https://www.raylib.com/)
- [raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Game Programming in C++](https://gameprogrammingpatterns.com/)

### C++ Resources
- [C++ Reference](https://en.cppreference.com/)
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)

### Open Source & Git
- [First Contributions](https://github.com/firstcontributions/first-contributions)
- [Git Handbook](https://guides.github.com/introduction/git-handbook/)
- [Hacktoberfest Guidelines](https://hacktoberfest.digitalocean.com/participation/)

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **raylib community** for the amazing graphics library
- **MSTC DA-IICT** members for their contributions
- **Hacktoberfest** for promoting open-source development
- All **contributors** who help make this project better!

## 🌟 Show Your Support

If you found this project helpful:
- ⭐ **Star** this repository
- 🍴 **Fork** it for your own experiments
- 📢 **Share** it with friends
- 🤝 **Contribute** to make it better!

---

**Happy Coding! 🚀**

Made with ❤️ by [MSTC DA-IICT](https://github.com/MSTC-DA-IICT) for **Hacktoberfest 2025**