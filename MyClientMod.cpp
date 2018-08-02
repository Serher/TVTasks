//---------------------------------------------------------------------------


#pragma hdrstop

#include "MyClientMod.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)



// GLOBAL FUNCS
//------------------------------------------------------------------------------
AnsiString CodeString(AnsiString str, int nCode)
{
    int k, nLen = str.Length();
    for(k=0; k<nLen; k++)
        str.c_str()[k] = str.c_str()[k]^nCode;
return str;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------





// ErrorMess
//------------------------------------------------------------------------------
AnsiString ErrorMess::IdToStr(int nID)
{
    AnsiString sMess;
    switch(nID)
    {
        case PASS_FAILED:           sMess = "�������� ������!"; break;
        case MYSQL_SYNT_FAILED:     sMess = "�������� ��������� �������!"; break;
        case TIME_OUT:              sMess = "����� ��������� ������� �������!"; break;
        case TRY_LATER:             sMess = "������ �����, ���������� �����..."; break;
        case REQ_FORMAT_FAILED:     sMess = "�������� ������ �������!"; break;
        case COMMAND_ACCEPTED:      sMess = "������� �������!"; break;
        case UNKNOWN_COMMAND:       sMess = "����������� �������!"; break;
        case TRANSMIT_SUCCESS:      sMess = "�������� ������� ���������!"; break;
        case MYSQL_COMMAND_ACCEPTED:sMess = "MySQL ������� �������!"; break;
        case INVALID_REQUEST_ID:    sMess = "�������� ����� �������!"; break;
        case FINISH_ACCEPTED:       sMess = "���������� �������� ������������!"; break;


        default:                sMess = IntToStr(nID); break;
        // ��� ������� 7, �������� ��� ������ ������� ���������
    }
return sMess;
}
//------------------------------------------------------------------------------
AnsiString ErrorMess::Message(int nID, AnsiString sMess)
{
    if(sMess != "")
        return "m" + DIVIDER_1 + IntToStr(CUSTOM_MESSAGE) + DIVIDER_1 + sMess;
    else
        return "m" + DIVIDER_1 + IntToStr(nID);
}
//------------------------------------------------------------------------------






// MyClientMod
//------------------------------------------------------------------------------
MyClientMod::MyClientMod(TIdTCPClient *iCli, TStringGrid *iSG, TMemo *iMemo)
{
    Cli = iCli;
    SG = iSG;
    Memo = iMemo;

    sIP = Cli->Host;
    nCode = 0;
}
//------------------------------------------------------------------------------
// Private funcs
//------------------------------------------------------------------------------
void MyClientMod::ClearSG()
{
    for(int i = 0; i < SG->RowCount; i++)
        SG->Rows[i]->Clear();
    SG->RowCount = 2;
    SG->ColCount = 2;
    SG->ColWidths[1] = 20;// ������ ������ �������. ����� ����� �� ������ ���� ����������� ����� �� ������ � RecieveTable()
}
//------------------------------------------------------------------------------
void MyClientMod::Log(AnsiString sMess)
{
    if(Memo)
        Memo->Lines->Add(sMess);
}
//------------------------------------------------------------------------------
bool MyClientMod::GetIsConnectionOK()
{
    if(sIP != "")
        Cli->Host = sIP;

    // ��������� �����������
    try
    {
        Cli->Connect();
    }
    catch(Exception &exception)
    {
        Log(exception.Message);
    }
return Cli->Connected();
}
//------------------------------------------------------------------------------
bool MyClientMod::TakeCodeFromServ()
{
    AnsiString sReply = SendAndRecieve("15974398791235875", false);
    sReply = GetStringLeft(sReply, sReply.Length()-5);
    sReply = GetStringRight(sReply, sReply.Length()-5);

    try
    {
        nCode = StrToInt(sReply);
    }
    catch(...)
    {
        Log(sReply + " is not int!");
        nCode = 0;
    }
return nCode;
}
//------------------------------------------------------------------------------
int MyClientMod::GetErrors(AnsiString sMessage)
{
    StringsContainer strCont;
    strCont.BreakString(sMessage, DIVIDER_1);
    if(strCont.Vector[0] != "m")
        return 0;


    // ��������� �����-�� ������, ���� ������ ��������� ����� �������� ��� �� ����� ������ ������
    Cli->Disconnect();
    Log("Disconnect.");


    int nErrNumber;
    try
    {
        nErrNumber = StrToInt(strCont.Vector[1]);
    }
    catch(...)
    {
        Log("������������ ����� ������: " + strCont.Vector[1]);
        return CUSTOM_MESSAGE;
    }

    if(nErrNumber == CUSTOM_MESSAGE)
    {
        if(strCont.Vector.size() == 3)
            Log(strCont.Vector[2]);
        else
            Log("����� ������ �����������!");
    }
    else
        Log(ErrorMess().IdToStr(nErrNumber));
return nErrNumber;
}
//------------------------------------------------------------------------------
// Public funcs
//------------------------------------------------------------------------------
void MyClientMod::Send(AnsiString sCommand, bool bNeedCode)
{
    //Log("Send: " + sCommand);
    if(bNeedCode)
        sCommand = CodeString(sCommand, nCode);
    Cli->Write(sCommand);
}
//------------------------------------------------------------------------------
AnsiString MyClientMod::Recieve(bool bNeedCode)
{
    AnsiString sReply = Cli->CurrentReadBuffer();
    if(bNeedCode)
        sReply = CodeString(sReply, nCode);
    //Log("Recieve: " + sReply);
return sReply;
}
//------------------------------------------------------------------------------
AnsiString MyClientMod::SendAndRecieve(AnsiString sMessage, bool bNeedCode)
{
    Send(sMessage, bNeedCode);
    return Recieve(bNeedCode);
}
//------------------------------------------------------------------------------
void MyClientMod::SendCommand(AnsiString sCommand, AnsiString sBase)
{
    ClearSG();
    if(Loggin == "" || Pass == "")
    {
        Log("��� ������ ��� ������!");
        return;
    }

    if(GetIsConnectionOK())
    {
        Log("Connection... Success!");
        if(nCode || TakeCodeFromServ())
        {
            sCommand = Loggin + DIVIDER_2 + Pass + DIVIDER_1 + sCommand + DIVIDER_2 + sBase;
            // ���� ������ �����, ������ ��������� ������� ������� + ID �������
            AnsiString sReply = SendAndRecieve(sCommand);
            if(GetErrors(sReply))
                return;

            StringsContainer strCont;
            strCont.BreakString(sReply, DIVIDER_1);
            if(strCont.Vector.size()<3)
            {
                Log("������������ ����� �������: " + sReply);
                return;
            }


            int nRowCount, nColumnsCount;
            try
            {
                nRowCount = StrToInt(strCont.Vector[0]);
                nColumnsCount = StrToInt(strCont.Vector[1]);
            }
            catch(...)
            {
                Log("������������ ������� �������: " + sReply);
                return;
            }
            if(strCont.Vector[2] == "")
            {
                Log("����� ������� ����: " + sReply);
                return;
            }

            //����� � �������� ���������� �� 1 ������ ��� �������.
            //nRowCount++;
            //nColumnsCount++;
            if(nRowCount<2) nRowCount = 1;
            if(nColumnsCount<2) nColumnsCount = 1;
            SG->RowCount = nRowCount;
            SG->ColCount = nColumnsCount;
            
            RecieveTable(strCont.Vector[2]);
        }
    }
    else
    {
        Log("Connection... Failed.");
        exit(0);
    }
}
//------------------------------------------------------------------------------
void MyClientMod::RecieveTable(AnsiString sRequestID)
{
    int nY = 1;
    int nX = 0;
    AnsiString sRequest, sReply;

    Log("�������� ������ � �������...");
    while(true)
    {
        sRequest = sRequestID + DIVIDER_1 + IntToStr(nY) + DIVIDER_1 + IntToStr(nX);
        sReply = SendAndRecieve(sRequest);

        // ��������� �� ������ � ��� �� ����� ���
        if(GetErrors(sReply))
            return;

        // ���� ��� ��, ����� ���� � �������

        if(nY == 0) // ��� ������ ������� �������� ������ �������
            SG->ColWidths[nX] = 0;

        SG->Cells[nX][nY-1] = sReply;
        CheckFieldWidth(sReply, nX);
        nX++;
        if(nX >= SG->ColCount)
        {
            nX = 0;
            nY++;
            if(nY > SG->RowCount) //������ �������� ��� ��� ����������
            {
                // sRequestID + DIVIDER_1 + "-1" + DIVIDER_1 + "-1"; // ������ ����� ������ ������ ������ �� ����� ����
                nY = -1;
                nX = -1;
            }
        }
    }
}
//------------------------------------------------------------------------------
void MyClientMod::CheckFieldWidth(AnsiString sValue, int nX)
{
    int nWidth = 8 * sValue.Length() + 12;
    if(SG->ColWidths[nX]<nWidth)
        SG->ColWidths[nX] = nWidth;
}
//------------------------------------------------------------------------------
AnsiString MyClientMod::GetFieldValue(int nY, int nX)
{
    return SG->Cells[nX][nY];
}
//------------------------------------------------------------------------------
