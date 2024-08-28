#ifndef CONVERTERJS_H
#define CONVERTERJS_H

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include "nlohmann/json.hpp"
#include "config.h"
using namespace std;
using namespace nlohmann;

//Класс для работы с JSON-файлами
class ConverterJSON
{
private:
    json jsonWork;
    fstream fileOpen;
    int maxResponses = 5;

public:
    ConverterJSON() = default;
    //Метод получения содержимого файлов
    //Возвращает список с содержимым файлов перечисленных в config.json
    vector<string> getTextDocuments();

    //Метод считывает поле max_responses для определения предельного количества ответов на один запрос @return
    int getResponsesLimit() const;

    //Метод получения запросов из файла requests.json @return возвращает список запросов из файла requests.json
    vector<string> getRequests();

    //Положить в файл answers.json результаты поисковых запросов
    void putAnswers(vector<vector<pair<int, float>>>answers);

    //Показать содержимое файла Config.json
    void runFileConfigJSON();
};

#endif // CONVERTERJS_H
