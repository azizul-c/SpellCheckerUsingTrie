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
    void searchPrefix(std::string prefix);
    bool erase(std::string word);
    void printWords();
    void spellcheck(std::string word);
    bool isEmpty();
    bool clear();
    void size();

private:
    Node *findNode(std::string word);
    void inOrderTraversal(Node *currNode, std::string prefix);
    Node *root;
    Node *currentNode;
    Node *nullNode;
    char currentCharacter;
    int countPrefixInstances;
};

class Node
{
public:
    Node();
    Node(char c);
    char getCharacter();
    bool isEndOfWord();
    bool isNodeValid();
    void setCharAsEndOfWord(bool lastLetter);

    Node **children;

private:
    char character;
    bool endOfWord;
    bool validNode;
};