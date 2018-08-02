object Form3: TForm3
  Left = 654
  Top = 142
  BorderStyle = bsDialog
  Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077
  ClientHeight = 719
  ClientWidth = 681
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 520
    Top = 8
    Width = 137
    Height = 113
    Caption = #1053#1086#1074#1086#1089#1090#1080
    TabOrder = 5
    object Button6: TButton
      Left = 8
      Top = 48
      Width = 121
      Height = 25
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1085#1086#1074#1086#1089#1090#1080
      TabOrder = 1
      OnClick = Button6Click
    end
    object Button8: TButton
      Left = 8
      Top = 80
      Width = 121
      Height = 25
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1085#1086#1074#1086#1089#1090#1080
      TabOrder = 2
      OnClick = Button8Click
    end
    object Button9: TButton
      Left = 8
      Top = 16
      Width = 121
      Height = 25
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1085#1086#1074#1086#1089#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = Button9Click
    end
  end
  object GroupBox1: TGroupBox
    Left = 24
    Top = 8
    Width = 345
    Height = 81
    Caption = #1042#1099#1073#1088#1072#1090#1100' '#1088#1072#1081#1086#1085
    TabOrder = 4
    object ComboBox1: TComboBox
      Left = 8
      Top = 16
      Width = 185
      Height = 23
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ItemHeight = 15
      ParentFont = False
      TabOrder = 0
    end
    object Button1: TButton
      Left = 200
      Top = 16
      Width = 137
      Height = 25
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1085#1086#1074#1099#1081' '#1088#1072#1081#1086#1085
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      TabStop = False
      OnClick = Button1Click
    end
    object Button3: TButton
      Left = 200
      Top = 48
      Width = 137
      Height = 25
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1091#1083#1080#1094#1091' '#1074' '#1088#1072#1081#1086#1085
      TabOrder = 2
      TabStop = False
      OnClick = Button3Click
    end
  end
  object Button2: TButton
    Left = 32
    Top = 96
    Width = 161
    Height = 25
    BiDiMode = bdLeftToRight
    Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100' '#1085#1086#1074#1099#1077' '#1079#1072#1103#1074#1082#1080
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 0
    OnClick = Button2Click
  end
  object Memo1: TMemo
    Left = 24
    Top = 160
    Width = 633
    Height = 265
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Times New Roman'
    Font.Style = []
    OEMConvert = True
    ParentFont = False
    ParentShowHint = False
    ScrollBars = ssBoth
    ShowHint = False
    TabOrder = 2
  end
  object Button5: TButton
    Left = 32
    Top = 128
    Width = 161
    Height = 25
    Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1087#1086#1089#1083#1077#1076#1085#1080#1077' '#1079#1072#1103#1074#1082#1080
    TabOrder = 1
    OnClick = Button5Click
  end
  object Button7: TButton
    Left = 592
    Top = 144
    Width = 65
    Height = 17
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100
    TabOrder = 3
    OnClick = Button7Click
  end
  object Btn10: TButton
    Left = 344
    Top = 128
    Width = 177
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1082#1086#1084#1084#1077#1085#1090#1072#1088#1080#1080' '#1084#1072#1089#1090#1077#1088#1072
    TabOrder = 6
    OnClick = Btn10Click
  end
  object ED1: TEdit
    Left = 256
    Top = 128
    Width = 81
    Height = 21
    TabOrder = 7
  end
  object Pan1: TPanel
    Left = 184
    Top = 216
    Width = 297
    Height = 129
    BevelWidth = 5
    TabOrder = 8
    Visible = False
    object Label1: TLabel
      Left = 42
      Top = 24
      Width = 212
      Height = 22
      Alignment = taCenter
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1085#1086#1074#1099#1081' '#1088#1072#1081#1086#1085'?'
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clRed
      Font.Height = -19
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentFont = False
      Layout = tlCenter
    end
    object BitBtn1: TBitBtn
      Left = 40
      Top = 80
      Width = 81
      Height = 25
      Caption = #1044#1072
      TabOrder = 0
      TabStop = False
      OnClick = BitBtn1Click
      Kind = bkYes
    end
    object BitBtn2: TBitBtn
      Left = 168
      Top = 80
      Width = 81
      Height = 25
      TabOrder = 1
      TabStop = False
      OnClick = BitBtn2Click
      Kind = bkNo
    end
  end
  object ChB1: TCheckBox
    Left = 200
    Top = 96
    Width = 17
    Height = 17
    Checked = True
    Enabled = False
    State = cbChecked
    TabOrder = 9
  end
  object Memo2: TMemo
    Left = 24
    Top = 504
    Width = 633
    Height = 193
    ScrollBars = ssBoth
    TabOrder = 10
  end
  object Button4: TButton
    Left = 256
    Top = 472
    Width = 217
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1079#1072#1103#1074#1082#1080' '#1080#1079' '#1043#1091#1075#1083#1044#1086#1082#1072' '#1074' '#1088#1072#1081#1086#1085
    TabOrder = 11
    OnClick = Button4Click
  end
  object ComboBox2: TComboBox
    Left = 24
    Top = 472
    Width = 217
    Height = 23
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ItemHeight = 15
    ParentFont = False
    TabOrder = 12
  end
  object Button10: TButton
    Left = 584
    Top = 488
    Width = 75
    Height = 17
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100
    TabOrder = 13
    OnClick = Button10Click
  end
end
