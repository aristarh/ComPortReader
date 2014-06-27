//---------------------------------------------------------------------------

#include <vcl.h>
#include <conio.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPort"
#pragma link "CPortCtl"
#pragma resource "*.dfm"
TForm1 *Form1;
AnsiString OFile="1";
AnsiString Statment="0";
double Sec=0;
double Xmultiplicity,Ymultiplicity,maxX,maxY,minX=50,minY=20;
const int countX=25,countY=10, startX=25, startY=20;
double lastY=20,lastX=startX,realX,realY;
double Y;
//увеличение
double ZoomMinX,ZoomMaxX,ZoomMinY,ZoomMaxY;
bool ToDraw=true;
TColor number=clBlack,scale=clBlue,line=clBlack;
#define SetPortA SetPort
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void draw(TObject *Sender)
{
        AnsiString out,formatsec;
        //out.sprintf("%5s | %5s",Statment,Sec);
        formatsec.printf("%.2f", Sec);
        out=Statment+"        "+formatsec;
    Form1->Memo1->Lines->Add(out);
    Form1->Memo2->Lines->Add(Statment);
    Form1->Memo3->Lines->Add(Sec);

    if (Sec>maxX)
    {
        maxX=maxX*2;
        if (ToDraw)
        Form1->FormResize(Sender);
    }
    Y=StrToInt(Statment);
    if  (Y>maxY)
    {
        maxY=maxY*2;
        if (ToDraw)
        Form1->FormResize(Sender);
    }
        if (ToDraw)
        {
            Form1->Image1->Canvas->Pen->Color=line;
            realX=(Sec*countX/maxX)*Xmultiplicity+startX;
            realY=Form1->Image1->Height-startY-Y*countY/maxY*Ymultiplicity;
            Form1->Image1->Canvas->MoveTo(lastX,lastY);
            Form1->Image1->Canvas->LineTo(realX,realY);
            lastY=realY;        lastX=realX;
         }
}

void __fastcall TForm1::FormResize(TObject *Sender)
{
        if (ToDraw)
{
        Image1->Picture->Bitmap->FreeImage();
        Image1->Picture->Bitmap = NULL;
        Image1->Width=Form1->ClientWidth;
        Image1->Height=Form1->ClientHeight-Memo1->Height-10;
        Memo1->Top=Image1->Height+10;
  //      BitBtn1->Top=Memo1->Top+10;
        StaticText1->Top=Memo1->Top;
        BitBtn1->Top=StaticText1->Top+StaticText1->Height+20;
     //   BitBtn2->Top=BitBtn1->Top+BitBtn1->Height+10;
        BitBtn2->Top=BitBtn1->Top+BitBtn1->Height+20;
        BitBtn3->Top=BitBtn2->Top;
        Button1->Top=BitBtn2->Top;
        ComComboBox1->Top=BitBtn1->Top+1;
        ComComboBox2->Top=ComComboBox1->Top+ComComboBox1->Height+7;
        ComLed1->Top=StaticText1->Top+10;
        Label1->Top=ComLed1->Top;

        //---------------Подпись осей---------------------//
        Image1->Canvas->Pen->Color=scale;
        Xmultiplicity=(Image1->Width-40-Image1->Canvas->TextWidth("X"))/countX;
        Ymultiplicity=(Image1->Height-20-Image1->Canvas->TextHeight("X"))/countY;
        for (int i=0;i<=countY;i++)
        {
                Image1->Canvas->MoveTo(0,Image1->Height-20-i*Ymultiplicity);
                Image1->Canvas->LineTo(startX,Image1->Height-20-i*Ymultiplicity);
                Image1->Canvas->Pen->Style=psDot;
                Image1->Canvas->LineTo(Image1->Width,Image1->Height-20-i*Ymultiplicity);
                Image1->Canvas->Pen->Style=psSolid;
                if (i!=0)
                Image1->Canvas->TextOutA(0,Image1->Height-21-i*Ymultiplicity,maxY*i/10);
        }
        for (int i=0;i<=countX;i++)
        {
                Image1->Canvas->MoveTo(startX+i*Xmultiplicity,Image1->Height-20);
                Image1->Canvas->LineTo(startX+i*Xmultiplicity,Image1->Height);
                Image1->Canvas->Pen->Style=psDot;
                Image1->Canvas->LineTo(startX+i*Xmultiplicity,0);
                Image1->Canvas->Pen->Style=psSolid;
                Image1->Canvas->TextOutA(20+i*Xmultiplicity-Image1->Canvas->TextWidth(i)-1,Image1->Height-20,maxX*i/25);
        }
        //---------------Создание осей координат----------//
        Image1->Canvas->MoveTo(25,0);
        Image1->Canvas->LineTo(25,Image1->Height-20);
        Image1->Canvas->LineTo(Image1->Width-20,Image1->Height-20);
        Image1->Canvas->TextOutA(25,0,"Y");
        Image1->Canvas->TextOutA(Image1->Width-20,Image1->Height-20,"X");
        Image1->Canvas->MoveTo(startX-10,Image1->Canvas->TextHeight("Y")+30);
        Image1->Canvas->LineTo(startX,Image1->Canvas->TextHeight("Y"));
        Image1->Canvas->LineTo(startX+10,Image1->Canvas->TextHeight("Y")+30);
        Image1->Canvas->MoveTo(Image1->Width-40,Image1->Height-30);
        Image1->Canvas->LineTo(Image1->Width-20,Image1->Height-20);
        Image1->Canvas->LineTo(Image1->Width-40,Image1->Height-10);
        //------------------------------------------------//
        //---------------Рисование графика----------------//
        Image1->Canvas->Pen->Color=line;
        Image1->Canvas->MoveTo(25,Image1->Height-20);
        lastX=startX; lastY=Image1->Height-20;
        for (int i=0;i<Memo3->Lines->Count;i++)
        {
                 realX=(StrToFloat(Memo3->Lines->Strings[i]) *countX/maxX)*Xmultiplicity+startX;
                 Y=StrToFloat(Memo2->Lines->Strings[i]);
                 realY=Image1->Height-20-Y*countY/maxY*Ymultiplicity;
                 if(i==0)       Image1->Canvas->MoveTo(realX-1,realY-1);else
                 Image1->Canvas->LineTo(realX,realY);
                 lastY=realY;        lastX=realX;
         }
        //------------------------------------------------//
 }      else//увеличение
 {
        int MemoMin,MemoMax,temp=0;
        while (ZoomMinX>StrToFloat(Memo3->Lines->Strings[temp]))
        {temp++;}       MemoMin=temp;    temp=0;
        if (ZoomMaxX<StrToFloat(Memo3->Lines->Strings[Memo3->Lines->Count-1]))
        {while (ZoomMaxX>StrToFloat(Memo3->Lines->Strings[temp]))
        {temp++;} MemoMax=temp;} else MemoMax=Memo3->Lines->Count;

//**********************************************
        Image1->Picture->Bitmap->FreeImage();
        Image1->Picture->Bitmap = NULL;
        Image1->Width=Form1->ClientWidth;
        Image1->Height=Form1->ClientHeight-Memo1->Height-10;
        Memo1->Top=Image1->Height+10;
        StaticText1->Top=Memo1->Top;
        BitBtn1->Top=StaticText1->Top+StaticText1->Height+20;
        BitBtn2->Top=BitBtn1->Top+BitBtn1->Height+20;
        BitBtn3->Top=BitBtn2->Top;
        ComComboBox1->Top=BitBtn1->Top+1;
        ComComboBox2->Top=ComComboBox1->Top+ComComboBox1->Height+7;
        ComLed1->Top=StaticText1->Top+10;
        Label1->Top=ComLed1->Top;
        //---------------Подпись осей---------------------//
        Image1->Canvas->Pen->Color=scale;
        Xmultiplicity=(Image1->Width-40-Image1->Canvas->TextWidth("X"))/countX;
        Ymultiplicity=(Image1->Height-20-Image1->Canvas->TextHeight("X"))/countY;
        AnsiString OutY,OutX;
        for (int i=0;i<=countY;i++)
        {
                Image1->Canvas->MoveTo(0,Image1->Height-20-i*Ymultiplicity);
                Image1->Canvas->LineTo(startX,Image1->Height-20-i*Ymultiplicity);
                Image1->Canvas->Pen->Style=psDot;
                Image1->Canvas->LineTo(Image1->Width,Image1->Height-20-i*Ymultiplicity);
                Image1->Canvas->Pen->Style=psSolid;
                if (i!=0)
                {
                        OutY.printf("%.2f", ZoomMinY+(ZoomMaxY-ZoomMinY)*i/10);
                        Image1->Canvas->TextOutA(0,Image1->Height-21-i*Ymultiplicity,OutY);
                }
        }
        for (int i=0;i<=countX;i++)
        {
                Image1->Canvas->MoveTo(startX+i*Xmultiplicity,Image1->Height-20);
                Image1->Canvas->LineTo(startX+i*Xmultiplicity,Image1->Height);
                Image1->Canvas->Pen->Style=psDot;
                Image1->Canvas->LineTo(startX+i*Xmultiplicity,0);
                Image1->Canvas->Pen->Style=psSolid;
                OutX.printf("%.2f", ZoomMinX+(ZoomMaxX-ZoomMinX)*i/25);
                Image1->Canvas->TextOutA(20+i*Xmultiplicity-Image1->Canvas->TextWidth(i)-1,Image1->Height-20,OutX);
        }
        //---------------Создание осей координат----------//
        Image1->Canvas->MoveTo(25,0);
        Image1->Canvas->LineTo(25,Image1->Height-20);
        Image1->Canvas->LineTo(Image1->Width-20,Image1->Height-20);
        Image1->Canvas->TextOutA(25,0,"Y");
        Image1->Canvas->TextOutA(Image1->Width-20,Image1->Height-20,"X");
        Image1->Canvas->MoveTo(startX-10,Image1->Canvas->TextHeight("Y")+30);
        Image1->Canvas->LineTo(startX,Image1->Canvas->TextHeight("Y"));
        Image1->Canvas->LineTo(startX+10,Image1->Canvas->TextHeight("Y")+30);
        Image1->Canvas->MoveTo(Image1->Width-40,Image1->Height-30);
        Image1->Canvas->LineTo(Image1->Width-20,Image1->Height-20);
        Image1->Canvas->LineTo(Image1->Width-40,Image1->Height-10);
        //------------------------------------------------//
        //---------------Рисование графика----------------//
        Image1->Canvas->Pen->Color=line;
        Image1->Canvas->MoveTo(startX,Image1->Height-20);
        lastX=startX; lastY=Image1->Height-20;
        for (int i=MemoMin;i<MemoMax;i++)
        {
                 realX=((StrToFloat(Memo3->Lines->Strings[i])-ZoomMinX)*countX/(ZoomMaxX-ZoomMinX))*Xmultiplicity+startX;
                 Y=StrToFloat(Memo2->Lines->Strings[i]);
                 realY=Image1->Height-20-(Y-ZoomMinY)*countY/(ZoomMaxY-ZoomMinY)*Ymultiplicity;
                 if (i==MemoMin)                 Image1->Canvas->MoveTo(realX,realY);else
                 Image1->Canvas->LineTo(realX,realY);
                 lastY=realY;        lastX=realX;
         }
        //------------------------------------------------//

//**********************************************
 }
}
//---------------------------------------------------------------------------
/*
void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
        ComPort1->Port=ComComboBox1->Text;
          if (ComComboBox1->ItemIndex>-1)
        {
                ComPort1->Open();
                //ComComboBox1->ComPort->Open();
                ComComboBox1->Enabled=false;
                BitBtn1->Enabled=false;
                Statment="0";
                MainMenu1->Items->Enabled=false;
                Timer1->Enabled=true;
        }
        else
        MessageBox(0,"Оберіть будь-ласка компорт",":(", MB_OK);
} */
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)
{
        if (OFile!="1")
         {
                if  (MessageBox(0,"Ви впевненні, що бажаєте перезаписати файл?","Увага!",  MB_YESNO | MB_ICONWARNING)  == IDYES)
                Memo1->Lines->SaveToFile(OFile);
         }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::N2Click(TObject *Sender)
{
      if (OpenDialog1->Execute())
        {
            OFile=OpenDialog1->FileName;
            if  (OFile!="1")
            {
              if (MessageBox(0,"Ви впевненні, що бажаєте відкрити файл?","Увага!",  MB_YESNO | MB_ICONWARNING)== IDYES )
              {
                       Memo1->Lines->Clear();
                       Memo1->Lines->LoadFromFile(OFile) ;
                       Memo2->Lines->Clear();
                       Memo3->Lines->Clear();                       
              }
            }
        for(int i=0;i<Memo1->Lines->Count;i++)
        {
                AnsiString str,temp="";
                str=Memo1->Lines->Strings[i];
                //Label2->Caption=str;
                int j=1;
                while ((str[j]==' '||str[j]=='\t')&&j<=str.Length())     j++;
                while (str[j]!=' '&&j<str.Length())
                {
                    if (str[j]=='.')temp+=',';
                            else temp+=str[j];j++;
                }
                Memo2->Lines->Add(temp);        temp="";
                while ((str[j]==' '||str[j]=='\t')&&j<=str.Length())     j++;
                while (str[j]!=' '&&j<str.Length())
                {
                        if (str[j]=='.')temp+=',';
                                else temp+=str[j]; j++;
                }
                temp+=str[j];
                Memo3->Lines->Add(temp);        temp="";
        }
        draw(Sender);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
        if      (SaveDialog1->Execute())
        {
                OFile=SaveDialog1->FileName;
                Memo1->Lines->SaveToFile(OFile+".txt" );
        }
}
//---------------------------------------------------------------------------
  /*
void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
        Timer1->Enabled=false;
        BitBtn1->Enabled=true;
        ComComboBox1->Enabled=true;
        ComComboBox1->ComPort->Close();
        MainMenu1->Items->Enabled=true;
}   */
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
        Sec+=Timer1->Interval/1000.0;
        if (Form2->RadioGroup1->ItemIndex==0)
        {
                draw(Sender);
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ComPort1RxChar(TObject *Sender, int Count)
{
        AnsiString co="1234567890.";
        AnsiString c;
        ComPort1->ReadStr(c,Count);
        Statment=NULL;
        for (int i=1;i<5;i++)
                for (int j=1;j<12;j++)
                        if (c[i]==co[j])  Statment+=c[i];
    if (Form2->RadioGroup1->ItemIndex==1)
        {
                draw(Sender);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
        Sec=0;
        maxX=minX;
        maxY=minY;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------






void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
        StaticText1->Caption=Statment+"  "+"мкA";
//        StaticText1->Caption=Statment[1]+' '+Statment[2]+' '+Statment[3]+' '+Statment[4]+' ';
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        BitBtn2->Click();
        ComPort1->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
        Timer1->Interval = Form2->Edit4->Text.ToInt();
        if (Form2->CheckBox3->Checked)
        {
                Button1->Caption="SEND '" + Form2->Edit3->Text + "'";
                Button1->Enabled=true;
        }
        ComPort1->Port=ComComboBox1->Text;
        if (ComComboBox1->ItemIndex>-1)
        {
                ComPort1->Open();
                //ComComboBox1->ComPort->Open();
                if (Form2->CheckBox1->Checked)
                {
                        ComPort1->WriteStr(Form2->Edit1->Text);
                }
                ComComboBox1->Enabled=false;
                BitBtn1->Enabled=false;
                BitBtn3->Enabled=false;
                ComComboBox2->Enabled=false;
                Statment="0";
                MainMenu1->Items->Enabled=false;
                Timer1->Enabled=true;
        }
        else
        MessageBox(0,"Оберіть будь-ласка компорт",":(", MB_OK);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
        if (Form2->CheckBox2->Checked)
        {
                ComPort1->WriteStr(Form2->Edit2->Text);
        }
        Button1->Enabled=false;
        ComPort1->Close();
        Timer1->Enabled=false;
        ComComboBox2->Enabled=true;
        BitBtn1->Enabled=true;
        BitBtn3->Enabled=true;
        ComComboBox1->Enabled=true;
        ComComboBox1->ComPort->Close();
        MainMenu1->Items->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
     if (Application->MessageBox(
        "Ви впевнені, що бажаєте звільнити пам'ять?",
        "????????",
                 MB_YESNO + MB_ICONQUESTION) == IDYES)
        {
                Memo1->Lines->Clear();
                Memo2->Lines->Clear();
                Memo3->Lines->Clear();
                Form1->FormResize(Sender);
                maxX=minX; maxY=minY;
                Sec=0;
        }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        if ((Button==mbLeft)&&(ToDraw))
        {
                ZoomMinX=(X-startX)*maxX/(countX*Xmultiplicity);
                ZoomMinY=(Form1->Image1->Height-startY-Y)*maxY/(countY*Ymultiplicity);
        }
//        Label1->Caption=FloatToStr(ZoomMinX)+','+FloatToStr(ZoomMinY);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        if ((Button==mbLeft)&&(ToDraw)&&(Memo3->Lines->Count>0))
        {
                ZoomMaxX=(X-startX)*maxX/(countX*Xmultiplicity);
                ZoomMaxY=(Form1->Image1->Height-startY-Y)*maxY/(countY*Ymultiplicity);
                double temp;
                if (ZoomMaxX<ZoomMinX)
                {
                        temp=ZoomMaxX;
                        ZoomMaxX=ZoomMinX;
                        ZoomMinX=temp;
                }
                if (ZoomMaxY<ZoomMinY)
                {
                        temp=ZoomMaxY;
                        ZoomMaxY=ZoomMinY;
                        ZoomMinY=temp;
                }
                if (ZoomMinX<0) ZoomMinX=0;                if (ZoomMaxX<0) ZoomMaxX=0;
                if ((ZoomMinX<StrToFloat(Memo3->Lines->Strings[Memo3->Lines->Count-1]))&&(ZoomMaxX!=ZoomMinX))
                {
                        ToDraw=false;
                }
                Form1->FormResize(Sender);
        }       else  if (Button==mbRight)
        {ToDraw=true;                Form1->FormResize(Sender);}
//        Label1->Caption=FloatToStr(ZoomMaxX)+','+FloatToStr(ZoomMaxY);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
        Label1->Caption=IntToStr(X)+','+IntToStr(Y);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N6Click(TObject *Sender)
{
        Form2->Enabled=true;
        Form2->Visible=true;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
        if (Form2->CheckBox3->Checked)
        {
                ComPort1->WriteStr(Form2->Edit3->Text);
        }
}
//---------------------------------------------------------------------------

