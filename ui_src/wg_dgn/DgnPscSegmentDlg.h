#if !defined(AFX_DgnPscSegmentDlg_H__)
#define AFX_DgnPscSegmentDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnPscSegmentDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnPscSegmentDlg dialog

class CDgnPscSegmentDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnPscSegmentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CDgnPscSegmentDlg)
	enum { IDD = IDD_DGN_PSC_SEGM_DLG };
	int		m_nOption;
	int		m_nAssignType;
	int		m_nSelectType; 
	int		m_nJointType;
 	CListCtrl	m_List;
	MButton  m_wndAllowSingleElemMember;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnPscSegmentDlg)
 	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	virtual ~CDgnPscSegmentDlg();
	void CtrlManager();
 	void InitSelectedItem();
	void MemberDefine();    
	void MemberDefineManual();  // 선택된 요소로부터 Member를 생성하는 함수(Manual의 Add/Replace 일때 사용)
	void MemberDefineAuto();    // 선택된 요소로부터 Member를 생성하는 함수(Automatic의 Add/Replace 일때 사용)  
	void DelAssignedMember();   // 선택된 요소로부터 이들이 속한 Member를 지우는 함수(Manual, Automatic의 Delete일때 사용)
//  BOOL GetFinalSurvivedMember(CArray<T_PSEG_D, T_PSEG_D&>& aDataMemb);
//  BOOL GetSuperiorMember(CArray<T_PSEG_D, T_PSEG_D&>& aDataMemb);
//  BOOL AddSingleElemMember(CArray<T_PSEG_D, T_PSEG_D&>& aDataMemb);
	BOOL DelInferiorMember();
	BOOL AddSingleElemMember();
	void AlignElemListInOrder(T_PSEG_D& rData);  // Member의 요소를 i, j순서대로 다시 배열하는 함수
	void GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz);
	
//  BOOL IsContinuousMember(T_PSEG_D& rData);
	BOOL IsExistColumnWallLower(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem);
	BOOL IsExistColumnWallUpper(T_NODE_K KeyNode);
	BOOL IsStoryLevel(T_NODE_K KeyNode);
	BOOL IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL IsSupportAndColumnWallUpperExist(T_ELEM_K KeyElem, T_NODE_K KeyNode);
	BOOL IsSupportExist(T_ELEM_K KeyElem, T_NODE_K KeyNode);

	// Generated message map functions
	//{{AFX_MSG(CDgnPscSegmentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnAssignType();
	afx_msg void OnDgnOption();	
	afx_msg void OnDgnMemberSelect();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
protected:
	CArray<T_ELEM_K, T_ELEM_K> m_aSelFrmKey;   // 선택된 element 중에서 frame type의 element list
	CArray<T_ELEM_K, T_ELEM_K> m_aNonFrmKey;   // 선택된 element 중에서 frame type 이외의 element list
	CArray<T_PSEG_D, T_PSEG_D&> m_aDataMemb;   // IsContinuousMember() 함수에서 사용할 임시적으로 설정된 멤버
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> m_mapSelFrmKey;  // 선택된 element 중에서 frame type의 element list
	CArray<UINT, UINT> m_aCtrlAll;
	NM_LISTVIEW m_TempListView;
	BOOL        m_bSentMessage;

protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	void OnDelKeyPressedInList();    
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void DisplaySelElemList();  
	void GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void InsertItem(T_PSEG_K Key);
	void DeleteItem(T_PSEG_K Key);
	void SetListFocus(CArray<int, int>& aSelItem);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnPscSegmentDlg_H__)
