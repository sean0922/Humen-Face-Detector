//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

int mx_x=0,mx_y=0,mi_x=999,mi_y=999;

//---------------------------------------------------------------------------
int min(int a,int b)
{
	if(a>=b)
	{
		return b;
	}
	else
	{
		return a;
	}
}

int max(int a,int b)
{
	if(a>=b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int compare(const void *a,const void *b)
{
	int aa=*(int *)a;
	int bb=*(int *)b;

	if(aa<bb) return -1;
	if(aa>bb) return 1;
	else
	return 0;
}

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  if(OpenPictureDialog1->Execute())
  {
	Image1->Picture->LoadFromFile(OpenPictureDialog1->FileName);
	Bmp=Image1->Picture->Bitmap;


	Image2->Left=Image1->Left+Image1->Width+50;
   //	Image2->Width=100;
   //	Image2->Height=Bmp->Height;
	Image2->Picture->Bitmap=new Graphics::TBitmap();
	Image2->Picture->Bitmap->Width=100;
	Image2->Picture->Bitmap->Height=Bmp->Height;
	Image2->Picture->Bitmap->PixelFormat=pf24bit;

	Image3->Top=Image1->Top+Image1->Height+20;
   //	Image3->Height=100;
   //	Image3->Width=Bmp->Width;
	Image3->Picture->Bitmap=new Graphics::TBitmap();
	Image3->Picture->Bitmap->Width=Bmp->Width;
	Image3->Picture->Bitmap->Height=100;
	Image3->Picture->Bitmap->PixelFormat=pf24bit;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  Colorr(Image1->Picture->Bitmap,Image1->Picture->Bitmap);
  Image1->Repaint();
}
void __fastcall TForm1::Button3Click(TObject *Sender)
{
 ToBinary_S(Image1->Picture->Bitmap,Image1->Picture->Bitmap,235);
 Image1->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  Bmp=NULL;
  hProjection=NULL;
  vProjection=NULL;
}
//----------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
  float scale;

  if(hProjection!=NULL)
  {
	  delete [] hProjection;
	  hProjection=NULL;
  }
  else
  {
	  try
	  {
		  hProjection=new int [Image1->Height];

		  for(int i=0;i<Image1->Height;i++)
			  hProjection[i]=0;
	  }
	  catch (...)
	  {
		  ShowMessage("error1");
		  return;
	  }
  }

  H_Projection(Image1->Picture->Bitmap,&hProjection[0]);


  Image2->Canvas->Brush->Color=clWhite;
  Image2->Canvas->FillRect(TRect(0,0,Image2->Width,Image2->Height));

  scale=(float)Image1->Width/(float)Image2->Width;

  DrawHProjection(&hProjection[0],Image2,Image2->Height,scale);
  Image2->Repaint();

  if(hProjection!=NULL)
  {
	  delete hProjection;
	  hProjection=NULL;
  }

  //-------------------------------------------------------

  if(vProjection!=NULL)
  {
	  delete [] vProjection;
	  vProjection=NULL;
  }
  else
  {
	  try
	  {
		  vProjection=new int [Image1->Width];

		  for(int i=0;i<Image1->Width;i++)
			  vProjection[i]=0;
	  }
	  catch (...)
	  {
		  ShowMessage("error1");
		  return;
	  }
  }

  V_Projection(Image1->Picture->Bitmap,&vProjection[0]);

  Image3->Canvas->Brush->Color=clWhite;
  Image3->Canvas->FillRect(TRect(0,0,Image3->Width,Image3->Height));

  scale=(float)Image1->Height/(float)Image3->Height;

  DrawVProjection(&vProjection[0],Image3,Image3->Width,scale);
  Image3->Repaint();

  if(vProjection!=NULL)
  {
	  delete vProjection;
	  vProjection=NULL;
  }
}
//----------------------------------------------------------------------------
//--------------------------灰階----------------------------------------------
void __fastcall TForm1::ToGray(Graphics::TBitmap *sbmp,Graphics::TBitmap *tbmp)
{
  Byte *s_ptr,*t_ptr;
  int r,g,b;
  int w,h;
  int gray;

  w=(sbmp->Width)*3;
  h=sbmp->Height;


  for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)sbmp->ScanLine[y];
	t_ptr=(Byte *)tbmp->ScanLine[y];

	for(int x=0;x < w ; x+=3)
	{
	  r=s_ptr[x+2];
	  g=s_ptr[x+1];
	  b=s_ptr[x];

	  gray=b*0.114+g*0.587+r*0.299;
	  //gray=(b+g+r)/3;

	  t_ptr[x+2]=gray;
	  t_ptr[x+1]=gray;
	  t_ptr[x]=gray;
	}
  }
}
//-------------------------取HSV----------------------------------------------
void __fastcall TForm1::RGBToHSV(int r, int g, int b, int *ch, int *cs, int *cv)
{
  BYTE *ptr;
  float temp,mi,delta;
  int inter,c;
  float h,s,v;

  mi=min(min(r,g),b);
  v=max(max(r,g),b);
  delta=v-mi;

  if(delta==0)
	h=0;
  else
    {
	  if(r==v)
		h = 60.0 * (g - b) / delta;
      if(g==v)
        h = 120.0 + 60.0 * (b - r) / delta;
      if(b==v)
        h = 240.0 + 60.0 * (r - g) / delta;
      if(h<0.0)
        h = h + 360.0;
    }

  if(v==0.0)
	s=0;
  else
    s=delta/v;

  s*=255.0;

  *ch=h;
  *cs=s;
  *cv=v;
}
//------------------------膚色擷取--------------------------------------------
void __fastcall TForm1::Colorr(Graphics::TBitmap *sbmp, Graphics::TBitmap *tbmp)
{
	Byte *sptr,*tptr;
	int w,h;
	int r,g,b;
	int hh,ss,vv;

	w=(sbmp->Width)*3;
	h=sbmp->Height;

	for(int y=0;y<h;y++)
	{
	   sptr=(Byte *)sbmp->ScanLine[y];
	   tptr=(Byte *)tbmp->ScanLine[y];
		for(int x=0;x<w;x+=3)
		{
		  RGBToHSV(sptr[x+2], sptr[x+1], sptr[x], &hh,&ss,&vv);

		  if(hh>=20 && hh<=40 && ss>=0 && ss<=255 && vv>=0 && vv<=255)
		  {
		   tptr[x]=sptr[x];
		   tptr[x+1]=sptr[x+1];
		   tptr[x+2]=sptr[x+2];
		  }
		  else
		  {
			tptr[x]=BINARY_BACKGROUND;
			tptr[x+1]=BINARY_BACKGROUND;
			tptr[x+2]=BINARY_BACKGROUND;
		  }
		}
	}
}
//------------------------二值化取小--------------------------------------------
void __fastcall TForm1::ToBinary_S(Graphics::TBitmap *sbmp,Graphics::TBitmap *tbmp,int th)
{
   Byte *s_ptr,*t_ptr;
  int r,g,b;
  int w,h;
  int gray;

  w=(sbmp->Width)*3;
  h=sbmp->Height;


  for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)sbmp->ScanLine[y];
	t_ptr=(Byte *)tbmp->ScanLine[y];

	for(int x=0;x < w ; x+=3)
	{
	  b=s_ptr[x];

	  if(b<th)
	  {
		  t_ptr[x+2]=BINARY_FOREGROUND;
		  t_ptr[x+1]=BINARY_FOREGROUND;
		  t_ptr[x]=BINARY_FOREGROUND;
	  }
	  else
	  {
		  t_ptr[x+2]=BINARY_BACKGROUND;
		  t_ptr[x+1]=BINARY_BACKGROUND;
		  t_ptr[x]=BINARY_BACKGROUND;
	  }
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::H_Projection(Graphics::TBitmap *bmp,int H[])
{
 Byte *ptr;
 int w;
 w=bmp->Width*3;

 for(int y=0;y<bmp->Height;y++)
 {
  ptr=(Byte *)bmp->ScanLine[y];

  for(int x=0;x<w;x+=3)
  {
	  if(ptr[x]==0)
		H[y]++;
  }

 }
}
//--------------------------------------------------------------------
void __fastcall TForm1::DrawHProjection(int H[],TImage *img,int h,float scale)
{
 int c;
 img->Canvas->Pen->Color=clBlack;

 for(int i=0;i<h;i++)
 {
   c=(float)H[i]/scale;
   img->Canvas->MoveTo(img->Width-c,i);
   img->Canvas->LineTo(img->Width,i);
 }

}
//-----------------------------------------------------------------------------
void __fastcall TForm1::V_Projection(Graphics::TBitmap *bmp,int V[])
{
 Byte *ptr;
 int w,h;
 w=bmp->Width*3;
 h=bmp->Height;
 for(int y=0;y<h;y++)
 {
  ptr=(Byte *)bmp->ScanLine[y];

  for(int x=0;x<w;x+=3)
  {
	  if(ptr[x]==0)
		V[x/3]++;
  }

 }
}
//--------------------------------------------------------------------
void __fastcall TForm1::DrawVProjection(int V[],TImage *img,int v,float scale)
{
 int c;
 img->Canvas->Pen->Color=clBlack;

 for(int i=0;i<v;i++)
 {
   c=(float)V[i]/scale;
   img->Canvas->MoveTo(i,img->Height-c);
   img->Canvas->LineTo(i,img->Height);
 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
 int a,b;
 Image1->Canvas->Pen->Color=clRed;

 a=Image1->Width/10;
 b=Image1->Height/10;

 for(int i=0;i<Image1->Width;i+=a)
 {
   Image1->Canvas->MoveTo(i,0);
   Image1->Canvas->LineTo(i,Image1->Height);
 }

  for(int i=0;i<Image1->Height;i+=b)
 {
   Image1->Canvas->MoveTo(0,i);
   Image1->Canvas->LineTo(Image1->Width,i);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
  int a,b,v=7;
  re(Image1->Picture->Bitmap,Image1->Picture->Bitmap,v);
  Image1->Repaint();


}
//---------------------------------------------------------------------------
void __fastcall TForm1::re(Graphics::TBitmap *sbmp,Graphics::TBitmap *tbmp,int v)
{

  int w,h;
  Byte *s_ptr,*t_ptr;

  w=(sbmp->Width)*3;
  h=sbmp->Height;
  v=(Image1->Height/10)*v;

  for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)sbmp->ScanLine[y];
	t_ptr=(Byte *)tbmp->ScanLine[y];

	for(int x=0;x < w ; x+=3)
	{
	  if(y>=v)
	  {
	   t_ptr[x+2]=BINARY_BACKGROUND;
	   t_ptr[x+1]=BINARY_BACKGROUND;
	   t_ptr[x]=BINARY_BACKGROUND;
	  }
	}
  }
}

void __fastcall TForm1::Button7Click(TObject *Sender)
{   /*
  int x1,x2,y1,y2,No,Num;
  int **LabelMax;
  String str;
  int *CS;
  COO *CPOS;

  CC.Initial(Image1->Width,Image1->Height,Image1->Picture->Bitmap,20000);
  CC.SetRemovalSize(100);
  CC.Connection(Image1->Picture->Bitmap);
  CC.ShowComponents();

  Image1->Repaint();

  //-----show些資本的資訊----------
  Num=CC.GetCompntNum(1);
  //Label8->Caption=CC.FindMaxCompnt(&x1,&y1,&x2,&y2,&No);
  //Label9->Caption=No;
  //Label11->Caption=Num;


   //-----show出LabelMatrix的內容-----
  LabelMax=CC.GetLabelMatrix();
  //Memo3->ScrollBars=ssNone;
 // Memo3->Visible=false;
  //Memo3->Lines->Clear();

  for(int y=0;y<=Image1->Height;y++)
    {
      str="";
      for(int x=0;x<=Image1->Width; x++)
        str+=LabelMax[y][x];

	 // Memo3->Lines->Add(str);
    }
   //Memo3->ScrollBars=ssBoth;
  // Memo3->Visible=true;


  //----show出ConnectSet的內容-----
  int l;
  l=CC.GetLabels();
  str="";
  CS=CC.GetConnectSet();
  for(int i=1;i<l+1; i++)
    str+=IntToStr(CS[i])+", ";

  //Memo4->Lines->Add(str);

  //-----框出每個物件的範圍----------
  CPOS=CC.GetCompntPos();
  Image1->Canvas->Brush->Style=bsClear;
  Image1->Canvas->Pen->Color=clBlack;

  for(int i=1;i<l+1;i++)
    {
      if(CPOS[i].x2==-1)
        continue;

	  Image1->Canvas->Rectangle(TRect(CPOS[i].x1,CPOS[i].y1,CPOS[i].x2,CPOS[i].y2));
    }

  //---最大的物件用紅色框出來------
  Image1->Canvas->Brush->Style=bsClear;
  Image1->Canvas->Pen->Color=clRed;
  Image1->Canvas->Rectangle(TRect(CPOS[No].x1,CPOS[No].y1,CPOS[No].x2,CPOS[No].y2));

 */
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button8Click(TObject *Sender)
{
 //int mx_x=150,mx_y=290,mi_x=25,mi_y=70;
  Byte *s_ptr;
  int w,h;

  w=(Image1->Width)*3;
  h=Image1->Height;


  for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)Image1->Picture->Bitmap->ScanLine[y];

	for(int x=0;x < w ; x+=3)
	{

	  if(s_ptr[x]==0&&s_ptr[x+1]==0&&s_ptr[x+2]==0)
	  {
	   if(x<=mi_x)
		mi_x=(x);
	   if(x>=mx_x)
		mx_x=(x);
	   if(y<=mi_y)
		mi_y=y;
	   if(y>=mx_y)
        mx_y=y;
	  }
	}
  }
  mx_x/=3;
  mi_x/=3;
  ShowMessage(IntToStr(mx_x)+" "+IntToStr(mx_y)+" "+IntToStr(mi_x)+" "+IntToStr(mi_y));


  Image1->Canvas->Pen->Color=clRed;
  Image1->Canvas->MoveTo(mi_x,mi_y);
  Image1->Canvas->LineTo(mi_x,mx_y);

  Image1->Canvas->MoveTo(mi_x,mi_y);
  Image1->Canvas->LineTo(mx_x,mi_y);

  Image1->Canvas->MoveTo(mx_x,mi_y);
  Image1->Canvas->LineTo(mx_x,mx_y);

  Image1->Canvas->MoveTo(mi_x,mx_y);
  Image1->Canvas->LineTo(mx_x,mx_y);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
 Byte *s_ptr;
  int w,h;

  w=(Image1->Width)*3;
  h=Image1->Height;


  for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)Image1->Picture->Bitmap->ScanLine[y];

	for(int x=0;x < w ; x+=3)
	{

	  if(s_ptr[x]!=0 && y>=mi_y && y<=mx_y && x>=mi_x && x<=mx_x)
	  {

	  }
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EC(Graphics::TBitmap *sbmp,Graphics::TBitmap *tbmp)//八連通
{
  //255是白、0是黑
  int w,h;
  Byte *s_ptr,*t_ptr;
  //bool ok=false;//f=黑點不保留
  //int okk=0;
  w=(sbmp->Width)*3;
  h=sbmp->Height;
  int a=1,b=1;
  //a+=h;
  //b+=w/3;
  /*int [][]img12=new int [a][b];
  int xx=0;//跑橫的
  for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)Image1->Picture->Bitmap->ScanLine[y];
	xx=0;
	for(int x=0;x < w ; x+=3)
	{
		if(s_ptr[x]==0)
		{
			img12[y][xx]=1;
		}
		else
		{
        	img12[y][xx]=0;
        }
        xx+=1;
	}
  }
  */







  /*for (int y = 0; y < h; y++)
  {
	s_ptr=(Byte *)sbmp->ScanLine[y];
	t_ptr=(Byte *)tbmp->ScanLine[y];

	for(int x=0;x < w ; x+=3)
	{
		ok=false;
		okk=0;
		if(s_ptr[x]==0)
		{
			if(y>0)
			{
				up=(Byte *)sbmp->ScanLine[y-1];
				if(up[x]==0)
				{
					ok=true;
					okk+=1;
				}

			}
			if(y<h-1)
			{
				down=(Byte *)sbmp->ScanLine[y+1];
				if(down[x]==0)
				{
					ok=true;
					okk+=1;
				}

			}
			if(x>0)
			{
				left=s_ptr[x-3];
				if(left==0)
				{
					ok=true;
					okk+=1;
				}

			}
			if(x<w-3)
			{
				right=s_ptr[x+3];
				if(right==0)
				{
					ok=true;
                    okk+=1;
				}

			}
			if(okk==2)
			{
				t_ptr[x+2]=0;
				t_ptr[x+1]=0;
				t_ptr[x]=0;
			}
			else
			{
				t_ptr[x+2]=255;
				t_ptr[x+1]=255;
				t_ptr[x]=255;
            }
		}
	}
  } */
}


void __fastcall TForm1::Button10Click(TObject *Sender)
{
	EC(Image1->Picture->Bitmap,Image1->Picture->Bitmap);
 	Image1->Repaint();
}
//---------------------------------------------------------------------------

