// BndrSospIntegral.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ResponseDispLoadMode3Dlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\UndoCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_common\TBUtilFunc.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\TreeMenuMode.h"

#define DIR_NORMAL 0
#define DIR_LX     1
#define DIR_LY     2
#define DIR_LZ     3
#define DIR_GX     4
#define DIR_GY     5
#define DIR_GZ     6

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode3Dlg dialog


ResponseDispLoadMode3Dlg::ResponseDispLoadMode3Dlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(ResponseDispLoadMode3Dlg::IDD, pParent)
{
		m_pDoc = CDBDoc::GetDocPoint();
}


void ResponseDispLoadMode3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ResponseDispLoadMode3Dlg)
		DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
		DDX_Control(pDX, IDC_TM_BF_CBO_LC,    m_LoadCase);
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA1,m_EditGroudZ );
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA2,m_EditDisp   );
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA3,m_EditModulus);
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA1,m_UnitGroudZ );
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA2,m_UnitDisp   );
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA3,m_UnitModulus);
		DDX_Control(pDX, IDC_COMBO_RSMM1_DIR,m_CobxDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ResponseDispLoadMode3Dlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(ResponseDispLoadMode3Dlg)
	//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDC_TM_EXECUTE, &ResponseDispLoadMode3Dlg::OnBnClickedTmExecute)
		ON_BN_CLICKED(IDC_TM_CLOSE, &ResponseDispLoadMode3Dlg::OnBnClickedTmClose)
		ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, &ResponseDispLoadMode3Dlg::OnBnClickedTmDefineLdgrBtn)
		ON_BN_CLICKED(IDC_TM_BF_BTN_LC, &ResponseDispLoadMode3Dlg::OnBnClickedTmBfBtnLc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode3Dlg message handlers

BOOL ResponseDispLoadMode3Dlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
		CDlgUtilEx::CtrlCheck(this, IDC_RADIO_OPTION_ADD, TRUE);
		CDlgUtilEx::CtrlCheck(this, IDC_RADIO_PROJECT_YES, TRUE);
		m_LoadCase.AddStaticIncludeType(_T("E"), FALSE);
		m_LoadCase.AddStaticIncludeType(_T("USER"), FALSE);
		m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);    
		m_EditGroudZ  .SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_EditDisp    .SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_EditModulus .SetUnitType(D_UNITSYS_BASE_PRESSURE);
		m_UnitGroudZ  .SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_UnitDisp    .SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_UnitModulus .SetUnitType(D_UNITSYS_BASE_PRESSURE);
		//
		m_CobxDir.AddString(_LS(IDS_TM_0524Local_x)) ;m_CobxDir.SetItemData(0,DIR_LX);
		m_CobxDir.AddString(_LS(IDS_TM_0524Local_y)) ;m_CobxDir.SetItemData(1,DIR_LY);
		m_CobxDir.AddString(_LS(IDS_TM_0524Local_z)) ;m_CobxDir.SetItemData(2,DIR_LZ);
		m_CobxDir.AddString(_LS(IDS_TM_0524Global_X));m_CobxDir.SetItemData(3,DIR_GX);
		m_CobxDir.AddString(_LS(IDS_TM_0524Global_Y));m_CobxDir.SetItemData(4,DIR_GY);
		m_CobxDir.AddString(_LS(IDS_TM_0524Global_Z));m_CobxDir.SetItemData(5,DIR_GZ);
		m_CobxDir.SetCurSel(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void ResponseDispLoadMode3Dlg::OnBnClickedTmExecute()
{
		// TODO: 在此添加控件通知处理程序代码
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
		pIGM->GetSelectedElemKeyList(arElemKeyList); 
		T_KEY_LIST lstSlabs;
		T_KEY_LIST lstBeams;
		T_ELEM_D ElemData;
		for(int i=arElemKeyList.GetSize()-1; i >= 0; i--)  // 急琶等 Element 吝俊辑 Plane鸥涝父 急琶茄促.
		{
				m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i), ElemData);	
				if(m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
						lstSlabs.Add(arElemKeyList[i]);
				else if(m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
						lstBeams.Add(arElemKeyList[i]);
		}
		if(lstSlabs.GetSize() < 1 && lstBeams.GetCount() <1)
		{
				GSaveHistoryFormatNF(_LS(IDS_CMD2_SOD_SEL_ELEM_NO_SEL_ELEM));
				return;
		}    
		//
		T_STLD_K stldK = 0;
		UINT     CaseType = 0;
		if(!m_LoadCase.GetSelectedLoad(CaseType, stldK))
		{
				GSaveHistoryNF(_LS(IDS_CMD_TDNT_ITEM_Parameter_Error__));
				return;
		}
		T_LDGR_K LdgrK = 0;
		m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
		if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_OPTION_DEL))
		{
				if(Delete(lstSlabs, lstBeams, stldK, LdgrK))
						m_pDoc->m_pViewCtrl->UnselectAll(NULL);
				return;
		}
		//
		double dPara[] = {0,0,0};
		if(!GetPara(dPara))
		{
				GSaveHistoryNF(_LS(IDS_CMD_TDNT_ITEM_Parameter_Error__));
				return;
		}
		//
		if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Pressure_Load)), CMDTYPE_REMOVE_ANALYSIS))
				return;
		BOOL bAddData = FALSE;
		BOOL bRePlace = CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_OPTION_MOD);
		if(lstSlabs.GetCount()>0)
		{
				T_PRES_D Data;
				Data.Initialize();
				Data.LoadCaseKey = stldK;
				Data.GroupKey    = LdgrK;
				Data.LoadCommandTypeNew = 1;
				Data.LoadType = 1;
				Data.nEdgeFace = 1;
				Data.LoadDirectionNew = this->m_CobxDir.GetItemData(m_CobxDir.GetCurSel());
				if(Data.LoadDirectionNew == 4 || Data.LoadDirectionNew == 5 || Data.LoadDirectionNew == 6)
				{
						if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_PROJECT_YES))
								Data.Projection = 1;
						else
								Data.Projection = 0;
				}
				else
						Data.Projection = 0;
				CArray<T_ELEM_K, T_ELEM_K> rKey;
				CArray<T_PRES_D, T_PRES_D&> rData;
				for (int i=0; i<lstSlabs.GetCount(); i++)
				{
						memset(Data.Force, 0, sizeof(Data.Force));
						if(!CalForce(lstSlabs[i], dPara, &Data.Force[1]))
								continue;
						rKey.Add(lstSlabs[i]);
						rData.Add(Data);
				}    
				//
				if(rData.GetCount() > 0 && !m_pDoc->m_pDataCtrl->AddPres(rKey, rData, bRePlace, FALSE, FALSE))
				{
						m_pDoc->m_pUndoCtrl->CancelEditDB();
						return;
				}
				bAddData = TRUE;
		}
		//
		if(lstBeams.GetCount()>0)
		{ 
				T_BMLD_D BmldData;
				BmldData.Initialize();
				if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_PROJECT_YES))
						BmldData.Projection = 1;
				BmldData.LoadDirection = this->m_CobxDir.GetItemData(m_CobxDir.GetCurSel());
				BmldData.LoadCommandType = 1; // BML, LBL, FBL, TBL;
				BmldData.LoadCaseKey = stldK;
				BmldData.GroupKey    = LdgrK;
				BmldData.LoadType    = 3;
				BmldData.DistanceRatio[0] = 0;
				BmldData.DistanceRatio[1] = 1;
				int nForceIndex = BmldData.LoadType -1;
				CArray<T_ELEM_K, T_ELEM_K> rKey;
				CArray<T_BMLD_D, T_BMLD_D&> rData;
				double dUnitBemLen = GetBeamWidth();
				for (int i=0; i<lstBeams.GetCount(); i++)
				{
						memset(BmldData.Force_Type[nForceIndex], 0, sizeof(BmldData.Force_Type[nForceIndex]));
						if(!CalForce(lstBeams[i], dPara, BmldData.Force_Type[nForceIndex], dUnitBemLen))
								continue;
						rKey.Add(lstBeams[i]);
						rData.Add(BmldData);
				}    
				//
				if(rData.GetCount()>0 && !m_pDoc->m_pDataCtrl->AddBmld(rKey, rData, bRePlace, FALSE, FALSE, FALSE))
				{
						m_pDoc->m_pUndoCtrl->CancelEditDB();
						return;
				}
				bAddData = TRUE;
		}
		unsigned int nCmd = lstBeams.GetCount()>0 ?  LT_BMLD_CMD: LT_PRES_CMD;
		if(bAddData)
		{
				m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, nCmd);
				 m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		}
		else
				m_pDoc->m_pUndoCtrl->CancelEditDB();   
}


void ResponseDispLoadMode3Dlg::OnBnClickedTmClose()
{
		// TODO: 在此添加控件通知处理程序代码
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}


void ResponseDispLoadMode3Dlg::OnBnClickedTmDefineLdgrBtn()
{
		// TODO: 在此添加控件通知处理程序代码
		CreateOrActivateDlg(m_pDoc, CCMLoadGroupDlg::IDD);	
}


void ResponseDispLoadMode3Dlg::OnBnClickedTmBfBtnLc()
{
		// TODO: 在此添加控件通知处理程序代码
		AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

BOOL ResponseDispLoadMode3Dlg::Delete(T_KEY_LIST&lstSlabs, T_KEY_LIST&lstBeams, T_KEY stldK, T_KEY groupK)
{
		if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Delete_Pressure_Load)), CMDTYPE_REMOVE_ANALYSIS))
				return FALSE;
	 if(lstSlabs.GetCount() >0 && !m_pDoc->m_pDataCtrl->DelPres(lstSlabs, stldK, groupK, FALSE))
	 {
			 m_pDoc->m_pUndoCtrl->CancelEditDB();
			 return FALSE;
	 }
	 if(lstBeams.GetCount() >0 && !m_pDoc->m_pDataCtrl->DelBmld(lstBeams, stldK, groupK, FALSE))
	 {
			 m_pDoc->m_pUndoCtrl->CancelEditDB();
			 return FALSE;
	 }
	 unsigned int nCmd = lstBeams.GetCount()>0 ?  LT_BMLD_CMD: LT_PRES_CMD;
	 m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, nCmd);
	 return TRUE;
}
double ResponseDispLoadMode3Dlg::GetBeamWidth()
{
	 double d1M = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
	 return d1M;
}
BOOL ResponseDispLoadMode3Dlg::GetPara(double dPara[])
{
		dPara[0] = m_EditGroudZ .GetEditValue();
		dPara[1] = m_EditDisp   .GetEditValue();
		dPara[2] = m_EditModulus.GetEditValue();
		if(fabs(fabs(dPara[1]) < 1e-6 || fabs(dPara[2]) < 1e-6))
				return FALSE;
		return TRUE;
}
double ResponseDispLoadMode3Dlg::GetGroudZ()
{
		T_BLDC_D data;
		if(!m_pDoc->m_pAttrCtrl->GetBldc(data))
				data.Initialize();
		if(data.bUseBaseLevel)
				return data.dBaseLevel;
		return 0.0;
}
BOOL ResponseDispLoadMode3Dlg::CalForce(const T_KEY &ElemK, double dPara[], double dForce[], double dLen)
{
		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
				return FALSE;
		double dGroundZ = GetGroudZ();
		T_NODE_D NodeD;
		for (int i=0; i<D_ELEM_MAXNOD; i++)
		{
				T_KEY nNodeK = ElemD.elnod[i];
				if(nNodeK == 0)
						return TRUE;
				if(!m_pDoc->m_pAttrCtrl->GetNode(nNodeK, NodeD))
						return FALSE;
				if(NodeD.z > dGroundZ)
				{
						GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Warning___ELEMISONGROUND_), ElemK);
						return FALSE;
				}
				double dGd = dPara[2];
				double dH  = dPara[0];
				double dUmax = dPara[1];
				double dz = dGroundZ - NodeD.z;
				dForce[i] = dGd/2.0/dH*M_PI/2.0*dUmax*sin(M_PI*dz/2.0/dH)*dLen;
		}
		return TRUE;
}