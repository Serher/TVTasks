object Form4: TForm4
  Left = 552
  Top = 351
  Width = 219
  Height = 213
  Caption = #1055#1072#1088#1086#1083#1100
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 37
    Top = 16
    Width = 138
    Height = 22
    Caption = #1042#1074#1077#1076#1080#1090#1077' '#1087#1072#1088#1086#1083#1100
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Edit1: TEdit
    Left = 38
    Top = 48
    Width = 137
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = #1042#1074#1077#1076#1080#1090#1077' '#1087#1072#1088#1086#1083#1100
    OnEnter = Edit1Enter
  end
  object Button1: TButton
    Left = 54
    Top = 88
    Width = 105
    Height = 25
    Caption = #1042#1074#1077#1089#1090#1080
    TabOrder = 1
    OnClick = Button1Click
  end
  object BitBtn1: TBitBtn
    Left = 68
    Top = 128
    Width = 75
    Height = 25
    Caption = #1042#1099#1093#1086#1076
    TabOrder = 2
    OnClick = BitBtn1Click
    Kind = bkCancel
  end
end
