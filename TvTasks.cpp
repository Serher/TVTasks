//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("UTvTasks.cpp", Form1);
USEFORM("UTvTasks2.cpp", Form2);
USEFORM("UTvTasks3.cpp", Form3);
USEFORM("UTvTasks4.cpp", Form4);
USEFORM("UTvTasks5.cpp", Form5);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TForm2), &Form2);
         Application->CreateForm(__classid(TForm3), &Form3);
         Application->CreateForm(__classid(TForm4), &Form4);
         Application->CreateForm(__classid(TForm5), &Form5);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
