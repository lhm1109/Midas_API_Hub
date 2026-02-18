// DgnSelectPrintLanguageDlg.h: interface for the CDgnSelectPrintLanguageDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnSelectPrintLanguageDlg_H__)
#define AFX_DgnSelectPrintLanguageDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"


// CDgnSelectPrintLanguageDlg 대화 상자입니다.

class __MY_EXT_CLASS__ CDgnSelectPrintLanguageDlg : public CDialogMove
{
public:
	CDgnSelectPrintLanguageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnSelectPrintLanguageDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_PRINT_SELECT_LANGUAGE };

    CComboBox m_CmbSelLang;

private:
    int m_iSelLang;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
    int GetSelLang() { return m_iSelLang; };
};

#include "HeaderPost.h"

#endif // !defined(AFX_DgnPSCSelectPrintUnitDlg_H__)