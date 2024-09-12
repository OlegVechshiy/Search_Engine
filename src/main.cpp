#include "ConverterJs.h"
#include "SearchServer.h"

int main()
{
    ConverterJSON converterJson;
    InvertedIndex invertedIndex;
    try
    {
        converterJson.runFileConfigJSON();
    }
    catch (const char* error_message)
    {
        cerr << error_message << endl;
        return 0;
    }
    cout << "Please, input the one of command: go or exit: ";
    string cmd;
    cin >> cmd;
    if(cmd == "go")
    {
        cout << "Search Engine Starting!\n";
        vector <string> listFiles = converterJson.getTextDocuments();
        invertedIndex.updateDocumentsList(listFiles);
        SearchServer searchServer(invertedIndex);
        try
        {
            vector <string> listQuery = converterJson.getRequests();
            vector<vector<RelativeIndex>> listRelativeIndex = searchServer.search(listQuery, converterJson.getResponsesLimit());
            vector<vector<pair<int, float>>>answers;
            for(auto & i : listRelativeIndex)
            {
                pair <int, float> pairResult;
                vector<pair<int,float>> answersPair;
                for(auto & j : i)
                {
                    pairResult = make_pair(j.docId,j.rank);
                    answersPair.push_back(pairResult);
                }
                answers.push_back(answersPair);
            }
            try
            {
                converterJson.putAnswers(answers);
            }
            catch (const char* error_message)
            {
                cerr << error_message << endl;
                return 0;
            }
        }
        catch (const char* error_message)
        {
            cerr << error_message << endl;
            return 0;
        }
        cout << "The transaction was successful!\n";
    }
    else if (cmd != "exit")
        cout << "Command input wasn't correct!\n";
    return 0;
}
