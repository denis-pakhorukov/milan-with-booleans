#ifndef CMILAN_SCANNER_H
#define CMILAN_SCANNER_H

#include <fstream>
#include <string>
#include <map>

using namespace std;

enum Token {
    T_EOF,          // Конец текстового потока
    T_ILLEGAL,      // Признак недопустимого символа
    T_IDENTIFIER,   // Идентификатор
    T_NUMBER,       // Целочисленный литерал
    T_BEGIN,        // Ключевое слово "begin"
    T_END,          // Ключевое слово "end"
    T_IF,           // Ключевое слово "if"
    T_THEN,         // Ключевое слово "then"
    T_ELSE,         // Ключевое слово "else"
    T_FI,           // Ключевое слово "fi"
    T_WHILE,        // Ключевое слово "while"
    T_DO,           // Ключевое слово "do"
    T_OD,           // Ключевое слово "od"
    T_WRITE,        // Ключевое слово "write"
    T_READ,         // Ключевое слово "read"
    T_FALSE,        // Ключевое слово "false"
    T_TRUE,         // Ключевое слово "true"
    T_ASSIGN,       // Оператор ":="
    T_ADDOP,        // Сводная лексема для "+" и "-" (операция типа сложения)
    T_MULOP,        // Сводная лексема для "*" и "/" (операция типа умножения)
    T_BITWISEANDOP, // Лексема для "&" (побитовое И)
    T_BITWISEOROP,  // Лексема для "|" (побитовое ИЛИ)
    T_LOGICALANDOP, // Лексема для "&&" (логическое И)
    T_LOGICALOROP,  // Лексема для "||" (побитовое ИЛИ)
    T_LOGICALNOTOP, // Лексема для "!" (логическое НЕ)
    T_CMP,          // Сводная лексема для операторов отношения
    T_LPAREN,       // Открывающая скобка
    T_RPAREN,       // Закрывающая скобка
    T_SEMICOLON     // ";"
};

// Функция tokenToString возвращает описание лексемы.
// Используется при печати сообщения об ошибке.
const char *tokenToString(Token t);

// Виды операций сравнения
enum Cmp {
    C_EQ = 0,    // Операция сравнения "="
    C_NE = 1,    // Операция сравнения "!="
    C_LT = 2,    // Операция сравнения "<"
    C_GT = 3,    // Операция сравнения ">"
    C_LE = 4,    // Операция сравнения "<="
    C_GE = 5     // Операция сравнения ">="
};

// Виды арифметических операций
enum Arithmetic {
    A_PLUS,        // Операция "+"
    A_MINUS,       // Операция "-"
    A_MULTIPLY,    // Операция "*"
    A_DIVIDE       // Операция "/"
};

// Лексический анализатор
class Scanner {
public:
    // Конструктор. В качестве аргумента принимает имя файла и поток,
    // из которого будут читаться символы транслируемой программы.
    explicit Scanner(const string &fileName, istream &input)
            : fileName_(fileName), lineNumber_(1), input_(input) {
        keywords_["begin"] = T_BEGIN;
        keywords_["end"] = T_END;
        keywords_["if"] = T_IF;
        keywords_["then"] = T_THEN;
        keywords_["else"] = T_ELSE;
        keywords_["fi"] = T_FI;
        keywords_["while"] = T_WHILE;
        keywords_["do"] = T_DO;
        keywords_["od"] = T_OD;
        keywords_["false"] = T_FALSE;
        keywords_["true"] = T_TRUE;
        keywords_["write"] = T_WRITE;
        keywords_["read"] = T_READ;

        nextChar();
    }

    // Деструктор
    virtual ~Scanner() = default;

    // getters всех private переменных
    const string &getFileName() const {
        return fileName_;
    }

    int getLineNumber() const {
        return lineNumber_;
    }

    Token token() const {
        return token_;
    }

    int getIntValue() const {
        return intValue_;
    }

    string getStringValue() const {
        return stringValue_;
    }

    Cmp getCmpValue() const {
        return cmpValue_;
    }

    Arithmetic getArithmeticValue() const {
        return arithmeticValue_;
    }

    // Переход к следующей лексеме.
    // Текущая лексема записывается в token_ и изымается из потока.
    void nextToken();

private:

    // Пропуск всех пробельные символы.
    // Если встречается символ перевода строки, номер текущей строки
    // (lineNumber) увеличивается на единицу.
    void skipSpace();

    void nextChar(); // Переходит к следующему символу

    // Проверка символа на возможность быть началом имени переменной
    bool isIdentifierStart(char c) {
        return ((c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z'));
    }

    // Проверка символа на возможность входить в имя переменной.
    // Эта проверка не используется для определения того, может ли символ быть первым в имени переменной.
    bool isIdentifierBody(char c) {
        return isIdentifierStart(c) || isdigit(c);
    }

    const string fileName_;            // Входной файл
    int lineNumber_;                   // Номер текущей строки кода

    Token token_;                      // Текущая лексема
    int intValue_{};                   // Значение текущего целого
    string stringValue_;               // Имя переменной
    Cmp cmpValue_;                     // Значение оператора сравнения (>, <, =, !=, >=, <=)
    Arithmetic arithmeticValue_;       // Значение арифметического оператора (+, -, *, /)

    map<string, Token> keywords_; /* Ассоциативный массив с лексемами и соответствующими им
                                     зарезервированными словами в качестве индексов */

    istream &input_;                   // Входной поток для чтения из файла.
    char ch_;                          // Текущий символ
};

#endif
