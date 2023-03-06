#include <iostream>
#include <string>
#include "SpellChecker.hpp"

/*

ECE 250 - Project 3: Spell-Checking Using Trie ADT

Name:   Azizul Chowdhury
ID:     20893907
Email:  a48chowd@uwaterloo.ca

*/

int main()
{
    std::string command, word, prefix;

    while (std::cin >> command)
    {
        if (command == "load")
        {
        }

        else if (command == "i") // insert a word
        {
            std::cin >> word;
        }

        else if (command == "c") // search for a prefix
        {
            std::cin >> prefix;
        }

        else if (command == "e") // erase a word
        {
            std::cin >> word;
        }

        else if (command == "p") // print all the words in the trie, alphabetically
        {
        }

        else if (command == "spellcheck")
        {
            std::cin >> word;
        }

        else if (command == "empty") // check if trie is empty
        {
        }

        else if (command == "clear") // clear the trie
        {
        }

        else if (command == "size") // prints number of words in trie
        {
        }

        else if (command == "exit")
        {
            return 0;
        }
    }

    return 0;
}