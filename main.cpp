#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <chrono>

// Trie node structure (data structure 1)
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    std::vector<std::string> songs;
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {}
};

// song struct to store songs
struct Song {
    std::string artist;
    std::string song;
    // take in song and artist
    Song(std::string  artist, std::string  song)
            : artist(std::move(artist)), song(std::move(song)) {}
};

// create global variables of the songHashTable and the songNameTrieRoot which we will compare as two data structures
std::unordered_map<std::string, std::vector<Song>> songHashTable;
TrieNode* songNameTrieRoot = new TrieNode;

// function that inserts song into the hashtable and the trienode
void insertSong(const Song& song) {
    songHashTable[song.artist].push_back(song);
    TrieNode* current = songNameTrieRoot;
    for (char c : song.song) {
        if (current->children.find(c) == current->children.end()){
            current->children[c] = new TrieNode;
        }
        current = current->children[c];
    }
    current->isEndOfWord = true;
    current->songs.push_back(song.song);
}

// function that searches by artist in the hash table
std::vector<Song> searchByArtist(const std::string& artist) {
    if (songHashTable.find(artist) != songHashTable.end()) {
        return songHashTable[artist];
    }
    return {
    };
}
int main() {
    // take in file w songs
    std::ifstream file("spotify_millsongdata.csv");
    if (!file.is_open()) {
        std::cerr << "we got a problem boss.";
        return -1;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string artist, song, link, text;
        // take in the aritst and song
        std::getline(iss, artist, ',');
        std::getline(iss, song, ',');
        // skip other fields bc we don't want em
        Song newSong(artist, song);
        insertSong(newSong);
    }
    bool done = false;
    // take user input for the trie search
    std::cout<< "--------------------------------------------------------------- \n";
    std::cout<< "Welcome to Song Search! \n";
    std::cout<<"You will get to input an artist's name and see all their most popular songs either using a hashmap or trie! \n";
    std::cout<< "--------------------------------------------------------------- \n";
    while (!done){
        std::cout << "Enter artist name for Trie search: ";
        std::string artistInputTrie;
        std::getline(std::cin, artistInputTrie);
        // start timer to test speed of trie retrieval
        auto start = std::chrono::high_resolution_clock::now();
        // search for songs by that artist using a trie structure
        std::vector<Song> matchingSongsByArtistTrie = searchByArtist(artistInputTrie);
        // output the matching songs by that artist and the artist's name
        if (!matchingSongsByArtistTrie.empty()) {
            std::cout << "Matching songs by artist using Trie:\n";
            for (const auto& song : matchingSongsByArtistTrie) {
                std::cout << " - " << song.song << " by " << song.artist << "\n";
            }
        } else {
            // if no songs found by that artist, then output this
            std::cout << "That artist is a LOSER, they have NO songs\n";
        }
        // stop timer for trie retrieval
        auto stop = std::chrono::high_resolution_clock::now();
        // calculate duration
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Song retrieval using Trie ran in " << duration.count() << " milliseconds\n\n";

        // now do the same thing using hash
        std::cout << "Enter artist name for Hash search: ";
        std::string artistInputHash;
        std::getline(std::cin, artistInputHash);
        // start timer to test speed of hash retrieval
        start = std::chrono::high_resolution_clock::now();
        // now it searches for the artist using the hash table instead
        std::vector<Song> matchingSongsByArtistHash = searchByArtist(artistInputHash);
        // output the matching songs by that artist and the artist's name
        if (!matchingSongsByArtistHash.empty()) {
            std::cout << "Matching songs by artist using Hash:\n";
            for (const auto& song : matchingSongsByArtistHash) {
                std::cout << " - " << song.song << " by " << song.artist << "\n";
            }
        } else {
            // if no songs are found by that artist...
            std::cout << "That artist is a LOSER, they have NO songs! Try again... or QUIT\n";
        }
        // stop timer for hash retrieval
        stop = std::chrono::high_resolution_clock::now();
        // calculate duration
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Song retrieval using Hash ran in " << duration1.count() << " milliseconds\n\n";
        if (duration1 < duration){
            std::cout << "Hash is faster than Trie here! \n";}
        else{
            std::cout << "Trie is faster than Hash here! \n";}
        std::string answer;
        std::cout << "Continue searching? (Y or N)\n";
        std::cin >> answer;
        // Clear any remaining characters in the input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (answer == "N"){
            done = true;
        }
    }
    std::cout << "Thanks for playing! Hope you have a musical day :)";
    return 0;
}