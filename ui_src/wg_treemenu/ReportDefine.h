#if !defined(AFX_REPORTDEFINE_H__)
#define AFX_REPORTDEFINE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDefine.h : header file
//

#include "StringDeepCopyCollector.h"
#include "FontEdit.h"

#define WM_USER     0x0400
#define WU_TEMPLETE 0x0401

#define WM_MY_MESSAGE_OK           WM_USER+100
#define WM_MY_MESSAGE_CANCEL       WM_USER+101
#define WM_MY_EDIT_CANCEL          WM_USER+102
#define WM_MY_CHANGE_SLAVETITLE    WM_USER+103

// GEN_IFTAG의 iAppendFlag값
#define D_WORD_APPEND_INSERT		 0 // 삽입
#define D_WORD_APPEND_COMPARE		 1 // 비교
#define D_WORD_APPEND_DELETE		 2 // 삭제
#define D_WORD_APPEND_REGENERATE	 9 // 리제너레이션
#define D_WORD_APPEND_REGENERATE_CH  10 // 리제너레이션

#define D_WORD_LOAD_WITH_EDITOR     0   // Word Editor 로드함.
#define D_WORD_LOAD_WITHOUT_EDITOR  1   // Word Editor를 로드하지 않고 일괄출력함.

struct GEN_IFTAG 
{
	TCHAR*	sProductInfo;	// 제품정보
	TCHAR*	sCaption;		// Caption
	TCHAR*	sType;			// Type(image, table, image file, chart, text)
	TCHAR*	sFullPath;		// 파일경로 및 파일명(그림파일, 테이블Html파일)
	TCHAR*	sExtInfo;		// 추출정보
	TCHAR*	sContents;		// 내용(text일때 Word에 삽입할 내용)
	int		iAppendFlag;	// 모드 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default로 0세팅
	TCHAR*	sExt;			// 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG()
	{
		Initialize();
	}
	void Initialize()
	{
		sProductInfo  =NULL;
		sCaption      =NULL;
		sType         =NULL;
		sFullPath     =NULL;
		sExtInfo      =NULL;
		sContents     =NULL;
		iAppendFlag   =0;
		sExt          =NULL;
	}
}; 

struct GEN_IFTAG_STR
{
	CString	sProductInfo; // 제품정보
	CString	sCaption    ; // Caption
	CString	sType       ; // Type(image, table, image file, chart, text)
	CString	sFullPath   ; // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    ; // 추출정보
	CString	sContents   ; // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag ; // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString sExt        ; // 향후를 대비한 예비 member. default는 NULL
	
	GEN_IFTAG_STR()
	{
		Initialize();
	}
	void Initialize()
	{
		sProductInfo  =_T("");
		sCaption      =_T("");
		sType         =_T("");
		sFullPath     =_T("");
		sExtInfo      =_T("");
		sContents     =_T("");
		iAppendFlag   =0;
		sExt          =_T("");
	}
}; 

typedef HWND  (*DoDllTest)(HWND, LPTSTR);
typedef HWND  (*EditorCreate)(HWND, LPTSTR, LPTSTR, LPTSTR, LPTSTR);
typedef HWND  (*EditorLoad)(HWND, LPTSTR, int);
typedef void  (*EditorRedraw)(HWND);
typedef BOOL  (*EditorQuit)(void);
typedef int   (*EditorTempleteImport)(int*, struct GEN_IFTAG**);
typedef int   (*EditorExport)(int, struct GEN_IFTAG**);
typedef int   (*EditorImport)(int, struct GEN_IFTAG**, int, int*, struct GEN_IFTAG**);
typedef void  (*EditorSave)(LPTSTR);
typedef void  (*EditorHeader)(LPTSTR);
typedef void  (*EditorFooter)(LPTSTR);
typedef void  (*EditorSetupStyle)(int);
typedef void  (*EditorPageBreak)(void);
typedef void  (*EditorInsertContents)(int);
typedef void  (*EditorUpdateContents)(void);
typedef void  (*EditorInsertPageNumber)(int,int,bool);

// Font
enum TEXT_ALIGN 
{  
	ALIGN_LEFT=0, 
	ALIGN_CENTER, 
	ALIGN_RIGHT 
};
enum EDIT_TYPE  
{  
	EDIT_CAPTION=0, 
	EDIT_DESCRIPTION, 
	EDIT_TABLE_ROWTITLEFONT, 
	EDIT_TABLE_COLTITLEFONT, 
	EDIT_TABLE_CELLTITLEFONT
};
enum CAPTION_POSITION
{  
	POSITION_TOP=0, 
	POSITION_BOT, 
	POSITION_NONE
};
enum TABLE_TYPE
{  
	TABLE_HEADER=0, 
	TABLE_CELL
};

struct FontData
{
	CFontEdit *pTargetWnd;
	CString     strFont;
	UINT        nFontSize;
	COLORREF    dwColor;  //Table에서 Row, Col, Cell에서도 사용함
	TEXT_ALIGN  eAlign;
	BOOL        bBold;
	BOOL        bItalic;
	BOOL        bUnderline;

	FontData()
	{
		Initialize();
	}
	void FontData::Initialize()
	{
		pTargetWnd  =NULL;
		strFont     =_T("");
		nFontSize   =0;
		dwColor     =0;
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
		SetDefault();
	}
	void FontData::SetDefault()
	{
#if defined(_ORG)
		SetDefaultORG();
#elif defined(_CH)
		SetDefaultCH();
#elif defined(_JP)
		SetDefaultJP();
#elif defined(_US)
		SetDefaultUS();
#elif defined(_RUS)
		SetDefaultUS();
#else
#error Unknown Language Definition!
		SetDefaultUS();
#endif
	}
	void FontData::SetDefaultORG()
	{
		pTargetWnd  =NULL;
		strFont     =_T("맑은 고딕");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
	}
	void FontData::SetDefaultCH()
	{
		pTargetWnd  =NULL;
		strFont     =_T("芥竟");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
	}
	void FontData::SetDefaultJP()
	{
		pTargetWnd  =NULL;
		strFont     =_T("굃굍 긕긘긞긏");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
	}
	void FontData::SetDefaultUS()
	{
		pTargetWnd  =NULL;
		strFont     =_T("Courier New");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
	}
};

struct FontShadeData
{
	CFontEdit *pTargetWnd;
	CString     strFont;
	UINT        nFontSize;
	COLORREF    dwColor;  //Table에서 Row, Col, Cell에서도 사용함
	TEXT_ALIGN  eAlign;
	BOOL        bBold;
	BOOL        bItalic;
	BOOL        bUnderline;
	COLORREF    dwShadeColor; // Table에서 배경색 (음영)
	
	FontShadeData()
	{
		Initialize();
	}
	void FontShadeData::Initialize()
	{
		pTargetWnd  =NULL;
		strFont     =_T("");
		nFontSize   =0;
		dwColor     =0;
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
		dwShadeColor=RGB(255,255,255);
		SetDefault();
	}
	void FontShadeData::SetDefault()
	{
#if defined(_ORG)
		SetDefaultORG();
#elif defined(_CH)
		SetDefaultCH();
#elif defined(_JP)
		SetDefaultJP();
#elif defined(_US)
		SetDefaultUS();
#elif defined(_RUS)
		SetDefaultUS();
#else
#error Unknown Language Definition!
		SetDefaultUS();
#endif
	}
	void FontShadeData::SetDefaultORG()
	{
		pTargetWnd  =NULL;
		strFont     =_T("맑은 고딕");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
		dwShadeColor=RGB(255,255,255);
	}
	void FontShadeData::SetDefaultCH()
	{
		pTargetWnd  =NULL;
		strFont     =_T("芥竟");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
		dwShadeColor=RGB(255,255,255);
	}
	void FontShadeData::SetDefaultJP()
	{
		pTargetWnd  =NULL;
		strFont     =_T("굃굍 긕긘긞긏");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
		dwShadeColor=RGB(255,255,255);
	}
	void FontShadeData::SetDefaultUS()
	{
		pTargetWnd  =NULL;
		strFont     =_T("Courier New");
		nFontSize   =8;
		dwColor     =RGB(0,0,0);
		eAlign      =ALIGN_LEFT;
		bBold       =FALSE;
		bItalic     =FALSE;
		bUnderline  =FALSE;
		dwShadeColor=RGB(255,255,255);
	}
};

struct CH_GEN_IFTAG_STR : public GEN_IFTAG_STR
{
	CH_GEN_IFTAG_STR()
	{
		iLevel = 0;
	}
	int iLevel;    //0-攣匡 1-9
};
struct GenSegmentElements
{
	GenSegmentElements()
	{
		Clear();
	}
	void Clear()
	{
		m_strSegmentName = _T("");
		m_SegmentElements.clear();

	}
	CString m_strSegmentName;
	std::vector<CH_GEN_IFTAG_STR> m_SegmentElements;
};

#endif // !defined(AFX_REPORTDEFINE_H__)
