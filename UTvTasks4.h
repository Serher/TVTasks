//---------------------------------------------------------------------------

#ifndef UTvTasks4H
#define UTvTasks4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *Edit1;
    TButton *Button1;
    TBitBtn *BitBtn1;
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Edit1Enter(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    
private:	// User declarations
public:		// User declarations
    __fastcall TForm4(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
