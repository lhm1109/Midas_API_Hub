#if !defined(__CASSESSMENTRPTPSC_H__)
#define __CASSESSMENTRPTPSC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentRptPSC.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAssessmentRptPSC: public CDialogMove
{
public:
	CAssessmentRptPSC(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentRptPSC();

	enum { IDD = IDD_DGN_ASSESSMENT_RPT_PSC_DLG};

protected: 
	// 함수(컨트럴)
	afx_msg void OnCancel();
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

	// 함수(상속)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 함수(일반)
	void SetControl();
	BOOL Dlg2Data();
	void Data2Dlg();

	// 변수(컨트럴)
	MButton m_chkEmf;
	CArray<UINT, UINT> m_aFileOpt;

	// 변수(일반)
	CDBDoc* m_pDoc;
	BOOL m_bEmf;
	int nFileOpt;
};
#include "HeaderPost.h" 

#endif // !defined(__CASSESSMENTRPTPSC_H__)


