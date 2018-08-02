//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UTvTasks2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::BitBtn1Click(TObject *Sender)
{
    PrepareForm2(Form2);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
    PrepareForm2(Form2);
}
//---------------------------------------------------------------------------

