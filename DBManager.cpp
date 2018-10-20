//---------------------------------------------------------------------------


#pragma hdrstop

#include "DBManager.h"
#include "VariantFuncs.h"
#include "UTvTasks.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)





//---------------------------------------------------------------------------
void Task::GetTaskFromExcel(Variant vSheet, int nID)
{
    AnsiString str;
    int i;
    for(i=1; i<=8; i++)
    {
        str = VarToStr(GetCell(vSheet, nID, i));
        Data[i-1] = str;
    }
    Data[8] = VarToStr(GetCell(vSheet, nID, 12));

    //Комментарии из Гугл дока
    Data[9] = "";
    for(i=13; i<=20; i++)
    {
        str = VarToStr(GetCell(vSheet, nID, i));
        if(str != "")
            Data[9] += str + '\n';
    }
}
//---------------------------------------------------------------------------
void Task::GetTaskFromReply(TStringGrid *SG, int nY)
{
    if(nY >= SG->RowCount)
        return;
    if(SG->ColCount < 11)
        return;
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
    // + Transmit(bool)

    for(int nX = 0; nX<11; nX++)
    {
        if(nX==9)//Transmit
            Transmit = (SG->Cells[nX][nY] == "0") ? false : true;
        else if(nX == 10) // Master comment
            Data[9] = SG->Cells[nX][nY];
        else
            Data[nX] = SG->Cells[nX][nY];
    }
}
//---------------------------------------------------------------------------
void Task::GetTaskFromForm(TForm *Form)
{
    TForm1 *Form1 = dynamic_cast<TForm1 *>(Form);
    if(Form1)
    {
        //Melnik fix :)
        AnsiString sOper = Form1->LabeledEdit5->Text;
        if(sOper.LowerCase() == "я" || sOper.LowerCase() == " я" || sOper.LowerCase() == "я " || sOper.LowerCase() == " я ")
            sOper = "Мельник";

        Data[0] = "";                           //0 - number
        Data[1] = Now().DateString();           //1 - Date
        Data[2] = Form1->ComboBox1->Text;       //2 - Street
        Data[3] = Form1->LabeledEdit2->Text;    //3 - House
        Data[4] = Form1->LabeledEdit3->Text;    //4 - Flat
        Data[5] = Form1->LabeledEdit4->Text;    //5 - Tel
        Data[6] = Form1->Memo1->Text;           //6 - Description
        Data[7] = sOper;                        //7 - Support
        Data[8] = Now().TimeString();           //8 - Time
        Data[9] = "";                           //9 - Master comment
        Transmit = false;                       // + Transmit(bool)
    }
}
//---------------------------------------------------------------------------
void Task::GetTaskFromString(AnsiString str)
{
    Data[0] = "";
    StringsContainer strCont;
    strCont.BreakString(str, "\t");
    for(unsigned i=0; i<strCont.Vector.size(); i++)
    {
        if(i>9)
            Data[9] += strCont.Vector[i];
        else
            Data[i+1] = strCont.Vector[i];
    }
    Transmit = false;
}
//---------------------------------------------------------------------------
int Task::FindEmptySGRow(TStringGrid *SG1)
{
    int nY = 0;
    while(SG1->Cells[0][nY] != "")
        nY++;

    if(nY == SG1->RowCount)
        SG1->RowCount++;
return nY;
}
//---------------------------------------------------------------------------
void Task::Show(TStringGrid *SG1)
{
    int i, nY = FindEmptySGRow(SG1);
    for(i=0; i<=8; i++)
        SG1->Cells[i][nY] = Data[i];

    if(Data[9]!="" && Data[9]!=" ")
        SG1->Cells[9][nY] = "+";
}
//---------------------------------------------------------------------------
void Task::Show(TMemo *Memo)
{
    AnsiString str = Data[0];
    for(int i=1; i<=8; i++)
    {
        str += "\t" + Data[i];
        if(i == 7)
            str += "\t\t\t";
    }
    Memo->Lines->Add(str);
}
//---------------------------------------------------------------------------
void Task::Clear()
{
    for(int i=0; i<=9; i++)
        Data[i] = "";
    Transmit = false;
}
//------------------------------------------------------------------------------



//SearchResults
//------------------------------------------------------------------------------
void SearchResults::Add(Task Task1)
{
    Results.push_back(Task1);
}
//------------------------------------------------------------------------------
void SearchResults::Show(TStringGrid *SG1)
{
    for(unsigned i=0; i < Results.size(); i++)
        Results[i].Show(SG1);
}
//------------------------------------------------------------------------------
void SearchResults::Show(TMemo *Memo)
{
    for(unsigned i=0; i < Results.size(); i++)
        Results[i].Show(Memo);
}
//------------------------------------------------------------------------------







// DBManager
//------------------------------------------------------------------------------
DBManager::DBManager(TIdTCPClient *iCli, TStringGrid *iSG, TMemo *iMemo)
{
    //Client = MySQLClient(iCli, iSG, iMemo);
    Client = MyClientMod(iCli, iSG, iMemo);
    Client.Loggin = "test";
    Client.Pass = "test";
}
//------------------------------------------------------------------------------
AnsiString DBManager::GetRegID(AnsiString sRegion)
{
    Client.SendCommand("select id from regions where name like '" + sRegion + "'");
return Client.GetFieldValue();
}
//------------------------------------------------------------------------------
int DBManager::GetMaxID(AnsiString sTabName)
{
    Client.SendCommand("select max(id) from " + sTabName);
    AnsiString str = Client.GetFieldValue();
    if(str != "" && str != " ")
        return StrToInt(str);
    else
        return 0;
}
//------------------------------------------------------------------------------
AnsiString DBManager::TasksTabName(AnsiString sRegID)
{
    return "tasks_" + sRegID;
}
//------------------------------------------------------------------------------
AnsiString DBManager::InvertDate(AnsiString sDate)
{
    AnsiString sRes = sDate;
    StringsContainer strCont;
    strCont.BreakString(sDate, ".");
    if(strCont.Vector.size() == 1)
        strCont.BreakString(sDate, ",");
    if(strCont.Vector.size() == 1)
        strCont.BreakString(sDate, "/");

    if(strCont.Vector.size() == 3)
    {
        LeftDigitsOnly(strCont.Vector[0]);
        LeftDigitsOnly(strCont.Vector[1]);
        LeftDigitsOnly(strCont.Vector[2]);
        sRes = strCont.Vector[2] + "." + strCont.Vector[1] + "." + strCont.Vector[0];
    }
    else
        sRes = "2001.01.01";
return sRes;
}
//------------------------------------------------------------------------------
bool DBManager::TurnOffProgram()
{
    Client.SendCommand("select off from info");
    return StrToInt(Client.GetFieldValue());
}
//------------------------------------------------------------------------------
AnsiString DBManager::GetRegionFromStreet(AnsiString sStreet)
{
    Client.SendCommand("select region from streets where name like '" + sStreet + "'");
    AnsiString sRegID = Client.GetFieldValue();
    Client.SendCommand("select name from regions where id = " + sRegID);
return Client.GetFieldValue();
}
//------------------------------------------------------------------------------
bool DBManager::GetIsLastVersion(AnsiString sCurrentVersion)
{
    Client.SendCommand("select version from info");
    AnsiString sLastVersion = Client.GetFieldValue();
return sCurrentVersion == sLastVersion;
}
//------------------------------------------------------------------------------
bool DBManager::GetIsServerUP()
{
    try
    {
        Client.Cli->Connect();
    }
    catch(...)
    {
        return false;
    }
    Client.Cli->Disconnect();
return true;
}




// ADD
//------------------------------------------------------------------------------
void DBManager::AddRegion(AnsiString sName)
{
    Client.SendCommand("insert into regions (name) values ('" + sName + "')");
    Client.SendCommand("select max(id) from regions");
    AnsiString sID = Client.GetFieldValue();
    Client.SendCommand("create table " + TasksTabName(sID) + " (id smallint unsigned NOT NULL PRIMARY KEY auto_increment, date date, street varchar(80), house varchar(20), flat varchar(50), tel varchar(100), task text, oper varchar(50), time varchar(30), transmit tinyint(1) default 0, comment text)");
}
//------------------------------------------------------------------------------
void DBManager::AddStreet(AnsiString sName, AnsiString sRegion)
{
    AnsiString sRegID = GetRegID(sRegion);
    Client.SendCommand("insert into streets (name, region) values ('" + sName + "', " + sRegID + ")");
}
//------------------------------------------------------------------------------
void DBManager::AddStreet(AnsiString sName, int nRegID)
{
    Client.SendCommand("insert into streets (name, region) values ('" + sName + "', " + IntToStr(nRegID) + ")");
}
//------------------------------------------------------------------------------
void DBManager::AddTask(AnsiString sRegion, Task *Task1)
{
    //Invert date
    AnsiString sTemp;
    sTemp = InvertDate(Task1->Data[1]);
    Task1->Data[1] = sTemp;

    AnsiString sID = GetRegID(sRegion);
    AnsiString sPart1 = "insert into " + TasksTabName(sID) + "(date, street, house, flat, tel, task, oper, time, comment";
    if(Task1->Transmit)
        sPart1+= ", transmit";
    if(Task1->Data[0] != "")
        sPart1 += ", id";

    AnsiString sPart2 = ") values ('" + Task1->Data[1] + "'";
    for(int i = 2; i<=9; i++)
        sPart2 += ", '" + Task1->Data[i] + "'";
    if(Task1->Transmit)
        sPart2+=", 1";
    if(Task1->Data[0] != "")
        sPart2+=", " + Task1->Data[0];

    sPart2 += ")";
    Client.SendCommand(sPart1 + sPart2);
}
//------------------------------------------------------------------------------
void DBManager::AddTask(int nRegID, Task *Task1)
{
    //Invert date
    AnsiString sTemp;
    sTemp = InvertDate(Task1->Data[1]);
    Task1->Data[1] = sTemp;
    
    AnsiString sID = IntToStr(nRegID);
    AnsiString sPart1 = "insert into " + TasksTabName(sID) + " (date, street, house, flat, tel, task, oper, time, comment)";
    AnsiString sPart2 = " values ('" + Task1->Data[1] + "'";
    for(int i = 2; i<=9; i++)
        sPart2 += ", '" + Task1->Data[i] + "'";
    sPart2 += ")";

    Client.SendCommand(sPart1 + sPart2);
}
//------------------------------------------------------------------------------
void DBManager::AddMasterComents(AnsiString sComment, AnsiString sRegion, int nTaskID)
{
    if(sComment == "" || sRegion == "" || nTaskID<=0)
        return;

    AnsiString sRegID = GetRegID(sRegion);
    AnsiString sTabName = TasksTabName(sRegID);
    Client.SendCommand("update " + sTabName + " set comment = '" + sComment + "' where id = " + IntToStr(nTaskID));
}





// NEWS
//------------------------------------------------------------------------------
void DBManager::AddNews(AnsiString sNews, AnsiString sDate)
{
    if(sNews != "")
    {
        if(sDate == "")
            sDate = Now().DateString();
        sDate = InvertDate(sDate);
        Client.SendCommand("insert into news (date, news) values ('" + sDate + "', '" + sNews + "')");
    }
}
//------------------------------------------------------------------------------
void DBManager::LoadNews(TRichEdit *RE)
{
    RE->Clear();
    Client.SendCommand("select date, news from news order by id desc");
    //Загружаем в RE
    AnsiString str;
    for(int nY = 0; nY<Client.SG->RowCount; nY++)
    {
        str = "[" + Client.SG->Cells[0][nY] + "] " + Client.SG->Cells[1][nY];
        RE->Lines->Add(str);
        RE->Lines->Add("");
        RE->Lines->Add("");
        RE->Lines->Add("--------------------------------------------------------");
    }
    FormatRE(RE);
}
//------------------------------------------------------------------------------
bool DBManager::GetIsMessageNew(AnsiString sDate)
{
    StringsContainer cont;
    cont.BreakString(sDate, ".");
    int nDay = cont.Vector[0].ToInt();
    int nMon = cont.Vector[1].ToInt();
    int nYear = cont.Vector[2].ToInt();
    int Days1 = nYear*365 + (nMon-1)*30 + nDay;

    cont.Vector.clear();
    cont.BreakString(Now().CurrentDate(), ".");
    int nDay2 = cont.Vector[0].ToInt();
    int nMon2 = cont.Vector[1].ToInt();
    int nYear2 = cont.Vector[2].ToInt();
    int Days2 = nYear2*365 + (nMon2-1)*30 + nDay2;

    if((Days2-Days1) < 7)
        return true;
return false;
}
//------------------------------------------------------------------------------
void DBManager::FormatRE(TRichEdit *RE1)
{
    //Форматирование текста
    StringsContainer cont;
    AnsiString str, sMessage, sFirstLine;
    int nSelStart;
    for(int i = 0; i<RE1->Lines->Count; i++)
    {
        sMessage = sFirstLine = "";
        str = RE1->Lines->Strings[i];
        if(str.SubString(0, 1) == "[")//Возможно строка содержит дату
        {
            cont.BreakString(str, "]");//получаем нулевым элементом что-то вроде "[01.01.2017"
            cont.Vector[0].Delete(1, 1);  //получаем что-то вроде "01.01.2017"
            if(GetIsDateValid(cont.Vector[0]))
            {
                if(GetIsMessageNew(cont.Vector[0]))
                {
                    sMessage = sFirstLine = str;
                    str = RE1->Lines->Strings[++i];
                    while(str != "")
                    {
                        sMessage+=str + "\n";
                        str = RE1->Lines->Strings[++i];
                    }
                    nSelStart = AnsiPos(sFirstLine, RE1->Text)-1;
                    if(nSelStart < 0)
                        nSelStart = 0;

                    RE1->SelStart = nSelStart;
                    RE1->SelLength = sMessage.Length()+1;
                    RE1->SelAttributes->Color = clRed;
                    RE1->SelAttributes->Size = 12;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
bool DBManager::GetIsDateValid(AnsiString sDate)
{
    StringsContainer cont;
    cont.BreakString(sDate, ".");
    if(cont.Vector.size() == 3)
        return true;
return false;
}
//------------------------------------------------------------------------------




// SEARCH
//------------------------------------------------------------------------------
void DBManager::AddSearchResultsToResults()
{
    if(Client.SG->ColCount<11)
        return;

    Task tTask;
    for(int i = 0; i<Client.SG->RowCount; i++)
    {
        tTask.GetTaskFromReply(Client.SG, i);
        srResults.Add(tTask);
    }
}
//------------------------------------------------------------------------------
void DBManager::SearchTaskIDwithRegID(int nTaskID, int nRegID)
{
    AnsiString sID = IntToStr(nRegID);
    Client.SendCommand("select * from " + TasksTabName(sID) + " where id = " + IntToStr(nTaskID));
    AddSearchResultsToResults();
}
//------------------------------------------------------------------------------
void DBManager::SearchTaskIDinRegion(TStringGrid *SG1, int nTaskID, AnsiString sRegion)
{
    srResults.Clear();
    if(sRegion == "")
        SearchTaskIDinAllRegions(nTaskID);
    else
    {
        AnsiString sID = GetRegID(sRegion);
        SearchTaskIDwithRegID(nTaskID, StrToInt(sID));
    }
    srResults.Show(SG1);
}
//------------------------------------------------------------------------------
void DBManager::SearchTaskIDinAllRegions(int nTaskID)
{
    int nRegCount = GetMaxID("regions");
    if(!nRegCount)
        return;

    for(int i = 1; i<= nRegCount; i++)
        SearchTaskIDwithRegID(nTaskID, i);
}
//------------------------------------------------------------------------------
void DBManager::SearchParamsWithRegID(int nRegID, AnsiString sHouse, AnsiString sStreet, AnsiString sFlat, AnsiString sTask)
{
    if(sHouse == "")
        return;
    if(nRegID <= 0)
        return;

    AnsiString sID = IntToStr(nRegID);
    AnsiString sCommand = "select * from " + TasksTabName(sID) + " where house like '%" + sHouse + "%'";

    if(sStreet != "")
        sCommand += " && street like '%" + sStreet + "%'";
    if(sFlat != "")
        sCommand += " && flat like '%" + sFlat + "%'";
    if(sTask != "")
        sCommand += " && task like '%" + sTask + "%'";

    Client.SendCommand(sCommand);
    AddSearchResultsToResults();
}
//------------------------------------------------------------------------------
void DBManager::SearchParamsInRegion(TStringGrid *SG1, AnsiString sRegion, AnsiString sHouse, AnsiString sStreet, AnsiString sFlat, AnsiString sTask)
{
    srResults.Clear();
    if(sRegion == "")
        SearchParamsInAllRegions(sHouse, sStreet, sFlat, sTask);
    else
    {
        int nRegID = StrToInt(GetRegID(sRegion));
        SearchParamsWithRegID(nRegID, sHouse, sStreet, sFlat, sTask);
    }
    srResults.Show(SG1);
}
//------------------------------------------------------------------------------
void DBManager::SearchParamsInAllRegions(AnsiString sHouse, AnsiString sStreet, AnsiString sFlat, AnsiString sTask)
{
    int nMax = GetMaxID("regions");
    for(int i = 1; i<= nMax; i++)
        SearchParamsWithRegID(i, sHouse, sStreet, sFlat, sTask);
}
//------------------------------------------------------------------------------
void DBManager::SearchForNewTasksInRegion(int nRegID)
{
    AnsiString sTabName = TasksTabName(IntToStr(nRegID));
    Client.SendCommand("select * from " + sTabName + " where transmit = 0");
    AddSearchResultsToResults();
    Client.SendCommand("update " + sTabName + " set transmit = 1 where transmit = 0");
}
//------------------------------------------------------------------------------
void DBManager::SearchForNewTasks(TMemo *Memo)
{
    AnsiString sRegName;
    int nMaxReg = GetMaxID("regions");
    for(int i=1; i <= nMaxReg; i++)
    {
        srResults.Clear();
        SearchForNewTasksInRegion(i);
        if(srResults.Results.size()>0)
        {
            Client.SendCommand("select name from regions where id = " + IntToStr(i));
            sRegName = Client.GetFieldValue();
            Memo->Lines->Add(sRegName);
            srResults.Show(Memo);
            Memo->Lines->Add("");
            Memo->Lines->Add("");
        }
    }
}
//------------------------------------------------------------------------------
void DBManager::LastTaskInRegion(int nRegID)
{
    AnsiString sTabName = TasksTabName(IntToStr(nRegID));
    int nMaxID = GetMaxID(sTabName);
    Client.SendCommand("select * from " + sTabName + " where id = " + IntToStr(nMaxID));
    AddSearchResultsToResults();
}
//------------------------------------------------------------------------------
void DBManager::LastTasks(TMemo *Memo)
{
    AnsiString sRegName;
    int nMaxReg = GetMaxID("regions");
    for(int i=1; i <= nMaxReg; i++)
    {
        srResults.Clear();
        LastTaskInRegion(i);
        if(srResults.Results.size()>0)
        {
            Client.SendCommand("select name from regions where id = " + IntToStr(i));
            sRegName = Client.GetFieldValue();
            Memo->Lines->Add(sRegName);
            srResults.Show(Memo);
            Memo->Lines->Add("");
            Memo->Lines->Add("");
        }
    }
}
//------------------------------------------------------------------------------






// LOAD
//------------------------------------------------------------------------------
void DBManager::LoadRegions(TStrings *Strings)
{
    Strings->Clear();
    Client.SendCommand("select name from regions");
    for(int nY = 0; nY < Client.SG->RowCount; nY++)
        Strings->Add(Client.SG->Cells[0][nY]);
}
//------------------------------------------------------------------------------
void DBManager::LoadStreets(TStrings *Strings, AnsiString sRegion)
{
    Strings->Clear();
    AnsiString sCommand = "select name from streets";
    if(sRegion != "")
        sCommand+= " where region = " + GetRegID(sRegion);

    Client.SendCommand(sCommand);
    for(int nY = 0; nY < Client.SG->RowCount; nY++)
        Strings->Add(Client.SG->Cells[0][nY]);
}
//------------------------------------------------------------------------------
