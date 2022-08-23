#include "reading.hpp"

TextData TextData::load(const std::string& filename) {
    std::ifstream ifs{filename};
    if (!ifs) throw std::runtime_error{"Não foi possivel abrir o arquivo."};
    
    TextData td;
    if (!(ifs >> td)) throw std::runtime_error("Arquivo inválido.");
    return td;
}

std::ostream& operator<<(std::ostream& os, const TextData& td) {
    os << td.size;
    for (std::size_t i = 0; i < td.size; ++i) {
        const std::string& text = td.texts[i];
        os << text;
        if (i < td.size - 1) os << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, TextData& td) {
    std::string line;
    std::getline(is, line);
    std::size_t size = 0;

    // FAIL: Invalid Size
    try { size = std::stoi(line); } 
    catch (const std::invalid_argument& err) { is.setstate(std::ios_base::failbit); }
    if (size < 0) is.setstate(std::ios_base::failbit);

    // Push lines to vector
    td.size = size;
    while (size > 0 && std::getline(is, line)) { 
        td.texts.push_back(line); 
        --size;    
    }
    return is;
}
