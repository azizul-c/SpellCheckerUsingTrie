#include <iostream>
#include "SpellChecker.hpp"

/*
===================================================================
===  Node Functions Definitions ===
===================================================================
*/
Node::Node()
{
    // Empty constructor
}

Node::Node(char c)
{
    character = c;
    endOfWord = false;
    children = new Node[26];
}

char Node::getCharacter()
{
    return character;
}

bool Node::isEndOfWord()
{
    return endOfWord;
}

/*
===================================================================
===  Trie Functions Definitions ===
===================================================================
*/

Trie::Trie()
{
    root = new Node("0");
}

/*
===================================================================
===  illegal_exception Functions Definitions ===
===================================================================
*/