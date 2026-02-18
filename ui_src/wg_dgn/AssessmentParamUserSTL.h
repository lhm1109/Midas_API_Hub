#if !defined(__CASSESSMENTPARAMUSERSTL_H__)
#define __CASSESSMENTPARAMUSERSTL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentParamUserSTL.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"

class CDBDoc;
class CAssessmentParamUserSTL : public CDialogMove
{
public:
	CAssessmentParamUserSTL(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentParamUserSTL();

	enum { IDD = IDD_DGN_ASSESSMENT_PARAM_USER_STL_DLG };

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
	void SetText();
	BOOL Dlg2Data();
	void Data2Dlg();

	// 변수(컨트럴)
	CEditUnit m_edtCsUlsReinforce;
	CEditUnit m_edtCsUlsConcrete;
	CEditUnit m_edtCsUlsStructStl;
	CEditUnit m_edtCsUlsStiffComp;
	CEditUnit m_edtCsUlsStiffTens;
	CEditUnit m_edtCsUlsShearConc;
	
	CEditUnit m_edtCsSlsReinforce;
	CEditUnit m_edtCsSlsConcrete;
	CEditUnit m_edtCsSlsStructStl;
	CEditUnit m_edtCsSlsStiffComp;
	CEditUnit m_edtCsSlsStiffTens;
	CEditUnit m_edtCsSlsShearConc;
	
	CEditUnit m_edtWsUlsReinforce;
	CEditUnit m_edtWsUlsConcrete;
	CEditUnit m_edtWsUlsStructStl;
	CEditUnit m_edtWsUlsStiffComp;
	CEditUnit m_edtWsUlsStiffTens;
	CEditUnit m_edtWsUlsShearConc;
	
	CEditUnit m_edtWsSlsReinforce;
	CEditUnit m_edtWsSlsConcrete;
	CEditUnit m_edtWsSlsStructStl;
	CEditUnit m_edtWsSlsStiffComp;
	CEditUnit m_edtWsSlsStiffTens;
	CEditUnit m_edtWsSlsShearConc;
	
	CEditUnit m_edtUlsGamma_f3;
	CEditUnit m_edtSlsGamma_f3;

	// 변수(일반)
	CDBDoc* m_pDoc;
	T_RPSS_FT_D m_Data;
};

#endif // !defined(__CAssessmentParamUserSTL_H__)


