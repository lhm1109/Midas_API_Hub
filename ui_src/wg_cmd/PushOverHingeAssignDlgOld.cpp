// PushOverHingeAssignDlgOld.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PushOverHingeAssignDlgOld.h"


#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_common\wg_common_Query.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlgOld dialog

//IMPLEMENT_DYNCREATE(CPushOverHingeAssignDlgOld, CCMChildBarBase)

CPushOverHingeAssignDlgOld::CPushOverHingeAssignDlgOld()
	: CCMChildBarBase(CPushOverHingeAssignDlgOld::IDD)
{
	//{{AFX_DATA_INIT(CPushOverHingeAssignDlgOld)
	m_nLocation = 0;
	m_nElemType = 1;
	m_nCommandType = 0;
	//}}AFX_DATA_INIT
	m_nHingeType = 0;
}


void CPushOverHingeAssignDlgOld::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPushOverHingeAssignDlgOld)
	DDX_Control(pDX, IDC_PUSHOVER_ASSIGN_HINGETYPE, m_wndHingeTypeInform);
	DDX_Control(pDX, IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB, m_cmbHingeType);
	DDX_Radio(pDX, IDC_PUSHOVER_ASSIGN_LOC_I, m_nLocation);
	DDX_Radio(pDX, IDC_PUSHOVER_ELEMTYPE_TRUSS, m_nElemType);
	DDX_Radio(pDX, IDC_PUSHOVER_ASSIGN_ADDREPL, m_nCommandType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPushOverHingeAssignDlgOld, CCMChildBarBase)
	//{{AFX_MSG_MAP(CPushOverHingeAssignDlgOld)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_CBN_SELCHANGE(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB, OnSelchangePushoverAssignHingetypeCmb)
	ON_BN_CLICKED(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN, OnPushoverAssignHingetypelistBtn)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_BEAM, OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_TRUSS, OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_WALL, OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ASSIGN_DELETE, OnPushoverAssignCmd)
	ON_BN_CLICKED(IDC_PUSHOVER_ASSIGN_ADDREPL, OnPushoverAssignCmd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlgOld message handlers

/*
void CPushOverHingeAssignDlgOld::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMChildBarBase::OnInitialUpdate();

#if defined(_CIVIL)
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(FALSE);
#else
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(TRUE);
#endif

	OnSelchangePushoverAssignHingetypeCmb();
}
*/


BOOL CPushOverHingeAssignDlgOld::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMChildBarBase::OnInitDialog();	 

#if defined(_CIVIL)
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(FALSE);
#else
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(TRUE);
#endif

	OnSelchangePushoverAssignHingetypeCmb();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPushOverHingeAssignDlgOld::OnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey, aFailKey, aRealKey;
	GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_element_is_selected_));
		return;
	}
	
	if(m_nCommandType == 0)// Add Or Replace, Filtering by Element type
	{
		switch(m_nElemType)
		{
			case 0:// Truss
				GetElementByKind(aSelKey,TRUSS_EL);
				if (aSelKey.GetSize() == 0) 
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Truss_is_selected__));
					return;
				}
				break;
			case 1:// Beam
				GetElementByKind(aSelKey,BEAM_EL);
				if (aSelKey.GetSize() == 0) 
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Beam_is_selected__));
					return;
				}
				break;
			case 2:// Wall
				GetElementByKind(aSelKey,WALL_EL);
				if (aSelKey.GetSize() == 0) 
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Wall_is_selected__));
					return;
				}
				break;
		}
	}

	int i=0;
	aFailKey.RemoveAll();
	aRealKey.RemoveAll();
	T_ELEM_D ElemD; ElemD.Initialize();
	T_MATL_D MatlD; MatlD.Initialize();
	T_SECT_D SectD; SectD.Initialize();
	int nMembType=0;

	T_HNGT_D Hngt_d;
	T_HNGT_K Hngt_k;
	T_HNGE_D Hnge_d;
	Hngt_d.Initialize();

	if(!m_cmbHingeType.GetSelectedHngt(Hngt_k)) return;
	if(!m_pDoc->m_pAttrCtrl->GetHngt(Hngt_k,Hngt_d)) return;
	
	Hnge_d.HingeType = Hngt_k;

	BOOL bReal = ((Hngt_d.nMultiATCType==0 && Hngt_d.Prop.bHysUseCalcValue) || (Hngt_d.nMultiATCType==1 && Hngt_d.Prop.bUseCalcValue));
	if(!bReal) goto GT_ASGN_SLECT;

	// KJH20031229 => Assign 할 수 없는 단면 형상을 골라내고 메시지...(신미영)
	for(i=0; i<aSelKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemD);
		m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD);
		if(MatlD.Type == _T("C"))
		{
			nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
			m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
#if defined(_MGEN)	// Gen.
			if(m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
			{
				// Change by ZINU.('04.02.10). Wall is NOT Section.
				aRealKey.Add(aSelKey[i]);
			}
			else if(nMembType == D_MBTP_BEAM)
#else	// Civil.
			if(nMembType == D_MBTP_BEAM)
#endif
			{
		    if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					(SectD.SectBefore.Shape==_T("T") || SectD.SectBefore.Shape==_T("SB")))
					aRealKey.Add(aSelKey[i]);
				else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
					aRealKey.Add(aSelKey[i]);
				else aFailKey.Add(aSelKey[i]);
			}
			else if(nMembType == D_MBTP_COLUMN || nMembType == D_MBTP_BRACE)
			{
#if defined(_CIVIL)	// Civil.
				if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					(SectD.SectBefore.Shape==_T("B")   || SectD.SectBefore.Shape==_T("P")    ||
			     SectD.SectBefore.Shape==_T("SR")  || SectD.SectBefore.Shape==_T("SB")   ||
			     SectD.SectBefore.Shape==_T("OCT") || SectD.SectBefore.Shape==_T("SOCT") ||
			     SectD.SectBefore.Shape==_T("TRK") || SectD.SectBefore.Shape==_T("STRK") ||
			     SectD.SectBefore.Shape==_T("HTRK")))
					 aRealKey.Add(aSelKey[i]);
				else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
					aRealKey.Add(aSelKey[i]);
				else aFailKey.Add(aSelKey[i]);
#else
				if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					(SectD.SectBefore.Shape==_T("P") || SectD.SectBefore.Shape==_T("SR") ||
			     SectD.SectBefore.Shape==_T("SB")))
					 aRealKey.Add(aSelKey[i]);
				else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
					aRealKey.Add(aSelKey[i]);
				else aFailKey.Add(aSelKey[i]);
#endif
			}
			else aFailKey.Add(aSelKey[i]);
		}
		else if(MatlD.Type == _T("S"))
		{
			m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
			if(SectD.nStype == D_SECT_TYPE_REGULAR)
			{
				if(SectD.SectBefore.Shape==_T("L")  || SectD.SectBefore.Shape==_T("C")  ||
		       SectD.SectBefore.Shape==_T("H")  || SectD.SectBefore.Shape==_T("T")  ||
		       SectD.SectBefore.Shape==_T("B")  || SectD.SectBefore.Shape==_T("P")  ||
		       SectD.SectBefore.Shape==_T("SR") || SectD.SectBefore.Shape==_T("SB") ||
		       SectD.SectBefore.Shape==_T("2L") || SectD.SectBefore.Shape==_T("2C"))
		       aRealKey.Add(aSelKey[i]);
				else aFailKey.Add(aSelKey[i]);
			}
			else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
				aRealKey.Add(aSelKey[i]);
			else aFailKey.Add(aSelKey[i]);
		}
		else if(MatlD.Type == _T("SRC"))
		{
			m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
			if(SectD.nStype == D_SECT_TYPE_SRC)
			{
				if(SectD.SectBefore.Shape==_T("RBO") || SectD.SectBefore.Shape==_T("RBC") ||
					 SectD.SectBefore.Shape==_T("RPO") || SectD.SectBefore.Shape==_T("RPC") ||
					 SectD.SectBefore.Shape==_T("RHB") || SectD.SectBefore.Shape==_T("CBO") ||
					 SectD.SectBefore.Shape==_T("CBC") || SectD.SectBefore.Shape==_T("CPO") ||
					 SectD.SectBefore.Shape==_T("CPC") || SectD.SectBefore.Shape==_T("CHB") ||
					 SectD.SectBefore.Shape==_T("EBC") || SectD.SectBefore.Shape==_T("EPC") )
					 aRealKey.Add(aSelKey[i]);
				else aFailKey.Add(aSelKey[i]);
			}
			else aFailKey.Add(aSelKey[i]);
		}
		else aFailKey.Add(aSelKey[i]);
	}
// END KJH
GT_ASGN_SLECT:
	BOOL bSuccess;
	if (m_nCommandType == 0) // add or replace
	{
		if(m_nLocation !=3)
		{
			Hnge_d.nHingeLocation = m_nLocation +1;	
			if(bReal)
			  bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aRealKey, Hnge_d);
			else 
				bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aSelKey, Hnge_d);
		}
		else
		{
			Hnge_d.nHingeLocation = 1;	
			if(bReal)
			  bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aRealKey, Hnge_d);
			else
				bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aSelKey, Hnge_d);
			Hnge_d.nHingeLocation = 3;	
			if(bReal)
			  bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aRealKey, Hnge_d)*bSuccess;
			else 
				bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aSelKey, Hnge_d)*bSuccess;
		}
	}
	else if (m_nCommandType == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelHnge(aSelKey);	

	if(bReal)
	{
		CNumericOptimizer optimizer;
		int nFailCount = aFailKey.GetSize();
		if(nFailCount>0 && m_nCommandType==0)
		{
			CString msg = _T("");
		
			long* aNum = new long[nFailCount];
			for(int i=0; i<nFailCount; i++) aNum[i] = aFailKey.GetAt(i);

			qsort((void*)aNum, nFailCount, sizeof(long), CNumericOptimizer::comparei);
			msg = optimizer.Optimize(aNum, nFailCount);
			delete [] aNum;

			GSaveHistoryFormatNF(_LS(IDS_CMD_PUSH_ERROR__SECT_SHAP), msg);
		}
	}
	if(bSuccess)
	{
		if(m_nCommandType == 0)  // Add or Replace
		{
			if(bReal)
				m_pDoc->m_pViewCtrl->UnSelectElem(NULL, aRealKey, TRUE);
			else 
				m_pDoc->m_pViewCtrl->UnSelectElem(NULL, aSelKey, TRUE);
		}
		else
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CPushOverHingeAssignDlgOld::OnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
	
}

void CPushOverHingeAssignDlgOld::OnSelchangePushoverAssignHingetypeCmb() 
{
	T_HNGT_D Hngt_d;
	T_HNGT_K Hngt_k;
	if(!m_cmbHingeType.GetSelectedHngt(Hngt_k)) return;
	if(!m_pDoc->m_pAttrCtrl->GetHngt(Hngt_k,Hngt_d)) return;
	m_nHingeType = Hngt_d.nHingeType;
	SetHingeTypeInform(m_nHingeType);
	switch(m_nHingeType)
	{
		case 0://P
		case 2:// Shear
		case 4:// Torsion
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_BEAM)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->EnableWindow(TRUE);
			m_nLocation = 1;// Center
			if(m_nHingeType==2) // Modified by KJH20040713 from 강은경
			{
				GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(FALSE);
				if(m_nElemType==0) m_nElemType = 1;
			}
			else
			{
				GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(TRUE);
			}
			break;
		case 1://M
		case 3://PMM
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_BEAM)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->EnableWindow(TRUE);
			
			if(m_nLocation == 1)
				m_nLocation = 0;// I
			if(m_nElemType == 0)
				m_nElemType = 1;
			break;
	}
	
	UpdateData(FALSE);
}

void CPushOverHingeAssignDlgOld::OnPushoverAssignHingetypelistBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PUSHOVER_DEFINE_HINGE, 0));
	
}

void CPushOverHingeAssignDlgOld::OnPushOverElemType() 
{
	UpdateData(TRUE);
	if(m_nElemType == 0)// Truss
	{
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
		m_nLocation = 1;// Center
	}
	else
	{
		if(m_nHingeType == 1 || m_nHingeType == 3) // M or PMM
		{	
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		}
		else// P,V,T
		{	
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		}

		
	}
	UpdateData(FALSE);

	
}

void CPushOverHingeAssignDlgOld::GetElementByKind(CArray<UINT,UINT>& aKey,int nElemType)
{
	CQueryCtrl QueryCtrl;
	CString strElement;
	strElement.Format(_T("%ld"),nElemType);
	CArray<UINT,UINT> ElemKeys;
	CArray<UINT,UINT> SelKeys;
	SelKeys.Copy(aKey);
	aKey.RemoveAll();

	CQueryCondition QCond;          // 요소의 종류에 따른 쿼리 조건을 만든다.
	QCond.m_nCategory = CAT_ELEM_TYPE;
	QCond.m_nOperation = QC_OP_AND;
	QCond.m_aValue.Add(strElement);
	
	QueryCtrl.SelectKeyFromElem(ElemKeys,QCond);
	QueryCtrl.IntersectKey(ElemKeys,SelKeys,aKey);

}	
	

void CPushOverHingeAssignDlgOld::SetHingeTypeInform(int nHingeType)
{
	switch(nHingeType)
	{
		case 0:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Axial_P));
			break;
		case 1:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Moment_My_Mz));
			break;
		case 2:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Shear_Vy_Vz));
			break;
		case 3:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__P_My_Mz));
			break;
		case 4:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Torsion_T));
			break;
		default:
			m_wndHingeTypeInform.SetWindowText(_T(""));
			break;
	}
}

void CPushOverHingeAssignDlgOld::OnPushoverAssignCmd() 
{
	UpdateData(TRUE);
	if(m_nCommandType == 1)// delete
	{
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ELEMTYPE_BEAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN)->EnableWindow(TRUE);
		OnSelchangePushoverAssignHingetypeCmb();
	}
}

