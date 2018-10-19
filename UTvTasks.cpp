//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UTvTasks.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

#include "UTvTasks2.h"
#include "UTvTasks4.h"
#include "UTvTasks5.h"



int FindEmptySGRow(TStringGrid *SG1)
{
    int nY = 0;
    while(SG1->Cells[0][nY] != "")
        nY++;

    if(nY == SG1->RowCount)
        SG1->RowCount++;
return nY;
}
//---------------------------------------------------------------------------
Entry::Entry(Variant vSheet, int nID)
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
void Entry::Show(TStringGrid *SG1)
{
    int i, nY = FindEmptySGRow(SG1);
    for(i=0; i<=8; i++)
        SG1->Cells[i][nY] = Data[i];

    if(Data[9]!="")
        SG1->Cells[9][nY] = "+";
}
//---------------------------------------------------------------------------



/*
//StringsContainer
//------------------------------------------------------------------------------
unsigned int StringsContainer::BreakString(AnsiString str, AnsiString sDivider)
{
    Vector.clear();

    AnsiString sTemp;
    int nDivLen = sDivider.Length();
    int nPos = str.AnsiPos(sDivider);
    while(nPos)
    {
        sTemp = str.SubString(0, nPos-1);
        Vector.push_back(sTemp);
        str.Delete(1, nPos+nDivLen-1);
        
        nPos = str.AnsiPos(sDivider);
    }
    if(str != "")
        Vector.push_back(str);//Запихиваем в вектор последнюю часть(то что осталось, если что-то осталось)

return Vector.size();
}
//------------------------------------------------------------------------------
int StringsContainer::IndexOf(AnsiString sValue)
{
    int nResult = -1;
    for(unsigned int i = 0; i<Vector.size(); i++)
    {
        if(Vector[i] == sValue)
        {
            nResult = i;
            return nResult;
        }
    }
return nResult;
}
*/





//Filler
//------------------------------------------------------------------------------
void Filler::Regions()
{
    Variant vSheet;
    AnsiString sName;
    int nSheetsCount = eFile->GetSheetsCount();
    for(int i = 1; i<nSheetsCount; i++)//Последний лист не берем - это тех.инфа
    {
        vSheet = eFile->GetSheet(i);
        sName = vSheet.OlePropertyGet("Name");
        if(sName != "")
            DBMan1->AddRegion(sName);
    }
    SetAutoIncrements();
}
//------------------------------------------------------------------------------
void Filler::SetAutoIncrements()
{
    DBMan1->Client.SendCommand("alter table " + DBMan1->TasksTabName("1") + " auto_increment = 2916");
    DBMan1->Client.SendCommand("alter table " + DBMan1->TasksTabName("2") + " auto_increment = 779");
    DBMan1->Client.SendCommand("alter table " + DBMan1->TasksTabName("3") + " auto_increment = 791");
    DBMan1->Client.SendCommand("alter table " + DBMan1->TasksTabName("4") + " auto_increment = 34");
}
//------------------------------------------------------------------------------
void Filler::Streets()
{
    int nSheetsCount = eFile->GetSheetsCount();
    Variant vSheet = eFile->GetSheet(nSheetsCount); //Берем последний лист - это тех.инфа

    int nY, nX=1;
    AnsiString str = GetValue(vSheet, 1, nX); // Берем район для теста конца базы
    while(str != "")
    {
        nY = 3;
        str = GetValue(vSheet, nY, nX); // Берем первую улицу и движемся вниз по списку
        while(str != "")
        {
            DBMan1->AddStreet(str, nX);
            str = GetValue(vSheet, ++nY, nX);
        }
        str = GetValue(vSheet, 1, ++nX);
    }
}
//------------------------------------------------------------------------------
void Filler::Tasks(int nRegID)
{
    int nSheetsCount = eFile->GetSheetsCount();
    if(nRegID >= nSheetsCount) // Последний лист тоже не берем - там тех инфа
        return;

    Task tTask;
    Variant vSheet = eFile->GetSheet(nRegID);
    int nY = 1;
    AnsiString str = GetValue(vSheet, nY, 1);
    while(str != "")
    {
        tTask.GetTaskFromExcel(vSheet, nY);
        DBMan1->AddTask(nRegID, &tTask);
        str = GetValue(vSheet, ++nY, 1);
    }
}
//------------------------------------------------------------------------------
void Filler::AllTasks()
{
    int nSheetsCount = eFile->GetSheetsCount();
    for(int i = 1; i<nSheetsCount; i++)
        Tasks(i);
}
//------------------------------------------------------------------------------















//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    if(mysql->Checked) // v5.0
    {
        DBMan1 = DBManager(IdTCPClient1, SG2, Memo3);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LabeledEdit2Exit(TObject *Sender)
{
    LabeledEdit2->Text = CheckSpaces(LabeledEdit2->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LabeledEdit3Exit(TObject *Sender)
{
    LabeledEdit3->Text = CheckSpaces(LabeledEdit3->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormActivate(TObject *Sender)
{
    if(!mysql->Checked)
    {
        OFFTimer(Form1);
        LoadNews(RE1, Memo5);
        LoadStreetsAndRegions(StreetLocalBase, ComboBox1, RadioGroup1, Panel1);
    }
    else // v5.0
    {
        if(!DBMan1.GetIsServerUP())
        {
            Panel1->Height = 755;
            Panel1->Width = 905;
            Panel1->Left = 8;
            Panel1->Top = 8;
            Panel1->Caption = "Сервер недоступен. Попробуйте позже...";
            Panel1->Visible = true;
            return;
        }
        OFFTimer(Form1);


        if(!DBMan1.GetIsLastVersion(CURRENT_VERSION))
        {
            Panel1->Height = 755;
            Panel1->Width = 905;
            Panel1->Left = 8;
            Panel1->Top = 8;
            Panel1->Caption = "Версия программы устарела";
            Panel1->Visible = true;
        }
        else
        {
            DBMan1.LoadNews(RE1);
            DBMan1.LoadRegions(RadioGroup1->Items);
            DBMan1.LoadStreets(ComboBox1->Items);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)//Добавить
{
    if( (ComboBox1->Text == "") || (ComboBox1->Text == "Нет такой улицы в выбранном районе!") ||
        (LabeledEdit2->Text == "") ||
        (LabeledEdit3->Text == "") ||
        (LabeledEdit4->Text == "") ||
        (LabeledEdit5->Text == "") ||
        (Memo1->Lines->Count == 0))
    {
        Memo2->Lines->Clear();
        Memo2->Lines->Add("Информация неполная!");
        return;
    }

    int nEntryNumber;
    if(!mysql->Checked)
    {
        int nSheetNumber = 1 + RadioGroup1->ItemIndex;
        AnsiString sDate = Now().DateString();//2
        AnsiString sStreet = ComboBox1->Text; //3
        AnsiString sHouse = LabeledEdit2->Text; //4
        AnsiString sFlat = LabeledEdit3->Text;  //5
        AnsiString sPhone = LabeledEdit4->Text; //6
        AnsiString sProblem = CheckSpaces(Memo1->Text, true);   //7
        AnsiString sOper = LabeledEdit5->Text;  //8
        AnsiString sTime = Now().TimeString(); //12
        AnsiString sEntry;

        //Melnik fix :)
        if(sOper.LowerCase() == "я" || sOper.LowerCase() == " я" || sOper.LowerCase() == "я " || sOper.LowerCase() == " я ")
            sOper = "Мельник";

        //Openfile process

        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp);
        if(GetIsReadOnly(vBook))
        {
            ComboBox1->SetFocus();
            Form2->Label1->Caption = "НЕТ ДОСТУПА!";
            Form2->Label2->Visible = true;
            Form2->Visible = true;
        
            CloseExFile(vApp);
            return;
        }


        Variant vSheet = GetSheet(vBook, nSheetNumber);
        int nY = GetEmptyY(vSheet);

        nEntryNumber = 1+StrToInt(VarToStr(GetCell(vSheet, (nY-1), 1)));//подсчет номера новой задачи
        SetValue(vSheet, nY, 1, nEntryNumber);
        SetValue(vSheet, nY, 2, sDate.c_str());
        SetValue(vSheet, nY, 3, sStreet.c_str());
        SetValue(vSheet, nY, 4, sHouse.c_str());
        SetValue(vSheet, nY, 5, sFlat.c_str());
        SetValue(vSheet, nY, 6, sPhone.c_str());
        SetValue(vSheet, nY, 7, sProblem.c_str());
        SetValue(vSheet, nY, 8, sOper.c_str());
        SetValue(vSheet, nY, 12, sTime.c_str());

        vBook.OleProcedure("Save");

        sEntry = PackEntry(vSheet, nY);
        Memo2->Lines->Add(sEntry);

        CloseExFile(vApp);
        //End
        LoadStreetsFromLocalBase(-1, ComboBox1, StreetLocalBase);
        StreetLocalBase->Lines->Strings[0] = "-1";
    }
    else // v5.0
    {
        Task Task1;
        Task1.GetTaskFromForm(Form1);
        AnsiString sRegion = RadioGroup1->Items->Strings[RadioGroup1->ItemIndex];
        DBMan1.AddTask(sRegion, &Task1);

        Task1.Show(SG1);
        AnsiString sRegID = DBMan1.GetRegID(sRegion);
        AnsiString sTabName = DBMan1.TasksTabName(sRegID);
        nEntryNumber = DBMan1.GetMaxID(sTabName);

    }


    ComboBox1->ItemIndex = -1;
    ComboBox1->Text = "";
    RadioGroup1->ItemIndex = -1;

    LabeledEdit2->Text = "";
    LabeledEdit3->Text = "";
    LabeledEdit4->Text = "";
    LabeledEdit5->Text = "";
    Memo1->Lines->Clear();
    Memo4->Lines->Clear();

    Form2->Label1->Caption = "Номер заявки: " + IntToStr(nEntryNumber);
    Form2->Visible = true;
}
//---------------------------------------------------------------------------


void ClearSGInfo(TStringGrid *SG1)
{
    int nCount = SG1->RowCount;
    for(int i=1; i<nCount; i++)
        SG1->Rows[i]->Clear();

    SG1->RowCount = 10;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn2Click(TObject *Sender)//Поиск
{
    ClearSGInfo(SG1);

    AnsiString sHouse = LabeledEdit2->Text;
    AnsiString sFlat = LabeledEdit3->Text;
    AnsiString sStreet = ComboBox1->Text;
    AnsiString sTask = Memo1->Text;
    int nRadioId = RadioGroup1->ItemIndex;

    Memo2->Lines->Clear();
    if((sHouse == "")/* || (sFlat == "")*/)
    {
        Memo2->Lines->Add("Обязательно введите номер дома!");
        return;
    }

    if(!mysql->Checked)
    {
        Entries.clear();
        SearchProcess(sHouse, Form1, sFlat, sStreet, nRadioId);
    }
    else // v5.0
    {
        if(nRadioId == -1)
            DBMan1.SearchParamsInRegion(SG1, "", sHouse, sStreet, sFlat, sTask);
        else
            DBMan1.SearchParamsInRegion(SG1, RadioGroup1->Items->Strings[nRadioId], sHouse, sStreet, sFlat, sTask);
    }

    if(SG1->Cells[0][1] == "")
        Memo2->Lines->Add("Совпадений не найдено.");
    else
        Memo2->Lines->Add("Поиск завершен. НАЖМИТЕ ДВОЙНЫМ КЛИКОМ ПО СТРОЧКЕ АДРЕСА С ПЛЮСОМ, ЧТОБЫ ВИДЕТЬ КОММЕНТАРИИ МАСТЕРА");

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    if(!mysql->Checked)
        LoadNews(RE1, Memo5);
    else // v5.0
        DBMan1.LoadNews(RE1);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormCreate(TObject *Sender)
{
    //LoadStreetsAndRegions(StreetLocalBase, ComboBox1, RadioGroup1);
    SG1->Cells[0][0] = "Номер";
    SG1->Cells[1][0] = "Дата";
    SG1->Cells[2][0] = "Улица";
    SG1->Cells[3][0] = "Дом";
    SG1->Cells[4][0] = "Квартира";
    SG1->Cells[5][0] = "Телефон";
    SG1->Cells[6][0] = "Описание";
    SG1->Cells[7][0] = "Кто принял";
    SG1->Cells[8][0] = "Время";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Exit(TObject *Sender)
{
    int nSupposedStrID;
    AnsiString sText = ComboBox1->Text;
    if(sText != "")
    {
        nSupposedStrID = CheckExistingStr(sText, ComboBox1);
        if(nSupposedStrID >= 0)
        {
            ComboBox1->ItemIndex = -1;
            ComboBox1->ItemIndex = nSupposedStrID;
        }
        else
        {
            ComboBox1->Text = "Нет такой улицы в выбранном районе!";
            return;
        }

        if(!mysql->Checked)
            RadioGroup1->ItemIndex = GetRegionFromStreet(StreetLocalBase, sText);
        else // v5.0
            RadioGroup1->ItemIndex = RadioGroup1->Items->IndexOf(DBMan1.GetRegionFromStreet(sText));
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup1Click(TObject *Sender)
{
    int nOldID = GetRegionSelected(StreetLocalBase);
    int nCurrentID = RadioGroup1->ItemIndex;
    if(nOldID != nCurrentID)
    {
        if(!mysql->Checked)
        {
            StreetLocalBase->Lines->Strings[0] = IntToStr(nCurrentID);
            LoadStreetsFromLocalBase(nCurrentID, ComboBox1, StreetLocalBase);
        }
        else // v5.0
        {
            DBMan1.LoadStreets(ComboBox1->Items, RadioGroup1->Items->Strings[nCurrentID]);
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{
    Form1->Visible = false;
    Form4->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    ClearSGInfo(SG1);
    Memo2->Lines->Clear();

    AnsiString sNumber = Edit1->Text;
    LeftDigitsOnly(sNumber);
    if(sNumber == "")
    {
        Edit1->Text = "Введите номер!";
        return;
    }
    int nNumber = StrToInt(Edit1->Text);

    if(!mysql->Checked)
    {
        Entries.clear();

        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp, true);
        Variant vSheet;
        int nSheet, nSheetCount = GetSheetsCount(vBook);//StrToInt(VarToStr(GetCell(GetBaseSheet(vBook), 2, 1)));
        for(nSheet=1; nSheet<=nSheetCount; nSheet++)
        {
            vSheet = GetSheet(vBook, nSheet);
            FindEntry(vSheet, nNumber, Form1);
        }
        CloseExFile(vApp);
    }
    else // v5.0
    {
        if(RadioGroup1->ItemIndex>=0)
            DBMan1.SearchTaskIDinRegion(SG1, nNumber, RadioGroup1->Items->Strings[RadioGroup1->ItemIndex]);
        else
            DBMan1.SearchTaskIDinRegion(SG1, nNumber, "");
    }


    if(SG1->Cells[0][1] == "")
        Memo2->Lines->Add("Ничего не найдено.");
    else
        Memo2->Lines->Add("Поиск завершен. НАЖМИТЕ ДВОЙНЫМ КЛИКОМ ПО СТРОЧКЕ АДРЕСА С ПЛЮСОМ, ЧТОБЫ ВИДЕТЬ КОММЕНТАРИИ МАСТЕРА");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
    //Form1->Visible = false;
    Form5->Visible = true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
    Pan2->Visible = false;    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SG1DblClick(TObject *Sender)
{
    RE2->Clear();
    unsigned nRow = SG1->Row;
    if(!nRow)
        return;

    if(!mysql->Checked)
    {
        if(nRow-1 >= Entries.size())
            return;

        RE2->Text = Entries[nRow-1].Data[9];
    }
    else // v5.0
    {
        if(nRow-1 >= DBMan1.srResults.Results.size())
            return;

        RE2->Text = DBMan1.srResults.Results[nRow-1].Data[9];

    }
    



    Pan2->Left = 616;
    Pan2->Top = 248;
    Pan2->Height = 273;
    Pan2->Width = 289;
    Pan2->Visible = true;
}
//---------------------------------------------------------------------------
bool TForm1::TurnOffProgram()
{
    if(!mysql->Checked)
    {
        if(GetIsFileExist("Y:\\SUPPORT\\TVTasks\\OFF.txt"))
            return true;
    }
    else // v5.0
    {
        return DBMan1.TurnOffProgram();        
    }
return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OFFTimer(TObject *Sender)
{
    if(TurnOffProgram())
        Form1->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
    ExcelInfo eFile = ExcelInfo("Z:\\SUPPORT\\TVTasks\\Base.xlsx");
    Filler fill = Filler(&eFile, &DBMan1);
    fill.Regions();
    fill.Streets();
    fill.AllTasks();
    eFile.CloseExFile();
}
//---------------------------------------------------------------------------











void AddRowToExcel(TStringGrid *SG1, int nRow, Variant vSheet, TMemo *Memo)
{
    AnsiString sValue;
    for(int nX=0; nX<SG1->ColCount; nX++)
    {
        sValue = SG1->Cells[nX][nRow];
        Memo->Lines->Add("sValue: "+ sValue);
        SetValue(vSheet, nRow+1, nX+1, sValue.c_str());
    }
}




void __fastcall TForm1::Button6Click(TObject *Sender)
{
    /*
    ExcelInfo eFile = ExcelInfo("D:\\C++\\Edalgo.xlsx", false);
    Variant vSheet = eFile.GetSheet(StrToInt(LabeledEdit2->Text));

    AnsiString str;
    for(int i=0; i<SG1->RowCount; i++)
    {
        AddRowToExcel(SG1, i, vSheet, Memo3);
    }


    eFile.SaveFile();
    eFile.CloseExFile();
    */
}
//---------------------------------------------------------------------------

