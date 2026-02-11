#if !defined(AFX_MESHMAPDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_MESHMAPDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshMapDlg.h : header file
//

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CMeshMapDlg dialog
#include "..\wg_base\NotifyCtrl.h"

#include "..\wg_db\EditMatl.h"
#include "..\wg_db\EditThik.h"
#include "..\wg_db\CobxMatl.h"
#include "..\wg_db\CobxThik.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_db\MeshUtil.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

class CMeshMapDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
protected:
	static int m_nCurETypeIndex;

public:
	CMeshMapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeshMapDlg();
	virtual void Execute();
	
public:
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);

	virtual BOOL ExternalInit(UINT key);
	void SetTempElemDataByCurSetting();
	void InitPropertyCtrls();
	void InitElemTypeCombo();  
	int  GetCurElemTypeID();// 현재 설정된 Element의 Type ID를 얻어낸다.  

	BOOL CheckArg(const BOOL bSimul = FALSE); 
	BOOL CheckAlign();
	BOOL CheckCross();

	BOOL GetPropIDS(int nElemType,int& nMatID,int& nThkID);

// Dialog Data
	//{{AFX_DATA(CMeshMapDlg)
	enum { IDD = IDD_TM_MESH_MAP };
	CMouseEdit m_EditNodeList;
	CEditUnit m_Edit12DirDivision;
	CEditUnit m_Edit14DirDivision;
	CEditUnit m_Edit12DirBiasRatio;
	CEditUnit m_Edit14DirBiasRatio;
	mit::frx::MButton m_RadioTri;
	mit::frx::MButton m_RadioQuad;
	mit::frx::MComboBox m_ComboElemType;
	CEditThik	m_EditThik;	
	CEditMatl	m_EditMatl;
	CCobxThik	m_CobxThik;	
	CCobxMatl	m_CobxMatl;  
	mit::frx::MEdit m_EditDomainName;
	mit::frx::MButton m_ChkDelLineElem;
	mit::frx::MButton m_ChkSubDivLineElem;
	//}}AFX_DATA
	int m_nThickThin;// 0:Thick, 1:Thin
	BOOL m_bDOF;
	int m_nMembPlate;//0:Membrane, 1:Plate
	int m_nWallIDType;//0:Fixed No., 1:Auto Inc, 2:Last N0.+1
	int m_nWallID;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc*   m_pDoc;

	BOOL m_bPropInit;
	CArray<int,int> m_ElemTypeIDs;

	T_NODE_D m_NodeD1;
	T_NODE_D m_NodeD2;
	T_NODE_D m_NodeD3;
	T_NODE_D m_NodeD4;

	T_MESH_NODE_D m_MashNodeD1;
	T_MESH_NODE_D m_MashNodeD2;
	T_MESH_NODE_D m_MashNodeD3;
	T_MESH_NODE_D m_MashNodeD4;

	void InitData();

	CArray<unsigned int,unsigned int> m_NodeList;
	CArray<UINT, UINT&> m_arNodeK;
	CArray<T_NODE_D, T_NODE_D&> m_arNodeD; 

	void ConvertOrgNodeData(T_MESH_NODE_D& MeshNodeD, T_NODE_D& OrgNodeD);
	void ConvertMashNodeData(T_NODE_D& OrgNodeD, T_MESH_NODE_D& MeshNodeD);
	BOOL IsSameNode(const T_MESH_NODE_D& OrgNodeD, const T_MESH_NODE_D& NodeD);

	BOOL GetDomainType(int& nType);
	void ChangeElemTypeComboWidth();

	void SetDomainName();
	
	// Generated message map functions
	//{{AFX_MSG(CMeshMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusTmNodesEdit();
	afx_msg void OnTmMatButton();
	afx_msg void OnTmThickButton();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnDelectLineElem();
	afx_msg void OnElemTypeButton();  
	afx_msg void OnTmDomainButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHMAPDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
