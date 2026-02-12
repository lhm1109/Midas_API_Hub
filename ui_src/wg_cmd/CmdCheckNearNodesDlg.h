#pragma once
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
class CDBDoc;

class CmdCheckNode  
{
public:
	unsigned int  m_Key;
	double      m_Val;
	CmdCheckNode    *l_child, *r_child;
	void Insert(unsigned int key, double value);
	void FastInsertSortedList(CArray<UINT, UINT>& aKey, CArray<double, double>& aVal, int nStart, int nEnd);
	void SearchValueRange(CmdCheckNode *root, double lower_bound, double upper_bound,CArray <unsigned int, unsigned int> &node_list,unsigned int nCurKey);
	CmdCheckNode::CmdCheckNode();
	CmdCheckNode::CmdCheckNode(unsigned int key, double val);
	CmdCheckNode::~CmdCheckNode();
};
class CCmdCheckNearNodesDlg : public CCMDlgBase
{
// Construction
public:
	CCmdCheckNearNodesDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdCheckNearNodesDlg();

	enum { IDD = IDD_CMD_CHECK_NEARLY_NODES };
public:
	CDBDoc* m_pDoc;

protected:
	CListCtrl	m_lstGrvSel       ;
	CListCtrl	m_lstGrvUnsel     ;
	CListCtrl	m_lstEffLoad      ;
	CComboBox m_cmbEffLoadCase  ;
	double    m_dEffLoadFactor  ;

protected:
// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	//virtual void OnOK();
	//virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnApplyBtn();
	afx_msg void OnCloseBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CheckNearNode();
	BOOL CheckShortBeam();
	BOOL MergeShortBeam(T_ELEM_K_LIST& aSearchElemK); 
	CTextUnit	m_wndToleranceUnit;
	CEditUnit	m_wndToleranceEdit;
	CTextUnit	m_wndToleranceUnit2;
	CEditUnit	m_wndToleranceEdit2;
	void FindSameDistNodeList(double distance, double tolerance,CArray<T_NODE_K, T_NODE_K>& node_list,T_NODE_K nCurKey);
	void InitTree(const CArray<T_NODE_K, T_NODE_K>& aNodeK);
	void InitSortedList(CArray<UINT, UINT>& aKey, CArray<double, double>& aDist);
	//CDistSortedList m_DistSortedList;
	int		m_nOption;
	CmdCheckNode* m_Tree;

//protected:
//	CArray<T_STLD_K, T_STLD_K> m_aSelGravityLoad;
//	CArray<T_STLD_K, T_STLD_K> m_aUnselGravityLoad;
//	CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&> m_aEffectiveLoad;
};

