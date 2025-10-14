# Project Setup Guide 🚀

This guide will help you set up the Memory Card Flip Game project on your local machine for development and contribution.

## 📋 Table of Contents

- [Prerequisites](#prerequisites)
- [Quick Setup](#quick-setup)
- [Detailed Setup](#detailed-setup)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Running Tests](#running-tests)
- [Contributing](#contributing)
- [Troubleshooting](#troubleshooting)

## ✅ Prerequisites

Before you begin, ensure you have the following installed:

### Required
- **C++ Compiler** supporting C++17 or later
  - GCC 7+ (Linux/macOS)
  - Clang 5+ (Linux/macOS)
  - MSVC 2017+ (Windows)
- **CMake 3.16** or later
- **Git** for version control

### Optional but Recommended
- **Visual Studio Code** with C/C++ extensions
- **CLion** or **Visual Studio 2022**
- **Valgrind** (Linux) for memory debugging

## ⚡ Quick Setup

```bash
# 1. Clone the repository
git clone https://github.com/MSTC-DA-IICT/memory-card-game.git
cd memory-card-game

# 2. Create build directory
mkdir build && cd build

# 3. Configure and build
cmake ..
make -j4

# 4. Run the game
./memory_game
```

## 🔧 Detailed Setup

### 1. Fork and Clone

1. **Fork** the repository on GitHub
2. **Clone** your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/memory-card-game.git
   cd memory-card-game
   ```

3. **Add upstream** remote:
   ```bash
   git remote add upstream https://github.com/MSTC-DA-IICT/memory-card-game.git
   ```

### 2. Install Dependencies

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

#### Linux (CentOS/RHEL)
```bash
sudo yum groupinstall "Development Tools"
sudo yum install cmake git
sudo yum install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel
```

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake
```

#### Windows
1. Install **Visual Studio 2022** with C++ support
2. Install **CMake** from https://cmake.org/download/
3. Install **Git** from https://git-scm.com/

### 3. Project Structure

```
memory-card-game/
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp           # Game logic
│   ├── Card.cpp           # Card implementation
│   ├── GameBoard.cpp      # Board management
│   ├── AudioManager.cpp   # Audio system
│   ├── ScoreManager.cpp   # Score tracking
│   └── Utils.cpp          # Utility functions
├── include/               # Header files
│   ├── Game.h
│   ├── Card.h
│   ├── GameBoard.h
│   ├── AudioManager.h
│   ├── ScoreManager.h
│   └── Utils.h
├── tests/                 # Unit tests
│   ├── test_main.cpp
│   ├── test_card.cpp
│   ├── test_gameboard.cpp
│   └── test_utils.cpp
├── assets/                # Game assets
│   ├── textures/          # Images and sprites
│   │   ├── cards/         # Card textures
│   │   ├── ui/            # UI elements
│   │   └── backgrounds/   # Background images
│   ├── sounds/            # Audio files
│   │   ├── sfx/           # Sound effects
│   │   └── music/         # Background music
│   └── fonts/             # Font files
├── docs/                  # Documentation
├── .github/               # GitHub configuration
│   ├── workflows/         # CI/CD workflows
│   ├── ISSUE_TEMPLATE/    # Issue templates
│   └── PULL_REQUEST_TEMPLATE.md
├── CMakeLists.txt         # Build configuration
├── README.md              # Project overview
├── CONTRIBUTING.md        # Contribution guidelines
├── CODE_OF_CONDUCT.md     # Code of conduct
├── LICENSE                # MIT License
└── .gitignore             # Git ignore rules
```

## 🏗️ Building the Project

### Debug Build (Development)
```bash
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4
```

### Release Build (Production)
```bash
mkdir build-release && cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

### Using Different Generators

#### Visual Studio (Windows)
```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

#### Ninja (Cross-platform)
```bash
cmake -G Ninja ..
ninja
```

## 🧪 Running Tests

```bash
# Build with tests enabled (default)
cmake -DBUILD_TESTS=ON ..
make -j4

# Run tests
make test
# or
ctest --output-on-failure
```

## 📝 Development Workflow

### 1. Create a Feature Branch
```bash
git checkout -b feature/amazing-new-feature
```

### 2. Make Your Changes
- Edit source files in `src/` and `include/`
- Add tests in `tests/`
- Update documentation if needed

### 3. Test Your Changes
```bash
# Build and test
make -j4
make test

# Run the game manually
./memory_game
```

### 4. Commit and Push
```bash
git add .
git commit -m "feat: add amazing new feature"
git push origin feature/amazing-new-feature
```

### 5. Create Pull Request
- Go to GitHub and create a pull request
- Fill out the PR template
- Wait for review and address feedback

## 🎯 VS Code Setup

### Recommended Extensions
- **C/C++** - Microsoft
- **CMake Tools** - Microsoft  
- **GitLens** - GitKraken
- **Better Comments** - Aaron Bond

### Settings (.vscode/settings.json)
```json
{
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.configureArgs": [
        "-DCMAKE_BUILD_TYPE=Debug"
    ],
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    }
}
```

## 🛠️ Troubleshooting

### Common Issues

#### CMake Can't Find raylib
**Solution**: raylib is downloaded automatically via CMake FetchContent. Ensure you have internet connectivity during first build.

#### Build Fails on Windows
**Solution**: 
- Ensure Visual Studio 2017+ is installed
- Use "x64 Native Tools Command Prompt" for building
- Try using the Visual Studio generator

#### Audio Not Working
**Solutions**:
- Linux: Install ALSA development packages
- Check that audio device is not being used by other applications
- Verify audio files exist in `assets/sounds/`

#### Missing OpenGL Libraries (Linux)
**Solution**:
```bash
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
```

#### Permission Denied (Linux/macOS)
**Solution**:
```bash
chmod +x memory_game
```

### Getting Help

If you encounter issues:

1. **Check the logs**: Look for error messages in the console
2. **Search existing issues**: Check our [GitHub Issues](https://github.com/MSTC-DA-IICT/memory-card-game/issues)
3. **Create a new issue**: If your problem isn't covered, create a detailed bug report
4. **Ask the community**: Use [GitHub Discussions](https://github.com/MSTC-DA-IICT/memory-card-game/discussions)
5. **Contact us**: Email [microsoftclub@daiict.ac.in](mailto:microsoftclub@daiict.ac.in)

## 📚 Additional Resources

### Learning C++ Game Development
- [raylib Examples](https://www.raylib.com/examples.html)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [Modern C++ Tutorial](https://changkun.de/modern-cpp/en-us/00-preface/)

### raylib Documentation
- [raylib.com](https://www.raylib.com/)
- [raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raylib GitHub](https://github.com/raysan5/raylib)

### CMake Resources
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)

---

**Ready to contribute? Check out our [Contributing Guide](CONTRIBUTING.md)!**

**Happy Coding! 🎮**

*Made with ❤️ by [MSTC DA-IICT](https://github.com/MSTC-DA-IICT) for Hacktoberfest 2025*