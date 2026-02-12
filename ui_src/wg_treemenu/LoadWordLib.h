#if !defined(__LOADWORDLIB_H__)
#define __LOADWORDLIB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CLoadWordLib  
{
protected:
	CLoadWordLib();  // 이 클래스는 싱글톤으로써 사용된다. 절대 외부에서 생성 할수 없게 Protected로 막아줌..
public:
	virtual ~CLoadWordLib();

public:
	static CLoadWordLib* GetInstance();

	BOOL  LoadWordLibrary();
	BOOL  IsLoadedWordLibrary();
	HINSTANCE GetWordLibrary();
	void  FreeWordLibrary();

	void  SetDialogOption(int nDocOpt, CString strFileName);
	void  GetDialogOption(int* nDocOpt, CString* strFileName);
	
public:
	HINSTANCE m_hMod;
	int     m_nDocOpt;  // Dynamic Report Generation Dialog, 0: New Document, 1: Open Document
	CString m_strFileName;  // Dynamic Report Generation Dialog, FileName
};
#include "HeaderPost.h"

#endif // !defined(__LOADWORDLIB_H__)
