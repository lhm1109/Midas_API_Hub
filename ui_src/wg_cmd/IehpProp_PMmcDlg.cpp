// IehpPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

// * 혹시 BiLinear 가 사용되면 아래부분 풀어서 사용
// #include "IehpProp_PMmcDlg.h"
// #include "IehpProp_PivotPicDlg.h"
// #include "IehpPropDeformation.h"
// 
// #include "..\wg_base\wg_base_DlgUtil.h"
// #include "..\wg_base\wg_base_StrParser.h"
// #include "..\wg_base\wg_base_CompFunc.h"
// 
// #include "..\wg_db\wg_db_EditData.h"
// 
// #include "..\wg_db\wg_db_UnitCtrl.h"
// #include "..\wg_db\wg_db_DbToolPmcv.h"
// 
// #include "..\wg_base\wg_base_ServiceProvider.h"
// #include "..\wg_rps\Rps_ServiceDef.h"
// 
// 
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
// 
// /////////////////////////////////////////////////////////////////////////////
// // CIehpProp_PMmcDlg dialog
// 
// 
// CIehpProp_PMmcDlg::CIehpProp_PMmcDlg(CWnd* pParent /*=NULL*/)
// 	: CDialogMove(CIehpProp_PMmcDlg::IDD, pParent)
// {
// 	m_Data.Initialize();
// 	m_nDof = 0; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz  
// 	m_PMMultiData.Initialize();
// 
// 	m_aOK.RemoveAll();
// 	m_aOK.Add(IDOK);
// 	m_aOK.Add(IDCANCEL);
// 	m_aOK.FreeExtra();
// 
// 	m_aTypeRdo.RemoveAll();
// 	m_aTypeRdo.Add(IDC_IEHP_PMMC_TYPE_SYMM_RDO);
// 	m_aTypeRdo.Add(IDC_IEHP_PMMC_TYPE_ASYM_RDO);
// 	m_aTypeRdo.FreeExtra();
// 
// 	m_aAxialCtrl.RemoveAll();
// 	m_aAxialCtrl.Add(IDC_IEHP_PMMC_AXIAL_FORCE_CMB);
// 	m_aAxialCtrl.Add(IDC_IEHP_PMMC_AXIAL_FORCE_STC);
// 	m_aAxialCtrl.Add(IDC_IEHP_PMMC_AXIAL_FORCE_BTN);
// 	m_aAxialCtrl.Add(IDC_IEHP_PMMC_AXIAL_FORCE_UNT);
// 	m_aAxialCtrl.FreeExtra();
// 
// 	m_aBetaCtrl.RemoveAll();
// 	m_aBetaCtrl.Add(IDC_IEHP_PMMC__HARDENING_BETA_STC);
// 	m_aBetaCtrl.Add(IDC_IEHP_PMMC__HARDENING_BETA_EDT);
// 	m_aBetaCtrl.FreeExtra();
// 
// 	m_aHardeningRdo.RemoveAll();
// 	m_aHardeningRdo.Add(IDC_IEHP_PMMC__HARDENING_ISO_RDO);
// 	m_aHardeningRdo.Add(IDC_IEHP_PMMC__HARDENING_KIN_RDO);
// 	m_aHardeningRdo.Add(IDC_IEHP_PMMC__HARDENING_MIX_RDO);
// 	m_aHardeningRdo.FreeExtra();
// 
// 	m_Data.Initialize();
// 	for(int i=0; i<7; i++)
// 	{
// 		m_Data.AllProp.PROP[i].KINEMA.Initialize();
// 		m_Data.AllProp.PROP[i].ORIGIN.Initialize();
// 		m_Data.AllProp.PROP[i].PEAK.Initialize();
// 		m_Data.AllProp.PROP[i].CLOUGH.Initialize();
// 		m_Data.AllProp.PROP[i].DEGRAD.Initialize();
// 		m_Data.AllProp.PROP[i].TAKEDA.Initialize();
// 		m_Data.AllProp.PROP[i].YLDSUR.Initialize();
// 		m_Data.AllProp.PROP[i].NORBIL.Initialize();
// 		m_Data.AllProp.PROP[i].ELABIL.Initialize();
// 		m_Data.AllProp.PROP[i].ELATRI.Initialize();
// 		m_Data.AllProp.PROP[i].ELATET.Initialize();
// 		m_Data.AllProp.PROP[i].MULTLIN.Initialize();
// 	}
// 
// 	m_nSymmetry = 1;
// 	m_nComboBoxIndex = 0;
// 	m_nDeformDefineType = 0;
// 	m_dZero = 0.0;
// }
// 
// CIehpProp_PMmcDlg::~CIehpProp_PMmcDlg()
// {
// 
// }
// void CIehpProp_PMmcDlg::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialogMove::DoDataExchange(pDX);
// 	//{{AFX_DATA_MAP(CIehpProp_PMmcDlg)
// 
// 	//DDX_Control(pDX, IDC_IEHP_PMMC_MULTI_CURVE_CHK, m_chkMultiCurve);
// 	DDX_Control(pDX, IDC_IEHP_PMMC_AXIAL_FORCE_CMB, m_cmbAxialForce);
// 	DDX_Control(pDX, IDC_IEHP_PMMC__HARDENING_BETA_EDT, m_edtBeta);
// 	DDX_Control(pDX, IDC_IEHP_PMMC_STIFF_EDT,       m_edtStiff);
// 	DDX_Control(pDX, IDC_IEHP_PMMC_STIFF_UNT,       m_untStiff);
// 	DDX_Control(pDX, IDC_IEHP_PMMC_AXIAL_FORCE_UNT, m_untAxial);
// 	DDX_Radio(pDX, IDC_IEHP_PMMC_TYPE_SYMM_RDO,     m_nSymmetry);
// 
// 	//}}AFX_DATA_MAP
// 
// }
// 
// 
// BEGIN_MESSAGE_MAP(CIehpProp_PMmcDlg, CDialogMove)
// 	//{{AFX_MSG_MAP(CIehpProp_PMmcDlg)
// 	ON_WM_CTLCOLOR ()
// 	ON_BN_CLICKED(IDC_IEHP_PMMC_TYPE_SYMM_RDO, OnSelectTypeRdo)
// 	ON_BN_CLICKED(IDC_IEHP_PMMC_TYPE_ASYM_RDO, OnSelectTypeRdo)
// 
// 	ON_BN_CLICKED(IDC_IEHP_PMMC__HARDENING_ISO_RDO, OnSelectHardeningRdo)
// 	ON_BN_CLICKED(IDC_IEHP_PMMC__HARDENING_KIN_RDO, OnSelectHardeningRdo)
// 	ON_BN_CLICKED(IDC_IEHP_PMMC__HARDENING_MIX_RDO, OnSelectHardeningRdo)
// 
// 
// 	//ON_BN_CLICKED(IDC_IEHP_PMMC_MULTI_CURVE_CHK, OnMultiCurveChk)
// 	ON_BN_CLICKED(IDC_IEHP_PMMC_AXIAL_FORCE_BTN, OnAxialForceBtn)
// 	ON_BN_CLICKED(IDC_DEFORMATION_INDEX,		     OnDeformationIndex)
// 	
// 	ON_CBN_SELCHANGE(IDC_IEHP_PMMC_AXIAL_FORCE_CMB,OnSelChangeAxialCombo)
//  
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()
// 
// /////////////////////////////////////////////////////////////////////////////
// // CIehpProp_PMmcDlg Data & Window handlers
// 
// void CIehpProp_PMmcDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_PMMULT data, int nFg1, int nFg2)
// {
// 	m_Data = iehpD;
// 
// 	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
// 	int nD = m_nDof;
// 	if(m_nDof>5) nD = m_nDof-3;
// 
// 	m_Data.AllProp.PMMULT[nD] = data;
// 	m_PMMultiData = data;  
// }
// 
// 
// /////////////////////////////////////////////////////////////////////////////////////////
// void CIehpProp_PMmcDlg::Data2Dlg()
// {
// 	int nD = m_nDof;
// 	if(m_nDof>5) nD = m_nDof-3;
// 
// 	m_edtStiff.SetEditUnit(m_PMMultiData.aAxial[m_nComboBoxIndex].dInitStiffPlus); 
// 	m_nSymmetry = m_PMMultiData.nSymmetryType;
// 
// 	int nSelect=m_PMMultiData.nStnHdType;
// 	CDlgUtil::CtrlRadioSetCheck(this,m_aHardeningRdo, nSelect);  // 0=Isotropic, 1=Kinematic  2=Mixed
// 	m_edtBeta.SetEditUnit(m_PMMultiData.dBeta);
// 	
// 	//m_chkMultiCurve.SetCheck(m_PMMultiData.bMultiCurve);
// 	ResetAxialCombo(); 
// 
// 	// 그리드 보여주기
// 	m_wndGrid.Setdata(&m_PMMultiData, m_nSymmetry, m_nComboBoxIndex);
// 
// 	// 그리드의 force를 컨트롤에 보여주기
// 	// 그래프 그리기
// 	OnDataChanged();
// 
// 	UpdateData(FALSE);
// }
// 
// BOOL CIehpProp_PMmcDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
// {
// 	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
// 	return FALSE;
// }
// 
// #define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
// BOOL CIehpProp_PMmcDlg::CheckData(BOOL bMsg/*=TRUE*/)
// {
// 	UpdateData(TRUE);
// 	T_IEHP_PMMULT PMMultiDTemp = m_PMMultiData;
// 
// 	if(PMMultiDTemp.nSymmetryType < 0 || PMMultiDTemp.nSymmetryType > 1) return CheckError(_LS(IDS_DB_SYMMETRY_TYPE), bMsg);
// 
// 	if(PMMultiDTemp.nStnHdType < 0 || PMMultiDTemp.nStnHdType > 2) return CheckError(_LS(IDS_DB_PMMC_STRAIN_HARDENIN_TYPE), bMsg);
// 	if(PMMultiDTemp.dBeta < 0.0    || PMMultiDTemp.dBeta > 1.0)    return CheckError(_LS(IDS_DB_PMMC_STRAIN_HARDENIN_TYPE), bMsg);
// 
// 	if(PMMultiDTemp.nDeformDefineType < 0 || PMMultiDTemp.nDeformDefineType > 1)  return CheckError(_LS(IDS_DB_DEFORM_CAPACITY_DEFINE_TYPE), bMsg);
// 	for(int j = 0; j < 5; j++)
// 	{
// 		if(PMMultiDTemp.dDeformCapacity[0][j] < 0) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY), bMsg);
// 		if(PMMultiDTemp.dDeformCapacity[1][j] < 0) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY), bMsg);
// 		if(j > 0)
// 		{
// 			if(PMMultiDTemp.dDeformCapacity[0][j] <= PMMultiDTemp.dDeformCapacity[0][j-1]) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY_COM), bMsg);
// 			if(PMMultiDTemp.dDeformCapacity[1][j] <= PMMultiDTemp.dDeformCapacity[1][j-1]) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY_COM), bMsg);
// 		}
// 	}        
// 
// 	if(PMMultiDTemp.aAxial.GetSize()<2) return CheckError(_LS(IDS_DB_MULTI_CURVE_NUM_PMMC), bMsg);
// 
// 	BOOL bMultiCurve =  TRUE;// m_chkMultiCurve.GetCheck();
// 	if(!bMultiCurve)
// 	{
// 		PMMultiDTemp.aAxial.RemoveAll();
// 		PMMultiDTemp.aAxial.Add(m_PMMultiData.aAxial[m_nComboBoxIndex]);
// 	}
// 
// 	int nCountAx = PMMultiDTemp.aAxial.GetSize();
// 
// 	for (int nAx=0; nAx<nCountAx; nAx++)
// 	{
// 		BOOL bExistZero = FALSE;
// 		BOOL bNonExistSmallDisp = FALSE;
// 		BOOL bNonExistSmallForce = FALSE;
// 		BOOL bNonExistSmallForceFinal = FALSE;
// 		BOOL bExistDiffSign = FALSE;
// 
// 		int nNumForcPlus=0, nNumForcMinus=0, nNumDispPlus=0, nNumDispMinus=0;
// 		double dDisp=0.0, dForce=0.0, dPreDisp=0.0, dPreForce=0.0, dMulti=0.0;
// 
// 		BOOL bCheckSameForc = TRUE;
// 		BOOL bMinusSameForcFound=FALSE, dPlusSameForcFound=FALSE;
// 		double dMinusSameForc=0.0, dPlusSameForc=0.0;
// 
// 
// 		// nSize 만큰 반복
// 		int nCount = 5;
// 		T_IEHP_PMMULT_AXIAL Axial = PMMultiDTemp.aAxial[nAx];
// 
// 		for (int i=0; i<nCount; i++)
// 		{
// 			dDisp  = Axial.dDispNotUse[i];
// 			dForce = Axial.dForcNotUse[i];
// 
// 			int nDispCase  = CCompFunc::CompRealTol(dDisp , 0.0, m_dZero);
// 			int nForceCase = CCompFunc::CompRealTol(dForce, 0.0, m_dZero);
// 
// 			if( nDispCase == 1 )
// 				nNumDispPlus++;
// 			else if( nDispCase == -1 )
// 				nNumDispMinus++;
// 
// 			if(nDispCase == 0 && nForceCase == 0)
// 			{
// 				bExistZero = TRUE;
// 			}
// 			else if(nForceCase ==  1)
// 			{	// 양수
// 				nNumForcPlus++;
// 				if( bCheckSameForc )
// 				{
// 					if( !dPlusSameForcFound )
// 					{
// 						if( CCompFunc::CompRealTol(dForce, dPlusSameForc, m_dZero) == 0 )
// 							dPlusSameForcFound = TRUE;
// 						dPlusSameForc = dForce;
// 					}
// 					else
// 					{	// 동일한 값이 발견 된 이후에 다른 값이 들어오는 것을 허용하지 않음
// 						if( CCompFunc::CompRealTol(dForce, dPlusSameForc, m_dZero) != 0 )
// 							return CheckError(_LS(IDS_DB_MULTI_LINEAR_FINAL_FORCE_ERROR), bMsg);
// 					}
// 				}
// 			}
// 			else if(nForceCase == -1)
// 			{	// 음수
// 				nNumForcMinus++;
// 				if( bCheckSameForc && !bMinusSameForcFound )
// 				{
// 					if( CCompFunc::CompRealTol(dForce, dMinusSameForc, m_dZero) == 0 )
// 					{
// 						bMinusSameForcFound = TRUE;
// 						if( i > 1 ) // 음수 구간 중 첫 동일한 값이 발견 된 지점이 0와 1의 관게가 아니라면
// 							return CheckError(_LS(IDS_DB_MULTI_LINEAR_FINAL_FORCE_ERROR), bMsg);
// 					}
// 					else
// 						dMinusSameForc = dForce;
// 				}
// 			}
// 
// 			dMulti = dDisp*dForce;
// 			if(CCompFunc::CompRealTol(dMulti, 0.0, m_dZero) == -1)
// 			{
// 				bExistDiffSign = TRUE;
// 			}
// 
// 			if(i > 0)
// 			{
// 				dPreDisp  = Axial.dDispNotUse[i-1];
// 				dPreForce = Axial.dForcNotUse[i-1];
// 				if(CCompFunc::CompRealTol(dDisp, dPreDisp, m_dZero) != 1)
// 				{
// 					bNonExistSmallDisp = TRUE;
// 				}
// 
// 				if(CCompFunc::CompRealTol(dForce, dPreForce, m_dZero) != 1)
// 				{
// 					if(i!=nCount-1 && i!=1)   bNonExistSmallForce      = TRUE;
// 					//else                      bNonExistSmallForceFinal = TRUE; // 마지막 force가 이전보다 작아지는 경우. 체크 해제
// 				}
// 			}
// 
// 			if(Axial.dInitStiffPlus<=0.0) return CheckError(_LS(IDS_DB_MULTI_LINEAR_Initial_Stiffness_PMMC), bMsg);
// 			Axial.dInitStiffMinus = Axial.dInitStiffPlus; ASSERT(0); // 여기 dInitStiffMinus 관련 처리 필요 함
// 			if(Axial.dInitStiffMinus<=0.0) return CheckError(_LS(IDS_DB_MULTI_LINEAR_Initial_Stiffness_PMMC), bMsg);
// 
// 		} // nCount
// 
// 		if(nNumForcPlus != nNumForcMinus)
// 		{
// 			return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_COUNT_PMMC), bMsg);
// 		}
// 
// 		if(!bExistZero)              return CheckError(_LS(IDS_DB_MULTI_LINEAR_ZERO_VALUE), bMsg);
// 		if(bNonExistSmallDisp)       return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_VALUE), bMsg);
// 		if(bNonExistSmallForce)      return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_FORCE), bMsg); 
// 		if(bNonExistSmallForceFinal) return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_FORCE_FINAL), bMsg); 
// 		if(bExistDiffSign)           return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_SIGN), bMsg);
// 
// 	} // nAx
// 
// 	m_PMMultiData = PMMultiDTemp;
// 
// 	return TRUE;
// #undef CheckError
// }
// 
// BOOL CIehpProp_PMmcDlg::Dlg2Data()
// {
// 	if(!CheckData()) return FALSE;
// 	UpdateData(TRUE);
// 
// 	int nD = m_nDof;
// 	if(m_nDof>5) nD = m_nDof-3;
// 
// 	m_PMMultiData.nSymmetryType     = m_nSymmetry;
// 
// 	int nOption=0;
// 	CDlgUtil::CtrlRadioGetCheck(this,m_aHardeningRdo, nOption);
// 
// 	m_PMMultiData.nStnHdType        = nOption;
// 	m_PMMultiData.dBeta             = m_edtBeta.GetEditValue();
// 	//m_PMMultiData.bMultiCurve       = m_chkMultiCurve.GetCheck();
// 	m_PMMultiData.nDeformDefineType = m_nDeformDefineType;
// 	DWORD nCode;
// 	CDlgUtil::CobxGetItemDataByCurSel(m_cmbAxialForce, nCode);    
// 	m_nComboBoxIndex = (int)nCode;
// 
// 	return TRUE;
// }
// void CIehpProp_PMmcDlg::SetUnitAndTitle()
// {  
// 	int nD = m_nDof;
// 	if(m_nDof>5) nD = m_nDof-3;
// 
// 	m_edtBeta.SetUnitType(0);
// 	m_edtStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROPMulti.dInitStiff); 
// 	m_untStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROPMulti.dInitStiff);
// 	m_untAxial.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROPMulti.dAxialForc);
// 
// 	// Set Title
// 	CString str; str.Empty();
// 	this->GetWindowText(str);
// 	this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_PM_MULTI_CURVE));             // P-M Multi-Curve Type
// 
// }
// 
// BOOL CIehpProp_PMmcDlg::OnInitDialog() 
// {
// 	CDialogMove::OnInitDialog();
// 
// 	InitGrid();
// 	InitGraph();
// 	SetUnitAndTitle();
// 
// 	Data2Dlg();
// 
// 	m_edtStiff.SetReadOnly(TRUE);
// 	CtrlEnableDisable();
// 	//m_wndGrid.MakeItemEx();
// 
// 	return TRUE;  
// }
// 
// void CIehpProp_PMmcDlg::OnOK() 
// {
// 	if(!Dlg2Data()) return;
// 	CDialogMove::OnOK();
// }
// 
// void CIehpProp_PMmcDlg::OnDataChanged()
// {
// 	UpdateGraph();
// 	UpdateInitStiff();
// }
// 
// void CIehpProp_PMmcDlg::OnSelectTypeRdo()
// { 
// 	UpdateData(TRUE);
// 	// 그리드 보여주기
// 	m_wndGrid.Setdata(&m_PMMultiData, m_nSymmetry, m_nComboBoxIndex);
// 
// 	// 그리드의 force를 컨트롤에 보여주기
// 	// 그래프 그리기
// 	OnDataChanged();
// }
// 
// void CIehpProp_PMmcDlg::OnSelectHardeningRdo()
// { 
// 	int nOption=0;
// 	CDlgUtil::CtrlRadioGetCheck(this,m_aHardeningRdo, nOption);
// 
// 	BOOL bShow = (nOption == 2)? TRUE : FALSE; 
// 	CDlgUtil::CtrlEnableDisable(this, m_aBetaCtrl, bShow);
// }
// 
// 
// // void CIehpProp_PMmcDlg::OnMultiCurveChk() 
// // {
// //   UpdateData(TRUE);
// //   CtrlEnableDisable();
// // }
// 
// void CIehpProp_PMmcDlg::OnAxialForceBtn() 
// {
// 	T_AGEN_D AgenD;
// 	AgenD.nDlgID = D_RPS_DO_MODAL_PO_DEFINE_AXIAL;
// 
// 	T_IEHP_PMMULT PmmultiBak;
// 	PmmultiBak.aAxial.Copy(m_PMMultiData.aAxial);
// 	AgenD.pData  = (void*)&PmmultiBak;
// 
// 	T_IEHP_PMMULT_AXIAL Axial;
// 
// 	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
// 	if(pHeadOffice->ReqService(_ULS(rps), D_RPS_DO_MODAL, (void*)&AgenD))
// 	{
// 		if(PmmultiBak.aAxial.GetSize()==0) {ASSERT(0); return;}
// 
// 		m_PMMultiData.aAxial.RemoveAll();
// 		for(int i=0; i<PmmultiBak.aAxial.GetSize(); i++)
// 		{
// 			Axial = PmmultiBak.aAxial[i];
// 			m_PMMultiData.aAxial.Add(Axial);
// 		}
// 	}
// 
// 	ResetAxialCombo();
// 
// 	OnSelChangeAxialCombo(); 
// }
// 
// void CIehpProp_PMmcDlg::ResetAxialCombo() 
// {
// 	m_cmbAxialForce.ResetContent();
// 	for(int i=0; i<m_PMMultiData.aAxial.GetSize(); i++)
// 	{
// 		CString strAxial;
// 		strAxial.Format(_T("%.3f"), m_PMMultiData.aAxial[i].dAxialForc);
// 		CDlgUtil::CobxAddItem(m_cmbAxialForce, strAxial, i);
// 	}
// 	m_cmbAxialForce.SetCurSel(0);
// }
// 
// void CIehpProp_PMmcDlg::OnDeformationIndex()
// {	
// 	T_IEHP_ALL_PROP AllProp; 
// 	AllProp.Initialize();
// 
// 	int nD = m_nDof;
// 	if(m_nDof>5) nD = m_nDof-3;
// 
// 	for(int i = 0; i < 2; i++)
// 	{
// 		for(int j = 0; j < 5; j++)
// 		{ 
// 			AllProp.PROP[nD].AXIAL.dDeformCapacity[i][j] = m_PMMultiData.dDeformCapacity[i][j];
// 		}
// 	}
// 	int nModelType = D_IEHP_PMMC; 
// 	CIehpPropDeformation DeformationDlg(m_nSymmetry, nModelType, m_nDeformDefineType, m_nDof, AllProp.PROP);
// 	if(DeformationDlg.DoModal() == IDOK)
// 	{
// 		m_nDeformDefineType = DeformationDlg.GetDeformDefineType();
// 		m_PMMultiData.nDeformDefineType = m_nDeformDefineType;
// 		for(int i = 0; i < 2; i++)
// 		{ 
// 			for(int j = 0; j < 5; j++)
// 			{         
// 				m_PMMultiData.dDeformCapacity[i][j] = AllProp.PROP[nD].AXIAL.dDeformCapacity[i][j];
// 			}
// 		}
// 	}
// }
// 
// void CIehpProp_PMmcDlg::OnSelChangeAxialCombo() 
// {
// 	DWORD nCode;
// 	CDlgUtil::CobxGetItemDataByCurSel(m_cmbAxialForce, nCode);    
// 	m_nComboBoxIndex = (int)nCode;
// 
// 	// 그리드 보여주기
// 	m_wndGrid.Setdata(&m_PMMultiData, m_nSymmetry, m_nComboBoxIndex);
// 
// 	// 그리드의 force를 컨트롤에 보여주기
// 	// 그래프 그리기
// 	OnDataChanged();
// }
// 
// void CIehpProp_PMmcDlg::CtrlEnableDisable()
// {
// 	BOOL bMultiCurve = TRUE; // m_chkMultiCurve.GetCheck();
// 	CDlgUtil::CtrlEnableDisable(this, m_aAxialCtrl, bMultiCurve);
// 
// 	OnSelectHardeningRdo();
// 
// }
// 
// void CIehpProp_PMmcDlg::InitGrid()
// {
// 	m_wndGrid.SubclassDlgItem(IDC_IEHP_PMMC_MLIN_TABLE, this);
// 
// 	T_UNIT_SYSTEM UnitSystem;
// 	m_wndGrid.Initialize(&m_PMMultiData, UnitSystem, m_nSymmetry, m_nDof, m_nComboBoxIndex);
//  
// 	// always show vertical scrollbar
// 	//m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
// }
// 
// void CIehpProp_PMmcDlg::InitGraph()
// {
// 	CWnd* pGraph=GetDlgItem(IDC_PICTURE);
// 	ASSERT(pGraph);
// 
// 	CRect rect;
// 	pGraph->GetClientRect(&rect);
// 	m_Draw.Create(pGraph, rect, NULL);
// 	m_Draw.FitToParent(rect);
// }
// 
// void CIehpProp_PMmcDlg::UpdateInitStiff()
// {
// 	int nCount = m_PMMultiData.aAxial.GetSize();
// 	 
// 	double dDist  = m_PMMultiData.aAxial[m_nComboBoxIndex].dDispNotUse[3];
// 	double dForc  = m_PMMultiData.aAxial[m_nComboBoxIndex].dForcNotUse[3];
// 	double dStiff = (dDist==0.0)? 0.0 : dForc/dDist;
// 
// 	m_PMMultiData.aAxial[m_nComboBoxIndex].dInitStiffPlus = dStiff;
// 	m_edtStiff.SetEditUnit(dStiff);
// }
// void CIehpProp_PMmcDlg::UpdateGraph()
// {
// 	// 그리드의 값에 Sclae Factor를 곱하여 최종값을 계산
// 	int nD = m_nDof;
// 	if(m_nDof>5) nD = m_nDof-3;
// 	T_IEHP_DT aDataRescaled;
// 	aDataRescaled.RemoveAll();
// 
// 	T_IEHP_BASE Data;
// 	// 1
// 	Data.dblDisp = m_PMMultiData.aAxial[m_nComboBoxIndex].dDispNotUse[0];
// 	Data.dblForce = 0.0;
// 	aDataRescaled.Add(Data);
// 
// 	// 2~6
// 	for(int i=0; i<5; i++)
// 	{  
// 		Data.dblDisp  = m_PMMultiData.aAxial[m_nComboBoxIndex].dDispNotUse[i];
// 		Data.dblForce = m_PMMultiData.aAxial[m_nComboBoxIndex].dForcNotUse[i];
// 		aDataRescaled.Add(Data);
// 	}
// 	// 7
// 	Data.dblDisp = m_PMMultiData.aAxial[m_nComboBoxIndex].dDispNotUse[i-1];
// 	Data.dblForce = 0.0;
// 	aDataRescaled.Add(Data);
// 
// 	m_Draw.SetDefaultData(20, aDataRescaled, TRUE);
// 	m_Draw.DrawMultiLinearGraph();
// }
// 
// BOOL CIehpProp_PMmcDlg::DestroyWindow()
// {
// 	return CDialog::DestroyWindow();
// }
// 
