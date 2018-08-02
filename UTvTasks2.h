//---------------------------------------------------------------------------

#ifndef UTvTasks2H
#define UTvTasks2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TBitBtn *BitBtn1;
    TLabel *Label2;
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif

void PrepareForm2(TForm2 *Form2)
{
    Form2->Visible = false;
    Form2->Label1->Caption = "";
    Form2->Label2->Visible = false;
}
