#include "../include/SearchServer.h"

mutex updateDate;

SearchServer::SearchServer(InvertedIndex &idx)
{
    index = idx;
}

//Метод обработки поисковых запросов, возвращает отсортированный спискок релеватных ответов
vector<vector<RelativeIndex>> SearchServer::search(const vector<string> &quriesInput, int responsesLimit)
{
    vector<thread> searchThread;

    if(quriesInput.size() > 1000)
    {
        cerr << "The number of requests is more than 1000";
        resultSearch.resize(1000);
    }
    else
        resultSearch.resize(quriesInput.size());

    for(int i = 0; i < quriesInput.size() && quriesInput.size() != 1000; i++)
        searchThread.emplace_back(&SearchServer::handleRequest, this, ref(quriesInput[i]), i, responsesLimit);

    for(auto & i : searchThread)
        i.join();

    return resultSearch;
}

//Метод заполнения отсортированного списка релеватных ответов для заданного запроса
void SearchServer::handleRequest(const string& request, int i, int responsesLimit)
{
    RelativeIndex relativeIndex{};
    vector<RelativeIndex> result;

    InvertedIndex invertedIndex;
    invertedIndex.getRowDoc(request, i);

    map <string, vector<Entry>> mapsFreqDictionary = invertedIndex.getFreqDictionary();

    map<string, size_t> wordsCount;

    int count = 0;
    for(const auto &it : mapsFreqDictionary)
    {
        wordsCount.insert({it.first, it.second[0].count});
        count += it.second[0].count;
    }

    if(count > 10)
    {
        cerr << "No-correct size request #" << i << endl;
        wordsCount.clear();
    }

    multimap<size_t, string> countWords;
    for(const auto &itWordsCount : wordsCount)
        countWords.insert({itWordsCount.second,itWordsCount.first});

    set<size_t> listNumberDocId;
    set<size_t>::iterator itThree;

    for(const auto &itCountWords : countWords)
    {
        if(index.getWordCount(itCountWords.second).empty())
            continue;

        for(int k = 0; k < index.getWordCount(itCountWords.second).size(); k++)
            listNumberDocId.insert(index.getWordCount(itCountWords.second)[k].docId);
    }

    lock_guard<mutex> guard(updateDate);

    if(listNumberDocId.empty())
        resultSearch[i] = result;
    else
    {
        float maxAbsoluteRelative = 0;
        for(itThree = listNumberDocId.begin(); itThree != listNumberDocId.end(); itThree++)
        {
            relativeIndex.rank = 0;
            relativeIndex.docId = *itThree;
            for(const auto &itCountWords : countWords)
            {
                for(int k = 0; k < index.getWordCount(itCountWords.second).size(); k++)
                {
                    if(index.getWordCount(itCountWords.second)[k].docId == relativeIndex.docId)
                        relativeIndex.rank += (float)index.getWordCount(itCountWords.second)[k].count;
                }
            }

            if(relativeIndex.rank > maxAbsoluteRelative)
                maxAbsoluteRelative = relativeIndex.rank;

            result.push_back(relativeIndex);
        }

        for(int j = 0; j < result.size(); j++)
        {
            for(int k = 0; k < result.size() - 1 - j; k++)
            {
                if(result[k].rank < result[k + 1].rank)
                    swap(result[k], result[k + 1]);
            }
        }
        auto begin = result.cbegin();
        auto end = result.cend();

        if(result.size() >= responsesLimit)
            result.erase(begin + responsesLimit, end);

        for (auto & j : result)
            j.rank /= maxAbsoluteRelative;

        resultSearch[i] = result;
    }
}
