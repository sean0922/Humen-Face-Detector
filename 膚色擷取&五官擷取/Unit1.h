//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------

#include "CC.h"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ExtDlgs.hpp>
#define BINARY_FOREGROUND 0
#define BINARY_BACKGROUND 255
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button3;
	TButton *Button1;
	TButton *Button2;
	TImage *Image1;
	TOpenPictureDialog *OpenPictureDialog1;
	TImage *Image2;
	TImage *Image3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);

private:	// User declarations
public:		// User declarations

	Graphics::TBitmap *Bmp;
	int *hProjection;
	int *vProjection;

	__fastcall TForm1(TComponent* Owner);
	void __fastcall ToGray(Graphics::TBitmap *,Graphics::TBitmap *);
	void __fastcall RGBToHSV(int , int , int , int *, int *, int *);
	void __fastcall Colorr(Graphics::TBitmap *, Graphics::TBitmap *);
	void __fastcall ToBinary_S(Graphics::TBitmap *,Graphics::TBitmap *,int );
	void __fastcall H_Projection(Graphics::TBitmap *,int *);
	void __fastcall DrawHProjection(int *,TImage *,int ,float );
	void __fastcall V_Projection(Graphics::TBitmap *,int *);
	void __fastcall DrawVProjection(int *,TImage *,int ,float );
	void __fastcall re(Graphics::TBitmap *,Graphics::TBitmap *,int ); //去除不要的部分
	void __fastcall EC(Graphics::TBitmap *,Graphics::TBitmap *);//8連通


};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
