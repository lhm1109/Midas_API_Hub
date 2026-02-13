#if !defined(__CASSESSMENTPARAMUSERSTLNRDLG_H__)
#define __CASSESSMENTPARAMUSERSTLNRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentParamUserSTLNRDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"

class CDBDoc;
class CAssessmentParamUserSTLNRDlg : public CDialogMove
{
public:
	CAssessmentParamUserSTLNRDlg(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentParamUserSTLNRDlg();

	enum { IDD = IDD_DGN_ASSESSMENT_PARAM_USER_STL_NR_DLG };

	void SetData(T_RPSS_FT_D& Data);
	void GetData(T_RPSS_FT_D& Data);

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
	CEditUnit m_edtCsUlsSteel;
	CEditUnit m_edtCsUlsBuckStiff;
	CEditUnit m_edtCsUlsYieldStiff;
	CEditUnit m_edtCsUlsWFRivet;
	CEditUnit m_edtCsUlsOtherRivet;

	CEditUnit m_edtWsUlsSteel;
	CEditUnit m_edtWsUlsBuckStiff;
	CEditUnit m_edtWsUlsYieldStiff;
	CEditUnit m_edtWsUlsWFRivet;
	CEditUnit m_edtWsUlsOtherRivet;
	
	CEditUnit m_edtUlsGamma_f3;

	// 변수(일반)
	CDBDoc* m_pDoc;
	T_RPSS_FT_D m_Data;
};

#endif // !defined(__CASSESSMENTPARAMUSERSTLNRDLG_H__)


