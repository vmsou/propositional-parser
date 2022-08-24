#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct TextData {
    std::size_t size;
    std::vector<std::string> texts;
    
    void read(const std::string& filename);
    static TextData load(const std::string& filename);
};

std::ostream& operator<<(std::ostream& os, const TextData& td);
std::istream& operator>>(std::istream& is, TextData& td);
