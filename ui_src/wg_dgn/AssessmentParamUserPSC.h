#if !defined(__CASSESSMENTPARAMUSERPSC_H__)
#define __CASSESSMENTPARAMUSERPSC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentParamUserPSC.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"

class CDBDoc;
class CAssessmentParamUserPSC: public CDialogMove
{
public:
	CAssessmentParamUserPSC(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentParamUserPSC();

	enum { IDD = IDD_DGN_ASSESSMENT_PARAM_USER_PSC_DLG};

	void SetData(T_ASPM_FT_D& Data);
	void GetData(T_ASPM_FT_D& Data);

protected: 
	// 함수(컨트럴)
	afx_msg void OnUpdateByCode();
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
	CEditUnit m_edtCsTndn;
	CEditUnit m_edtCsConcrete;
	CEditUnit m_edtCsShearConc;
	CEditUnit m_edtWsTndn;
	CEditUnit m_edtWsConcrete;
	CEditUnit m_edtWsShearConc;
	CEditUnit m_edtSfTriCmp;
	CEditUnit m_edtSfUniformCmp;
	CEditUnit m_edtSfPreTens;
	CEditUnit m_edtSfPostTens;


	// 변수(일반)
	CDBDoc* m_pDoc;
	T_ASPM_FT_D m_Data;
};

#endif // !defined(__CASSESSMENTPARAMUSERPSC_H__)


