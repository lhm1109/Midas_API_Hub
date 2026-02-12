// CCMTendonProfileCopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonProfileCopyDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "CMTendonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileCopyDlg dialog

#define CDialog CCMDlgBase
CCMTendonProfileCopyDlg::CCMTendonProfileCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMTendonProfileCopyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonProfileCopyDlg)
	m_nOption = 0;
	m_nMode = 0;
	//}}AFX_DATA_INIT
	m_pParentDlg = pParent;
	((CCMTendonDlg*)m_pParentDlg)->SetProfDlgFlag(TRUE);
}


void CCMTendonProfileCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonProfileCopyDlg)
	DDX_Control(pDX, IDC_CMD_TENDON_COPY_ELEM_EDIT, m_ElemEdit);
// 	DDX_Control(pDX, IDC_CMD_TENDON_COPY_ELEM_SPIN, m_IncreSpin);
	DDX_Control(pDX, IDC_CMD_TENDON_COPY_VECTOR_EDIT, m_VectorEdit);
	DDX_Control(pDX, IDC_CMD_TENDON_VECTOR_UNIT, m_VectorUnit);
	DDX_Control(pDX, IDC_CMD_TENDON_INSERTION_EDIT, m_wndInsertion);
	DDX_Control(pDX, IDC_CMD_TENDON_ASSIGNED_EDIT, m_wndAssignElem);
	DDX_Control(pDX, IDC_CMD_TENDON_COPY_RADIO_ELEM, m_OptionRDO);
	DDX_Radio(pDX, IDC_CMD_TENDON_COPY_RADIO_ELEM, m_nOption);
	DDX_Radio(pDX, IDC_CMD_TENDON_COPY_COPY, m_nMode);
	DDX_Control(pDX, IDC_CMD_TENDON_COPY_AUTO_CHK, m_wndAutoChk);
	DDX_Control(pDX, IDC_CMD_TENDON_COPY_CURRENT_CHK, m_wndCurrentChk);
	DDX_Control(pDX, IDC_CMD_TENDON_COPY_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileCopyDlg public interface functions


/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileCopyDlg implementation





BEGIN_MESSAGE_MAP(CCMTendonProfileCopyDlg, CDialog)
	//{{AFX_MSG_MAP(CCMTendonProfileCopyDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnDlgClose)
	ON_BN_CLICKED(IDC_CMD_TENDON_COPY_ADD, OnCmdElemAdd)
	ON_BN_CLICKED(IDC_CMD_TENDON_COPY_DELETE, OnCmdElemDelete)
	ON_BN_CLICKED(IDC_CMD_TENDON_COPY_MODIFY, OnCmdElemModify)
	ON_BN_CLICKED(IDC_CMD_TENDON_COPY_RADIO_ELEM, OnCmdTendonCopyRadioElem)
	ON_BN_CLICKED(IDC_CMD_TENDON_COPY_RADIO_EQUAL, OnCmdTendonCopyRadioElem)
	ON_BN_CLICKED(IDC_CMD_TENDON_COPY_RADIO_NEW, OnCmdTendonCopyRadioElem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileCopyDlg message handlers

BOOL CCMTendonProfileCopyDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();
	m_wndAssignElem.Connect(SC_ID_ELEM,&m_wndAssignElem);
	m_wndInsertion.SetAttElemList();
	m_wndInsertion.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndInsertion.SetMaxElemKeyNum(1);
	
	m_arIncre.Add(IDC_CMD_TENDON_COPY_ELEM_EDIT);
	m_arIncre.Add(IDC_CMD_TENDON_COPY_ELEM_SPIN);
	m_arEqual.Add(IDC_CMD_TENDON_COPY_VECTOR_EDIT);
	m_arEqual.Add(IDC_CMD_TENDON_COPY_CURRENT_CHK);

	m_arNew.Add(IDC_CMD_TENDON_COPY_ADD);
	m_arNew.Add(IDC_CMD_TENDON_COPY_MODIFY);
	m_arNew.Add(IDC_CMD_TENDON_COPY_DELETE);
	m_arNew.Add(IDC_CMD_TENDON_ASSIGNED_EDIT);
	m_arNew.Add(IDC_CMD_TENDON_INSERTION_EDIT);
	m_arNew.Add(IDC_CMD_TENDON_COPY_TABLE);

	m_arOption.Add(IDC_CMD_TENDON_COPY_RADIO_ELEM);
	m_arOption.Add(IDC_CMD_TENDON_COPY_RADIO_EQUAL);
	m_arOption.Add(IDC_CMD_TENDON_COPY_RADIO_NEW);
	m_arCopyMove.Add(IDC_CMD_TENDON_COPY_COPY);
	m_arCopyMove.Add(IDC_CMD_TENDON_COPY_MOVE);
	m_VectorEdit.SetAttUcsDistance();
	m_VectorEdit.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	//m_VectorEdit.SetDistValueMode(TRUE);
	m_VectorUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ElemEdit.SetValue(1);
	m_ElemEdit.SetRange(0,100);
	m_ElemEdit.SetInteger(TRUE);

	InitGrid();
	EnableControls();
	
	OnCmdTendonCopyRadioElem();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTendonProfileCopyDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();	
}

void CCMTendonProfileCopyDlg::OnCmdElemAdd() 
{
	if(m_nOption!=2) return;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	Dlg2Data();
	if(!m_Data.strAssigned.IsEmpty()&& m_Data.nInsertion!=0)
	{
		m_aElems.Add(m_Data);
		m_wndGrid.InsertRecordByKey(m_aElems.GetSize(), m_Data);
		m_wndGrid.LockUpdate(bOldLock);
		if (!bOldLock) m_wndGrid.Redraw();
		UnselectAll();
		m_wndAssignElem.SetWindowText(_T(""));
		m_wndInsertion.SetWindowText(_T(""));
	}
	

}

void CCMTendonProfileCopyDlg::OnCmdElemDelete() 
{
	if(m_nOption!=2) return;
	ROWCOL nRow;
	CRowColArray aSelectedRows;
	int nDataRow,nSize;
	m_wndGrid.GetSelectedRows(aSelectedRows,TRUE,FALSE);
	nSize=aSelectedRows.GetSize();
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for(int i=nSize-1;i>=0;i--)
	{
		nRow=aSelectedRows[i];
		nDataRow=nRow-1;
		m_wndGrid.DeleteRecordByKey(nRow, m_aElems[nDataRow]);
		m_aElems.RemoveAt(nDataRow);
	}
	m_wndGrid.ReStoreKeys();
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMTendonProfileCopyDlg::OnCmdElemModify()
{

	if(m_nOption!=2) return;
	Dlg2Data();
	int nKey;
	int nDataRow;
	if(!m_wndGrid.GetCurrentElem(nKey))return;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	nDataRow=nKey-1;
	m_wndGrid.ModifyRecordByKey(nKey,m_Data);
	m_aElems.SetAt(nDataRow,m_Data);
	m_wndGrid.ReStoreKeys();
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	
	
}


void CCMTendonProfileCopyDlg::OnCmdTendonCopyRadioElem() 
{
	CDlgUtil::CtrlRadioGetCheck(this, m_arOption, m_nOption);	
	CDlgUtil::CtrlEnableDisable(this, m_arIncre, m_nOption==0);
	CDlgUtil::CtrlEnableDisable(this, m_arEqual, m_nOption==1);
	CDlgUtil::CtrlEnableDisable(this, m_arNew, m_nOption==2);
 
	
}
BOOL CCMTendonProfileCopyDlg::Dlg2Data()
{
	m_Data.strAssigned.Empty();
	m_Data.nInsertion=0;
	m_wndAssignElem.GetWindowText(m_Data.strAssigned);
	CString StrText;
	m_wndInsertion.GetWindowText(StrText);
	m_Data.nInsertion=_ttoi(StrText);
	//if (!m_VectorEdit.GetCoordData(&m_TdnaData.dOffsetY, NULL, NULL)) return FALSE;
	return TRUE;

}
BOOL CCMTendonProfileCopyDlg::Data2Dlg()
{
	m_wndAssignElem.SetWindowText(m_Data.strAssigned);
	CString StrText;
	StrText.Format(_T("%d"),m_Data.nInsertion);
	m_wndInsertion.SetWindowText(StrText);
	return TRUE;
}

void CCMTendonProfileCopyDlg::InitGrid()
{
	m_wndGrid.Initialize(m_pDoc, this);
}
void CCMTendonProfileCopyDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pParentDlg && m_pParentDlg->GetSafeHwnd())
	{
		m_pParentDlg->ShowWindow(SW_SHOW);
		((CCMTendonDlg*)m_pParentDlg)->SetProfDlgFlag(FALSE);
	}
	
	CDialog::PostNcDestroy();
}
void CCMTendonProfileCopyDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!DoApply())
	{
		return;
	}
	UnselectAll();
	DestroyWindow();
}
void CCMTendonProfileCopyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	UnselectAll();
	DestroyWindow();
}


void CCMTendonProfileCopyDlg::UnselectAll()
{
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	//if (!m_bExternalSetting) return;
	if (!I_GENModelBase::GetCurMySelfST()) return;
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	I_GENModelBase::GetCurMySelfST()->ViewInvalidate(FALSE);
}
BOOL CCMTendonProfileCopyDlg::DoApply()
{
	CArray<CString,CString&> TendonNames;
	CArray<T_TDNA_K,T_TDNA_K> SelectedKeys;
	T_TDNA_D TdnaData;
	if(m_pParentDlg) 
		((CCMTendonDlg*)m_pParentDlg)->GetSelectedTendon(SelectedKeys);
	for(int i=0;i<SelectedKeys.GetSize();i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetTdna(SelectedKeys[i],TdnaData))
		{
			 ASSERT(0);return FALSE;
		}
		TendonNames.Add(TdnaData.TendonName);
	}
	int nCopyMove;
	CDlgUtil::CtrlRadioGetCheck(this, m_arCopyMove, nCopyMove);
	BOOL bDelOriginal,bAutoChk,bCurrentChk;
	bDelOriginal=(nCopyMove==0)?FALSE:TRUE;
	bAutoChk=m_wndAutoChk.GetCheck();
	bCurrentChk=m_wndCurrentChk.GetCheck();
	switch(m_nOption)
	{
	case 0:
		{
			int nIncre;
			CString strElem;
			m_ElemEdit.GetWindowText(strElem);
			nIncre=_ttoi(strElem);
			if(!m_pDoc->m_pDataCtrl->CopyTendonProfile(TendonNames,_T(""),_T(""),nIncre,bDelOriginal,bAutoChk)) return FALSE;
		}
		break;
	case 1:
		{
			I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
			double Udx,Udy,Udz,Wdx,Wdy,Wdz;
			CString strEQDist;
			m_VectorEdit.GetWindowText(strEQDist);
			if(TRUE == m_VectorEdit.IsKeyPressed())
				if(CStrParser::GetEQDistance(strEQDist,Udx, Udy, Udz)) 
				{
					pIGM->GetWcsVector(Udx, Udy, Udz,Wdx, Wdy, Wdz);
				}
				else return FALSE;
			else
			{
				//m_wndEQDist.GetCurMousePosDist(dwx,dwy,dwz,NT.m_EQ_DX,NT.m_EQ_DY,NT.m_EQ_DZ);
				m_VectorEdit.GetCurMousePosDist(Wdx, Wdy, Wdz,
																			 Udx, Udy, Udz);
			}
			T_NODE_D NodeData;
			NodeData.x=Wdx;
			NodeData.y=Wdy;
			NodeData.z=Wdz;

			m_pDoc->m_pUnitCtrl->ConvertUnitNodeIn(NodeData);
			if(!m_pDoc->m_pDataCtrl->CopyTendonProfile(TendonNames,_T(""),_T(""),NodeData.x,NodeData.y,NodeData.z,bDelOriginal,bCurrentChk,bAutoChk)) return FALSE;
		}
		break;
	case 2:
		{
			if(SelectedKeys.GetSize()!=1)
			{
				ASSERT(0);
				return FALSE;
			}
			CArray<CString, CString&> rstrElement;
			CArray<T_ELEM_K,T_ELEM_K> Insertion;
			rstrElement.SetSize(m_aElems.GetSize());
			Insertion.SetSize(m_aElems.GetSize());
			for(int i=0;i<m_aElems.GetSize();i++)
			{
				rstrElement[i]=m_aElems[i].strAssigned;
				Insertion[i]=m_aElems[i].nInsertion;
			}
			if(bDelOriginal && m_aElems.GetSize()>1)
			{
				AfxMessageBox(_LS(IDS_CMD_TDNA_CANNOT_MOVE_MULTI));
				return FALSE;
			}        
			if(!m_pDoc->m_pDataCtrl->CopyTendonProfile(TendonNames[0],_T(""),_T(""),rstrElement,Insertion,bDelOriginal,bAutoChk)) return FALSE;
		}
		break;
	default:
		ASSERT(0);
	}
	
	return TRUE;
}
void CCMTendonProfileCopyDlg::EnableControls()
{
	BOOL bElemOnly;
	bElemOnly=TRUE;
	if(m_pParentDlg) 
	{
		CArray<T_TDNA_K,T_TDNA_K> SelectedKeys;
		T_TDNA_D TdnaData;
		if(m_pParentDlg) 
			((CCMTendonDlg*)m_pParentDlg)->GetSelectedTendon(SelectedKeys);
		else ASSERT(0);
		for(int i=0;i<SelectedKeys.GetSize();i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetTdna(SelectedKeys[i],TdnaData))
			{
				 ASSERT(0);return;
			}
			if(TdnaData.nTendonShape!=2)
			{
				bElemOnly=FALSE;
				break;
			}
		}
		CArray<UINT,UINT> arDisableMethod;
		arDisableMethod.Add(IDC_CMD_TENDON_COPY_RADIO_ELEM);
		arDisableMethod.Add(IDC_CMD_TENDON_COPY_RADIO_NEW);
		arDisableMethod.Add(IDC_CMD_TENDON_COPY_AUTO_CHK);
		CDlgUtil::CtrlEnableDisable(this, arDisableMethod,bElemOnly);
		GetDlgItem(IDC_CMD_TENDON_COPY_RADIO_NEW)->EnableWindow(bElemOnly&&SelectedKeys.GetSize()==1);
		if(bElemOnly)
		{
			m_nOption=0;
		}
		else
		{
			m_nOption=1;
		}
		 CDlgUtil::CtrlRadioSetCheck(this, m_arOption, m_nOption);
		 CDlgUtil::CtrlRadioSetCheck(this, m_arCopyMove, 0);
	}
	else ASSERT(0);
}
void CCMTendonProfileCopyDlg::OnChangeCurrentElem(int key)
{
	int nDataRow;
	nDataRow=key-1;
	m_Data=m_aElems.GetAt(nDataRow);
	Data2Dlg();
}

