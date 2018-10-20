//---------------------------------------------------------------------------

#ifndef DBManagerH
#define DBManagerH

#include "MySQLClient.h"
#include "MyClientMod.h"
#include <vector>
#include <ComCtrls.hpp>



//---------------------------------------------------------------------------
class Task  // Решил не использовать класс Entry т.к. его ресурсы были мной криво залинкованы, из-за чего его сложно перенести, есть вероятность повреждения рабочей версии проги.
{
    public:
    //0 - number
    //1 - Date
    //2 - Street
    //3 - House
    //4 - Flat
    //5 - Tel
    //6 - Description
    //7 - Support
    //8 - Time
    //9 - Master comment
    AnsiString Data[10];
    bool Transmit;

    //Constructors
    Task(){Transmit = false;}

    //Funcs
    int FindEmptySGRow(TStringGrid *SG1);


    void GetTaskFromExcel(Variant vSheet, int nID);
    void GetTaskFromReply(TStringGrid *SG, int nY = 0);
    void GetTaskFromForm(TForm *Form);
    void GetTaskFromString(AnsiString str);

    void Show(TStringGrid *SG1);
    void Show(TMemo *Memo);
    void Clear();
};
//------------------------------------------------------------------------------
class SearchResults
{
    public:
    std::vector<Task> Results;

    //Construstors
    //SearchResults();

    void Add(Task Task1);
    void Show(TStringGrid *SG1);
    void Show(TMemo *Memo);
    void Clear(){Results.clear();}
};
//------------------------------------------------------------------------------
class DBManager
{
    public:
    //MySQLClient Client;
    MyClientMod Client;
    SearchResults srResults;

    //Constructors
    //DBManager(){Client = MySQLClient();}
    DBManager(){Client = MyClientMod();}
    DBManager(TIdTCPClient *iCli, TStringGrid *iSG, TMemo *iMemo);

    // Misc
    AnsiString GetRegID(AnsiString sRegion);
    int GetMaxID(AnsiString sTabName);
    AnsiString TasksTabName(AnsiString sRegID);
    AnsiString InvertDate(AnsiString sDate);
    bool TurnOffProgram();
    AnsiString GetRegionFromStreet(AnsiString sStreet);
    bool GetIsLastVersion(AnsiString sCurrentVersion);
    bool GetIsServerUP();


    // Add
    void AddRegion(AnsiString sName);
    void AddStreet(AnsiString sName, AnsiString sRegion);
    void AddStreet(AnsiString sName, int nRegID);
    void AddTask(AnsiString sRegion, Task *Task1);
    void AddTask(int nRegID, Task *Task1);
    void AddMasterComents(AnsiString sComment, AnsiString sRegion, int nTaskID);


    // News
    void AddNews(AnsiString sNews, AnsiString sDate);
    void LoadNews(TRichEdit *RE);
    bool GetIsMessageNew(AnsiString sDate);
    void FormatRE(TRichEdit *RE1);
    bool GetIsDateValid(AnsiString sDate);


    // Search
    void AddSearchResultsToResults();
    //Search TaskID
    void SearchTaskIDwithRegID(int nTaskID, int nRegID);
    void SearchTaskIDinRegion(TStringGrid *SG1, int nTaskID, AnsiString sRegion = "");
    void SearchTaskIDinAllRegions(int nTaskID);
    // Search params
    void SearchParamsWithRegID(int nRegID, AnsiString sHouse, AnsiString sStreet = "", AnsiString sFlat = "", AnsiString sComment = "");
    void SearchParamsInRegion(TStringGrid *SG1, AnsiString sRegion, AnsiString sHouse, AnsiString sStreet = "", AnsiString sFlat = "", AnsiString sComment = "");
    void SearchParamsInAllRegions(AnsiString sHouse, AnsiString sStreet = "", AnsiString sFlat = "", AnsiString sComment = "");

    // Search for new tasks
    void SearchForNewTasks(TMemo *Memo);
    void SearchForNewTasksInRegion(int nRegID);
    // Search for last tasks
    void LastTaskInRegion(int nRegID);
    void LastTasks(TMemo *Memo);


    // Load
    void LoadRegions(TStrings *Strings);
    void LoadStreets(TStrings *Strings, AnsiString sRegion = "");

};
//------------------------------------------------------------------------------



#endif
