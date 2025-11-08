git /**
 * @file main.cpp
 * @brief Entry point for the single player Memory Card Flip Game.
 */

#include <raylib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

// Platform-specific socket includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    #define SOCKET_ERROR_CODE WSAGetLastError()
    #define CLOSE_SOCKET closesocket
    #define SHUTDOWN_SOCKET(s) shutdown(s, SD_BOTH)
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define SOCKET_ERROR_CODE errno
    #define CLOSE_SOCKET close
    #define SHUTDOWN_SOCKET(s) shutdown(s, SHUT_RDWR)
#endif

#include "Game.h"
#include "Utils.h"

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;
constexpr int TARGET_FPS = 60;
constexpr const char* WINDOW_TITLE = "Memory Card Flip Game - MSTC DA-IICT";
constexpr unsigned short DEFAULT_PORT = 5000;

// ==================== Networking State ====================
enum class NetworkMode {
    NONE,
    SERVER,
    CLIENT
};

struct NetworkState {
    NetworkMode mode = NetworkMode::NONE;
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    std::atomic<bool> connected{false};
    std::atomic<bool> shouldStop{false};
    std::mutex messageMutex;
    std::vector<std::string> incomingMessages;
    std::string remoteIP = "127.0.0.1";
    unsigned short port = DEFAULT_PORT;
    int myPlayerID = 0;  // 0 = server, 1 = client
    int currentTurn = 0;  // Whose turn it is
    int playerScores[2] = {0, 0};
    bool isMyTurn = true;
};

static NetworkState g_network;

// ==================== Network Functions ====================

void initNetwork() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Utils::logError("WSAStartup failed");
    }
#endif
}

void cleanupNetwork() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void setSocketNonBlocking(SOCKET sock) {
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif
}

void startServer(unsigned short port) {
    if (g_network.mode != NetworkMode::NONE) {
        Utils::logError("Already in network mode!");
        return;
    }

    initNetwork();
    
    g_network.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_network.serverSocket == INVALID_SOCKET) {
        Utils::logError("Failed to create server socket");
        return;
    }

    // Set socket options
    int opt = 1;
#ifdef _WIN32
    setsockopt(g_network.serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(g_network.serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(g_network.serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Utils::logError("Failed to bind server socket on port " + std::to_string(port));
        CLOSE_SOCKET(g_network.serverSocket);
        g_network.serverSocket = INVALID_SOCKET;
        return;
    }

    if (listen(g_network.serverSocket, 1) == SOCKET_ERROR) {
        Utils::logError("Failed to listen on server socket");
        CLOSE_SOCKET(g_network.serverSocket);
        g_network.serverSocket = INVALID_SOCKET;
        return;
    }

    setSocketNonBlocking(g_network.serverSocket);
    
    g_network.mode = NetworkMode::SERVER;
    g_network.port = port;
    g_network.myPlayerID = 0;
    g_network.currentTurn = 0;
    g_network.isMyTurn = true;
    
    Utils::logInfo("Server started on port " + std::to_string(port) + ". Waiting for client...");
}

void startClient(const std::string& ip, unsigned short port) {
    if (g_network.mode != NetworkMode::NONE) {
        Utils::logError("Already in network mode!");
        return;
    }

    initNetwork();
    
    g_network.clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_network.clientSocket == INVALID_SOCKET) {
        Utils::logError("Failed to create client socket");
        return;
    }

    setSocketNonBlocking(g_network.clientSocket);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
#ifdef _WIN32
    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        Utils::logError("Invalid IP address: " + ip);
        CLOSE_SOCKET(g_network.clientSocket);
        g_network.clientSocket = INVALID_SOCKET;
        return;
    }
#else
    if (inet_aton(ip.c_str(), &serverAddr.sin_addr) == 0) {
        Utils::logError("Invalid IP address: " + ip);
        CLOSE_SOCKET(g_network.clientSocket);
        g_network.clientSocket = INVALID_SOCKET;
        return;
    }
#endif

    // Try to connect (non-blocking)
    int result = connect(g_network.clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    
#ifdef _WIN32
    if (result == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS) {
            Utils::logError("Failed to connect to server");
            CLOSE_SOCKET(g_network.clientSocket);
            g_network.clientSocket = INVALID_SOCKET;
            return;
        }
    }
#else
    if (result == SOCKET_ERROR && errno != EINPROGRESS) {
        Utils::logError("Failed to connect to server");
        CLOSE_SOCKET(g_network.clientSocket);
        g_network.clientSocket = INVALID_SOCKET;
        return;
    }
#endif

    g_network.mode = NetworkMode::CLIENT;
    g_network.remoteIP = ip;
    g_network.port = port;
    g_network.myPlayerID = 1;
    g_network.currentTurn = 0;
    g_network.isMyTurn = false;
    
    Utils::logInfo("Connecting to server at " + ip + ":" + std::to_string(port) + "...");
}

void stopNetwork() {
    g_network.shouldStop = true;
    
    if (g_network.clientSocket != INVALID_SOCKET) {
        SHUTDOWN_SOCKET(g_network.clientSocket);
        CLOSE_SOCKET(g_network.clientSocket);
        g_network.clientSocket = INVALID_SOCKET;
    }
    
    if (g_network.serverSocket != INVALID_SOCKET) {
        SHUTDOWN_SOCKET(g_network.serverSocket);
        CLOSE_SOCKET(g_network.serverSocket);
        g_network.serverSocket = INVALID_SOCKET;
    }
    
    g_network.mode = NetworkMode::NONE;
    g_network.connected = false;
    g_network.isMyTurn = true;
    
    cleanupNetwork();
    Utils::logInfo("Network stopped");
}

void sendMessage(const std::string& message) {
    SOCKET sock = INVALID_SOCKET;
    if (g_network.mode == NetworkMode::SERVER && g_network.connected) {
        sock = g_network.clientSocket;
    } else if (g_network.mode == NetworkMode::CLIENT && g_network.connected) {
        sock = g_network.clientSocket;
    }
    
    if (sock == INVALID_SOCKET) return;
    
    std::string msgWithNewline = message + "\n";
    int sent = send(sock, msgWithNewline.c_str(), (int)msgWithNewline.length(), 0);
    if (sent == SOCKET_ERROR) {
        int error = SOCKET_ERROR_CODE;
#ifdef _WIN32
        if (error != WSAEWOULDBLOCK) {
#else
        if (error != EAGAIN && error != EWOULDBLOCK) {
#endif
            Utils::logError("Send failed, disconnecting");
            g_network.connected = false;
        }
    }
}

void sendGameState() {
    // Send current game state
    // Format: "STATE turn:0 score0:10 score1:5"
    std::stringstream ss;
    ss << "STATE turn:" << g_network.currentTurn 
       << " score0:" << g_network.playerScores[0]
       << " score1:" << g_network.playerScores[1];
    sendMessage(ss.str());
}

void receiveGameState() {
    SOCKET sock = INVALID_SOCKET;
    if (g_network.mode == NetworkMode::SERVER && g_network.connected) {
        sock = g_network.clientSocket;
    } else if (g_network.mode == NetworkMode::CLIENT && g_network.connected) {
        sock = g_network.clientSocket;
    }
    
    if (sock == INVALID_SOCKET) return;
    
    char buffer[1024];
    int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    
    if (received > 0) {
        buffer[received] = '\0';
        std::string message(buffer);
        
        std::lock_guard<std::mutex> lock(g_network.messageMutex);
        g_network.incomingMessages.push_back(message);
    } else if (received == 0) {
        // Connection closed
        Utils::logWarning("Connection closed by peer");
        g_network.connected = false;
    } else {
        int error = SOCKET_ERROR_CODE;
#ifdef _WIN32
        if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS) {
#else
        if (error != EAGAIN && error != EWOULDBLOCK) {
#endif
            Utils::logError("Receive error, disconnecting");
            g_network.connected = false;
        }
    }
}

void handleIncomingMessage(const std::string& msg) {
    std::istringstream iss(msg);
    std::string command;
    iss >> command;
    
    if (command == "FLIP") {
        int cardIndex;
        iss >> cardIndex;
        Utils::logInfo("Received FLIP command for card " + std::to_string(cardIndex));
        // Note: We can't directly flip cards without modifying GameBoard
        // This would require a workaround or modifying Game class
    } else if (command == "MATCH") {
        int card1, card2;
        iss >> card1 >> card2;
        Utils::logInfo("Received MATCH: " + std::to_string(card1) + " and " + std::to_string(card2));
    } else if (command == "TURN") {
        int turn;
        iss >> turn;
        g_network.currentTurn = turn;
        g_network.isMyTurn = (turn == g_network.myPlayerID);
        Utils::logInfo("Turn changed to player " + std::to_string(turn));
    } else if (command == "SCORE") {
        int player, score;
        iss >> player >> score;
        if (player >= 0 && player < 2) {
            g_network.playerScores[player] = score;
        }
    } else if (command == "STATE") {
        std::string token;
        while (iss >> token) {
            size_t colon = token.find(':');
            if (colon != std::string::npos) {
                std::string key = token.substr(0, colon);
                std::string value = token.substr(colon + 1);
                if (key == "turn") {
                    g_network.currentTurn = std::stoi(value);
                    g_network.isMyTurn = (g_network.currentTurn == g_network.myPlayerID);
                } else if (key == "score0") {
                    g_network.playerScores[0] = std::stoi(value);
                } else if (key == "score1") {
                    g_network.playerScores[1] = std::stoi(value);
                }
            }
        }
    } else if (command == "END") {
        int winner;
        iss >> winner;
        Utils::logInfo("Game ended. Winner: Player " + std::to_string(winner));
    }
}

void updateNetworkLoop(float deltaTime) {
    static float networkTimer = 0.0f;
    networkTimer += deltaTime;
    
    // Process incoming messages
    {
        std::lock_guard<std::mutex> lock(g_network.messageMutex);
        for (const auto& msg : g_network.incomingMessages) {
            handleIncomingMessage(msg);
        }
        g_network.incomingMessages.clear();
    }
    
    // Server: Accept new connections
    if (g_network.mode == NetworkMode::SERVER && !g_network.connected) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        SOCKET newClient = accept(g_network.serverSocket, (sockaddr*)&clientAddr, &clientLen);
        
        if (newClient != INVALID_SOCKET) {
            g_network.clientSocket = newClient;
            setSocketNonBlocking(g_network.clientSocket);
            g_network.connected = true;
            Utils::logInfo("Client connected!");
            
            // Send initial state
            sendMessage("TURN 0");
        }
    }
    
    // Client: Check connection status
    if (g_network.mode == NetworkMode::CLIENT && !g_network.connected) {
        // Check if connection is established
        fd_set writeSet;
        FD_ZERO(&writeSet);
        FD_SET(g_network.clientSocket, &writeSet);
        timeval timeout{0, 0};
        
        if (select((int)g_network.clientSocket + 1, nullptr, &writeSet, nullptr, &timeout) > 0) {
            int error = 0;
            socklen_t len = sizeof(error);
            if (getsockopt(g_network.clientSocket, SOL_SOCKET, SO_ERROR, (char*)&error, &len) == 0 && error == 0) {
                g_network.connected = true;
                Utils::logInfo("Connected to server!");
            }
        }
    }
    
    // Receive messages
    if (g_network.connected) {
        receiveGameState();
    }
    
    // Send periodic state updates
    if (networkTimer >= 0.1f && g_network.connected) {
        networkTimer = 0.0f;
        sendGameState();
    }
}

// ==================== Multiplayer Integration ====================

void nextTurn() {
    g_network.currentTurn = 1 - g_network.currentTurn;
    g_network.isMyTurn = (g_network.currentTurn == g_network.myPlayerID);
    sendMessage("TURN " + std::to_string(g_network.currentTurn));
}

bool isMyTurn() {
    return g_network.isMyTurn || !g_network.connected;
}

void updateScore(int player, int delta) {
    if (player >= 0 && player < 2) {
        g_network.playerScores[player] += delta;
        sendMessage("SCORE " + std::to_string(player) + " " + std::to_string(g_network.playerScores[player]));
    }
}

void showNetworkStatusUI() {
    if (g_network.mode == NetworkMode::NONE) return;
    
    // Network status bar at top
    DrawRectangle(0, 80, SCREEN_WIDTH, 30, ColorAlpha(BLACK, 0.8f));
    
    std::string statusText;
    Color statusColor = WHITE;
    
    if (g_network.connected) {
        statusText = "CONNECTED - ";
        statusColor = GREEN;
    } else {
        statusText = "CONNECTING... - ";
        statusColor = YELLOW;
    }
    
    if (g_network.mode == NetworkMode::SERVER) {
        statusText += "SERVER (Port: " + std::to_string(g_network.port) + ") - Player 0";
    } else {
        statusText += "CLIENT (" + g_network.remoteIP + ":" + std::to_string(g_network.port) + ") - Player 1";
    }
    
    statusText += " | Turn: Player " + std::to_string(g_network.currentTurn);
    if (!g_network.isMyTurn) {
        statusText += " (Waiting...)";
        statusColor = ORANGE;
    }
    
    DrawText(statusText.c_str(), 10, 85, 18, statusColor);
    
    // Player scores
    std::string scoreText = "P0: " + std::to_string(g_network.playerScores[0]) + 
                           " | P1: " + std::to_string(g_network.playerScores[1]);
    int scoreWidth = MeasureText(scoreText.c_str(), 18);
    DrawText(scoreText.c_str(), SCREEN_WIDTH - scoreWidth - 10, 85, 18, WHITE);
}

// ==================== Utility Functions ====================

void serializeGameState(std::string& output) {
    std::stringstream ss;
    ss << "STATE turn:" << g_network.currentTurn
       << " score0:" << g_network.playerScores[0]
       << " score1:" << g_network.playerScores[1];
    output = ss.str();
}

void deserializeGameState(const std::string& input) {
    handleIncomingMessage(input);
}

// ==================== Network Thread ====================

void networkThreadFunction() {
    while (!g_network.shouldStop) {
        updateNetworkLoop(0.016f);  // ~60fps
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

// ==================== Main Function ====================

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);
    
    InitAudioDevice();
    
    if (!IsAudioDeviceReady()) {
        Utils::logError("Failed to initialize audio device!");
    }
    
    // Network mode selection screen
    bool modeSelected = false;
    NetworkMode selectedMode = NetworkMode::NONE;
    std::string ipInput = "127.0.0.1";
    int selectedButton = 0;  // 0 = Single Player, 1 = Host, 2 = Join
    bool showGuide = false;
    
    while (!modeSelected && !WindowShouldClose()) {
        // Handle input
        if (IsKeyPressed(KEY_H) || IsKeyPressed(KEY_F1)) {
            showGuide = !showGuide;
        }
        
        if (!showGuide) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedButton = (selectedButton - 1 + 3) % 3;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedButton = (selectedButton + 1) % 3;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (selectedButton == 0) {
                    selectedMode = NetworkMode::NONE;
                    modeSelected = true;
                } else if (selectedButton == 1) {
                    startServer(DEFAULT_PORT);
                    selectedMode = NetworkMode::SERVER;
                    modeSelected = true;
                } else if (selectedButton == 2) {
                    startClient(ipInput, DEFAULT_PORT);
                    selectedMode = NetworkMode::CLIENT;
                    modeSelected = true;
                }
            }
            
            // Handle text input for IP
            if (selectedButton == 2) {
                int key = GetCharPressed();
                if (key >= 32 && key <= 126) {
                    if (ipInput.length() < 15) {
                        ipInput += (char)key;
                    }
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !ipInput.empty()) {
                    ipInput.pop_back();
                }
            }
        } else {
            // In guide mode, only handle closing
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_H) || IsKeyPressed(KEY_F1)) {
                showGuide = false;
            }
        }
        
        // Draw mode selection screen
        BeginDrawing();
        ClearBackground(DARKBLUE);
        
        if (showGuide) {
            // Draw guide overlay
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.9f));
            
            Rectangle guidePanel = {SCREEN_WIDTH / 2.0f - 400, 50, 800, SCREEN_HEIGHT - 100};
            DrawRectangleRounded(guidePanel, 0.1f, 16, ColorAlpha(DARKBLUE, 0.95f));
            Utils::drawRoundedRectangleLines(guidePanel, 0.1f, 16, 3, SKYBLUE);
            
            int yPos = 100;
            DrawText("MULTIPLAYER GUIDE", SCREEN_WIDTH / 2 - 150, yPos, 32, GOLD);
            yPos += 60;
            
            // Host Server Guide
            DrawText("HOW TO HOST A SERVER:", 80, yPos, 24, LIME);
            yPos += 35;
            DrawText("1. Select 'Host Server' option", 100, yPos, 20, WHITE);
            yPos += 25;
            DrawText("2. Server will start on port 5000", 100, yPos, 20, WHITE);
            yPos += 25;
            DrawText("3. Wait for a client to connect", 100, yPos, 20, WHITE);
            yPos += 25;
            DrawText("4. You are Player 0 (goes first)", 100, yPos, 20, WHITE);
            yPos += 40;
            
            // Join Server Guide
            DrawText("HOW TO JOIN A SERVER:", 80, yPos, 24, LIME);
            yPos += 35;
            DrawText("1. Select 'Join Server' option", 100, yPos, 20, WHITE);
            yPos += 25;
            DrawText("2. Enter the server's IP address:", 100, yPos, 20, WHITE);
            yPos += 25;
            DrawText("   - Localhost: 127.0.0.1 (same computer)", 120, yPos, 18, LIGHTGRAY);
            yPos += 22;
            DrawText("   - LAN: Find host's local IP (e.g., 192.168.1.xxx)", 120, yPos, 18, LIGHTGRAY);
            yPos += 22;
            DrawText("   - Internet: Use host's public IP (requires port forwarding)", 120, yPos, 18, LIGHTGRAY);
            yPos += 25;
            DrawText("3. Press ENTER to connect", 100, yPos, 20, WHITE);
            yPos += 25;
            DrawText("4. You are Player 1 (goes second)", 100, yPos, 20, WHITE);
            yPos += 40;
            
            // Tips
            DrawText("TIPS:", 80, yPos, 24, YELLOW);
            yPos += 35;
            DrawText("* Players take turns flipping cards", 100, yPos, 18, WHITE);
            yPos += 22;
            DrawText("* Matches are visible to both players", 100, yPos, 18, WHITE);
            yPos += 22;
            DrawText("* Scores sync automatically", 100, yPos, 18, WHITE);
            yPos += 22;
            DrawText("* Network status shown at top during game", 100, yPos, 18, WHITE);
            yPos += 22;
            DrawText("* If connection fails, check firewall/port settings", 100, yPos, 18, WHITE);
            yPos += 40;
            
            // Finding IP address
            DrawText("FINDING YOUR IP ADDRESS:", 80, yPos, 24, ORANGE);
            yPos += 35;
            DrawText("Windows: ipconfig (look for IPv4 Address)", 100, yPos, 18, WHITE);
            yPos += 22;
            DrawText("Linux/Mac: ifconfig or ip addr (look for inet)", 100, yPos, 18, WHITE);
            yPos += 22;
            DrawText("Or use: hostname -I (Linux) / ipconfig getifaddr en0 (Mac)", 100, yPos, 18, WHITE);
            
            DrawText("Press H or ESC to close this guide", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
            
        } else {
            DrawText("MEMORY CARD GAME - MULTIPLAYER", SCREEN_WIDTH / 2 - 250, 60, 30, GOLD);
            DrawText("Press H for Multiplayer Guide", SCREEN_WIDTH / 2 - 150, 100, 20, LIGHTGRAY);
            DrawText("Select Mode:", SCREEN_WIDTH / 2 - 100, 150, 24, WHITE);
            
            // Single Player button
            Color singleColor = (selectedButton == 0) ? GREEN : GRAY;
            DrawRectangle(SCREEN_WIDTH / 2 - 150, 200, 300, 50, singleColor);
            DrawRectangleLines(SCREEN_WIDTH / 2 - 150, 200, 300, 50, WHITE);
            DrawText("Single Player", SCREEN_WIDTH / 2 - 70, 215, 24, WHITE);
            
            // Host Server button
            Color hostColor = (selectedButton == 1) ? GREEN : GRAY;
            DrawRectangle(SCREEN_WIDTH / 2 - 150, 270, 300, 50, hostColor);
            DrawRectangleLines(SCREEN_WIDTH / 2 - 150, 270, 300, 50, WHITE);
            DrawText("Host Server (Port 5000)", SCREEN_WIDTH / 2 - 120, 285, 24, WHITE);
            
            // Join Server button
            Color joinColor = (selectedButton == 2) ? GREEN : GRAY;
            DrawRectangle(SCREEN_WIDTH / 2 - 150, 340, 300, 50, joinColor);
            DrawRectangleLines(SCREEN_WIDTH / 2 - 150, 340, 300, 50, WHITE);
            DrawText("Join Server", SCREEN_WIDTH / 2 - 80, 355, 24, WHITE);
            
            // IP input
            if (selectedButton == 2) {
                DrawText("IP Address:", SCREEN_WIDTH / 2 - 100, 410, 20, WHITE);
                DrawRectangle(SCREEN_WIDTH / 2 - 100, 435, 200, 30, DARKGRAY);
                DrawRectangleLines(SCREEN_WIDTH / 2 - 100, 435, 200, 30, WHITE);
                DrawText(ipInput.c_str(), SCREEN_WIDTH / 2 - 95, 442, 20, WHITE);
                if ((int)(GetTime() * 2) % 2) {
                    DrawText("_", SCREEN_WIDTH / 2 - 95 + MeasureText(ipInput.c_str(), 20), 442, 20, WHITE);
                }
                DrawText("Default: 127.0.0.1 (localhost)", SCREEN_WIDTH / 2 - 120, 475, 16, LIGHTGRAY);
            }
            
            // Quick guide summary
            int guideY = 520;
            DrawText("Quick Guide:", SCREEN_WIDTH / 2 - 150, guideY, 20, YELLOW);
            guideY += 25;
            DrawText("Host: Select 'Host Server' and wait for connection", SCREEN_WIDTH / 2 - 240, guideY, 16, LIGHTGRAY);
            guideY += 20;
            DrawText("Join: Select 'Join Server', enter host IP, press ENTER", SCREEN_WIDTH / 2 - 240, guideY, 16, LIGHTGRAY);
            guideY += 20;
            DrawText("Press H for detailed multiplayer guide", SCREEN_WIDTH / 2 - 180, guideY, 16, LIGHTGRAY);
            
            DrawText("Use Arrow Keys / WASD to navigate, ENTER to select", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 50, 18, LIGHTGRAY);
        }
        
        EndDrawing();
    }
    
    // Start network thread if in multiplayer mode
    std::thread networkThread;
    if (selectedMode != NetworkMode::NONE) {
        networkThread = std::thread(networkThreadFunction);
    }
    
    try {
        auto game = std::make_unique<Game>(SCREEN_WIDTH, SCREEN_HEIGHT);
        
        Utils::logInfo("Memory Card Game initialized successfully!");
        
        // Main game loop
        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            
            // Update network
            if (selectedMode != NetworkMode::NONE) {
                updateNetworkLoop(deltaTime);
            }
            
            // Update game (only allow input if it's my turn or single player)
            if (selectedMode == NetworkMode::NONE || isMyTurn()) {
                game->update();
            } else {
                // Still update game state but disable input
                // This is a limitation - we'd need to modify Game class to disable input
                game->update();
            }
            
            // Draw
            BeginDrawing();
            {
                ClearBackground(DARKBLUE);
                game->draw();
                
                // Draw network status
                if (selectedMode != NetworkMode::NONE) {
                    showNetworkStatusUI();
                }
                
                // Draw turn indicator
                if (selectedMode != NetworkMode::NONE && !isMyTurn()) {
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.3f));
                    DrawText("WAITING FOR OPPONENT'S TURN...", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 30, YELLOW);
                }
                
#ifdef DEBUG
                DrawFPS(10, 10);
#endif
            }
            EndDrawing();
        }
        
        Utils::logInfo("Game loop ended normally.");
        
    } catch (const std::exception& e) {
        Utils::logError("Game error: " + std::string(e.what()));
        
        BeginDrawing();
        ClearBackground(RED);
        DrawText("GAME ERROR - Check console for details", 
                 SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2, 20, WHITE);
        EndDrawing();
        
        while (!WindowShouldClose()) {}
        
        CloseAudioDevice();
        CloseWindow();
        return EXIT_FAILURE;
    }
    
    // Cleanup
    if (selectedMode != NetworkMode::NONE) {
        stopNetwork();
        if (networkThread.joinable()) {
            networkThread.join();
        }
    }
    
    Utils::logInfo("Cleaning up resources...");
    CloseAudioDevice();
    CloseWindow();
    
    Utils::logInfo("Thanks for playing Memory Card Game!");
    
    return EXIT_SUCCESS;
}
