#if !defined(AFX_DgnConCodeSeisDefDlg_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DgnConCodeSeisDefDlg_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "..\wg_cmd\BtnSTNew.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"

const int iDgn_SLTermID = 6;
const int iDgn_ShortTermID = 8;

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeSeisDefDlg dialog
struct _DGN_STLD
{
	CString strName;
	UINT key;
	int iAnalType;
	int iSeqNum;
	void Initialize()
	{
		strName = _T("");
		key = 0;
		iAnalType=0;
		iSeqNum=0;
	}
};

class __MY_EXT_CLASS__ CDgnConCodeSeisDefDlg : public CDialogMove
{
	// Construction
public:
	CDgnConCodeSeisDefDlg(CWnd* pParent = NULL);   // standard constructor
	CDgnConCodeSeisDefDlg(CString strCode, CWnd* pParent = NULL);
	// Dialog Data
	//{{AFX_DATA(CDgnConCodeSeisDefDlg)
	enum { IDD = IDD_DGN_CON_DEFINE_DLG };

	CListCtrl	m_StrucGroupList;
	CListCtrl	m_ExcludeList;
	CListCtrl	m_NotSFSRList;
	
	CButtonSTNew	m_MainSortAscBtn;
	CButtonSTNew	m_MainSortDSCBtn;
	CButtonSTNew	m_Sub1SortAscBtn;
	CButtonSTNew	m_Sub1SortDSCBtn;
	CButtonSTNew	m_Sub2SortAscBtn;
	CButtonSTNew	m_Sub2SortDSCBtn;

	//}}AFX_DATA
	
public:
	////////////////////////////////////////////////////////////////////
	// All Group : 호출하는 곳에서 이 데이터를 주고 받는다.
	// 이 데이터는 (DB에 저장된 데이터 + Auto Gen 데이터)로 구성되어 있다.
	CArray<UINT, UINT>  m_aAllStructGroupK;    
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>  m_arAllGroup;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// Sub Group : 호출하는 곳에서 이 데이터를 주고 받는다.
	CArray<UINT, UINT> m_aExcludeGroupK;
	CArray<UINT, UINT> m_aNotSFSRGroupK;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 정렬 및 중복 이름 방지 용도
	CArray<CString, CString&> m_aAllStructGroupName;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 정렬 위한 용도
	CArray<CString, CString&> m_aStructGroupName;
	CArray<CString, CString&> m_aExcludeGroupName;
	CArray<CString, CString&> m_aNotSFSRGroupName;
	////////////////////////////////////////////////////////////////////

	CString GetGrupName(T_GRUP_K Key);
	CString GetGrupName(CString& strGrupName);
	T_GRUP_K GetGrupKey(CString& strGroupName);
	BOOL GetGrup(T_GRUP_K GrupK, T_GRUP_D& GrupD);

	void SortGroupKListByName(CArray<CString, CString&>& arGroupName, CArray<UINT, UINT>& arGroupKList);

	void SetGroupData(CArray<UINT, UINT>& aAllStructGroupK, CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>& mAllStructGroup);
	void SetGroupData(CArray<UINT, UINT>& aExcludeGroup, CArray<UINT, UINT>& aNotSFSRGroup);
	void GetGroupData(CArray<UINT, UINT>& aAllStructGroupK, CArray<UINT, UINT>& aExcludeGroupK, CArray<UINT, UINT>& aNotSFSRGroupK);
	void GetGroupData(CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>& mAllStructGroup);
	void Write_ListCtrl(CListCtrl& GroupList, CArray<CString, CString&>& m_aGroupName);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarSpaceDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	T_GRUP_K m_LastGrupK;

	void MakeListData();
	void UpdateList();
	void InitControlByCode();

	// Add by GAY. PMS:4076. ('10.04.15). 내진설계 대상에서 제외할 부재들의 Group 정보 생성.
	int Generate_StructGroupToExclude(CArray<T_GRUP_D, T_GRUP_D&>& aGrupData);
	BOOL Generate_GrupData(const CString& strName, const CArray<T_ELEM_K, T_ELEM_K>& aElemData, const CMapEx<T_NODE_K, T_NODE_K, int, int>& mapNodeData, T_GRUP_D& GrupD);

	CString m_strCode;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConBarSpaceDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	afx_msg void OnDgnAutoGroup();

	afx_msg void OnDgnAddExcludeBtn();
	afx_msg void OnDgnDelExcludeBtn();
	afx_msg void OnDgnAddNotSFSRBtn();
	afx_msg void OnDgnDelNotSFSRBtn();

	afx_msg void OnDgnMainASCButton();
	afx_msg void OnDgnMainDSCButton();
	afx_msg void OnDgnSub1ASCButton();
	afx_msg void OnDgnSub1DSCButton();
	afx_msg void OnDgnSub2ASCButton();
	afx_msg void OnDgnSub2DSCButton();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnConCodeSeisDefDlg_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
