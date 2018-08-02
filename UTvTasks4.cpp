//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UTvTasks4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "UTvTasks.h"
#include "UTvTasks3.h"

TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::BitBtn1Click(TObject *Sender)
{
    Label1->Visible = true;
    Edit1->Visible = true;
    Button1->Visible = true;

    Form4->Visible = false;
    Form1->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Edit1Enter(TObject *Sender)
{
    if(Edit1->PasswordChar != 'x')
    {
        Edit1->PasswordChar = 'x';
        Edit1->Text = "";
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
    AnsiString sEntered = Edit1->Text;
    if(sEntered == "12354")
    {
        Form3->GroupBox1->Enabled = true;
        Form3->GroupBox2->Enabled = true;
        Form3->Button2->Enabled = true;
        Form3->Button5->Enabled = true;
        Form3->Button7->Enabled = true;
        Form3->Memo1->Enabled = true;

        Form4->Visible = false;
        Form3->Visible = true;
    }
    else
    {
        Label1->Font->Color = clRed;
        Label1->Caption = "Пароль неверный!";
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormClose(TObject *Sender, TCloseAction &Action)
{
    Label1->Visible = true;
    Edit1->Visible = true;
    Button1->Visible = true;

    Form4->Visible = false;
    Form1->Visible = true;    
}
//---------------------------------------------------------------------------

