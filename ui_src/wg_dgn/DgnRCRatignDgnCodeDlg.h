#if !defined(__DGNRCRATINGDNGCODEDLG_H__)
#define __DGNRCRATINGDNGCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRatingCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatignDgnCodeDlg dialog

class __MY_EXT_CLASS__ CDgnRCRatingDgnCodeDlg : public CDialogMove
{
public:
	CDgnRCRatingDgnCodeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnRCRatingDgnCodeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_RATING_RC_CODE_DLG };
   MComboBox	m_DesignCode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
		void Initial_Data();

protected:
	CDBDoc* m_pDoc;

	void InitDgnCodeCombo();
	void Data2Dlg();
	void Dlg2Data();

   T_RCDC_D m_Data;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNRCRATINGDNGCODEDLG_H__)