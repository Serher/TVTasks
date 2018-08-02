//---------------------------------------------------------------------------


#pragma hdrstop

#include "MySQLClient.h"
#include "MyUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



MySQLClient::MySQLClient(TIdTCPClient *iCli, TStringGrid *iSG, TMemo *iMemo)
{
    Cli = iCli;
    SG = iSG;
    Memo = iMemo;

    sIP = Cli->Host;
}
//------------------------------------------------------------------------------
void MySQLClient::AddErrorMessage(AnsiString sID)
{
    AnsiString sMess;
    if(sID == "0")
        sMess = "�������� ������!";
    else if(sID == "1")
        sMess = "�������� ��������� �������!";
    else if(sID == "2")
        sMess = "����� ��������� ������� �������!";
    else if(sID == "3")
        sMess = "������ �����, ���������� �����...";
    else if(sID == "4")
        sMess = "�������� ������ �������!";
    else
        sMess = sID;
    
    Log(sMess);
    Cli->Disconnect();
    Log("Disconnect.");
}
//------------------------------------------------------------------------------
void MySQLClient::ReceiveReply(int nReqY, int nReqX)
{
    AnsiString sReply = Cli->CurrentReadBuffer();
    StringsContainer strCont;
    strCont.BreakString(sReply, "-||-");

    if(strCont.Vector.size() == 2)//��������� �� ������
    {
        if(strCont.Vector[0] == "0")
            AddErrorMessage(strCont.Vector[1]);
        else
            AddErrorMessage("������������ ����� �������: " + strCont.Vector[0] + "||" + strCont.Vector[1]);
    }
    else if(strCont.Vector.size() == 3)//������ �����. ������� ������� + ����� �������.
    {
        //���������, ���� �� �� ������ �����
        if(sRequestID != "")//�� ��� �������� ID ����� - �����-�� ������
            AddErrorMessage("������������ ����� �������: " + strCont.Vector[0] + "||" + strCont.Vector[1] + "||" + strCont.Vector[2]);
        else
        {
            int nY, nX;
            try
            {
                nY = StrToInt(strCont.Vector[0]);
                nX = StrToInt(strCont.Vector[1]);

            }
            catch(...)
            {
                AddErrorMessage("������������ ����� �������: " + strCont.Vector[0] + "||" + strCont.Vector[1] + "||" + strCont.Vector[2]);
            }

            if(nY == 0 || nX == 0)//���������� ������� �� ��������� ������ ���� "insert into...", "use..." � �.�.
            {
                Cli->Write(CodeString(strCont.Vector[2] + "-||-0-||-0")); // ���������� ������� ����������� ��� ��� ��, ����� �� �����������
                Log("Finish");
                Cli->Disconnect();
                Log("Disconnect.");
            }
            else if(nY<0 || nX<0)
                AddErrorMessage("������������ ����� �������: " + strCont.Vector[0] + "||" + strCont.Vector[1] + "||" + strCont.Vector[2]);
            else
            {
                SG->RowCount = nY;
                SG->ColCount = nX;
                sRequestID = strCont.Vector[2];

                //���������� ������ �� ��������� ������ ����� ���� � ���� �����
                Cli->Write(CodeString(sRequestID + "-||-1-||-0"));
                ReceiveReply();
            }
        }
    }
    else if(strCont.Vector.size() == 1)
    {
        if(sRequestID == "")//�����-�� �����, �.�. �� ��� �� �������� ID
            AddErrorMessage("������������ ����� �������: " + strCont.Vector[0]);
        else
        {
            SG->Cells[nReqX][nReqY] = strCont.Vector[0];
            nReqX++;
            if(nReqX >= SG->ColCount)
            {
                nReqX = 0;
                nReqY++;
                if(nReqY >= SG->RowCount)
                    nReqY = 0;
            }
            if(nReqY == 0 && nReqX == 0)//������ �������� ��� ��� ����������
            {
                Cli->Write(CodeString(sRequestID + "-||-0-||-0")); // ���������� ������� ����������� ��� ��� ��, ����� �� �����������
                sRequestID = ""; // ������� ����� ������������ �������
                Log("Finish");
                Cli->Disconnect();
                Log("Disconnect.");
            }
            else //���������� ������ �� ��������� ��������� �����
            {
                Cli->Write(CodeString(sRequestID + "-||-" + IntToStr(nReqY+1) + "-||-" + IntToStr(nReqX)));
                ReceiveReply(nReqY, nReqX);
            }
        }
    }
    else
        AddErrorMessage("������������ ����� �������!");
}
//------------------------------------------------------------------------------
void MySQLClient::SendCommand(AnsiString sCommand)
{
    ClearSG();

    if(sIP != "")
        Cli->Host = sIP;

    try
    {
        Cli->Connect();
    }
    catch(Exception &exception)
    {
        Log(exception.Message);
    }

    if(Cli->Connected())
    {
        Log("Connection... Success!");
        //Log("sCommand sent: " + sCommand);
        sCommand = CodeString("kjubytrc-||-" + sCommand + "||tvtask");
        Cli->Write(sCommand);
        ReceiveReply();
    }
    else
    {
        Log("Connection... Failed.");
        exit(0);
    }
}
//------------------------------------------------------------------------------
void MySQLClient::Log(AnsiString sMess)
{
    if(Memo)
        Memo->Lines->Add(sMess);
}
//------------------------------------------------------------------------------
AnsiString MySQLClient::CodeString(AnsiString str)
{
    int k, nLen = str.Length();
    for(k=0; k<nLen; k++)
        str.c_str()[k] = str.c_str()[k]^600;
return str;
}
//------------------------------------------------------------------------------
AnsiString MySQLClient::GetFieldValue(int nY, int nX)
{
    return SG->Cells[nX][nY];
}
//------------------------------------------------------------------------------
void MySQLClient::ClearSG()
{
    for(int i = 0; i < SG->RowCount; i++)
        SG->Rows[i]->Clear();
    SG->RowCount = 1;
    SG->ColCount = 1;
}
