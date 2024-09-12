#include "../include/InvertedIndex.h"

mutex addDate;

// Обновление списка документов
void InvertedIndex::updateDocumentsList(vector<string> updateDocs)
{
    vector<thread> updateThread;

    for(int i = 0; i < updateDocs.size(); i++)
    {
        fstream file;
        file.open(updateDocs[i]);
        if(!file)
        {
            cerr << updateDocs[i] + " file not found \n";
            continue;
        }
        string words;
        getline(file, words);
        docs.push_back(words);
        file.close();

        updateThread.emplace_back(&InvertedIndex::getRowDoc, this, words, i);
    }

    for(auto & i : updateThread)
        i.join();
}

//Заполнение частотного словаря
void InvertedIndex::getRowDoc(string rowDoc, int i)
{
    Entry entry{};
    string word;
    istringstream words(rowDoc);
    int countWords = 0;
    while(getline(words,word, ' '))
    {
        vector<Entry> vectorEntry;
        lock_guard<mutex> guard(addDate);

        if(word.length() > 100)
        {
            cerr << "No-correct size words in file #" << i << endl;
            continue;
        }

        if(countWords > 1000)
        {
            cerr << "No-correct size file #" << i << endl;
            break;
        }

        if(!freqDictionary.count(word))
        {
            entry.docId = i;
            entry.count = 1;
            vectorEntry.push_back(entry);

            freqDictionary.insert({word, vectorEntry});
        }
        else
        {
            auto it = freqDictionary.find(word);
            bool found = false;
            for(auto & j : it->second)
            {
                if(j.docId == i)
                {
                    j.count++;
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                entry.docId = i;
                entry.count = 1;
                it->second.push_back(entry);
            }
        }

        countWords++;
    }
}

//Получение словаря
map<string, vector<Entry>> InvertedIndex::getFreqDictionary()
{
    return freqDictionary;
}

//Проверка на количество вхождения слова в словарь
vector<Entry> InvertedIndex::getWordCount(const string &word)
{
    auto it = freqDictionary.find(word);
    if(freqDictionary.count(word))
        return it->second;
    else
    {
        vector<Entry> entryVector;
        return entryVector;
    }
}
