#pragma once
#include <vector>

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
    ~Trie();
    bool insert(std::string word);
    void searchPrefix(std::string prefix);
    bool erase(std::string word);
    void printWords();
    void spellcheck(std::string word);
    void isEmpty();
    bool clear();
    void size();

private:
    Node *findNode(std::string word);
    void inOrderTraversalForPrefix(Node *currNode);
    void inOrderTraversalForClear(Node *currNode);
    Node *root;
    Node *currentNode;
    Node *nullNode;
    char currentCharacter;
    int countPrefixInstances;
    int numberOfWords;
    Node **nodesToErase;
    std::vector<Node *> allNodesInTrie;
};

class Node
{
public:
    Node();
    ~Node();
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