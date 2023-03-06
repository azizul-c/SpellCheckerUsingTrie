#include <iostream>
#include <vector>
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

Node::~Node()
{
    // validNode = false;
    // character = '\0';
    // delete children;
}

Node::Node(char c)
{
    character = c;
    endOfWord = false;
    validNode = true;

    children = new Node *[26];

    for (int i = 0; i < 26; i++)
    {
        children[i] = new Node();
    }
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

void Node::modifyNodeValidity(bool modification)
{
    validNode = modification;
}

void Node::setCharacter(char c)
{
    character = c;
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
    numberOfWords = 0;
}

Trie::~Trie()
{
    delete root;
    delete nullNode;
    delete nodesToErase;

    // need to traverse the tree and delete everything too! (Call clear())
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
        }

        // Update the current node to be the current letter of the word
        currentNode = currentNode->children[currentCharacter - 'A'];
    }

    // currentNode will be the last letter of the word when the for loop ends, so mark it as the end of the word
    currentNode->setCharAsEndOfWord(true);
    numberOfWords++;

    return true;
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
        inOrderTraversalForPrefix(findNode(prefix));

        std::cout << "count is " << countPrefixInstances << "\n";
    }
    else
    {
        std::cout << "not found\n";
    }
}

void Trie::inOrderTraversalForPrefix(Node *currNode)
{
    // Base case
    if (currNode->isNodeValid() == false)
    {
        return;
    }

    // Traverse Left subtree
    if (currNode->children[0]->isNodeValid() == true)
    {
        inOrderTraversalForPrefix(currNode->children[0]);
    }

    // Check if current node is end of a word
    if (currNode->isEndOfWord())
    {
        countPrefixInstances++;
    }

    // Traverse right subtrees
    for (int i = 1; i < 26; i++)
    {
        if (currNode->children[i]->isNodeValid() == true)
        {
            inOrderTraversalForPrefix(currNode->children[i]);
        }
    }
}

bool Trie::erase(std::string word)
{
    // using a for loop, traverse down the trie until end of the given word
    // during the traversal, accumulate all the nodes that comprise the word into an array
    // iterate from the end of the array, and delete all the nodes until you reach another "endOfWord" node
    // (or if there's no other endOfWord, delete all the nodes)

    if (findNode(word)->isEndOfWord()) // if the word exists
    {
        nodesToErase = new Node *[word.length()];
        currentNode = root;

        for (int i = 0; i < word.length(); i++)
        {
            currentCharacter = word.at(i);
            currentNode = currentNode->children[currentCharacter - 'A'];
            nodesToErase[i] = currentNode;
        }

        for (int j = word.length() - 1; j >= 0; j--)
        {
            // if we come across another endOfWord besides the end of the word being erased
            if (nodesToErase[j]->isEndOfWord() && j != word.length() - 1)
            {
                delete nodesToErase;
                return true; // don't delete the rest of the letters, because they belong to another word
            }

            delete nodesToErase[j];
        }

        delete nodesToErase;
        numberOfWords--;
        return true;
    }
    else
    {
        return false; // failure
    }
}

bool Trie::clear()
{
    // use an in order traversal. accumulate all the nodes into a vector.
    // go thru the vector and delete all the nodes

    if (numberOfWords > 0)
    {
        inOrderTraversalForClear(root);

        for (int i = allNodesInTrie.size() - 1; i > 0; i--)
        {
            // allNodesInTrie[0] has the root, we dont want to override it

            allNodesInTrie[i]->modifyNodeValidity(false);
            allNodesInTrie[i]->setCharacter('\0');
            allNodesInTrie.pop_back();
        }

        allNodesInTrie.pop_back(); // pop back once more to remove root
        numberOfWords = 0;
    }

    return true;
}

void Trie::inOrderTraversalForClear(Node *currNode)
{
    // Base case
    if (currNode->isNodeValid() == false)
    {
        return;
    }

    // Traverse Left subtree
    if (currNode->children[0]->isNodeValid() == true)
    {
        inOrderTraversalForClear(currNode->children[0]);
    }

    // Add current node to a vector to collect it
    allNodesInTrie.push_back(currNode);

    // Traverse right subtrees
    for (int i = 1; i < 26; i++)
    {
        if (currNode->children[i]->isNodeValid() == true)
        {
            inOrderTraversalForClear(currNode->children[i]);
        }
    }
}

void Trie::isEmpty()
{
    if (numberOfWords == 0)
    {
        std::cout << "empty 1\n"; // trie is empty
    }
    else
    {
        std::cout << "empty 0\n"; // trie is not empty
    }
}

void Trie::size()
{
    std::cout << "number of words is " << numberOfWords << "\n";
}

/*
===================================================================
===  illegal_exception Functions Definitions ===
===================================================================
*/