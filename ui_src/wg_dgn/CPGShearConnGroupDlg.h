#if !defined(AFX_CPGSHEARCONNGROUPDLG_H__9F5FA4DF_6B91_4CBD_813D_4FE1C1C81B01__INCLUDED_)
#define AFX_CPGSHEARCONNGROUPDLG_H__9F5FA4DF_6B91_4CBD_813D_4FE1C1C81B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenMembDlg.h : header file
//
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnGroupDlg dialog

class CCPGShearConnGroupDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CCPGShearConnGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// Dialog Data
	//{{AFX_DATA(CCPGShearConnGroupDlg)
	enum { IDD = IDD_DGN_CPG_SHEAR_CONN_GROUP_DLG};

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;
	virtual ~CCPGShearConnGroupDlg();
	void CtrlManager();
	void SetInitUnit();
	void InitSelectedItem();
	BOOL MemberDefine();    
	BOOL MemberDefineAuto();    // 선택된 요소로부터 Member를 생성하는 함수
	void DelAssignedMember();   // 선택된 요소로부터 이들이 속한 Member를 지우는 함수

	BOOL DelInferiorMember();
	BOOL AddSingleElemMember();
	void AlignElemListInOrder(T_SCGD_D& rData);  // Member의 요소를 i, j순서대로 다시 배열하는 함수
	void GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz);

	BOOL IsExistColumnWallLower(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem);
	BOOL IsExistColumnWallUpper(T_NODE_K KeyNode);
	BOOL IsStoryLevel(T_NODE_K KeyNode);
	BOOL IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL IsSupportAndColumnWallUpperExist(T_ELEM_K KeyElem, T_NODE_K KeyNode);
	BOOL IsSupportExist(T_ELEM_K KeyElem, T_NODE_K KeyNode);

	// Generated message map functions
	//{{AFX_MSG(CCPGShearConnGroupDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnOption();	
	afx_msg void OnShearConnGroupLengthOption();	
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG

public:
	int	 m_nOption;
	int m_nGroupLengthOpt;

	CListCtrl	m_List;

	CEditUnit	m_dNum;
	CEditUnit	m_dFu;
	CEditUnit	m_dDs;
	CEditUnit	m_dHsc;
	CEditUnit	m_dSt;
	mit::frx::MEdit	m_dGroupName;
	CEditUnit	m_dLength;

	CTextUnit	m_dFuUnit;
	CTextUnit	m_dDsUnit;
	CTextUnit	m_dHscUnit;
	CTextUnit	m_dStUnit;
	CTextUnit	m_dLengthUnit;

protected:
	CArray<T_ELEM_K, T_ELEM_K> m_aSelFrmKey;   // 선택된 element 중에서 frame type의 element list
	CArray<T_ELEM_K, T_ELEM_K> m_aNonFrmKey;   // 선택된 element 중에서 frame type 이외의 element list
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> m_mapSelFrmKey;  // 선택된 element 중에서 frame type의 element list
	CArray<UINT, UINT> m_aCtrlAll;
	NM_LISTVIEW m_TempListView;
	BOOL        m_bSentMessage;
	int         m_nFailureType;

protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	void OnDelKeyPressedInList();    
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void DisplaySelElemList();  
	void GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void InsertItem(T_SCGD_K Key);
	void DeleteItem(T_SCGD_K Key);
	void SetListFocus(CArray<int, int>& aSelItem);
	void UpdataCtrlUnit();

	BOOL ShearConnData2Dlg(const T_SCGD_D &crData);
	BOOL Dlg2ShearConnData(T_SCGD_D &rData);
	BOOL CheckInData(const T_SCGD_D &crData);

	BOOL GetElemLength(CArray<T_ELEM_K, T_ELEM_K>& aSelElem, double &dLength);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGSHEARCONNGROUPDLG_H__9F5FA4DF_6B91_4CBD_813D_4FE1C1C81B01__INCLUDED_)
