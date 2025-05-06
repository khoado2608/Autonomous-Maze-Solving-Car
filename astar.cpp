#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cmath>
#include <cstring>
#include<algorithm>
#define WALL 1
#define PATH 0

using namespace std;

int ROWS, COLS;
vector<vector<int>> maze;

// Hướng di chuyển: lên, trái, phải, xuống
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};
char direction[4] = {'U', 'L', 'R', 'D'};

// Cấu trúc lưu thông tin của mỗi ô trong hàng đợi
struct Node {
    int x, y;     // Tọa độ hiện tại
    int g, h, f;  // Chi phí thực tế, heuristic và tổng chi phí
    int parent_x, parent_y; // Lưu vị trí cha để truy vết đường đi

    bool operator>(const Node &other) const {
        return f > other.f; // Ưu tiên ô có f nhỏ hơn
    }
};

// Hàm heuristic (ước lượng khoảng cách đến đích)
int heuristic(int x, int y, int goal_x, int goal_y) {
    return abs(x - goal_x) + abs(y - goal_y); // Khoảng cách Manhattan
}

// Hàm tìm đường bằng A*
bool AStar(int startX, int startY, int goalX, int goalY) {
    priority_queue<Node, vector<Node>, greater<Node>> openSet; // Hàng đợi ưu tiên
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));

    openSet.push({startX, startY, 0, heuristic(startX, startY, goalX, goalY), 0, -1, -1});
    
    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        int x = current.x, y = current.y;
        if (visited[x][y]) continue;
        visited[x][y] = true;
        parent[x][y] = {current.parent_x, current.parent_y};

        // Nếu tìm thấy đích, truy vết đường đi
        if (x == goalX && y == goalY) {
            vector<pair<int, int>> path;
            vector<char> pathdirection;
            while (x != startX || y != startY) {
                path.push_back({x, y});
                int px = parent[x][y].first;
                int py = parent[x][y].second;

                for (int i = 0; i < 4; i++) {
                    if (px + dx[i] == x && py + dy[i] == y) {
                        pathdirection.push_back(direction[i]);
                        break;
                    }
                }
                x = px;
                y = py;
            }
            path.push_back({startX, startY});
            reverse(path.begin(), path.end());
            reverse(pathdirection.begin(), pathdirection.end());

            // In đường đi
            cout << "PATH FOUND!\n";
            ofstream outfile("direction.txt");
            for (auto step : path) {
                cout << "(" << step.first << "," << step.second << ") -> ";
            }
            cout << "END\nDIRECTION: ";
            for (char dir : pathdirection) {
                cout << dir << " ";
                outfile << dir;
            }
            cout << endl;

            return true;
        }

        // Mở rộng các ô kề
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];

            if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS && maze[nx][ny] == PATH && !visited[nx][ny]) {
                int new_g = current.g + 1;
                int new_h = heuristic(nx, ny, goalX, goalY);
                int new_f = new_g + new_h;
                openSet.push({nx, ny, new_g, new_h, new_f, x, y});
            }
        }
    }

    cout << "NO PATH FOUND\n";
    return false;
}

// Hàm đọc mê cung từ file
void readMazefile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    file >> ROWS >> COLS;
    maze.resize(ROWS, vector<int>(COLS));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            file >> maze[i][j];
        }
    }

    file.close();
}

int main() {
    readMazefile("test2_thinh.txt");

    int startX = 0, startY = 0;
    int goalX = -1, goalY = -1;

    // Tìm mục tiêu (goal) trong hàng cuối cùng
    for (int j = 0; j < COLS; j++) {
        if (maze[ROWS-1][j] == PATH) {
            goalX = ROWS - 1;
            goalY = j;
            break;
        }
    }

    if (goalX == -1 || goalY == -1) {
        cout << "No goal found in the last row!" << endl;
        return 0; // Không có mục tiêu trong hàng cuối cùng
    }

    if (maze[startX][startY] == WALL) {
        bool found = false;

        // Dò từ trên xuống theo cột 0
        for (int i = 1; i < ROWS && !found; i++) {
            if (maze[i][0] == PATH) { // Nếu tìm thấy đường đi
                startX = i;
                startY = 0;
                found = true;
            }
        }
    }

    AStar(startX, startY, goalX, goalY);

    return 0;
}


