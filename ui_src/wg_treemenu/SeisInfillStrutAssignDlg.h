#if !defined(__SEIS_INFILL_STRUT_ASSIGN_Dlg_H__)
#define      __SEIS_INFILL_STRUT_ASSIGN_Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisInfillStrutAssign.h : header file
//

// 조적 채움벽 생성 대화상자
// ElemCreateDlg와 기본동작은 같다.
// 4개의 폭 너비를 입력받아 Tapered section을 생성하여 사용

/////////////////////////////////////////////////////////////////////////////
// CSeisInfillStrutAssignDlg dialog
#include "ElemCreateDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\EditMatl.h"
#include "..\wg_db\EditSect.h"
#include "..\wg_db\EditThik.h"
#include "..\wg_db\CobxMatl.h"
#include "..\wg_db\CobxSect.h"
#include "..\wg_db\CobxThik.h"
#include "..\wg_db\EditWid.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;
class CDBDoc;
class CSeisInfillStrutAssignDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSeisInfillStrutAssignDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisInfillStrutAssignDlg();

	enum EN_ASSIGN_METHOD
	{
		EN_METHOD_BY_NODE=0, EN_METHOD_BY_ELEM,
	};
public:
	void SetTempElemDataByCurSetting();
	void _CreateElement();
	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	/*********************************************************************
	 *
	 *
 *****   ELEMENT CREATION PARAMETER 설정 
	***
	 */
	BOOL GetNodeListAndNodeNum(int nElemType,
														 CArray<unsigned int,unsigned int>& _NodeList,
														 int& nMaxNodeNum);
	BOOL GetKNodeDatas(int nElemType,CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
										 double &BAngle,double& RPWx,double &RPWy,double &RPWz);
	BOOL GetMatlIDS(int nElemType,int& nMatID); 
	BOOL GetSectIDS(T_NODE_K NodeK, int& nSecID);
	BOOL GetParameter(_ElemCreate & EC);
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);
	void InitCtrls();
	void InitMsisCmb();
	void UpdateMsisCmb();
	void InitNodeCtrl();
	void InitPropertyCtrls();
	void AssignProperty();
	virtual void Execute();
	BOOL GetDistance(CMouseEdit* pDistEdit, double &dblDist);
	  
	// Dialog Data
	//{{AFX_DATA(CSeisInfillStrutAssignDlg)
	enum { IDD = IDD_TM_SEIS_INFILL_STRUT_ASSIGN };
	MButton	m_chkOrtho;
	CParseEdit	m_wndNodeUserEdit;
	MComboBox	m_cobxNodeEdit;
	MComboBox	m_cobxStrut;
	MEdit m_wndNodesTitle;
	MEdit m_wndMatTitle;
	CMouseEdit m_wndNodeListEdit;
	CCobxMatl	m_ctrlCobxMatl;
	CEditMatl	m_ctrlEditMatl;
	int		m_nNodeNumIndex;
// 	BOOL	m_bIntersectNode;
// 	BOOL	m_bIntersectElem;
	BOOL	m_nCheckOrtho;
	int		m_nAssignMethod;
	int   m_nOption;
	int   m_nBracintOption;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisInfillStrutAssignDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:
	void AlignControls();
	void InitUnit();
	BOOL CheckInfillStrut(T_ELEM_K ElemK);
	CDBDoc* m_pDoc;
	BOOL m_bPropInit;

	// Generated message map functions
	//{{AFX_MSG(CSeisInfillStrutAssignDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTmMatButton();
	afx_msg void OnTmStrutButton();
	afx_msg void OnSetfocusTmNodesEdit();
	afx_msg void OnTmIntstTol();
	afx_msg void OnTmOrthoCheck();
	afx_msg void OnTmAssignMethodBtn();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_INFILL_STRUT_ASSIGN_Dlg_H__)
