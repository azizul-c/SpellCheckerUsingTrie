#pragma once

// Class Declarations
class Trie;
class Node;
class illegal_exception;

/*
==========================
=== Class Definitions ===
==========================
*/
class Trie
{
public:
    Trie();
    bool insert(std::string word);
    bool searchPrefix(std::string prefix);
    bool erase(std::string word);
    void printWords();
    void spellcheck(std::string word);
    bool isEmpty();
    bool clear();
    void size();

private:
};

class Node
{
public:
    Node();
    Node(char c);
    char getCharacter();
    bool isEndOfWord();

private:
    char character;
    bool endOfWord;
    Node *children;
};