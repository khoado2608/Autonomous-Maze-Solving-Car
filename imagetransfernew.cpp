#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "lodepng.h"
using namespace std;

// Chuyển ảnh PNG thành ma trận mê cung với kích thước grid tuỳ chọn
vector<vector<int>> imageToMazeMatrix(
    const string& filename,
    unsigned gridCols,
    unsigned gridRows
) {
    vector<unsigned char> image;
    unsigned width, height;

    // Đọc ảnh PNG
    unsigned error = lodepng::decode(image, width, height, filename);
    if (error) {
        throw runtime_error("PNG decode error: " + string(lodepng_error_text(error)));
    }

    // Tính kích thước mỗi ô
    // Chia đều: ô cuối cùng có thể lớn hơn để bao phủ hết ảnh
    unsigned cellW = width  / gridCols;
    unsigned cellH = height / gridRows;
    if (cellW == 0 || cellH == 0)
        throw runtime_error("Grid size too large for image dimensions");

    cout << "Image: " << width << "x" << height
         << ", Grid: " << gridCols << "x" << gridRows
         << ", Cell: " << cellW    << "x" << cellH << "\n";

    vector<vector<int>> maze(gridRows, vector<int>(gridCols, 0));

    for (unsigned gy = 0; gy < gridRows; ++gy) {
        // Giới hạn xuống dưới (để ô cuối cùng lấy tới hết)
        unsigned y0 = gy * cellH;
        unsigned y1 = (gy == gridRows-1 ? height : y0 + cellH);

        for (unsigned gx = 0; gx < gridCols; ++gx) {
            unsigned x0 = gx * cellW;
            unsigned x1 = (gx == gridCols-1 ? width : x0 + cellW);

            // Đếm pixel tối
            unsigned blackCount = 0;
            unsigned totalPix   = (y1 - y0) * (x1 - x0);

            for (unsigned y = y0; y < y1; ++y) {
                unsigned rowBase = 4 * (y * width);
                for (unsigned x = x0; x < x1; ++x) {
                    unsigned idx = rowBase + 4*x;
                    float lum = 0.299f*image[idx]
                              + 0.587f*image[idx+1]
                              + 0.114f*image[idx+2];
                    if (lum < 128.0f) ++blackCount;
                }
            }

            maze[gy][gx] = (blackCount * 2 >= totalPix) ? 1 : 0;
        }
    }

    return maze;
}

// In ma trận mê cung ra màn hình
void printMaze(const vector<vector<int>>& maze) {
    for (auto& row : maze) {
        for (int cell : row) cout << cell << ' ';
        cout << '\n';
    }
}

// Lưu ma trận mê cung vào file
void saveMazeToFile(const vector<vector<int>>& maze, const string& filename) {
    ofstream file(filename);
    if (!file) throw runtime_error("Cannot open output file");

    unsigned rows = maze.size(), cols = maze[0].size();
    file << rows << ' ' << cols << '\n';
    for (auto& row : maze) {
        for (int cell : row) file << cell << ' ';
        file << '\n';
    }
    cout << "Saved maze to " << filename << " (" 
         << rows << "x" << cols << ")\n";
}

int main() {
    try {
        string  inputImage, outputText;
        unsigned gridCols, gridRows;

        // Nhập tham số từ người dùng
        cout << "Input PNG file: ";     getline(cin, inputImage);
        cout << "Output TXT file: ";    getline(cin, outputText);
        cout << "Grid cols (output width): ";  cin >> gridCols;
        cout << "Grid rows (output height): "; cin >> gridRows;

        auto maze = imageToMazeMatrix(inputImage, gridCols, gridRows);
        cout << "Resulting maze:\n";
        printMaze(maze);
        saveMazeToFile(maze, outputText);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
