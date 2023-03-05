#include <iostream>
#include "SpellChecker.hpp"

/*
===================================================================
===  Node Functions Definitions ===
===================================================================
*/
Node::Node()
{
    validNode = false;
}

Node::Node(char c)
{
    character = c;
    endOfWord = false;
    validNode = true;
    children = new Node *[26];
}

char Node::getCharacter()
{
    return character;
}

bool Node::isEndOfWord()
{
    return endOfWord;
}

bool Node::isNodeValid()
{
    return validNode;
}

void Node::setCharAsEndOfWord(bool lastLetter)
{
    endOfWord = lastLetter;
}

/*
===================================================================
===  Trie Functions Definitions ===
===================================================================
*/

Trie::Trie()
{
    root = new Node('\0');
    nullNode = new Node();
}

bool Trie::insert(std::string word)
{
    currentNode = root;

    for (int i = 0; i < word.length(); i++)
    {
        currentCharacter = word.at(i);

        if (currentNode->children[currentCharacter - 'A']->isNodeValid() == false)
        {
            // ASCII value of A = 65, B = 66, C = 67, and so on
            // By subtracting 'A', we subtract 65 from the ASCII value of the current character
            // which tells us its position in the alphabet (A = 0, B = 1, C = 2, so on)

            // Create a new node for the current letter of the word, if there isn't a valid node for that letter yet
            currentNode->children[currentCharacter - 'A'] = new Node(currentCharacter);

            // Update the current node to be the current letter of the word
            currentNode = currentNode->children[currentCharacter - 'A'];
        }
    }

    // currentNode will be the last letter of the word when the for loop ends, so mark it as the end of the word
    currentNode->setCharAsEndOfWord(true);
}

Node *Trie::findNode(std::string word)
{
    currentNode = root;

    for (int i = 0; i < word.length(); i++)
    {
        currentCharacter = word.at(i);

        if (currentNode->children[currentCharacter - 'A']->isNodeValid() == false)
        {
            // if at any point in the traversal, an expected letter of the word doesn't exist in the trie,
            // then the word isn't in the trie

            return nullNode;
        }

        currentNode = currentNode->children[currentCharacter - 'A'];
    }

    // by the end of the for loop, the word is guaranteed to be in the trie, so we return the node for the last letter of the word
    return currentNode;
}

void Trie::searchPrefix(std::string prefix)
{
    countPrefixInstances = 0;

    if (findNode(prefix)->isNodeValid()) // if the prefix exists
    {
        inOrderTraversal(findNode(prefix), prefix);

        std::cout << "count is " << countPrefixInstances << "\n";
    }
    else
    {
        std::cout << "not found\n";
    }
}

void Trie::inOrderTraversal(Node *currNode, std::string prefix)
{
    // Traverse Left subtree

    // Check if current node is end of a word

    // Traverse right subtree
}

bool Trie::isEmpty()
{
    // check that the root has no valid children nodes (they would exist if there are words)
    for (int i = 0; i < 26; i++)
    {
        if (root->children[i]->isNodeValid() == true)
        {
            return false;
        }
    }

    return true;
}

/*
===================================================================
===  illegal_exception Functions Definitions ===
===================================================================
*/