#if !defined(AFX_WINDPRESSURESTRUCTDLG_H__INCLUDED_)
#define AFX_WINDPRESSURESTRUCTDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldWindPressureStructDlg.h : header file
//

// CStldWindPressureStructDlg 대화 상자입니다.
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;


class CStldWindPressureStructDlg : public CDialogMove
{
	DECLARE_DYNAMIC(CStldWindPressureStructDlg)

public:
	CStldWindPressureStructDlg(CWnd* pParent = NULL, int nStructType = -1, int nCodeType = -1);   // 표준 생성자입니다.
	virtual ~CStldWindPressureStructDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TM_STLD_WINDP_STRUCT_DLG };

	int GetStructureType();

protected:
	MComboBox	m_cmbStructureType;
	int m_nStructType;
	int m_nCodeType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOk();
	void InitStructureTypeCombo();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_WINDPRESSURESTRUCTDLG_H__INCLUDED_)