#if !defined(AFX_DGNCONBEAMEQUALDLG_H__9F5FA4DF_6B91_4CBD_813D_4FE1C1C81B01__INCLUDED_)
#define AFX_DGNCONBEAMEQUALDLG_H__9F5FA4DF_6B91_4CBD_813D_4FE1C1C81B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamEqualDlg.h : header file
//
#include "..\MIT_frx\MButton.h"

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamEqualDlg dialog

class CDgnConBeamEqualDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBeamEqualDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CDgnConBeamEqualDlg)
	enum { IDD = IDD_DGN_CON_BEAM_EQUAL_DLG };
	int		    m_nOption;
	int		    m_nAssignType;
	int	    	m_nSelectType; 
 	CListCtrl	m_List;
	mit::frx::MButton   m_ChkNotConsider;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamEqualDlg)
 	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	virtual ~CDgnConBeamEqualDlg();
	void CtrlManager();
 	void InitSelectedItem();
	void MemberDefine();    
	void EqualizeBeamRebarDefineAll();  // 절점을 공유하는 Beam을 모두 동일 배근으로 지정..
	void DelAssignedMember();   // 선택된 요소로부터 이들이 속한 Member를 지우는 함수(Manual, Automatic의 Delete일때 사용)
	void GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz);
	
	BOOL IsExistColumn(CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem);
	BOOL IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);

	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamEqualDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnAssignType();
	afx_msg void OnDgnOption();	
	afx_msg void OnDgnMemberSelect();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDgnNotConsider();
	//}}AFX_MSG
protected:
	CArray<T_ELEM_K, T_ELEM_K> m_aSelBeamKey;  // 선택된 element 중에서 Beam type의 element list
	CArray<T_ELEM_K, T_ELEM_K> m_aNonBeamKey;  // Design Criteria for Rebars By Member가 설정된 Beam 이외의 element list
	CArray<T_ELEM_K, T_ELEM_K> m_aDcbbBeamKey; // 선택된 element 중에서 Beam type 이외의 element list
	CArray<T_MEMB_D, T_MEMB_D&> m_aDataMemb;   // IsContinuousMember() 함수에서 사용할 임시적으로 설정된 멤버
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> m_mapSelBeamKey;  // 선택된 element 중에서 Beam type의 element list
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
	void InsertItem(T_BREQ_K Key);
	void DeleteItem(T_BREQ_K Key);
	void SetListFocus(CArray<int, int>& aSelItem);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMEQUALDLG_H__9F5FA4DF_6B91_4CBD_813D_4FE1C1C81B01__INCLUDED_)
