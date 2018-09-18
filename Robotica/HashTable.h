#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <functional>
#include <vector>
#include <map>

typedef int HashKey;

template <typename ClassValue>
class HashTable{
    public:
        HashTable(function<HashKey(ClassValue)> hashFunction, function<bool(ClassValue, ClassValue)> compareFunction){
            this->hashFunction = hashFunction;
            this->compareFunction = compareFunction;
        }
        void insert(ClassValue cValue);
        bool exist(ClassValue cValue);
        void clear();
    private:
        map<HashKey, vector<ClassValue>> hashTable;
        function<HashKey(ClassValue)> hashFunction;
        function<bool(ClassValue, ClassValue)> compareFunction;


};

template <typename ClassValue>
void HashTable::insert(ClassValue cValue){
    this->hashTable[hashFunction(cValue)].push_back(cValue);
}

template <typename ClassValue>
bool HashTable::exist(ClassValue cValue){
    map<HashKey, vector<ClassValue>>::iterator findIter = hashTable.find(hashFunction(cValue));
    if(findIter == hashTable.end()) return false;
    for(auto iter = findIter.second.begin(); iter != findIter.second.end(); ++iter){
        if(compareFunction(cValue, *iter) == true) return true;
    }
    return false;
}

template <typename ClassValue>
void HashTable::clear(){
    for(auto iter = hashTable.begin(); iter != hashTable.end(); ++iter){
        iter.second.clear();
        iter.second.shrink_to_fit();
    }
    hashTable.clear();
}

#endif