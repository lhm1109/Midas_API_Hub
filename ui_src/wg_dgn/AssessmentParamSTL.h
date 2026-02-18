#if !defined(__CASSESSMENTPARAMSTL_H__)
#define __CASSESSMENTPARAMSTL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssessmentParamSTL.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAssessmentParamSTL : public CDialogMove
{
public:
	CAssessmentParamSTL(CWnd* pParent = NULL);   // standard constructor
	~CAssessmentParamSTL();

	enum { IDD = IDD_DGN_ASSESSMENT_PARAM_STL_DLG };

protected:
	// 함수(컨트럴)
	afx_msg void OnUser();
	afx_msg void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnChkUser();
	DECLARE_MESSAGE_MAP()

	// 함수(상속)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// 함수(일반)
	void SetControl();
	void SetText();
	void EnableCtrl();
	BOOL Dlg2Data();
	void Data2Dlg();


	// 변수(컨트럴)
	CEditUnit m_edtCondFactor;

	MButton m_chkUserInput;

	MButton m_chkUlsFlexure;
	MButton m_chkUlsShear;
	MButton m_chkUlsFST;
	MButton m_chkUlsLgShear;

	MButton m_chkSlsStress;
	MButton m_chkSlsLgShear;

	MButton m_chkRptUltLim;
	MButton m_chkRptServLim;

	CArray<UINT, UINT> m_aRdoGamma_m;

	// 변수(일반)
	CDBDoc* m_pDoc;
	T_RPSS_D m_Data;
};
#include "HeaderPost.h"    

#endif // !defined(__CAssessmentParamSTL_H__)





