#include "driver.h"
#include <fstream>


namespace frontend::parser {
// void Driver::Parse(const char *filename) {
//     ASSERT(filename != nullptr);
//     std::ifstream stream(filename);
//     parseHelper(stream);
// }

// void Driver::Parse(std::istream &stream) {
//     if (!stream.good() && stream.eof()) {
//         return;
//     }
//     parseHelper(stream); 
// }

// void Driver::parseHelper(std::istream &stream) {
//     ASSERT(stream.good());
//     delete scanner;
//     try {
//         scanner = new MC::MC_Scanner(&stream);
//     } catch (std::bad_alloc &ba) {
//         std::cerr << "Failed to allocate scanner: (" <<
//             ba.what() << "), exiting!!\n";
//         exit(EXIT_FAILURE);
//     }
    
//     delete syntax_parser; 
//     try {
//         syntax_parser = new lexer::Lexer(*scanner, *this);
//     } catch (std::bad_alloc &ba) {
//         std::cerr << "Failed to allocate parser: (" << 
//             ba.what() << "), exiting!!\n";
//         exit(EXIT_FAILURE);
//     }

//     const int accept = 0;
//     if (syntax_parser->parse() != accept) {
//         std::cerr << "Parse failed!!\n";
//     }
// }
}   // namespace frontend::parser
