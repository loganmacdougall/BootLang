#include <set>
#include <map>
#include <vector>
#include <optional>
#include "tokenizer.hpp"
#include "nodes/all_nodes.hpp"

class Parser {
    typedef std::vector<TokenData> Tokens;

    Tokens tokens;
    size_t pos;
    bool ignore_whitespace;

    public:
        Parser(Tokens tokens);

    private:
        std::optional<TokenData> peek();
        TokenData look();
        bool more();
        
        void ignoreWhitespace(bool ignore);
        void passAllWhitespace();

        TokenData consume(std::optional<TokenType> expected = std::nullopt);

        BlockNode parse();

    private:
        static inline std::set<TokenType> WHITESPACE {
            TokenType::t_NEWLINE, TokenType::t_INDENT, TokenType::t_DEDENT
        };

        static inline std::map<char, std::string> UNESCAPE_MAP {
            {'\'', "'"}, {'\"', "\""}, {'\\', "\\"}, {'n', "\n"},
            {'r', "\r"}, {'t', "\t"},  {'b', "\b"},  {'f', "\f"},
        };
};