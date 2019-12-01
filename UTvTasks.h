//---------------------------------------------------------------------------
#ifndef UTvTasksH
#define UTvTasksH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <Grids.hpp>


#include <ComCtrls.hpp>
#include <ComObj.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <vector>
//#include <utilcls.h>
#include <fstream>
#include "DBManager.h"
#include "MyUtils.h"
#include "VariantFuncs.h"

const AnsiString VERSION_FILE = "4_4.txt";
const AnsiString CURRENT_VERSION = "6.4";


//---------------------------------------------------------------------------
class Entry
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

    Entry(){};
    Entry(Variant vSheet, int nID);

    void Show(TStringGrid *SG1);
};



/*
//---------------------------------------------------------------------------
class StringsContainer
{
    public:
    std::vector<AnsiString> Vector;

    StringsContainer(){Vector.clear();}
    unsigned int BreakString(AnsiString str, AnsiString sDivider);
    int IndexOf(AnsiString sValue);
};
//---------------------------------------------------------------------------
*/



class Filler
{
    public:
    ExcelInfo *eFile;
    DBManager *DBMan1;

    //Constructor
    Filler(){eFile = 0; DBMan1 = 0;}
    Filler(ExcelInfo *ieFile, DBManager *iDBMan1){eFile = ieFile; DBMan1 = iDBMan1;}

    void Regions();
    void SetAutoIncrements();
    void Streets();
    void Tasks(int nRegID);
    void AllTasks();
};










//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TLabeledEdit *LabeledEdit2;
    TLabeledEdit *LabeledEdit3;
    TLabeledEdit *LabeledEdit4;
    TLabeledEdit *LabeledEdit5;
    TMemo *Memo1;
    TLabel *Label1;
    TMemo *Memo2;
    TLabel *Label2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TImage *Image1;
    TMemo *Memo4;
    TLabel *Label3;
    TTimer *Timer1;
    TMemo *Memo5;
    TComboBox *ComboBox1;
    TLabel *Label4;
    TRadioGroup *RadioGroup1;
    TMemo *StreetLocalBase;
    TButton *Button1;
    TLabel *Label5;
    TEdit *Edit1;
    TButton *Button2;
    TButton *Button3;
    TPanel *Panel1;
    TRichEdit *RE1;
    TStringGrid *SG1;
    TPanel *Pan2;
    TButton *Button4;
    TRichEdit *RE2;
    TTimer *OFF;
    TIdTCPClient *IdTCPClient1;
    TMemo *Memo3;
    TStringGrid *SG2;
    TCheckBox *mysql;
    TButton *Button5;
    TButton *Button6;
    TComboBox *CB2;

    void __fastcall LabeledEdit2Exit(TObject *Sender);
    void __fastcall LabeledEdit3Exit(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ComboBox1Exit(TObject *Sender);
    void __fastcall RadioGroup1Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall SG1DblClick(TObject *Sender);
    void __fastcall OFFTimer(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall CB2Select(TObject *Sender);

private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    bool TurnOffProgram();


    std::vector<Entry> Entries;
    DBManager DBMan1;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif









bool GetIsFileExist(AnsiString sPath)
{
    std::fstream F;
    F.open(sPath.c_str());
    if(F)
    {
        F.close();
        return true;
    }
return false;
}




//---------------------------------------------------------------------------
AnsiString CheckSpaces(AnsiString str_as_AnsiString, bool nEnter=false)
{
    char *sArrayStr = str_as_AnsiString.c_str();

    int i;
	int nLength = strlen(sArrayStr);

	for(i=0; i<=nLength; i++)
	{
        if(!nEnter)
        {
            if((sArrayStr[i] == ' ') || (sArrayStr[i] == '\t'))
			    sArrayStr[i] = '_';
        }
        else
        {
            if((sArrayStr[i] == '\n') /*|| (sArrayStr[i] == '\0')*/ || (sArrayStr[i] == '\r'))
                sArrayStr[i] = ' ';
        }
	}
return AnsiString(sArrayStr);
}
//---------------------------------------------------------------------------
bool GetIsDateValid(AnsiString sDate)
{
    StringsContainer cont;
    cont.BreakString(sDate, ".");
    if(cont.Vector.size() == 3)
        return true;
return false;
}
//---------------------------------------------------------------------------
bool GetIsMessageNew(AnsiString sDate)
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
//---------------------------------------------------------------------------
void LoadNews(TRichEdit *RE1, TMemo *Help)
{
    Help->Lines->Clear();
    //Help->Lines->LoadFromFile("Y:\\SUPPORT\\TVTasks\\TVNews.txt");
    Help->Lines->LoadFromFile("Z:\\SUPPORT\\TVTasks\\TVNews.txt");

    int nFinish=0, nCount = Help->Lines->Count;
    AnsiString str;
    while(nFinish<2 && nCount>=0)
    {
        str = Help->Lines->Strings[nCount];
        if(!nFinish || (str == ""))//normal archiv deleting
        {
            Help->Lines->Delete(nCount);
            nCount--;
        }
        else
            nFinish++;

        if(AnsiPos("--Архив--", str))
            nFinish=1;
    }

    //NewsMemo->Lines->Clear();
    //NewsMemo->Lines = Help->Lines;
    RE1->Lines->Clear();
    RE1->Lines = Help->Lines;

    //Форматирование текста
    StringsContainer cont;
    AnsiString sMessage, sFirstLine;
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
//--------------------------------------------------------------
Variant InitApp()
{
    Variant vApp;
    try
    {
        vApp=CreateOleObject("Excel.Application");
    }
    catch(...)
    {
        MessageBox(0, "Ошибка при открытии сервера Excel",
            "Ошибка", MB_OK);
        return 1;
    }
return vApp;
}
//---------------------------------------------------------------------------
int GetIsReadOnly(Variant vBook)
{
    Variant ReadOnly = vBook.OlePropertyGet("ReadOnly");
    if(VarToStr(ReadOnly) == "True")
        return true;
return false;
}
//---------------------------------------------------------------------------
Variant GetWorkBook(Variant vApp, int nReadOnly = false)
{
    Variant vBook, vBooks = vApp.OlePropertyGet("Workbooks");
    AnsiString vasPath = "Z:\\SUPPORT\\TVTasks\\Base.xlsx";
    //AnsiString vasPath = "Y:\\SUPPORT\\TVTasks\\Base.xlsx";

    //AnsiString vasPath = "D:\\TestBase.xlsx";    //for tests

    vBooks.OleProcedure("Open", vasPath.c_str(), 0, nReadOnly);
    vBook = vBooks.OlePropertyGet("Item",1);
return vBook;
}
//---------------------------------------------------------------------------
Variant GetSheet(Variant vBook, int nNumber)
{
    Variant vSheets = vBook.OlePropertyGet("Worksheets");
return vSheets.OlePropertyGet("Item", nNumber);
}
//---------------------------------------------------------------------------
Variant GetBaseSheet(Variant vBook)
{
    //int i=1;
    //Variant vSheet = GetSheet(vBook, i);
    //AnsiString sName = VarToStr(vSheet.OlePropertyGet("Name"));
    /*
    while((sName != "ForBase") && i<6)
    {
        i++;
        vSheet = GetSheet(vBook, i);
        sName = VarToStr(vSheet.OlePropertyGet("Name"));
    }*/

    Variant vSheets = vBook.OlePropertyGet("WorkSheets");
    int nCount = vSheets.OlePropertyGet("Count");
    Variant vSheet = GetSheet(vBook, nCount);

return vSheet;
}
//--------------------------------------------------------------------------
int GetSheetsCount(Variant vBook)
{
    Variant vSheets = vBook.OlePropertyGet("WorkSheets");
    int nCount = vSheets.OlePropertyGet("Count")-1;
return nCount;
}
//---------------------------------------------------------------------------
Variant GetCell(Variant vSheet, int nY, int nX)
{
	return vSheet.OlePropertyGet("Cells").OlePropertyGet("Item",nY,nX);
}
//---------------------------------------------------------------------------
void CloseExFile(Variant vApp)
{
    vApp.OleProcedure("Quit");
}
//---------------------------------------------------------------------------
AnsiString PackEntry(Variant vSheet, int nID, bool nTabs=false)
{
    AnsiString sResult, str;
    int i;
    for(i=1; i<=12; i++)
    {
        str = VarToStr(GetCell(vSheet, nID, i));
        if(str !="")
            sResult += (str+"\t");
        else if(nTabs && (i<12))
            sResult += "\t";

        if((i == 3) && !nTabs)
            sResult += "\t";
    }
return sResult;
}
//---------------------------------------------------------------------------
void SearchInRegion(Variant vBook, int nRegion, AnsiString sHouse, TForm1 *Form1, AnsiString sFlat = "", AnsiString sStreet = "")
{
    Entry ent;
    TStringGrid *SG1 = Form1->SG1;

    AnsiString sHouseInBase, sFlatInBase, sStreetInBase, sEntry;
    Variant vSheet = vBook.OlePropertyGet("Worksheets").OlePropertyGet("Item",nRegion);

    int i = 1;
    bool nSearchStreet = ((sStreet != "") && (sStreet != "Нет такой улицы в выбранном районе!"))?true:false;
    AnsiString sID = VarToStr(GetCell(vSheet, i, 1));//Number of entry
    while(sID != "")
    {
        sHouseInBase = VarToStr(GetCell(vSheet, i, 4));
        sFlatInBase = VarToStr(GetCell(vSheet, i, 5));
        if(nSearchStreet)
            sStreetInBase = VarToStr(GetCell(vSheet, i, 3));

        if(AnsiPos(sHouse, sHouseInBase))
        {
            if((sFlat == "") || AnsiPos(sFlat, sFlatInBase))
            {
                if((sStreetInBase == "") || (AnsiPos(sStreet, sStreetInBase)))
                {
                    //Print Entry
                    /* Old variant
                    sEntry = PackEntry(vSheet, i);
                    ResMemo->Lines->Add(sEntry);
                    */
                    ent = Entry(vSheet, i);
                    Form1->Entries.push_back(ent);
                    ent.Show(SG1);
                }
            }
        }
        i++;
        sID = VarToStr(GetCell(vSheet, i, 1));
    }
}
//---------------------------------------------------------------------------
void SearchProcess(AnsiString sHouse, TForm1 *Form1, AnsiString sFlat = "", AnsiString sStreet = "", int nRadioId=-1)
{
    int nRegion, nSheetsCount;
    Variant vApp, vBook;
    //OpenFile
    vApp = InitApp();
    vBook = GetWorkBook(vApp, true);

    if(nRadioId != -1)
    {
        nRegion = nRadioId+1;
        //SearchInRegion(vBook, nRegion, sHouse, ResMemo, sFlat, sStreet);
        SearchInRegion(vBook, nRegion, sHouse, Form1, sFlat, sStreet);
    }
    else
    {
        //search everywhere
        nSheetsCount = GetSheetsCount(vBook);
        for(nRegion=1; nRegion<=nSheetsCount; nRegion++)
            SearchInRegion(vBook, nRegion, sHouse, Form1, sFlat, sStreet);
    }
    CloseExFile(vApp);
}
//---------------------------------------------------------------------------
int CheckExistingStr(AnsiString sText, TComboBox *ComboBox)
{   
    int i=0;
    char *temp;

    AnsiString str = ComboBox->Items->Strings[i];
    while(str !="")
    {
        if(AnsiPos(str.UpperCase(), sText.UpperCase()) || AnsiPos(sText.UpperCase(), str.UpperCase()))
            return i;
        i++;
        str = ComboBox->Items->Strings[i];
    }
return -1;
}
//---------------------------------------------------------------------------
void SaveStreetToLocalBase(AnsiString sStreet, TMemo *LocalMemo, bool nNewRegion=false)
{
    if(nNewRegion)
        LocalMemo->Lines->Add("---");
    else
        LocalMemo->Lines->Add(sStreet);
}

//---------------------------------------------------------------------------
void LoadStreetsFromRegion(TComboBox *ComboBox1, Variant vSheet, int nRegion, TMemo *LocalMemo)
{
    int nID=3;
    AnsiString str = VarToStr(GetCell(vSheet, nID, nRegion));
    while(str !="")
    {   
        ComboBox1->Items->Add(str);
        SaveStreetToLocalBase(str, LocalMemo);
        nID++;
        str = VarToStr(GetCell(vSheet, nID, nRegion));
    }
}
//---------------------------------------------------------------------------
void LoadStreetsAndRegions(TMemo *LocalMemo, TComboBox *ComboBox1, TRadioGroup *RadioGroup1, TPanel *Panel1)
{
    Variant vApp, vBook, vBaseSheet;
    vApp = InitApp();
    vBook = GetWorkBook(vApp);
    vBaseSheet = GetBaseSheet(vBook);

    //if(VarToStr(GetCell(vBaseSheet, 2, 1)) != "-") //Спойлер. Добавить "-" в поле 2,1 в листе Base чтобы сделать версию программы неактуальной
    if(GetIsFileExist("Z:\\SUPPORT\\TVTasks\\" + VERSION_FILE))//Спойлер. Чтобы все работало норм, нужно чтобы в основной папке был файл с название как в константе VERSION_FILE
    {
        int nRegion = 1;
        //Nullify data
        LocalMemo->Lines->Clear();
        LocalMemo->Lines->Add("-1");
        RadioGroup1->Items->Clear();
        ComboBox1->Items->Clear();

        AnsiString sRegion = VarToStr(GetCell(vBaseSheet, 1, nRegion));
        while(sRegion != "")
        {
            RadioGroup1->Items->Add(sRegion);
            LoadStreetsFromRegion(ComboBox1, vBaseSheet, nRegion, LocalMemo);
            SaveStreetToLocalBase("", LocalMemo, true);
            nRegion++;
            sRegion = VarToStr(GetCell(vBaseSheet, 1, nRegion));
        }
    }
    else
    {
        Panel1->Height = 690;
        Panel1->Width = 905;
        Panel1->Left = 8;
        Panel1->Top = 8;
        Panel1->Visible = true;
    }
    CloseExFile(vApp);
}
//---------------------------------------------------------------------------
int GetRegionSelected(TMemo *LocalMemo)
{
    AnsiString sResult = LocalMemo->Lines->Strings[0];
    int nResult = StrToInt(sResult);
return nResult;
}
//---------------------------------------------------------------------------
void LoadStreetsFromLocalBase(int nRegion, TComboBox *ComboBox, TMemo *LocalMemo)
{
    ComboBox->Items->Clear();
    int i=1;
    int nDeffis=0;

    AnsiString str = LocalMemo->Lines->Strings[i];
    while(((nDeffis<=nRegion)  &&  (nRegion != -1)) ||
          (    (str != "")     &&  (nRegion == -1))   )
    {
        if(str == "---")
            nDeffis++;
        else
        {
            if((nDeffis==nRegion) || (nRegion<0))
                ComboBox->Items->Add(str);
        }
        i++;
        str = LocalMemo->Lines->Strings[i];
    }
}
//---------------------------------------------------------------------------
int GetRegionFromStreet(TMemo *mBase, AnsiString sStreet)
{
    int nRegion=0;
    int i=0;
    AnsiString str = mBase->Lines->Strings[i];
    while(str != "")
    {
        if(sStreet==str)
            return nRegion;
        if(str == "---")
            nRegion++;
        i++;
        str = mBase->Lines->Strings[i];
    }
return -1;
}
//---------------------------------------------------------------------------
int GetEmptyY(Variant vSheet)
{
    AnsiString TestStr;
    int i=0;
    do{
        i++;
        TestStr = VarToStr(GetCell(vSheet, i, 1));
    }while(TestStr != "");
return i;
}
//--------------------------------------------------------------------------
void LoadRegions(TComboBox *ComboBox1)
{
    ComboBox1->Items->Clear();
    Variant vApp, vBook, vSheet;
    vApp = InitApp();
    vBook = GetWorkBook(vApp);
    vSheet = GetBaseSheet(vBook);
    int i, nSheetsCount = GetSheetsCount(vBook);
    for(i = 1; i<=nSheetsCount; i++)
        ComboBox1->Items->Add(VarToStr(GetCell(vSheet, 1, i)));
    CloseExFile(vApp);
}
//--------------------------------------------------------------------------
void SetValue(Variant vSheet, int nY, int nX, char *sValue)
{
    Variant vCell = GetCell(vSheet, nY, nX);
    vCell.OlePropertySet("Value", sValue);
}
//--------------------------------------------------------------------------
void SetValue(Variant vSheet, int nY, int nX, int nValue)
{
    Variant vCell = GetCell(vSheet, nY, nX);
    vCell.OlePropertySet("Value", nValue);
}
//--------------------------------------------------------------------------
void AddEntryToExcelBase(AnsiString sEntry, Variant vSheet, int nY, int nX=2)
{
    char *sLine;
    char sPart[500];
    int n=0, k=0;
    bool nFinish = false;
    int nEntryNumber = 1+StrToInt(VarToStr(GetCell(vSheet, (nY-1), 1)));
    SetValue(vSheet, nY, 1, nEntryNumber);

    sLine = sEntry.c_str();
    while(!nFinish)
    {
        if(sLine[n] == '\0')
            nFinish=true;

        if((sLine[n] == '\t') || nFinish)
        {
            sPart[k] = '\0';
            SetValue(vSheet, nY, nX, sPart);
            k=0;
            nX++;
        }
        else
        {
            sPart[k]=sLine[n];
            k++;
        }
        n++;
    }
}
//--------------------------------------------------------------------------
void FindEntry(Variant vSheet, int nNumber, TForm1 *Form1)
{
    Entry ent;
    int nCell, nY=1;
    AnsiString sCell = VarToStr(GetCell(vSheet, nY, 1));
    while(sCell != "")
    {
        nCell = StrToInt(sCell);
        if(nCell == nNumber)
        {
            //Memo->Lines->Add(PackEntry(vSheet, nY));
            ent = Entry(vSheet, nY);
            Form1->Entries.push_back(ent);
            ent.Show(Form1->SG1);
        }
        nY++;
        sCell = VarToStr(GetCell(vSheet, nY, 1));
    }
}
//--------------------------------------------------------------------------
void ClearSheet(Variant vSheet)
{
    Variant vCell=vSheet.OlePropertyGet("Range","A:L");
    vCell.OlePropertySet("Value", "");
}
//--------------------------------------------------------------------------
/*
int FindYofValue(Variant vSheet, int nX, AnsiString sValue)
{
    int nY = 0;
    int i = 0;
    AnsiString str;
    do{
        str = VarToStr(GetCell(vSheet, ++i, nX));
        if(str == sValue)
            nY = i;
    }while(!nY && str != "" && i<=10000);

    return nY;
}*/
