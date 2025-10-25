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
        BlockNodePtr parse();

    private:
        std::optional<TokenData> peek();
        TokenData look();
        bool more();

        void push();
        void pop(bool replace = true);
        
        void ignoreWhitespace(bool ignore);
        void passAllWhitespace();

        TokenData consume(std::optional<Token::Type> expected = std::nullopt);

        BlockNodePtr parseBlock();
        NodePtr parseStatement();
        NodePtr parseIdentifier();
        
        std::unique_ptr<CallNode> parseCall(NodePtr&& left);
        NodePtr parseIndexOrSlice(NodePtr&& left);
        std::unique_ptr<PropertyAccessNode> parsePropertyAccess(NodePtr&& left);

        std::vector<NodePtr> parseExpressionList(Token::Type end_token);
        std::vector<std::string> parseIdentifierList();
        
        NodePtr parseIdentifierOrTuple();
        NodePtr parseAssignmentOrExpression();
        
        std::unique_ptr<AssignNode> parseAssignment();
        
        NodePtr parseExpression();
        NodePtr parseTupleExpression();
        NodePtr parseTernary();
        NodePtr parseBooleanOps();
        NodePtr parseBooleanNot();
        NodePtr parseComparison();
        NodePtr parseAdditive();
        NodePtr parseMultiplicative();
        NodePtr parseUnary();
        NodePtr parseAtom();

        std::unique_ptr<ListLiteralNode> parseListLiteral();
        NodePtr parseDictOrSetLiteral();

        std::unique_ptr<IfNode> parseIf();
        std::unique_ptr<WhileNode> parseWhile();
        std::unique_ptr<ForNode> parseFor();

        std::unique_ptr<FunctionDefinitionNode> parseDef();
        std::unique_ptr<ReturnNode> parseReturn();

    private:
        static inline std::set<Token::Type> WHITESPACE {
            Token::Type::NEWLINE, Token::Type::INDENT, Token::Type::DEDENT
        };

        static std::optional<Token::Type> optType(Token::Type type) {
            return std::optional<Token::Type>(type);
        }

        static std::string unescapeString(std::string str);
};