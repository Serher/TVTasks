//---------------------------------------------------------------------------

#ifndef UTvTasks3H
#define UTvTasks3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdTelnet.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
    TButton *Button2;
    TMemo *Memo1;
    TButton *Button1;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TComboBox *ComboBox1;
    TButton *Button7;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TButton *Button6;
    TButton *Button8;
    TButton *Button9;
    TLabel *Label1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TButton *Btn10;
    TEdit *ED1;
    TPanel *Pan1;
    TCheckBox *ChB1;
    TMemo *Memo2;
    TComboBox *ComboBox2;
    TButton *Button10;
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall Button8Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Btn10Click(TObject *Sender);
    void __fastcall Button9Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
    
    

private:	// User declarations
public:		// User declarations
    __fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif





