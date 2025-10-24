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
    std::vector<size_t> poses;
    bool ignore_whitespace;

    public:
        Parser(Tokens tokens);
        BlockNode parse();

    private:
        std::optional<TokenData> peek();
        TokenData look();
        bool more();

        void push();
        void pop(bool replace = true);
        
        void ignoreWhitespace(bool ignore);
        void passAllWhitespace();

        TokenData consume(std::optional<TokenType> expected = std::nullopt);

        BlockNode parseBlock();
        Node parseStatement();
        Node parseIdentifier();
        
        CallNode parseCall(Node& left);
        Node parseIndexOrSlice(Node& left);
        PropertyAccessNode parsePropertyAccess(Node& left);

        std::vector<Node> parseExpressionList(TokenType end_token);
        std::vector<std::string> parseIdentifierList();
        
        Node parseAssignmentOrExpression();
        
        AssignNode parseAssignment();
        
        Node parseExpression();
        Node parseTernary();
        Node parseBooleanOps();
        Node parseBooleanNot();
        Node parseComparison();
        Node parseAdditive();
        Node parseMultiplicative();
        Node parseUnary();
        Node parseAtom();

        Node parseListLiteral();
        Node parseDictOrSetLiteral();

        IfNode parseIf();
        WhileNode parseWhile();
        ForNode parseFor();

        FunctionDefinitionNode parseDef();
        ReturnNode parseReturn();

    private:
        static inline std::set<TokenType> WHITESPACE {
            TokenType::t_NEWLINE, TokenType::t_INDENT, TokenType::t_DEDENT
        };

        static std::optional<TokenType> optType(TokenType type) {
            return std::optional<TokenType>(type);
        }

        static std::string unescapeString(std::string str);
};