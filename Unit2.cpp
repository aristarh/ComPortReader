//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
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

void __fastcall TForm2::CheckBox1Click(TObject *Sender)
{
        Edit1->Enabled = CheckBox1->Checked;      
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox2Click(TObject *Sender)
{
        Edit2->Enabled=CheckBox2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
        Form2->Enabled=false;
        Form2->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormActivate(TObject *Sender)
{
        Edit2->Enabled=CheckBox2->Checked;
        Edit1->Enabled=CheckBox1->Checked;        
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox3Click(TObject *Sender)
{
        Edit3->Enabled=CheckBox3->Checked;        
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button1Click(TObject *Sender)
{
        Form2->Enabled=false;
        Form2->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Edit4KeyPress(TObject *Sender, char &Key)
{
if ((Key >= '0') && (Key <= '9')) {}  // цифры
else Key = 0;        
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Edit4Change(TObject *Sender)
{
        RadioGroup1->Items->Strings[0]="«читувати раз на " + Edit4->Text + " м≥л≥секунд";
}
//---------------------------------------------------------------------------



