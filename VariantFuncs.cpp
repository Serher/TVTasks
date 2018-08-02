//---------------------------------------------------------------------------


#pragma hdrstop

#include "VariantFuncs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)









//VARIANT FUNCS
//Private
//------------------------------------------------------------------------------
void ExcelInfo::InitApp()
{
    try
    {
        vApp=CreateOleObject("Excel.Application");
    }
    catch(...)
    {
        MessageBox(0, "Ошибка при открытии сервера Excel",
            "Ошибка", MB_OK);
        return;
    }
}
//------------------------------------------------------------------------------

//Public
//Rare used
//------------------------------------------------------------------------------
void ExcelInfo::ShowExcel()
{
    vApp.OlePropertySet("Visible",true);
}
//------------------------------------------------------------------------------
Variant ExcelInfo::GetWorkBook(AnsiString vasPath, bool nReadOnly)
{
    Variant vBook, vBooks = vApp.OlePropertyGet("Workbooks");

    //Adres correction
    AnsiString sSub = "\\";
    sSub.Insert("\\", 1);
    if(!AnsiPos(sSub, vasPath))
    {
        int nLen = vasPath.Length();
        while(nLen>1)
        {
            if(vasPath.SubString(nLen, 1) == '\\')
                vasPath.Insert('\\', nLen);
            nLen--;
        }
    }
    //Adres correction end

    vBooks.OleProcedure("Open", vasPath.c_str(), 0, nReadOnly);
    vBook = vBooks.OlePropertyGet("Item",1);
return vBook;
}
//---------------------------------------------------------------------------
int ExcelInfo::GetSheetsCount()
{
    Variant vSheets = vBook.OlePropertyGet("Worksheets");
return vSheets.OlePropertyGet("Count");
}
//---------------------------------------------------------------------------



//Often Used
ExcelInfo::ExcelInfo(AnsiString sPath, bool nReadOnly)
{
    InitApp();
    vBook = GetWorkBook(sPath, nReadOnly);
}
//------------------------------------------------------------------------------
Variant ExcelInfo::GetSheet(int nNumber)
{
    Variant vSheets = vBook.OlePropertyGet("Worksheets");
return vSheets.OlePropertyGet("Item", nNumber);
}




//Other Variant Funcs
//---------------------------------------------------------------------------
Variant GetCell(Variant vSheet, int nY, int nX)
{
	return vSheet.OlePropertyGet("Cells").OlePropertyGet("Item",nY,nX);
}
//---------------------------------------------------------------------------
AnsiString GetValue(Variant vCell)
{
    return VarToStr(vCell);
}
//---------------------------------------------------------------------------
AnsiString GetValue(Variant vSheet, int nY, int nX)
{
    return VarToStr(vSheet.OlePropertyGet("Cells").OlePropertyGet("Item",nY,nX));
}
//---------------------------------------------------------------------------
void SetValue(Variant vSheet, int nY, int nX, char *sValue)
{                            
    Variant vCell = GetCell(vSheet, nY, nX);
    vCell.OlePropertySet("Value", sValue);
}
//---------------------------------------------------------------------------
void SetValue(Variant vSheet, int nY, int nX, int nValue)
{
    Variant vCell = GetCell(vSheet, nY, nX);
    vCell.OlePropertySet("Value", nValue);
}
//---------------------------------------------------------------------------
void SetValue(Variant vCell, char *sValue)
{
    vCell.OlePropertySet("Value", sValue);
}
//---------------------------------------------------------------------------
void SetValue(Variant vCell, int nValue)
{
    vCell.OlePropertySet("Value", nValue);
}
//---------------------------------------------------------------------------
int GetCellColor(Variant vCell)
{
    return vCell.OlePropertyGet("Interior").OlePropertyGet("ColorIndex");
}
//---------------------------------------------------------------------------
void SetCellColor(Variant vCell, int nColor) //0-Black, 1-white, ?? 3-R, 4-G, 5-B, 6-Yellow, etc.
{
    vCell.OlePropertyGet("Interior").OlePropertySet("ColorIndex", nColor);
}
//---------------------------------------------------------------------------






//Other Funcs
//------------------------------------------------------------------------------
int FindEmptyY(Variant vSheet, int nX)
{
    int nY = 1;
    AnsiString str = GetValue(GetCell(vSheet, nY, nX));
    while(str != "")
        str = GetValue(GetCell(vSheet, ++nY, nX));
return nY;
}
//------------------------------------------------------------------------------
int FindEmptyX(Variant vSheet, int nY)
{
    int nX = 1;
    AnsiString str = GetValue(GetCell(vSheet, nY, nX));
    while(str != "")
        str = GetValue(GetCell(vSheet, nY, ++nX));
return nX;
}
//------------------------------------------------------------------------------
int FindXofValue(Variant vSheet, int nY, AnsiString sValue, int nLimit, bool bNeverZero)
{
    int nX = 0;
    int i = 0;
    AnsiString str;
    do{
        str = GetValue(GetCell(vSheet, nY, ++i));
        if(str == sValue)
            nX = i;
    }while(!nX   &&    ((nLimit==0)?(str != ""):(i<=nLimit)));

    if(bNeverZero && !nX)
        nX = i;

    return nX;
}
//------------------------------------------------------------------------------
int FindYofValue(Variant vSheet, int nX, AnsiString sValue, int nLimit, bool bNeverZero)
{
    int nY = 0;
    int i = 0;
    AnsiString str;
    do{
        str = GetValue(GetCell(vSheet, ++i, nX));
        if(str == sValue)
            nY = i;
    }while(!nY   &&    ((nLimit==0)?(str != ""):(i<=nLimit)));

    if(bNeverZero && !nY)
        nY = i;

    return nY;
}
//------------------------------------------------------------------------------
void LoadRow(Variant vSheet, TStrings *Tstr, int nY, int nStart)
{
    int nX = nStart;
    AnsiString sValue = GetValue(GetCell(vSheet, nY, nX));
    while(sValue != "")
    {
        Tstr->Add(sValue);
        sValue = GetValue(GetCell(vSheet, nY, ++nX));
    }
}
//------------------------------------------------------------------------------
void LoadColumn(Variant vSheet, TStrings *Tstr, int nX, int nStart)
{
    int nY = nStart;
    AnsiString sValue = GetValue(GetCell(vSheet, nY, nX));
    while(sValue != "")
    {
        Tstr->Add(sValue);
        sValue = GetValue(GetCell(vSheet, ++nY, nX));
    }
}
//------------------------------------------------------------------------------
