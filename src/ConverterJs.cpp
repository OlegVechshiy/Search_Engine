#include "ConverterJs.h"

//Получение содержимого документов
vector<string> ConverterJSON::getTextDocuments()
{
    vector<string> flowFiles;

    for (const auto &i: jsonWork["files"])
        flowFiles.push_back(i);

    return flowFiles;
}

//Возвращает максимальное количество ответов на запрос
int ConverterJSON::getResponsesLimit() const
{
    return maxResponses;
}

//Конвертация запросов из JSON файла в вектор
vector<string> ConverterJSON::getRequests()
{
    vector<string> flowQuery;
    jsonWork.clear();
    fileOpen.open("requests.json");

    if(!fileOpen)
        cerr << "File requests.json not found \n";
    else
    {
        try
        {
            fileOpen >> jsonWork;
        }
        catch (...)
        {
            throw "syntax of file requests.json not correct!";
        }
    }
    fileOpen.close();

    for(const auto &i: jsonWork["requests"])
        flowQuery.push_back(i);

    return flowQuery;
}

//Конвертация вектора с результатами поиска в JSON файл
void ConverterJSON::putAnswers(vector<vector<pair<int, float>>> answers)
{
    fileOpen.open("answers.json", ios::out);
    if(!fileOpen)
        cerr << "File answers.json not found\n";
    else
    {
        try
        {
            jsonWork = {{"Answers:",{}}};

            for(int i = 0; i < answers.size(); i++)
            {
                jsonWork["Answers:"]["request" + to_string(i) + ":"];

                if(answers[i].empty())
                {
                    jsonWork["Answers:"]["request" + to_string(i) + ":"] = {{"result:", "false"}};
                    continue;
                }
                else
                    jsonWork["Answers:"]["request" + to_string(i) + ":"] = {{"result:", "true"}};

                for(int j = 0; j < answers[i].size(); j++)
                    jsonWork["Answers:"]["request" + to_string(i) + ":"]["relevance:"].push_back({{"docid:", answers[i][j].first}, {"rank:", ceil(answers[i][j].second * 1000) / 1000}});
            }

            fileOpen << jsonWork;
        }
        catch (...)
        {
            throw "syntax of file answers.json not correct!";
        }
    }
    fileOpen.close();
}

//Проверка конфигурационного файла config.json
void ConverterJSON::runFileConfigJSON()
{
    fileOpen.open("config.json");

    if(!fileOpen)
        throw "Error. Config file is missing";

    else
    {
        try
        {
            fileOpen >> jsonWork;
        }
        catch (...)
        {
            throw "syntax of file config.json not correct!";
        }
    }
    fileOpen.close();

    if(!jsonWork.count("config"))
        throw "Error. Config file is empty";

    if(jsonWork["config"]["version"] != VERSION_APP)
        throw "Error. Config.json has incorrect file version";

    if(jsonWork["config"].count("max_responses"))
        maxResponses = jsonWork["config"]["max_responses"];

    cout << jsonWork["config"]["name"] <<" Starting. Version "<< jsonWork["config"]["version"] << endl;
}