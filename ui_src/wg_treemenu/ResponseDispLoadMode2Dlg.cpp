// BndrSospIntegral.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ResponseDispLoadMode2Dlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_common\TBUtilFunc.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\TreeMenuMode.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode2Dlg dialog


ResponseDispLoadMode2Dlg::ResponseDispLoadMode2Dlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(ResponseDispLoadMode2Dlg::IDD, pParent)
{
		m_pDoc = CDBDoc::GetDocPoint();
}


void ResponseDispLoadMode2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ResponseDispLoadMode2Dlg)
		DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
		DDX_Control(pDX, IDC_TM_BF_CBO_LC,    m_LoadCase);
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA1,m_UnitDisp );
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA2,m_UnitDepth);
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA3,m_UnitTs   );
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA4,m_UnitTsd  );
		DDX_Control(pDX, IDC_UNIT_LOAD_PARA5,m_UnitTsdn );

		DDX_Control(pDX, IDC_EDIT_LOAD_PARA1,m_EditDisp );
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA2,m_EditDepth);
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA3,m_EditTs   );
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA4,m_EditTsd  );
		DDX_Control(pDX, IDC_EDIT_LOAD_PARA5,m_EditTsdn );

		DDX_Control(pDX, IDC_EDIT_XDIR,    m_EditXDir   );
		DDX_Control(pDX, IDC_EDIT_ORIGIN,  m_EditOrigin );

		DDX_Control(pDX, IDC_GR_UNIT_DISTANCE1, m_DistUnit1);
		DDX_Control(pDX, IDC_GR_UNIT_DISTANCE2, m_DistUnit2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ResponseDispLoadMode2Dlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(ResponseDispLoadMode2Dlg)
	//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDC_TM_EXECUTE, &ResponseDispLoadMode2Dlg::OnBnClickedTmExecute)
		ON_BN_CLICKED(IDC_TM_CLOSE, &ResponseDispLoadMode2Dlg::OnBnClickedTmClose)
		ON_BN_CLICKED(IDC_TM_BF_BTN_LC, &ResponseDispLoadMode2Dlg::OnBnClickedTmBfBtnLc)
		ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, &ResponseDispLoadMode2Dlg::OnBnClickedTmDefineLdgrBtn)
		ON_CBN_SELCHANGE(IDC_COMBO_LOAD_TYPE, &ResponseDispLoadMode2Dlg::OnCbnSelchangeComboLoadType)
		ON_BN_CLICKED(IDC_RADIO_RELAT_DISP, &ResponseDispLoadMode2Dlg::OnBnClickedRadioRelatDisp)
		ON_BN_CLICKED(IDC_RADIO_ABS_DISP, &ResponseDispLoadMode2Dlg::OnBnClickedRadioAbsDisp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode2Dlg message handlers

BOOL ResponseDispLoadMode2Dlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
		CDlgUtilEx::CtrlCheck(this, IDC_RADIO_ADD, TRUE);
		CDlgUtilEx::CtrlCheck(this, IDC_RADIO_ABS_DISP, FALSE);
		CDlgUtilEx::CtrlCheck(this, IDC_RADIO_RELAT_DISP, TRUE);
		CDlgUtilEx::CtrlCheck(this, IDC_CHECK_DISP_DIR_X, TRUE);

		m_LoadCase.AddStaticIncludeType(_T("E"), FALSE);
		m_LoadCase.AddStaticIncludeType(_T("USER"), FALSE);
		m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);    
		CComboBox *pLoadType = (CComboBox *)GetDlgItem(IDC_COMBO_LOAD_TYPE);
		pLoadType->ResetContent();
		pLoadType->AddString(_LS(IDS_TM_Z_EQDISP_LOAD));
		pLoadType->AddString(_LS(IDS_TM_H_EQDISP_LOAD));    
		pLoadType->SetCurSel(1);
		OnCbnSelchangeComboLoadType();
		//
		m_UnitDepth.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_UnitDepth.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_UnitTs   .SetUnitType(D_UNITSYS_BASE_TIME);
		m_UnitTsd  .SetUnitType(D_UNITSYS_BASE_VELOCITY);
		m_UnitTsdn .SetUnitType(D_UNITSYS_BASE_VELOCITY);

		m_EditDisp .SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_EditDepth.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_EditTs   .SetUnitType(D_UNITSYS_BASE_TIME);
		m_EditTsd  .SetUnitType(D_UNITSYS_BASE_VELOCITY);
		m_EditTsdn .SetUnitType(D_UNITSYS_BASE_VELOCITY);
		//
		m_EditOrigin.SetModeToUse(MOUSEEDIT_USE_UCS_3POINT_ORI);
		m_EditOrigin.SetAttAll();
		m_EditXDir.SetModeToUse(MOUSEEDIT_USE_UCS_3POINT_XDIR);  
		m_EditXDir.SetAttAll();
		m_DistUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_DistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





void ResponseDispLoadMode2Dlg::OnBnClickedTmClose()
{
		// TODO: 在此添加控件通知处理程序代码
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}




void ResponseDispLoadMode2Dlg::OnBnClickedTmBfBtnLc()
{
		// TODO: 在此添加控件通知处理程序代码
		AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}


void ResponseDispLoadMode2Dlg::OnBnClickedTmDefineLdgrBtn()
{
		// TODO: 在此添加控件通知处理程序代码
		CreateOrActivateDlg(m_pDoc, CCMLoadGroupDlg::IDD);	
}
int ResponseDispLoadMode2Dlg::GetLoadType() const
{
	CComboBox *pLoadType = (CComboBox *)GetDlgItem(IDC_COMBO_LOAD_TYPE);
	return pLoadType->GetCurSel();
}

void ResponseDispLoadMode2Dlg::OnCbnSelchangeComboLoadType()
{
		BOOL bEnable = GetLoadType() == 0 ;//&& CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_ABS_DISP);
		GetDlgItem(IDC_STATIC_LOAD_PARA3)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_LOAD_PARA3)->EnableWindow(bEnable);
		GetDlgItem(IDC_UNIT_LOAD_PARA3)->EnableWindow(bEnable);
		GetDlgItem(IDC_STATIC_LOAD_PARA4)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_LOAD_PARA4)->EnableWindow(bEnable);
		GetDlgItem(IDC_UNIT_LOAD_PARA4)->EnableWindow(bEnable);
		GetDlgItem(IDC_STATIC_LOAD_PARA5)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_LOAD_PARA5)->EnableWindow(bEnable);
		GetDlgItem(IDC_UNIT_LOAD_PARA5)->EnableWindow(bEnable);
		GetDlgItem(IDC_WG_GR_STATIC1    )->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_ORIGIN      )->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_XDIR        )->EnableWindow(bEnable);
		GetDlgItem(IDC_WG_GR_STATIC2    )->EnableWindow(bEnable);
		GetDlgItem(IDC_GR_UNIT_DISTANCE1)->EnableWindow(bEnable);
		GetDlgItem(IDC_GR_UNIT_DISTANCE2)->EnableWindow(bEnable);
}

void ResponseDispLoadMode2Dlg::OnBnClickedRadioRelatDisp()
{
}


void ResponseDispLoadMode2Dlg::OnBnClickedRadioAbsDisp()
{
}


double ResponseDispLoadMode2Dlg::GetGroudZ()
{
		T_BLDC_D data;
		if(!m_pDoc->m_pAttrCtrl->GetBldc(data))
				data.Initialize();
		if(data.bUseBaseLevel)
				return data.dBaseLevel;
		return 0.0;
}

BOOL ResponseDispLoadMode2Dlg:: GetPara(ResponseDispLoad2CalPara &para)
{
		memset(&para, 0, sizeof(ResponseDispLoad2CalPara));
		//
		para.m_nOption = 0;
		if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_MOD))
				para.m_nOption = 1;
		else  if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_DEL))
				para.m_nOption = 2;
		//
		para.m_dGroudZ = GetGroudZ();
		UINT nType;
		if(!m_LoadCase.GetSelectedLoad(nType, para.m_StldK))
				return FALSE;
		if(!m_wndLdgrCobx.GetSelectedLdgr(para.m_LdGpK))
				para.m_LdGpK = 0;
		if(para.m_nOption == 2)
				return TRUE;
		para.m_nLoadType = GetLoadType();
		para.m_dUmax  =  m_EditDisp.GetEditValue();
		para.m_dDepth =  m_EditDepth.GetEditValue();
		para.m_dTs    =  m_EditTs.GetEditValue();
		para.m_dTsd   =  m_EditTsd.GetEditValue();
		para.m_dTsbd  =  m_EditTsdn.GetEditValue();
		para.m_bCalAbsDisp = CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_ABS_DISP);
		para.m_nDispDir = 0;
		if(CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_DISP_DIR_Y))
				para.m_nDispDir = 1;
		else  if(CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_DISP_DIR_Z))
				para.m_nDispDir = 2;
		//    
		if(!m_EditOrigin.GetCoordData(&para.m_PtOrg.x, &para.m_PtOrg.y, &para.m_PtOrg.z))
				para.m_PtOrg.Set(0,0,0);
		if(!m_EditXDir  .GetCoordData(&para.m_PtXDir.x, &para.m_PtXDir.y, &para.m_PtXDir.z))
				para.m_PtXDir.Set(1,0,0);
		//
		BOOL bCheck345 = GetLoadType() == 0;
		if(fabs(para.m_dUmax) < 1e-6 || fabs(para.m_dDepth) < 1e-6)
				return FALSE;
		if(bCheck345)
		{
				if(fabs(para.m_dTs) < 1e-6 || fabs(para.m_dTsd) < 1e-6 || fabs(para.m_dTsbd) < 1e-6)
						return FALSE;
		}
		return TRUE;
}

double ResponseDispLoadMode2Dlg::Cal_L(const ResponseDispLoad2CalPara &para )
{
		double dL1 = para.m_dTs * para.m_dTsd;
		double dL2 = para.m_dTs * para.m_dTsbd;
		double dL  = 2.0*dL1*dL2/(dL1+dL2);
		return dL;
}
double ResponseDispLoadMode2Dlg::CalUmaxZ(const ResponseDispLoad2CalPara &para, double dCalZ)
{
		double d50M = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 50.);
		double dDepth = para.m_dGroudZ - dCalZ;
		double dUmaxZ = para.m_dUmax;
		if(dDepth > d50M-1e-6)
				dUmaxZ = para.m_dUmax/2.0;
		else if(dDepth > 1e-6)
				dUmaxZ = para.m_dUmax/2.0 + (d50M-dDepth)/d50M*para.m_dUmax/2.0;
		return dUmaxZ;
}
 double ResponseDispLoadMode2Dlg::CalAbsUmax(const ResponseDispLoad2CalPara &para, const T_NODE_D &nodeCal, double dCalZ)
 {     
		 double dAbsUmax = 0;
		 if(para.m_nLoadType == 0) //线性
		 {
				 double dUMaxZ = CalUmaxZ(para, dCalZ);
				 //
				 double dL = Cal_L(para);
				 double dPtOrg[3] = {para.m_PtOrg.x, para.m_PtOrg.y, 0};
				 double dPtDir[3] = {para.m_PtXDir.x, para.m_PtXDir.y, 0};
					double dPtCal[3] = {nodeCal.x, nodeCal.y, 0};
				 double dPtPrj[3] = {0};
				 CMathFunc::project_on_line(dPtOrg, dPtDir, dPtCal, dPtPrj);
				 double dDist = CMathFunc::mathLength(dPtOrg[0], dPtOrg[1], dPtOrg[2], dPtPrj[0], dPtPrj[1], dPtPrj[2]);
				 dAbsUmax = dUMaxZ*sin(2.*M_PI*dDist/dL);
		 }
		 else
		 {
				 double dDepth = para.m_dGroudZ - dCalZ;
				 dAbsUmax = 0.5*para.m_dUmax*cos(M_PI*dDepth/para.m_dDepth/2.0);
		 }
		 return dAbsUmax;
 }

 double ResponseDispLoadMode2Dlg::GetStuctMinZBySlab()
 {
		 double dMinz = 0;
		 BOOL bSet = FALSE;
		 CArray<T_ELEM_K, T_ELEM_K> rKeyList;
		 m_pDoc->m_pAttrCtrl->GetElemKeyList(rKeyList);
		 T_ELEM_D ElemData;
		 T_NODE_D NodeD;
		 for (int i=0; i<rKeyList.GetCount(); i++)
		 {
				 if(!m_pDoc->m_pAttrCtrl->GetElem(rKeyList[i], ElemData))
						 continue;
				 if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
						 continue;
				 for (int k=0; k<D_ELEM_MAXNOD; k++)
				 {
						 if(ElemData.elnod[k] == 0)
								 break;
						 if(!m_pDoc->m_pAttrCtrl->GetNode(ElemData.elnod[k], NodeD))
								 break;
						 if(!bSet)
						 {
								 dMinz = NodeD.z;
								 bSet = TRUE;
						 }
						 else             
								 dMinz = min(dMinz, NodeD.z);
						 
				 }
		 }
			return dMinz;
 }
BOOL   ResponseDispLoadMode2Dlg::AddData(T_KEY_LIST &lstNodes, const ResponseDispLoad2CalPara &para)
{
		T_SDSP_D data;
		data.Initialize();
		data.GroupKey = para.m_LdGpK;
		data.LoadCaseKey = para.m_StldK;
		data.Flag[para.m_nDispDir] = '1';
		double dMinZ = 0;
		if(!para.m_bCalAbsDisp)
			 dMinZ = GetStuctMinZBySlab();
		CArray<T_NODE_K, T_NODE_K> rKey;
		CArray<T_SDSP_D, T_SDSP_D&> rData;
#ifdef _MGEN_CH
		//Modify by xuezc 2021/9/29 by wangjing
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSdsp();
		T_SDSP_K sdspK;
		T_SDSP_D sdspD;
		T_NODE_D NodeD;
		double dCHMinDisp = 1e7;
		double dCHMinZ = 0;
		while (pos)
		{
			m_pDoc->m_pAttrCtrl->GetNextSdsp(pos, sdspK, sdspD);
			m_pDoc->m_pAttrCtrl->GetNode(sdspK.key.entity, NodeD);
			double dDistp1 = CalAbsUmax(para, NodeD, NodeD.z);
			if (NodeD.z < dCHMinZ + 1e-7)
			{
				if (fabs(NodeD.z - dCHMinZ) < 1e-7)
					dCHMinDisp = min(dCHMinDisp, dDistp1);
				else
					dCHMinDisp = dDistp1;
				dCHMinZ = NodeD.z;
			}
		}
		if (!para.m_bCalAbsDisp)
		{
			for (int i = 0; i < lstNodes.GetCount(); i++)
			{
				T_NODE_D NodeD;
				if (!m_pDoc->m_pAttrCtrl->GetNode(lstNodes[i], NodeD))
					continue;
				double dDistp1 = CalAbsUmax(para, NodeD, NodeD.z);
				if (NodeD.z < dCHMinZ + 1e-7)
				{
					if (fabs(NodeD.z - dCHMinZ) < 1e-7)
						dCHMinDisp = min(dCHMinDisp, dDistp1);
					else
						dCHMinDisp = dDistp1;
					dCHMinZ = NodeD.z;
				}
			}
		}
#endif
		for(int i=0; i<lstNodes.GetCount(); i++)
		{
			T_NODE_D NodeD;
			if (!m_pDoc->m_pAttrCtrl->GetNode(lstNodes[i], NodeD))
				continue;
			//
			if (NodeD.z > para.m_dGroudZ)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Warning___NODEISONGROUND_), lstNodes[i]);
				continue;
			}
			//绝对位移
			if (para.m_bCalAbsDisp)
			{
				data.Displacement[para.m_nDispDir] = CalAbsUmax(para, NodeD, NodeD.z);
			}
			else //相对位移
			{
				double dDistp1 = CalAbsUmax(para, NodeD, NodeD.z);
				NodeD.z = dMinZ;
				double dDistp2 = CalAbsUmax(para, NodeD, dMinZ);
#ifdef _MGEN_CH
				data.Displacement[para.m_nDispDir] = dDistp1 - dCHMinDisp;
#else
				data.Displacement[para.m_nDispDir] = dDistp1 - dDistp2;
#endif 
			}
			rKey.Add(lstNodes[i]);
			rData.Add(data);
		}
		return m_pDoc->m_pDataCtrl->AddSdsp(rKey, rData, para.m_nOption == 1, FALSE);
}
void ResponseDispLoadMode2Dlg::OnBnClickedTmExecute()
{
		// TODO: 在此添加控件通知处理程序代码
		ResponseDispLoad2CalPara calPara;
		if(!GetPara(calPara))
		{
				GSaveHistoryNF(_LS(IDS_CMD_TDNT_ITEM_Parameter_Error__));
				return;
		}
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		T_KEY_LIST lstNode;
		pIGM->GetSelectedNodeKeyList(lstNode);
		if(lstNode.GetCount()<1)
		{
				GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
				return;
		}
		BOOL bSuccess = FALSE;
		if(calPara.m_nOption == 2)
		{
				bSuccess = m_pDoc->m_pDataCtrl->DelSdsp(lstNode, calPara.m_StldK, calPara.m_LdGpK);	
		} 
		//
		else if(calPara.m_nOption == 0 || calPara.m_nOption == 1)
		{
				bSuccess = AddData(lstNode, calPara);
		}
	 
		if (bSuccess) 
				m_pDoc->m_pViewCtrl->UnselectAll(NULL);
 }