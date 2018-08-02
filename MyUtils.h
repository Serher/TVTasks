//---------------------------------------------------------------------------

#ifndef MyUtilsH
#define MyUtilsH
//---------------------------------------------------------------------------
#include <StdCtrls.hpp> // Для Memo
#include <vector>
#include <fstream>
#include <ComCtrls.hpp> //для RE


//FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void DBM(TMemo *Memo, AnsiString str1, AnsiString str2 = "");
void DBM(TMemo *Memo, AnsiString str1 = "", int n = 0);

void NormalizeCB(TComboBox *ComboBox, bool bOne = false, bool bNoNew = true); //bOne - подставляет значение если оно одно в списке; bNoNew - не позволяет вводить то чего нет в списке
void ReplaceLine(TMemo *Memo, int nLine, AnsiString str);

void LeftDigitsOnly(AnsiString &str);

bool GetIsFileExist(AnsiString sPath);


AnsiString GetCurrentDate();
AnsiString GetCurDir();
AnsiString GetDesktopDir();
void CreateShortCut(const AnsiString &sFilePath, const AnsiString &sFileDir, const AnsiString &sShortcutDir, AnsiString sDescription = "");
void CreateShortcutOnDesktop(const AnsiString &sFilePath, const AnsiString &sFileDir, AnsiString sDescription = "");

AnsiString GetStringRight(AnsiString str, int nCount);
AnsiString GetStringLeft(AnsiString str, int nCount);






//CLASSES
////////////////////////////////////////////////////////////////////////////////

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


//For RE
//---------------------------------------------------------------------------
class htmlTag
{    
    public:

    int nFormatType;
    int nStart;
    int nLength;
    unsigned int nParam;



    htmlTag()
    {
        nFormatType = 0;
        nStart = 0;
        nLength = 0;
        nParam = -1;
    }

    void ApplyTagToRE(TRichEdit *RE);
    void FindParam(AnsiString sTag);
    void FindLength(AnsiString sTag);
    void htmlTagFromString(AnsiString sTag);
};
//---------------------------------------------------------------------------
class TaggedString
{
    public:
    //main components
    AnsiString sStrWithTags;
    AnsiString sStrWithoutTags;
    std::vector<htmlTag> TagList; //Массив тэгов
    bool bFormated;
    //---
    void ExtractTagAndSaveItToVector(AnsiString &str, int nStart);


    TaggedString()
    {
        sStrWithTags = "";
        sStrWithoutTags = "";
        TagList.clear();
        bFormated = false;
    }

    TaggedString operator=(AnsiString str);

    //Акскссорные
    //void SetStringWithTags(AnsiString str){sStrWithTags = str;}
    
    //Для внешнего использования
    void ExtractAllTags();
    void DoFormating(TRichEdit *RE);
    void ConvertToFormated(TRichEdit *RE);
    void RemoveFormat(TRichEdit *RE);
    AnsiString ToString(){return sStrWithTags;}
    //bool GetREIsFormated(TRichEdit *RE){return RE->Text == sStrWithoutTags/* && sStrWithTags!=""*/;}
};
//---------------------------------------------------------------------------







#endif
