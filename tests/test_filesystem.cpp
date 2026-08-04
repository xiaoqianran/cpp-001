#include "common/FilesystemUtils.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::string cwd = common::current_working_directory();
    assert(!cwd.empty());
    assert(cwd.length() > 1);

    const std::string tmp = "test_fs_tmp.txt";
    {
        std::ofstream out(tmp);
        out << "hello-fs";
    }
    assert(common::file_exists(tmp));
    auto content = common::read_text_file(tmp);
    assert(content.is_ok());
    assert(content.value() == "hello-fs");

    auto missing = common::read_text_file("no_such_file_xyz.txt");
    assert(missing.is_err());

    std::remove(tmp.c_str());

    std::cout << "test_filesystem: cwd=" << cwd << "\n";
    std::cout << "test_filesystem: all assertions passed\n";
    return 0;
}
