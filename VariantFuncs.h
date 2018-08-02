//---------------------------------------------------------------------------

#ifndef VariantPartsH
#define VariantPartsH
//---------------------------------------------------------------------------

#include <ComObj.hpp>
#include <Classes.hpp>




//---------------------------------------------------------------------------
//----Variant Functions------------------------------------------------------
//---------------------------------------------------------------------------
class ExcelInfo
{
    Variant vApp;
    Variant vBook;

    void InitApp();

    public:
    //Rare used
    void ShowExcel();
    Variant GetWorkBook(AnsiString vasPath, bool nReadOnly = true);
    int GetSheetsCount();

    //Often used
    ExcelInfo(AnsiString sPath, bool nReadOnly = true);
    Variant GetSheet(int nNumber);
    void SaveFile(){vBook.OleProcedure("Save");}
    void CloseExFile(){vApp.OleProcedure("Quit");}
};

Variant GetCell(Variant vSheet, int nY, int nX);
AnsiString GetValue(Variant vCell);
AnsiString GetValue(Variant vSheet, int nY, int nX);
void SetValue(Variant vSheet, int nY, int nX, char *sValue);
void SetValue(Variant vSheet, int nY, int nX, int nValue);
void SetValue(Variant vCell, char *sValue);
void SetValue(Variant vCell, int nValue);
int GetCellColor(Variant vCell);
void SetCellColor(Variant vCell, int nColor); //0-Black, 1-white, ?? 3-R, 4-G, 5-B, 6-Yellow, etc.





//---------------------------------------------------------------------------
//--End of Variant functions-------------------------------------------------
//---------------------------------------------------------------------------


int FindEmptyY(Variant vSheet, int nX = 1);
int FindEmptyX(Variant vSheet, int nY = 1);
int FindXofValue(Variant vSheet, int nY, AnsiString sValue, int nLimit = 0, bool bNeverZero = false);
int FindYofValue(Variant vSheet, int nX, AnsiString sValue, int nLimit = 0, bool bNeverZero = false);

void LoadRow(Variant vSheet, TStrings *Tstr, int nY, int nStart = 1);
void LoadColumn(Variant vSheet, TStrings *Tstr, int nX, int nStart = 1);






#endif
