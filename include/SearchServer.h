#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#pragma once
#include <set>
#include "InvertedIndex.h"
using namespace std;

//Структура относительного индекса
struct RelativeIndex
{
    size_t doc_id;
    float rank;
    bool operator == (const RelativeIndex& other) const
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

//Класс поисковых запросов
class SearchServer
{
private:
    InvertedIndex index;
    vector<vector<RelativeIndex>> resultSearch;

public:
//@param idx в конструктор класса передаётся ссылка на класс InvertedIndex, чтобы SearchServer мог узнать частоту слов встречаемых в запросе
    SearchServer(InvertedIndex& idx);

    //Метод обработки поисковых запросов @param queries_input поисковые запросы взятые из файла requests.json
    //@return возвращает отсортированный список релевантных ответов для заданных запросов
    vector<vector<RelativeIndex>> search(const vector<string> &quriesInput, int maxResponses);

    //Метод получения стоки с запросом, номер и максимальное количество ответов на запрос
    void handleRequest(const string& request, int i, int maxResponses);
};

#endif // SEARCHSERVER_H
