//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UTvTasks2.h"
#include "UTvTasks3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "UTvTasks.h"

TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject *Sender) //Новые заявки
{
    if(!Form1->mysql->Checked)
    {
        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp);
        if(GetIsReadOnly(vBook))
        {
            Form2->Label1->Caption = "НЕТ ДОСТУПА!";
            Form2->Label2->Visible = true;
            Form2->Visible = true;
        
            CloseExFile(vApp);
            return;
        }

        Variant vSheet;// = GetBaseSheet(vBook);
        AnsiString sText, sSheetName;
        int nFirstNewLine, nLastLine, nSheetNumber = 1;
        int nSheetsCount = GetSheetsCount(vBook);

        bool nNewEntries;
        while(nSheetNumber<=nSheetsCount)
        {
            nNewEntries=false;
            vSheet = GetSheet(vBook, nSheetNumber);
            nLastLine = GetEmptyY(vSheet)-1;
            nFirstNewLine = nLastLine;

            sText = VarToStr(GetCell(vSheet, nFirstNewLine, 11));
            while(sText != "+")
            {
                if(!nNewEntries)
                    nNewEntries=true;
                nFirstNewLine--;
                sText = VarToStr(GetCell(vSheet, nFirstNewLine, 11));
            }

            if(nNewEntries)
            {
                nFirstNewLine++;
                sSheetName = vSheet.OlePropertyGet("Name");
                Memo1->Lines->Add(sSheetName);
                for(nFirstNewLine; nFirstNewLine<=nLastLine; nFirstNewLine++)
                    Memo1->Lines->Add(PackEntry(vSheet, nFirstNewLine, true));

                GetCell(vSheet, nLastLine, 11).OlePropertySet("Value", "+");

                Memo1->Lines->Add("");
                Memo1->Lines->Add("");
            }
            nSheetNumber++;
        }
        vBook.OleProcedure("Save");
        CloseExFile(vApp);
    }
    else // v5.0
    {
        Memo1->Clear();
        Form1->DBMan1.SearchForNewTasks(Memo1);
    }

    if(Memo1->Lines->Strings[0] == "")
        Memo1->Lines->Add("Новых заявок нет.");    
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button5Click(TObject *Sender) //Последние заявки
{
    if(!Form1->mysql->Checked)
    {
        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp, true);
        Variant vSheet;// = GetBaseSheet(vBook);
        int i, nFirstLine, nLastLine, nSheetNumber;
        int nSheetCount = GetSheetsCount(vBook);
        for(nSheetNumber = 1; nSheetNumber<=nSheetCount; nSheetNumber++)
        {
            vSheet = GetSheet(vBook, nSheetNumber);
            i=1;
            nFirstLine = 1;
            nLastLine = 1;
            do{
                if(VarToStr(GetCell(vSheet, i, 11)) == "+")
                {
                    if(nFirstLine == 1)
                        nFirstLine = i;
                    else if(nFirstLine != nLastLine)
                        nFirstLine = nLastLine;
                    nLastLine = i;
                }
                i++;
            }while(VarToStr(GetCell(vSheet, i, 1)) != "");

            Memo1->Lines->Add(vSheet.OlePropertyGet("Name"));
            nFirstLine++;
            for(nFirstLine; nFirstLine<=nLastLine; nFirstLine++)
                Memo1->Lines->Add(PackEntry(vSheet, nFirstLine, true));

            Memo1->Lines->Add("");
            Memo1->Lines->Add("");
        }
        CloseExFile(vApp);

        if(Memo1->Lines->Strings[0] == "")
            Memo1->Lines->Add("Заявок нет.");
    }
    else // v5.0
    {
        Memo1->Clear();
        Form1->DBMan1.LastTasks(Memo1);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button7Click(TObject *Sender)
{
    Memo1->Lines->Clear();    
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
    Form3->Visible = false;
    Form1->Visible = true;    
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormActivate(TObject *Sender)
{
    if(Form1->Panel1->Height<50 && !ComboBox1->Items->Count)
    {
        Form1->DBMan1.LoadRegions(ComboBox1->Items);
        ComboBox2->Items = ComboBox1->Items;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{
    if(!Form1->mysql->Checked)
        LoadRegions(ComboBox1);
    /*else // v5.0
    {
        if(Form1->Panel1->Height<50)
            Form1->DBMan1.LoadRegions(ComboBox1->Items);
    }*/
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button4Click(TObject *Sender)
{
    if(Memo2->Lines->Strings[0] == "")
    {
        Memo2->Lines->Add("Сначала скопируйте сюда заявки из Гуглдок.");
        return;
    }
    int nRegion = ComboBox2->ItemIndex;//0-любера, 1-Юбилейный и т.д.
    if(nRegion == -1)
    {
        //nRegion = 0;
        ComboBox2->Text = "Нужно выбрать район!";
        return;
    }


    if(!Form1->mysql->Checked)
    {
        //Открыть файл
        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp);
        if(GetIsReadOnly(vBook))
        {
            Form2->Label1->Caption = "НЕТ ДОСТУПА!";
            Form2->Label2->Visible = true;
            Form2->Visible = true;
        
            CloseExFile(vApp);
            return;
        }

        Variant vSheet = GetSheet(vBook, (nRegion+1));
        int i=0, nY = GetEmptyY(vSheet);

        AnsiString asLine = Memo1->Lines->Strings[i];
        while(asLine != "")
        {
            AddEntryToExcelBase(asLine, vSheet, nY);
            nY++;

            i++;
            asLine = Memo1->Lines->Strings[i];
        }
        SetValue(vSheet, (nY-1), 11, "+");

        vBook.OleProcedure("Save");
        CloseExFile(vApp);
    }
    else // v5.0
    {
        Task Task1;
        for(int i = 0; i<Memo2->Lines->Count; i++)
        {
            Task1.Clear();
            Task1.GetTaskFromString(Memo2->Lines->Strings[i]);
            Task1.Transmit = ChB1->Checked;
            Form1->DBMan1.AddTask(ComboBox2->Items->Strings[nRegion], &Task1);
        }
    }

    Memo2->Lines->Clear();
    Memo2->Lines->Add("Задачи добавлены.");
    ComboBox2->ItemIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button6Click(TObject *Sender)
{
    if(Form1->mysql->Checked)// при работе версии с MySQL эта часть не нужна
        return;
    Memo1->Lines->Clear();
    Memo1->Lines->LoadFromFile("Z:\\SUPPORT\\TVTasks\\TVNews.txt");
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button8Click(TObject *Sender)
{
    if(Form1->mysql->Checked)// при работе версии с MySQL эта часть не нужна
        return;

    if(Memo1->Lines->Strings[0] == "")
    {
        Memo1->Lines->Add("Файл не может быть пустым!");
        return;
    }
    Memo1->Lines->SaveToFile("Z:\\SUPPORT\\TVTasks\\TVNews.txt");
}
//---------------------------------------------------------------------------


void __fastcall TForm3::Button1Click(TObject *Sender)
{
    AnsiString sName = ComboBox1->Text;
    if(sName == "" || sName == "Введите название!")
    {
        ComboBox1->Text = "Введите название!";
        return;
    }
    else
    {
        ComboBox1->Enabled = false;
        Memo1->Visible = false;
        Pan1->Visible = true;
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TForm3::BitBtn2Click(TObject *Sender)
{
    Pan1->Visible = false;
    ComboBox1->Enabled = true;
    Memo1->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::BitBtn1Click(TObject *Sender)
{
    AnsiString sName = ComboBox1->Text;
    if(sName == "") return;

    if(!Form1->mysql->Checked)
    {
        Variant vApp, vBook, vBaseSheet, vCell, vSheets, vSheet, vNewSheet, vLastSheet;
        vApp = InitApp();
        vBook = GetWorkBook(vApp);
        if(GetIsReadOnly(vBook))
        {
            Form2->Label1->Caption = "НЕТ ДОСТУПА!";
            Form2->Label2->Visible = true;
            Form2->Visible = true;
        
            CloseExFile(vApp);
            return;
        }

        vBaseSheet = GetBaseSheet(vBook);

        int nSheetsCount = GetSheetsCount(vBook);

        vSheets = vBook.OlePropertyGet("Worksheets");
        vLastSheet = vSheets.OlePropertyGet("Item", nSheetsCount);
        vLastSheet.OleProcedure("Copy", vBaseSheet);

        nSheetsCount++;
        vNewSheet = vSheets.OlePropertyGet("Item", nSheetsCount);
        vNewSheet.OlePropertySet("Name", sName.c_str());
        ClearSheet(vNewSheet);
        SetValue(vBaseSheet, 1, nSheetsCount, sName.c_str());//Вносим имя нового район в свободное поле

        vBook.OleProcedure("Save");
        CloseExFile(vApp);

        Form1->StreetLocalBase->Lines->Add("---");
        //--------------------------------
    }
    else //v5.0
    {
        Form1->DBMan1.AddRegion(sName);
    }


    ComboBox1->Items->Add(sName);
    Form1->RadioGroup1->Items->Add(sName);


    Pan1->Visible = false;
    ComboBox1->Enabled = true;
    Memo1->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button3Click(TObject *Sender)
{
    int nRegion = 1+ComboBox1->ItemIndex;//Введен ли район?
    if(!nRegion)
    {
        ComboBox1->Text = "Выберите район!";
        return;
    }

    //второе нажатие, с заполненным полем улица
    AnsiString sNewStreet = Memo1->Lines->Strings[0];
    if(Memo1->Lines->Count>1)
    {
        Memo1->Lines->Clear();
        Memo1->Lines->Add("Название улицы не может состоять из более чем 1 строки!");
        return;
    }
    if(sNewStreet == "")
    {
        Memo1->Lines->Add("Введите название улицы!");
        return;
    }
    if(strlen(sNewStreet.c_str())>20)
    {
        Memo1->Lines->Add("Название улицы слишком длинное!");
        return;
    }

    if(!Form1->mysql->Checked)
    {
        //Добавление в базу
        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp);
        if(GetIsReadOnly(vBook))
        {
            Form2->Label1->Caption = "НЕТ ДОСТУПА!";
            Form2->Label2->Visible = true;
            Form2->Visible = true;
        
            CloseExFile(vApp);
            return;
        }

        Variant vSheet = GetSheet(vBook, nRegion);
        Variant vBaseSheet = GetBaseSheet(vBook);


        int i = 3;
        AnsiString str = GetCell(vBaseSheet, i, nRegion);
        while(str!="")
        {
            if(str == sNewStreet)
            {
                CloseExFile(vApp);
                Memo1->Lines->Clear();
                Memo1->Lines->Add("Такая улица уже есть в базе!");
                return;
            }
            i++;
            str = GetCell(vBaseSheet, i, nRegion);
        }
        SetValue(vBaseSheet, i, nRegion, sNewStreet.c_str());
        vBook.OleProcedure("Save");
        CloseExFile(vApp);
    }
    else //v5.0
    {
        Form1->DBMan1.AddStreet(sNewStreet, ComboBox1->Text);
    }

    Memo1->Lines->Clear();
    Memo1->Lines->Add("Улица добавлена в базу.");
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Btn10Click(TObject *Sender)
{
    if(Memo1->Lines->Strings[0] == "")
    {
        Memo1->Lines->Add("Сначала скопируйте сюда информацию из Гуглдок.");
        return;
    }
    int nRegion = ComboBox1->ItemIndex;//0-любера, 1-Юбилейный и т.д.
    if(nRegion == -1)
    {
        ComboBox1->Text = "Нужно выбрать район!";
        return;
    }
    if(ED1->Text == "")
    {
        Memo1->Lines->Add("Не указан номер!");
        return;

    }


    if(!Form1->mysql->Checked)
    {
        //Открыть файл
        Variant vApp = InitApp();
        Variant vBook = GetWorkBook(vApp);
        if(GetIsReadOnly(vBook))
        {
            Form2->Label1->Caption = "НЕТ ДОСТУПА!";
            Form2->Label2->Visible = true;
            Form2->Visible = true;
        
            CloseExFile(vApp);
            return;
        }


        Variant vSheet = GetSheet(vBook, (nRegion+1));
        int i = 0;
        int nY = FindYofValue(vSheet, 1, ED1->Text);
        if(!nY)
        {
            Memo1->Lines->Add("Номер не найден!");
            return;
        }

        AnsiString asLine = Memo1->Lines->Strings[i];
        while(asLine != "")
        {
            AddEntryToExcelBase(asLine, vSheet, nY, 13);
            nY++;

            i++;
            asLine = Memo1->Lines->Strings[i];
        }

        vBook.OleProcedure("Save");
        CloseExFile(vApp);
    }
    else // v5.0
    {
        int nTaskID = StrToInt(ED1->Text);
        for(int i = 0; i < Memo1->Lines->Count; i++)
        {
            Form1->DBMan1.AddMasterComents(Memo1->Lines->Strings[i], ComboBox1->Text, nTaskID);
            nTaskID++;
        }
    }

    Memo1->Lines->Clear();
    Memo1->Lines->Add("Инфа добавлена.");
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button9Click(TObject *Sender)
{
    if(Form1->mysql->Checked)
    {
        AnsiString sDate, sNews = Memo1->Lines->Strings[0];
        StringsContainer strCont;
        strCont.BreakString(sNews, "]");
        if(strCont.Vector.size()>1) // Значит указана также дата
        {
            sDate = strCont.Vector[0].Delete(1, 1);
            sNews = strCont.Vector[1];
        }
        else
        {
            sDate = Now().DateString();
            sNews = strCont.Vector[0];
        }
        for(int i=1; i < Memo1->Lines->Count; i++)
            sNews += "\n" + Memo1->Lines->Strings[i];

        Form1->DBMan1.AddNews(sNews, sDate);

        Memo1->Clear();
        Memo1->Lines->Add("Новость добавлена!");
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm3::Button10Click(TObject *Sender)
{
    Memo2->Clear();    
}
//---------------------------------------------------------------------------


