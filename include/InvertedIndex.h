#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <thread>
#include <map>
#include <sstream>
using namespace std;

//Структура инвертированного индекса
struct Entry
{
    size_t doc_id, count;
    bool operator == (const Entry& other) const {return (doc_id == other.doc_id && count == other.count);}
};

//Класс индексации документов
class InvertedIndex
{
private:
    vector<string> docs;
    map<string, vector<Entry>> freqDictionary;

public:
    InvertedIndex() = default;

    //Обновление списка документов
    void updateDocumentsList(vector<string> updateDocs);

    //Получение строки (слова) документа и его порядковый номер
    void getRowDoc(string rowDoc, int i);

    //@return частотный словарь
    map<string, vector<Entry>> getFreqDictionary();

    //Получение слова поиска в частотном словаре @return
    vector<Entry> getWordCount(const string& word);
};

#endif // INVERTEDINDEX_H
