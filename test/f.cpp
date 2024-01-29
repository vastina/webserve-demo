// #include <iostream>
// #include <filesystem>

// namespace fs = std::filesystem;

// void listFiles(const fs::path& directory) {
//     for (const auto& entry : fs::directory_iterator(directory)) {
//         if (entry.is_directory()) {
//             listFiles(entry.path());  // 递归遍历子目录
//         } else if (entry.is_regular_file()) {
//             std::cout << entry.path().string() << std::endl;  // 打印文件名
//         }
//     }
// }

// int main() {
//     fs::path currentPath = fs::current_path();  // 获取当前路径

//     std::cout << "Files in " << currentPath.string() << " and its subdirectories:" << std::endl;
    
//     listFiles(currentPath);  // 开始遍历

//     return 0;
// }
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void listFiles(const fs::path& directory, const fs::path& relativePath = fs::path()) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            fs::path newRelativePath = relativePath / entry.path().filename();
            listFiles(entry.path(), newRelativePath);  // 递归遍历子目录
        } else if (entry.is_regular_file()) {
            std::cout << (relativePath / entry.path().filename()).string() << std::endl;  // 打印相对路径
        }
    }
}

int main() {
    fs::path currentPath = fs::current_path();  // 获取当前路径

    std::cout << "Files in " << currentPath.string() << " and its subdirectories:" << std::endl;
    
    listFiles(currentPath);  // 开始遍历

    return 0;
}
