#ifndef PARASL_PARSER_H_
#define PARASL_PARSER_H_

#include "ast/LayerNode.h"
#include <istream>
#include "../lexer/lexer.h"
#include "../utils/macros.h"
#include <string>
#include <vector>


namespace frontend::parser {
class Driver {
public:
    Driver() : location(0) {}
    virtual ~Driver() noexcept override = default;

    /** 
     * parse - parse from a file
     * @param filename - valid string with input file
     */
    // void Parse(const char *filename);
    /** 
     * parse - parse from a C++ input stream
     * @param iss - std::istream&, valid input stream
     */
    // void Parse(std::istream &iss);

    int Parse() {
        return syntax_parser.parse();
    }

    friend class Parser;
    friend class lexer::Lexer;

private:
    // void parseHelper(std::istream &stream);

    void addLayer(LayerNode *layer) {
        ASSERT(layer);
        layers.push_back(layer);
    }
    LayerNode *getLastLayer() {
        return layers.back();
    }

    void increaseLocation(unsigned int loc) {
        location += loc;
    }

    lexer::Lexer scanner;
    Parser syntax_parser;
    std::vector<LayerNode *> layers;
    unsigned int location;
};
}   // namespace frontend::parser

#endif /* END PARASL_PARSER_H_ */
