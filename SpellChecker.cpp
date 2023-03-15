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
    delete[] children;
}

Node::Node(char c)
{
    character = c;
    endOfWord = false;
    validNode = true;
    numberOfChildren = 0;

    children = new Node *[26];

    for (int i = 0; i < 26; i++)
    {
        children[i] = nullptr;
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

int Node::getNumberOfChildren()
{
    return numberOfChildren;
}

void Node::setNumberOfChildren(int numberOfKids)
{
    numberOfChildren = numberOfKids;
}

void Node::incrementNumberOfChildren()
{
    numberOfChildren++;
}

void Node::decrementNumberOfChildren()
{
    numberOfChildren--;
}

/*
===================================================================
===  Trie Functions Definitions ===
===================================================================
*/

Trie::Trie()
{
    root = new Node('\0');
    // nullNode = new Node();
    numberOfWords = 0;
}

Trie::~Trie()
{
    clear();
    delete root;

    // delete root;
}

bool Trie::insert(std::string word)
{
    // Verify that input is only uppercase characters
    for (int k = 0; k < word.length(); k++)
    {
        currentCharacter = word.at(k);
        if (!isupper(currentCharacter))
        {
            throw illegal_exception();
        }
    }

    if (findNode(word) != nullptr && findNode(word)->isEndOfWord()) // if the word exists already
    {
        return false;
    }

    currentNode = root;

    for (int i = 0; i < word.length(); i++)
    {
        currentCharacter = word.at(i);

        if (currentNode->children[currentCharacter - 'A'] == nullptr)
        {
            // ASCII value of A = 65, B = 66, C = 67, and so on
            // By subtracting 'A', we subtract 65 from the ASCII value of the current character
            // which tells us its position in the alphabet (A = 0, B = 1, C = 2, so on)

            // Create a new node for the current letter of the word, if there isn't a valid node for that letter yet
            currentNode->children[currentCharacter - 'A'] = new Node(currentCharacter);
            currentNode->incrementNumberOfChildren();
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

        if (currentNode->children[currentCharacter - 'A'] == nullptr)
        {
            // if at any point in the traversal, an expected letter of the word doesn't exist in the trie,
            // then the word isn't in the trie

            return nullptr;
        }

        currentNode = currentNode->children[currentCharacter - 'A'];
    }

    // by the end of the for loop, the word is guaranteed to be in the trie, so we return the node for the last letter of the word
    return currentNode;
}

void Trie::searchPrefix(std::string prefix)
{
    // Verify that input is only uppercase characters
    for (int k = 0; k < prefix.length(); k++)
    {
        currentCharacter = prefix.at(k);
        if (!isupper(currentCharacter))
        {
            throw illegal_exception();
        }
    }

    countPrefixInstances = 0;

    if (findNode(prefix) != nullptr) // if the prefix exists
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
    if (currNode == nullptr)
    {
        return;
    }

    // Traverse Left subtree
    if (currNode->children[0] != nullptr)
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
        if (currNode->children[i] != nullptr)
        {
            inOrderTraversalForPrefix(currNode->children[i]);
        }
    }
}

void Trie::spellcheck(std::string word)
{
    firstLetterOfWord = "";
    firstLetterOfWord += word.at(0);

    if (findNode(word) != nullptr)
    {
        if (findNode(word)->isEndOfWord()) // check if the word exists in the trie
        {
            std::cout << "correct\n";
        }
    }
    else if (findNode(firstLetterOfWord) == nullptr) // if the first letter of the word isn't in the tree
    {
        std::cout << "\n";
    }
    else
    {
        // traverse down the tree, following the path of the letters that comprise the given word.
        // if at any point in the traversal, a letter of the word cant be found, print out all the
        // other branches of that node.

        currentNode = root;
        tempPrefix = "";

        for (int i = 0; i < word.length(); i++)
        {
            currentCharacter = word.at(i);

            if (currentNode->children[currentCharacter - 'A'] == nullptr)
            {
                // if at any point in the traversal, an expected letter of the word isn't in the tree,
                // we print out what the other words that follow the same path

                printWords(currentNode, tempPrefix);
                std::cout << "\n";
                break;
            }
            else if (i == word.length() - 1) // if we've made it to the end of the for loop, print all the words with this prefix
            {
                if (i == 0) // special case: if the input was only one letter
                {
                    tempPrefix = word;
                    currentNode = currentNode->children[currentCharacter - 'A'];
                }

                printWords(currentNode, tempPrefix);
                std::cout << "\n";
                break;
            }

            currentNode = currentNode->children[currentCharacter - 'A'];
            tempPrefix += currentNode->getCharacter();
        }
    }
}

bool Trie::erase(std::string word)
{
    // using a for loop, traverse down the trie until end of the given word
    // during the traversal, accumulate all the nodes that comprise the word into an array
    // iterate from the end of the array, and delete all the nodes until you reach another "endOfWord" node
    // (or if there's no other endOfWord, delete all the nodes)

    // Verify that input is only uppercase characters
    for (int k = 0; k < word.length(); k++)
    {
        currentCharacter = word.at(k);
        if (!isupper(currentCharacter))
        {
            throw illegal_exception();
        }
    }

    if (findNode(word) != nullptr)
    {
        if (findNode(word)->isEndOfWord()) // if the word exists
        {
            if (findNode(word)->getNumberOfChildren() > 0) // if the word is a prefix for another word
            {
                // we dont want to delete the nodes, just stop marking it as a word
                findNode(word)->setCharAsEndOfWord(false);
                numberOfWords--;
                return true;
            }

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
                if (nodesToErase[j]->getNumberOfChildren() > 1)
                {
                    nodesToErase[j]->decrementNumberOfChildren();

                    // Delete nodes & deallocate the array
                    // for (int k = word.length(); k > j; k--)
                    // {
                    //     delete nodesToErase[k];
                    // }
                    delete[] nodesToErase;

                    numberOfWords--;
                    return true; // don't delete the rest of the letters, because they belong to other word(s)
                }

                // nodesToErase[j]->modifyNodeValidity(false);
                // nodesToErase[j]->setCharacter('\0');
                // nodesToErase[j]->setNumberOfChildren(0);
                currentCharacter = nodesToErase[j]->getCharacter();

                if (j > 0)
                {
                    delete nodesToErase[j - 1]->children[currentCharacter - 'A'];
                    nodesToErase[j - 1]->children[currentCharacter - 'A'] = nullptr;
                }
                else
                {
                    delete root->children[currentCharacter - 'A'];
                    root->children[currentCharacter - 'A'] = nullptr;
                }
            }

            // Deallocate nodesToErase
            // for (int k = 0; k < word.length(); k++)
            // {
            //     delete nodesToErase[k];
            // }
            delete[] nodesToErase;

            numberOfWords--;
            return true;
        }
    }

    return false; // failure
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

            // allNodesInTrie[i]->modifyNodeValidity(false);
            // allNodesInTrie[i]->setCharacter('\0');
            // allNodesInTrie[i]->setNumberOfChildren(0);

            currentCharacter = allNodesInTrie[i]->getCharacter();

            delete allNodesInTrie[i - 1]->children[currentCharacter - 'A'];
            allNodesInTrie[i - 1]->children[currentCharacter - 'A'] = nullptr;

            // else
            // {
            //     delete root->children[currentCharacter - 'A'];
            //     root->children[currentCharacter - 'A'] = nullptr;
            // }

            allNodesInTrie.pop_back();
        }

        // allNodesInTrie.pop_back(); // pop back once more to remove root
        numberOfWords = 0;
    }

    return true;
}

void Trie::inOrderTraversalForClear(Node *currNode)
{
    // Base case
    if (currNode == nullptr)
    {
        return;
    }

    // Traverse Left subtree
    if (currNode->children[0] != nullptr)
    {
        inOrderTraversalForClear(currNode->children[0]);
    }

    // Add current node to a vector to collect it
    allNodesInTrie.push_back(currNode);

    // Traverse right subtrees
    for (int i = 1; i < 26; i++)
    {
        if (currNode->children[i] != nullptr)
        {
            inOrderTraversalForClear(currNode->children[i]);
        }
    }
}

void Trie::runPrintWords()
{
    printWords(root, "");

    if (numberOfWords > 0)
    {
        std::cout << "\n";
    }
}

void Trie::printWords(Node *currNode, std::string prefix)
{
    // Base cases
    if (currNode == nullptr)
    {
        return;
    }
    else if (currNode->isEndOfWord())
    {
        std::cout << prefix << " ";

        if (currNode->getNumberOfChildren() == 0)
        {
            return;
        }
    }

    // Traverse subtrees for each node
    for (int i = 0; i < 26; i++)
    {
        if (currNode->children[i] != nullptr)
        {
            // Add current character to the prefix
            prefix += currNode->children[i]->getCharacter();

            printWords(currNode->children[i], prefix);
            prefix.pop_back();
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

// Note: I used ChatGPT to learn how to implement this
const char *illegal_exception::what() const throw()
{
    return "illegal argument";
}