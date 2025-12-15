//
// A bitwise logical expression is much like an arithmetic expression except
// that the operators are ! (not), ~ (complement), & (and), | (or), and ^
// (exclusive or). Each operator does its operation to each bit of its
// integer operands (see §25.5). ! and ~ are prefix unary operators. A ^
// binds tighter than a | (just as * binds tighter than +) so that x|y^z
// means x|(y^z) rather than (x|y)^z. The & operator binds tighter than ^ so
// that x^y&z means x^(y&z).
//

/*
  Grammar of a bitwise logical expression.

  Expression:
    Term
    Expression "|" Term
  Term:
    Subterm
    Term "^" Subterm
  Subterm:
    Primary
    Subterm "&" Primary
  Primary:
    Number
    "(" Expression ")"
    "!" Primary
    "~" Primary
  Number:
    integer_literal
*/

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <exception>
#include <iostream>
#include <istream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using Integer = uint32_t;
using Char = char;
using String = std::string;
using Istream = std::istream;
constexpr auto Ch_Zero = '0';

using Value = std::variant<Integer, String, std::monostate>;

class Token
{
  public:
    enum class Type
    {

        QUIT,
        PRINT,
        BIN_PRINT,

        LET,
        CONST,
        SET,
        NAME,

        INTEGER,

        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_NOT,

        NOT,

        LPAR,
        RPAR,
    };

    Token(Type type) : type(type), value(std::monostate())
    {}

    Token(Type type, Value value) : type(type), value(value)
    {}

    Type type;
    Value value;
};

class Token_Stream
{
  private:
    Istream& istream;
    std::optional<Token> buffer;

    static inline bool isvarallow(Char character)
    {
        return std::isalpha(character) or std::isdigit(character) or character == '_';
    }

    Token parse()
    {
        Char character;
        istream >> character;

        switch (character)
        {
        case 'q':
            return Token(Token::Type::QUIT);
        case ';':
            return Token(Token::Type::PRINT);
        case ':':
            return Token(Token::Type::BIN_PRINT);

        case '&':
            return Token(Token::Type::BITWISE_AND);
        case '|':
            return Token(Token::Type::BITWISE_OR);
        case '^':
            return Token(Token::Type::BITWISE_XOR);
        case '~':
            return Token(Token::Type::BITWISE_NOT);

        case '!':
            return Token(Token::Type::NOT);

        case '(':
            return Token(Token::Type::LPAR);
        case ')':
            return Token(Token::Type::RPAR);

        default:
            if (std::isdigit(character))
            {
                auto value = String();

                value += character;

                while (istream.get(character) and isvarallow(character))
                {
                    if (character == '_') { continue; }
                    value += character;
                }

                istream.putback(character);

                auto number = Integer();

                if (value[1] == 'b') { number = std::stoull(std::string(value.begin() + 2, value.end()), nullptr, 2); }
                else if (value[1] == 'x')
                {
                    number = std::stoull(std::string(value.begin() + 2, value.end()), nullptr, 16);
                }
                else { number = std::stoull(std::string(value.begin(), value.end()), nullptr, 10); }

                return Token(Token::Type::INTEGER, number);
            }

            if (isvarallow(character))
            {
                auto value = String();

                value += character;

                while (istream.get(character) and isvarallow(character))
                {
                    value += character;
                }

                istream.putback(character);

                if (value == "let") { return Token(Token::Type::LET); }
                else if (value == "const") { return Token(Token::Type::CONST); }
                else if (value == "set") { return Token(Token::Type::SET); }

                if (not std::all_of(value.begin(), value.end(), isvarallow))
                {
                    throw std::runtime_error("wrong let name");
                }

                return Token(Token::Type::NAME, value);
            }

            throw std::runtime_error("bad token");
        };
    }

  public:
    Token_Stream(Istream& istream) : istream(istream)

    {}

    Token get()
    {
        if (buffer)
        {
            auto temp = *buffer;
            buffer.reset();
            return temp;
        }

        return parse();
    }

    void putback(Token token)
    {
        if (buffer) { throw std::runtime_error("buffer isn't empty"); }
        buffer = token;
    }
};

class Parser
{
    Integer primary()
    {
        auto token = token_stream.get();

        if (token.type == Token::Type::INTEGER) { return std::get<Integer>(token.value); }
        if (token.type == Token::Type::NAME)
        {
            auto name = std::get<String>(token.value);

            if (not variables.contains(name)) { throw std::runtime_error("undefined variable"); }

            return variables[name].second;
        }

        if (token.type == Token::Type::BITWISE_NOT) { return ~primary(); }
        if (token.type == Token::Type::NOT) { return !primary(); }

        if (token.type == Token::Type::LPAR)
        {
            auto value = primary();

            token = token_stream.get();
            if (token.type != Token::Type::RPAR) { throw std::runtime_error("')' excepted"); }

            return value;
        }

        throw std::runtime_error("primary excepted");
    }

    Integer subterm()
    {
        auto left = primary();
        auto token = token_stream.get();

        if (token.type == Token::Type::BITWISE_XOR) { return left ^ primary(); }

        token_stream.putback(token);
        return left;
    }

    Integer term()
    {
        auto left = subterm();
        auto token = token_stream.get();

        if (token.type == Token::Type::BITWISE_AND) { return left & subterm(); }

        token_stream.putback(token);
        return left;
    }

    Integer expression()
    {
        auto left = term();
        auto token = token_stream.get();

        if (token.type == Token::Type::BITWISE_OR) { return left | term(); }

        token_stream.putback(token);
        return left;
    }

    void define_variable()
    {
        auto token = token_stream.get();
        auto is_mutable = token.type == Token::Type::LET;

        token = token_stream.get();
        if (token.type != Token::Type::NAME) { throw std::runtime_error("name of variable excepted"); }

        auto name = std::get<String>(token.value);

        if (variables.contains(name)) { throw std::runtime_error("variable is already defined"); }

        token = token_stream.get();

        if (not is_mutable and token.type != Token::Type::INTEGER)
        {
            throw std::runtime_error("excepted value for const");
        }

        variables.insert({name, {is_mutable, std::get<Integer>(token.value)}});
    }

    void set_variable()
    {
        auto token = token_stream.get();

        if (token.type != Token::Type::NAME) { throw std::runtime_error("name of variable excepted"); }

        auto name = std::get<String>(token.value);

        token = token_stream.get();

        if (token.type != Token::Type::INTEGER) { throw std::runtime_error("value of variable excepted"); }

        if (not variables[name].first) { throw std::runtime_error("can't set const value"); }

        variables[name] = {true, std::get<Integer>(token.value)};
    }

    Token_Stream& token_stream;
    std::map<String, std::pair<bool, Integer>>& variables;

  public:
    Parser(Token_Stream& token_stream, std::map<String, std::pair<bool, Integer>>& variables)
        : token_stream(token_stream), variables(variables)
    {}

    Integer parse()
    {
        auto token = token_stream.get();

        if (token.type == Token::Type::LET or token.type == Token::Type::CONST)
        {
            token_stream.putback(token);
            define_variable();

            return Integer();
        }
        else if (token.type == Token::Type::SET)
        {
            set_variable();
            return Integer();
        }
        else { token_stream.putback(token); }

        return expression();
    }
};

int main()
try
{
    auto bin = [](Integer value)
    {
        auto result = String();
        while (value > 0)
        {
            result = Char((value % 2) + Ch_Zero) + result;
            value /= 2;
        };
        return result;
    };

    auto token_stream = Token_Stream(std::cin);
    auto variables = std::map<String, std::pair<bool, Integer>>();

    auto parser = Parser(token_stream, variables);

    auto value = Integer();

    auto buffer = std::vector<Token>();

    while (std::cin)
    {
        try
        {
            auto token = token_stream.get();

            if (token.type == Token::Type::QUIT) { break; }
            if (token.type == Token::Type::PRINT) { std::cout << "= " << value << '\n'; }
            else if (token.type == Token::Type::BIN_PRINT) { std::cout << "= " << bin(value) << '\n'; }
            else
            {
                token_stream.putback(token);
                value = parser.parse();
            }
        }
        catch (std::exception& while_error)
        {
            std::cout << while_error.what() << '\n';
        }
    }
}
catch (std::exception& e)
{
    std::cerr << "error: " << e.what() << '\n';
    return 1;
}
catch (...)
{
    std::cerr << "Oops: unknown exception!\n";
    return 2;
}
