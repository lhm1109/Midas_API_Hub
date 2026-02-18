// PSCrailway.cpp: implementation of the CPSCrailway class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "PSCrailway.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\TendonInfo.h"
#include "..\wg_db\TendonGrup.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\..\dgnengine\src\CVL_DgnCalc_CH\DgnCalc_CH_RC_Struct.h"
#include "CH_DGN_INTERFACE_UNICODE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSCrailway::CPSCrailway()
{
	m_elemlist.RemoveAll();
	m_iTotLineNum = 0;
	m_Beam_PSC_KList.RemoveAll();
	m_Beam_cross_bend_KList.RemoveAll();
	m_Beam_incline_shear_KList.RemoveAll();
	m_Beam_cross_fracture_KList.RemoveAll();
	m_Beam_incline_fracture_KList.RemoveAll();
	m_Beam_incline_tp_KList.RemoveAll();
	m_Beam_con_c_SS_KList.RemoveAll();
	m_Beam_con_t_SS_KList.RemoveAll();
	m_Beam_tendon_SS_KList.RemoveAll();
	m_Beam_delta_tendon_KList.RemoveAll();
	m_Beam_shear_KList.RemoveAll();
	m_Beam_crack_KList.RemoveAll();
	m_Beam_tendon_CS_KList.RemoveAll();
	m_Beam_con_CS_KList.RemoveAll();

	m_Column_PSC_KList.RemoveAll();
	m_Column_compressive_KList.RemoveAll();
	m_Column_tension_KList.RemoveAll();

	m_Column_cross_fracture_KList.RemoveAll();
	m_Column_con_c_SS_KList.RemoveAll();
	m_Column_con_t_SS_KList.RemoveAll();
	m_Column_tendon_SS_KList.RemoveAll();
	m_Column_shear_KList.RemoveAll();
	m_Column_tendon_CS_KList.RemoveAll();
	m_Column_con_CS_KList.RemoveAll();
	m_Column_delta_tendon_KList.RemoveAll();

	m_dZero = 1.0E-07;
	m_iCastType = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_hDllCalc = NULL;
	ASSERT(m_pDoc);
}

CPSCrailway::~CPSCrailway()
{

}

BOOL CPSCrailway::PSCrailway_CH(BOOL isBeam)
{
	if (CDBDoc::GetDocPoint()->IsLimitedTrialVersion())
	{
		//AfxMessageBox(_LS(IDS_TRIAL_NO_LICENSE));
		CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
		return FALSE;
	}
	int iStageNum = m_pDoc->m_pAttrCtrl->GetCountStag();
	T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
	int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex() + 1;
	if (iStageNum > 0)
		m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
	if (!MakeData(isBeam))
	{
		if (iStageNum > 0)
			m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
		return FALSE;
	}

	CFileException FileException;
	CString strPathName = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strFilePathName, outFilePathName, ReportPathNamme;
	if (isBeam) {
		strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".jpo");
		outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".mrpt");
	}
	else {
		strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T("Column.jpo");
		outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T("Column.mrpt");
	}
	Set_FileName(strFilePathName);
	TRY
	{
		if (!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
		if (!file.Open(outFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
		if (!Write_ImportData(isBeam))
		{
			m_cfile.Close();
			file.Close();
			CFileStatus status;
			if (CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
			if (iStageNum > 0)
				m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
			return FALSE;
		}
		m_cfile.Close();
		file.Close();
	}
		CATCH(CFileException, e)
	{
		e->ReportError();
		m_cfile.Abort();
		file.Abort();
		CFileStatus status;
		return FALSE;
	}
	END_CATCH
		return TRUE;
}


CString CPSCrailway::Get_DllFileName(int iCode)
{
	BOOL bDebug = FALSE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

	if (iCode == TB_10002_3_05)
	{
		strDllName = bDebug ? _T("CVL_DgnCalc_CHD.dgne") : _T("CVL_DgnCalc_CH.dgne");
	}
	else
		ASSERT(0);

	return strDllName;
}


BOOL CPSCrailway::Write_ImportData(BOOL isBeam)
{
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	if (PscdD.iDgnCode != TB_10002_3_05) {
		ASSERT(0);
		return TRUE;
	}
	m_iCastType = PscdD.iCastType;

	CString strCalcDll = Get_DllFileName(TB_10002_3_05);


	{// AFX_MANAGE_STATE(AfxGetModuleState())篮 _T("{"),_T("}") 郴何俊 沥狼
		// LoadLibrary()俊辑 府家胶 眉牢阑 噶啊哆府绰 版快啊 乐绢辑
		// 捞甫 汗盔窍扁 困秦 AfxGetModuleState() 窃荐甫 荤侩窃.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if (m_hDllCalc == NULL)
		{
			if ((m_hDllCalc = LoadLibrary(strCalcDll)) == NULL)
			{
				CString strMsg = _T("Can't Find ") + strCalcDll;
				AfxMessageBox(strMsg);
			}
		}
	}

	for (int i = 0; i < 14; i++) {//beam design  
		m_beamcheckitem[i] = PscdD.bBeamPrintCH[i];
	}
	for (int i = 0; i < 10; i++) {//beam design  
		m_Columncheckitem[i] = PscdD.bColumnPrintCH[i];
	}
	m_iDimType = PscdD.iDimType; //0:2D,1:2D+torsion,2:3D;

	// Set Code Unit.
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	m_DataCtrl.Set_UnitIndex(CodeUnit);
	// m_DataCtrl.m_DataCtrl.Set_UnitIndex(CodeUnit); 
	m_DataCtrl.m_DataCtrl.Get_CurrUnitIndex(CodeUnit); // KyungSik

	//for beam
	if (isBeam) {
		if (m_beamcheckitem[0] == TRUE) PSCdegree(TRUE);            //梁预应力度验算 
		if (m_beamcheckitem[1] == TRUE) Concrete_CS(TRUE);          //传力锚固阶段－混凝土法向应力验算
		if (m_beamcheckitem[2] == TRUE) Tendon_CS(TRUE);            //传力锚固阶段－预应力钢筋锚下控制应力验算
		if (m_beamcheckitem[3] == TRUE) Cross_fracture(PscdD.iBrgType, TRUE);   //运营阶段－正截面抗裂验算 (混凝土拉应力验算被合并到此)
		if (m_beamcheckitem[4] == TRUE) beam_incline_fracture(PscdD.iBrgType); //运营阶段－斜截面抗裂验算 (混凝土主拉应力被合并)
		if (m_beamcheckitem[5] == TRUE) Con_c_SS(TRUE);         //运营阶段－混凝土压应力验算
		if (m_beamcheckitem[8] == TRUE) Tendon_SS(TRUE);        //运营阶段－预应力钢筋应力验算 
		if (m_beamcheckitem[9] == TRUE) Delta_tendon(PscdD.iBrgType, TRUE);     //运营阶段－预应力钢筋应力幅验算 
		if (m_beamcheckitem[10] == TRUE) Shear(TRUE);           //运营阶段－混凝土剪应力验算 
		if (m_beamcheckitem[11] == TRUE) beam_crack();           //运营阶段－裂缝宽度验算  
		if (m_beamcheckitem[12] == TRUE) beam_crossbend();        //梁正截面抗弯验算
		if (m_beamcheckitem[13] == TRUE) beam_inclineshear();     //梁斜截面抗剪验算
	}
	else {//for column
		if (m_Columncheckitem[0] == TRUE) PSCdegree(FALSE);           //柱预应力度验算 
		if (m_Columncheckitem[1] == TRUE) Concrete_CS(FALSE);         //传力锚固阶段－混凝土法向应力验算
		if (m_Columncheckitem[2] == TRUE) Tendon_CS(FALSE);      //传力锚固阶段－预应力钢筋锚下控制应力验算 
		if (m_Columncheckitem[3] == TRUE) Cross_fracture(PscdD.iBrgType, FALSE); //运营阶段－PSC柱正截面抗裂验算 
		if (m_Columncheckitem[4] == TRUE) Con_c_SS(FALSE);       //运营阶段－混凝土压应力验算
		if (m_Columncheckitem[6] == TRUE) Tendon_SS(FALSE);      //运营阶段－预应力钢筋应力验算
		if (m_Columncheckitem[7] == TRUE) Shear(FALSE);          //运营阶段－混凝土剪应力验算
		if (m_Columncheckitem[8] == TRUE) column_compressive();    //柱正截面抗压强度验算
		if (m_Columncheckitem[9] == TRUE) column_tension();        //柱正截面抗拉强度验算
//		Delta_tendon(FALSE);     //运营阶段－预应力钢筋应力幅验算  add new
	}
	return TRUE;
}


BOOL CPSCrailway::MakeData(BOOL isBeam)
{
	m_DataCtrl.Initialize(); //m_pDataCtrl->Initialize();
	if (!m_DataCtrl.m_DataCtrl.Cal_TdgrDataForDesignAll())	return FALSE;

	_DGN_TDGR_CRC TdgrCrc;

	UINT DgnTdgrK = 0;
	POSITION Pos = m_DataCtrl.m_DataCtrl.m_amTdgrK.GetStartPosition();
	while (Pos)
	{
		TdgrCrc.Initialize();
		m_DataCtrl.m_DataCtrl.m_amTdgrK.GetNextAssoc(Pos, DgnTdgrK, TdgrCrc);
		m_DgnTdgrMap.SetAt(DgnTdgrK, TdgrCrc.OrgTdgrK);
		m_DgnTdnaMap.SetAt(DgnTdgrK, TdgrCrc.TdnaK);
	}
	// Get ElemList for Design.
	if (!m_DataCtrl.Get_CrcElemListForDgn(TRUE, m_elemlist, isBeam))  return FALSE;
	// Get LcomData for Design.
	if (!m_DataCtrl.Get_LcomDataMap(m_DgnLcomKMap))	return FALSE;
	// Get Forces for Design.
	if (!m_DataCtrl.Get_ForcDataMap(TRUE, m_ComfDMap))	return FALSE;
	//////////////////////////////////////////////////////////////////////////
	BOOL bTd = FALSE;
	CArray<T_TDNA_K, T_TDNA_K> arTdnaK;
	CTendonGrup TendonGrup;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
	for (int i = 0; i < arTdnaK.GetSize(); i++)//tendon
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		CArray<T_ELEM_K, T_ELEM_K> raElemK;
		TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
		for (int k = 0; k < raElemK.GetSize(); k++)//elemk
		{
			int ElemK = raElemK.GetAt(k);
			for (int j = 0; j < m_elemlist.GetSize(); j++) {
				if (ElemK == m_elemlist.GetAt(j)) {
					bTd = TRUE;
					return TRUE;
				}
			}
		}
	}
	if (!bTd) return FALSE;

	return TRUE;
}


//  My1－My中的活载弯矩值     //  My2－My中的恒载弯矩值 
void CPSCrailway::get_beam_M(int ElemK, int iPosi, double* dMy, UINT* iMy, double* dMy1, double* dMy2, BOOL bRemoveGS)
{
	T_RAIL_BEAM_FORCE beamForce;
	m_DataCtrl.Get_RailBeamForce(ElemK, iPosi, beamForce, TRUE, bRemoveGS);
	for (int k = 0; k < 2; k++)
	{
		dMy[k] = beamForce.dMy[k];
		iMy[k] = beamForce.iMy[k];
		dMy1[k] = beamForce.dMy1[k];
		dMy2[k] = beamForce.dMy2[k];
		isaddforce[k] = beamForce.isAddForce[k]; //isaddforce该荷载是否为主力加附加力组合
	}
}

//  My1－My中的活载弯矩值     //  My2－My中的恒载弯矩值 
void CPSCrailway::get_beam_bend(int ElemK, int iPosi, double* dMy, UINT* iMy, double* dMy1, double* dMy2)
{
	T_RAIL_BEAM_FORCE beamForce;
	m_DataCtrl.Get_RailBeamForce(ElemK, iPosi, beamForce, FALSE);//not include CS(tendon primary)
	for (int k = 0; k < 2; k++)
	{
		dMy[k] = beamForce.dMy[k];
		iMy[k] = beamForce.iMy[k];
		dMy1[k] = beamForce.dMy1[k];
		dMy2[k] = beamForce.dMy2[k];
		isaddforce[k] = beamForce.isAddForce[k]; //isaddforce该荷载是否为主力加附加力组合
	}
}

void CPSCrailway::get_beam_V(int ElemK, int iPosi, double* dVz, UINT* iVz, double* dMy)
{
	T_RAIL_BEAM_FORCE beamForce;
	m_DataCtrl.Get_RailBeamForce(ElemK, iPosi, beamForce, FALSE);//not include CS(tendon primary)
	for (int k = 0; k < 2; k++)
	{
		dVz[k] = beamForce.dVz[k + 2];
		iVz[k] = beamForce.iVz[k + 2];
		dMy[k] = beamForce.dMy[k + 2];
		isaddforce[k] = beamForce.isAddForce[k + 2]; //isaddforce该荷载是否为主力加附加力组合
	}
}

void CPSCrailway::get_coloumn_MN(int ElemK, int iPosi, double* dMy, UINT* iMy, double* dNx, UINT* iNx)
{
	T_RAIL_COLUMN_FORCE columnForce;
	m_DataCtrl.Get_RailColumnForce(ElemK, iPosi, columnForce, FALSE);
	for (int i = 0; i < 4; i++)    //0--Fxmax 1--Fxmin 2--Mymax 3--Mymin
	{
		dNx[i] = columnForce.dNx[i];
		iNx[i] = columnForce.iNx[i];
		dMy[i] = columnForce.dMy[i];
		iMy[i] = columnForce.iMy[i];
		isaddforce[i] = columnForce.isAddForce[i];//该荷载是否为主力加附加力组合
	}
}

void CPSCrailway::Isaddforce(int t)
{
	if (isaddforce[t] == FALSE) { m_dK = 2;   m_dKf = 1.2; m_dWfk = 0.1; }
	else { m_dK = 1.8; m_dKf = 1.2; m_dWfk = 0.15; }
	//add by maxiao 2009-11-17
	if (m_iCastType == 1)
	{
		m_dK = m_dK * 1.1;
		m_dKf = m_dKf * 1.1;
	}
}

//Psc degree check. 
void CPSCrailway::PSCdegree(BOOL isBeam)
{
	BOOL bPrintTitle = TRUE;
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_PSCdegree"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_PSC_Degree_rail_W);
	sizeRes = sizeof(PREDEGREE);

	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		UINT elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		//get force
		_DGN_FORC_CRC aForce[12];
		if (!m_DataCtrl.Get_ForceFordgn(elemK, m_iDimType, aForce)) continue;

		int iCount = 0;
		for (int j = 0; j < 2; j++)
		{
			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK, j, isBeam, section)) continue;

			//get Np Npey Npez; 
			double dNp = 0.0, dNpey = 0.0, dNpez = 0.0;
			if (!m_DataCtrl.Get_PscNpe(elemK, j, dNp, dNpey, dNpez)) continue;

			T_PSC_Degree_rail_W DegreeD;//Input data
			DegreeD.Initialize();
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			DegreeD.bIsBeam = isBeam;
			DegreeD.nElemK = elemK;
			DegreeD.nPart = j;
			DegreeD.dNp = dNp;
			DegreeD.dNpey = dNpey;
			DegreeD.dNpez = dNpez;
			DegreeD.PscSect = WA_Convert::Convert_A2W(section);
			for (int jj = 0; jj < 12; jj++)
				DegreeD.aForce[jj] = WA_Convert::Convert_A2W(aForce[jj]);
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			PREDEGREE preDegreeD;//Output data
			preDegreeD.Initialize();
			preDegreeD.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j, 0);//check or not..
			if (!preDegreeD.bCHK) continue;
			//
			T_DETAIL_STR DetailStr;
			DetailStr.Initialize();
			//!/
			BOOL bVersionOK = FALSE;
			//==================================================================================================
			BOOL bCheckFunc = (*pDgnCalc)(&DegreeD, &preDegreeD, &DetailStr, sizeIn, sizeRes, bVersionOK);
			if (!bVersionOK) continue;
			//==================================================================================================

			//print or not print to report-data
			BOOL bPrinti = FALSE;
			bPrinti = m_DataCtrl.Get_PSCPrintOption(elemK, j, 3);//3:PSC
			if (bPrintTitle && bPrinti)
			{
				if (isBeam) file.WriteString(_T("**BEGIN PSC梁－预应力度验算: [TB 10002.3--2005]\n"));
				else  file.WriteString(_T("**BEGIN PSC柱－预应力度验算: [TB 10002.3--2005]\n"));
				bPrintTitle = FALSE;
			}
			if (bPrinti)
			{
				for (int p = 0; p < DetailStr.arString.GetSize(); p++)
				{
					file.WriteString(DetailStr.CW2A(p));
				}
			}
			if (!bCheckFunc) continue;//2008.04.27
			if (isBeam) 	Write_Command(_T("BPSC"));//验算名称
			else  	      Write_Command(_T("CPSC"));//验算名称
			Write_Int(elemK);//单元号
			Write_Int(j);//截面位置
			Write_Int(preDegreeD.Lcomk);//组合名称
			Write_Int(preDegreeD.itype);//类型   
			Write_Double(preDegreeD.dTL);
			Write_Double(preDegreeD.dTR);
			Write_Double(preDegreeD.dBR);
			Write_Double(preDegreeD.dBL);
			Write_Double(preDegreeD.dT);
			Write_Double(preDegreeD.dB);
			Write_Double(preDegreeD.dLamda);
			Write_Double(preDegreeD.dALW);

			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0)
		{
			if (isBeam)	m_Beam_PSC_KList.Add(elemK);
			else m_Column_PSC_KList.Add(elemK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}


void CPSCrailway::beam_crossbend()
{
	try
	{
		//==================================================================================================
		// Explicit Linkage for DLL : 
		typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
		PDGNCALC *pDgnCalc;
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_crossbend"));
		//==================================================================================================

		size_t sizeIn, sizeRes;
		sizeIn = sizeof(T_Beam_crossbend_rail_W);
		sizeRes = sizeof(BEND);

		double dMy[2] = { 0,0 }, dMy1[2] = { 0,0 }, dMy2[2] = { 0,0 };//验算过程参数
		UINT iMy[2] = { 0,0 };
		//double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];
		T_Tendon_Stress_D TndnStressD;
		double dfc = .0, dfct = .0, dfs = .0, df_s = .0, dfp = .0, df_p = .0, dfpk = .0, dEc = .0, dEs = .0, dEp = .0;
		BOOL bPrintTitle = TRUE;
		for (int i = 0; i < m_elemlist.GetSize(); i++)
		{
			T_BEAM_BEND_CH T_bend; T_bend.Initialize();

			//Check Calculating Item.
			int iElemK = m_elemlist.GetAt(i);
			if (!m_DataCtrl.Is_CalcPscItem(iElemK, 0))  continue;
			int iCount = 0;
			for (int j = 0; j < 4; j++)
			{
				get_beam_bend(iElemK, j / 2, dMy, iMy, dMy1, dMy2);
				Get_Tendon_Stress(iElemK, j / 2, TndnStressD);
				Get_Metl(iElemK, j / 2, dfc, dfct, dfs, df_s, dfp, df_p, dfpk, dEc, dEs, dEp);

				Isaddforce(j % 2);
				//get section
				_TB_PSC_SECT section;//get section
				if (!m_DataCtrl.Get_PscSect(iElemK, j / 2, TRUE, section)) continue;

				T_Beam_crossbend_rail_W CrossbendD; // Input Data
				CrossbendD.Initialize();
				CrossbendD.nElemK = iElemK;
				CrossbendD.nPart = j;
				CrossbendD.dK = m_dK;
				CrossbendD.PscSect = WA_Convert::Convert_A2W(section);
				CrossbendD.df_p = df_p;
				CrossbendD.df_s = df_s;
				CrossbendD.dfc = dfc;
				CrossbendD.dfp = dfp;
				CrossbendD.dfs = dfs;

				for (int ik = 0; ik < 2; ik++)
				{
					CrossbendD.dMy[ik] = dMy[ik];
					CrossbendD.iMy[ik] = iMy[ik];
					CrossbendD.dMy1[ik] = dMy1[ik]; //live load 
					CrossbendD.dMy2[ik] = dMy2[ik]; //dead load
				}
				for (int ik = 0; ik < 3; ik++)
				{
					CrossbendD.dstressp[ik] = TndnStressD.dstressp[ik];
					CrossbendD.dstress_p[ik] = TndnStressD.dstress_p[ik];
					for (int jk = 0; jk < 3; jk++)
					{
						CrossbendD.dstresslossp[ik][jk] = TndnStressD.dstresslossp[ik][jk];
						CrossbendD.dstressloss_p[ik][jk] = TndnStressD.dstressloss_p[ik][jk];
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++			
				BEND bend = T_bend.dataBase[j];  // Output Data	
				bend.bCHK = m_DataCtrl.Get_ChkPscFlag(iElemK, j / 2, 0);//check or not..
				if (!bend.bCHK) continue;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();

				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&CrossbendD, &bend, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;

				//==================================================================================================

				//report-data
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PSCPrintOption(iElemK, j / 2, j % 2);//true:print to report-data;
				if (bPrintTitle && bPrinti)
				{
					file.WriteString(_T("**BEGIN PSC梁－正截面抗弯验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

				//change unit
				bend.dMn = m_DataCtrl.Cng_MomentFromCodeToCurr(bend.dMn);
				bend.dMy = m_DataCtrl.Cng_MomentFromCodeToCurr(bend.dMy);
				Write_Command(_T("BECB"));//验算名称  
				Write_Int(iElemK);//单元号
				Write_Int(j / 2 + 1);//截面位置  1,2             
				Write_Int(j % 2 + 1);//验算位置  1,2            
				Write_Int(bend.Lcomk);//组合名称
				Write_Int(bend.itype);//类型                
				Write_Double(bend.dMy);
				Write_Double(bend.dMn);
				Write_Double(bend.dK);
				Write_Double(bend.dK_ALW);
				Write_Int(bend.bOK);
				m_iTotLineNum++;
				iCount++;
			}
			if (iCount > 0) m_Beam_cross_bend_KList.Add(iElemK);
		}
		if (!bPrintTitle) file.WriteString(_T("**END\n"));
	}
	catch (CException* e)
	{
		e->Delete();
	}

}

void CPSCrailway::beam_inclineshear()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_inclineshear"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Beam_inclineshear_rail_W);
	sizeRes = sizeof(INCLINE_SHEAR);

	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		int iElemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(iElemK, 1))  continue;
		int iCount = 0;

		T_BEAM_INCLINE_SHEAR_CH T_incline_shear;
		T_incline_shear.Initialize();

		double dVz[2], dMy[2];//验算过程参数
		UINT iVz[2];

		for (int j = 0; j < 4; j++)
		{
			get_beam_V(iElemK, j / 2, dVz, iVz, dMy);
			Isaddforce(j % 2);

			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(iElemK, j / 2, TRUE, section)) continue;
			double dfc = .0, dfct = .0, dfs = .0, df_s = .0, dfp = .0, df_p = .0, dfpk = .0, dEc = .0, dEs = .0, dEp = .0;
			Get_Metl(iElemK, j / 2, dfc, dfct, dfs, df_s, dfp, df_p, dfpk, dEc, dEs, dEp);
			Get_SubRebar(iElemK, j / 2, dfs);//此处使用的为箍筋钢筋

			if (dMy[j % 2] < -1.0E-07)
			{
				double dtemp = .0;
				dtemp = section.dA_p; section.dA_p = section.dAp; section.dAp = dtemp;
				dtemp = section.da_p; section.da_p = section.dap; section.dap = dtemp;
				dtemp = section.dA_pb; section.dA_pb = section.dAp; section.dApb = dtemp;
				dtemp = section.da_pb; section.da_pb = section.dAp; section.dapb = dtemp;
				dtemp = section.dA_pbcosa; section.dA_pbcosa = section.dAp; section.dApbcosa = dtemp;
				dtemp = section.dA_pbsina; section.dA_pbsina = section.dAp; section.dApbsina = dtemp;
				dtemp = section.dA_s; section.dA_s = section.dAp; section.dAs = dtemp;
				dtemp = section.da_s; section.da_s = section.dAp; section.das = dtemp;
			}

			T_Beam_inclineshear_rail_W InclineShearD; // Input Data
			InclineShearD.Initialize();
			InclineShearD.nElemK = iElemK;
			InclineShearD.nPart = j;
			InclineShearD.PscSect = WA_Convert::Convert_A2W(section);

			InclineShearD.dVz = dVz[j % 2];//
			InclineShearD.iVz = iVz[j % 2];
			InclineShearD.dK = m_dK;
			InclineShearD.dfs = dfs;
			InclineShearD.dfp = dfp;
			InclineShearD.dfct = dfct;
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			INCLINE_SHEAR incline_shear = T_incline_shear.dataBase[j];//out data
			incline_shear.bCHK = m_DataCtrl.Get_ChkPscFlag(iElemK, j / 2, 1);//check or not..
			if (!incline_shear.bCHK)  continue;
			//
			T_DETAIL_STR DetailStr;
			DetailStr.Initialize();
			//!/
			BOOL bVersionOK = FALSE;
			//==================================================================================================
			BOOL bCheck = (*pDgnCalc)(&InclineShearD, &incline_shear, &DetailStr, sizeIn, sizeRes, bVersionOK);
			if (!bVersionOK) continue;

			//report-data
			BOOL bPrinti = FALSE;
			bPrinti = m_DataCtrl.Get_PSCPrintOption(iElemK, j / 2, 2);//true:print to report-data;
			if (bPrinti&& bPrintTitle)
			{
				file.WriteString(_T("**BEGIN PSC梁－斜截面抗剪验算: [TB 10002.3--2005]\n"));
				bPrintTitle = FALSE;
			}
			if (bPrinti)
			{
				for (int p = 0; p < DetailStr.arString.GetSize(); p++)
				{
					file.WriteString(DetailStr.CW2A(p));
				}
			}

			//change unit
			incline_shear.dVn = m_DataCtrl.Cng_ForceFromCodeToCurr(incline_shear.dVn);
			incline_shear.dVz = m_DataCtrl.Cng_ForceFromCodeToCurr(incline_shear.dVz);
			Write_Command(_T("BEIS"));//验算名称
			Write_Int(iElemK);//单元号
			Write_Int(j / 2 + 1);//截面位置               
			Write_Int(j % 2 + 1);//验算位置               
			Write_Int(incline_shear.Lcomk);//组合名称
			Write_Int(incline_shear.itype);//类型                
			Write_Double(incline_shear.dVz);
			Write_Double(incline_shear.dVn);
			Write_Double(incline_shear.dK);
			Write_Double(incline_shear.dK_ALW);
			Write_Int(incline_shear.bOK);
			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0) m_Beam_incline_shear_KList.Add(iElemK);
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}


void CPSCrailway::Cross_fracture(int iType, BOOL isBeam) //前提条件是SafeType==0
{
	//iType == 2(not check). iType ==0 (6.3.9-1 6.3.11-1)  iType==1 (6.3.11-2)
	if (iType != 0 && iType != 1) return;
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Cross_fracture"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Cross_fracture_rail_W);
	sizeRes = sizeof(CON_CROSS_FRACTURE);

	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		UINT elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		int iCount = 0;

		//get force
		_DGN_FORC_CRC aForce[12];/* 0~5:Fx-max ~Mz-max. 6~11:Fx-min ~Mz-min _DGN_FORC_CRC:0-i \ 1-j. */
		if (!m_DataCtrl.Get_ForceFordgn(elemK, m_iDimType, aForce)) continue;



		//get dfct
		double dalpha = .0, dfc = .0, dfct = .0;
		if (!m_DataCtrl.Get_TBConMatl(elemK, dalpha, dfc, dfct)) continue;

		for (int j = 0; j < 2; j++)
		{
			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK, j, isBeam, section)) continue;

			//get Np Npey Npez; 
			double dNp = 0.0, dNpey = 0.0, dNpez = 0.0;
			if (!m_DataCtrl.Get_PscNpe(elemK, j, dNp, dNpey, dNpez)) continue;

			double afpApep[12], afp_Ap_ep[12], afsAses[12], afs_As_es[12], afpApZp[12], afp_Ap_Zp[12], afsAsZs[12], afs_As_Zs[12];
			double dfpApap, dfp_Ap_ap, dfsAsas, dfs_As_as, dfpAp, dfp_Ap, dfsAs, dfs_As;
			_DGN_STRS_CRC aTSelfStress[12];

			for (int ii = 0; ii < 12; ii++)
			{
				_DGN_FORC_CRC Force = aForce[ii];
				double dfpApep, dfp_Ap_ep, dfsAses, dfs_As_es, dfpApZp, dfp_Ap_Zp, dfsAsZs, dfs_As_Zs;
				m_DataCtrl.Get_fAezData(elemK, j, Force.dMuy[j], Force.dFxx[j], section.dh, dfpApep, dfp_Ap_ep, dfsAses, dfs_As_es, dfpApZp, dfp_Ap_Zp, dfsAsZs, dfs_As_Zs);
				afpApep[ii] = dfpApep; afp_Ap_ep[ii] = dfp_Ap_ep;
				afsAses[ii] = dfsAses; afs_As_es[ii] = dfs_As_es;
				afpApZp[ii] = dfpApZp; afp_Ap_Zp[ii] = dfp_Ap_Zp;
				afsAsZs[ii] = dfsAsZs; afs_As_Zs[ii] = dfs_As_Zs;

				if (!m_DataCtrl.Get_TSelfStressFordgn(elemK, aForce[ii].iRefNo[j], m_iDimType, isBeam, aTSelfStress[ii])) continue;
			}
			m_DataCtrl.Get_fAaData(elemK, j, section.dCzp, section.dCzm, dfpApap, dfp_Ap_ap, dfsAsas, dfs_As_as, dfpAp, dfp_Ap, dfsAs, dfs_As);

			for (int k = 0; k < 6; k++) //σc_TL,σc_TR,σc_BR,σc_BL,σc_T,σc_B 
			{
				T_Cross_fracture_rail_W CorssFractureD;
				CorssFractureD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				CorssFractureD.bIsBeam = isBeam;
				CorssFractureD.nElemK = elemK;
				CorssFractureD.nPart = j;
				CorssFractureD.dNp = dNp;
				CorssFractureD.dNpey = dNpey;
				CorssFractureD.dNpez = dNpez;
				CorssFractureD.dalpha = dalpha;
				CorssFractureD.dfc = dfc;				CorssFractureD.dfct = dfct;

				CorssFractureD.dfpApap = dfpApap;   CorssFractureD.dfp_Ap_ap = dfp_Ap_ap;
				CorssFractureD.dfsAsas = dfsAsas;   CorssFractureD.dfs_As_as = dfs_As_as;
				CorssFractureD.dfpAp = dfpAp;       CorssFractureD.dfp_Ap = dfp_Ap;
				CorssFractureD.dfsAs = dfsAs;       CorssFractureD.dfs_As = dfs_As;

				CorssFractureD.PscSect = WA_Convert::Convert_A2W(section);



				for (int ii = 0; ii < 12; ii++)
				{
					if (k == 0)
					{
						CorssFractureD.dTSelfStress[ii] = aTSelfStress[ii].dStl[j];
					}
					else if (k == 1)
					{
						CorssFractureD.dTSelfStress[ii] = aTSelfStress[ii].dStr[j];
					}
					else if (k == 2)
					{
						CorssFractureD.dTSelfStress[ii] = aTSelfStress[ii].dSbr[j];
					}
					else if (k == 3)
					{
						CorssFractureD.dTSelfStress[ii] = aTSelfStress[ii].dSbl[j];
					}
					else if (k == 4)
					{
						CorssFractureD.dTSelfStress[ii] = aTSelfStress[ii].dStop[j];
					}
					else if (k == 5)
					{
						CorssFractureD.dTSelfStress[ii] = aTSelfStress[ii].dSbot[j];
					}
					CorssFractureD.aForce[ii] = WA_Convert::Convert_A2W(aForce[ii]);
					CorssFractureD.dfpApep[ii] = afpApep[ii];   CorssFractureD.dfp_Ap_ep[ii] = afp_Ap_ep[ii];
					CorssFractureD.dfsAses[ii] = afsAses[ii];   CorssFractureD.dfs_As_es[ii] = afs_As_es[ii];
					CorssFractureD.dfpApZp[ii] = afpApZp[ii];   CorssFractureD.dfp_Ap_Zp[ii] = afp_Ap_Zp[ii];
					CorssFractureD.dfsAsZs[ii] = afsAsZs[ii];   CorssFractureD.dfs_As_Zs[ii] = afs_As_Zs[ii];


				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//
				CON_CROSS_FRACTURE_W ConCrossFrac;
				ConCrossFrac.Initialize();
				ConCrossFrac.itype = iType;
				ConCrossFrac.iCheckPos = k;
				ConCrossFrac.iElemk = elemK;
				ConCrossFrac.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j, 0);//check or not..
				if (!ConCrossFrac.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();

				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&CorssFractureD, &ConCrossFrac, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================

				//print or not print to report-data
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PSCPrintOption(elemK, j, 3);//3:PSC
				if (bPrintTitle && bPrinti)
				{
					if (isBeam) file.WriteString(_T("**BEGIN PSC梁－运营阶段正截面抗裂验算: [TB 10002.3--2005]\n"));
					else  file.WriteString(_T("**BEGIN PSC柱－运营阶段正截面抗裂验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

				if (isBeam) Write_Command(_T("BECF"));//验算名称
				else Write_Command(_T("CLCF"));//验算名称
				Write_Int(elemK);//单元号
				Write_Int(j);//截面位置    				        
				Write_Int(ConCrossFrac.itype);//验算类型      
				Write_Int(k);//验算位置
				Write_Int(ConCrossFrac.Lcomk);//组合名称
				//				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dKfSig));//Kfσ+σc 1-6
				//				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dKfSig_ALW));//[Kfσ+σc] 1-6
				//				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dSigct));
				//				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dSig_ALW));
				//根据新表格要求，更新了所有数据结构的显示方式2010-4-15
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dSig));
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dSigc));
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.drfct));
				Write_Double(ConCrossFrac.dKf);
				Write_Double(ConCrossFrac.dKf_ALW);
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dSigct));
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ConCrossFrac.dSigct_ALW));
				Write_Int(ConCrossFrac.bOK);
				m_iTotLineNum++;
				iCount++;
			}//end of for(k)
		}
		if (iCount > 0)
		{
			if (isBeam) m_Beam_cross_fracture_KList.Add(elemK);
			else m_Column_cross_fracture_KList.Add(elemK);
		}
	}
	if (!bPrintTitle)	file.WriteString(_T("**END\n"));
}

//混凝土斜截面验算 modified by maxiao...
void CPSCrailway::beam_incline_fracture(int iSafeType)	//前提条件是SafeType==0
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_incline_fracture"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Beam_incline_fracture_rail_W);
	sizeRes = sizeof(CON_INCLINE_FRACTURE);

	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		UINT elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		int iCount = 0;

		//get dfct
		double dalpha = .0, dfc = .0, dfct = .0;
		if (!m_DataCtrl.Get_TBConMatl(elemK, dalpha, dfc, dfct)) continue;

		//get stress
		T_BEAM_CON_INCLINE_FRACTURE_CH con_incline_fracture_elem;
		con_incline_fracture_elem.Initialize();
		if (!m_DataCtrl.Get_con_fracture(elemK, iSafeType, con_incline_fracture_elem)) continue;

		for (int j = 0; j < 4; j++)
		{
			//print or not print to report-data
			BOOL bPrinti = FALSE;
			bPrinti = m_DataCtrl.Get_PSCPrintOption(elemK, j / 2, 2);//2:SHEAR

			T_Beam_incline_fracture_rail_W BeamInclFracD;
			BeamInclFracD.Initialize();
			BeamInclFracD.bIsBeam = TRUE;
			BeamInclFracD.nElemK = elemK;
			BeamInclFracD.iSafeType = iSafeType;
			BeamInclFracD.nPart = j;
			BeamInclFracD.dalpha = dalpha;
			BeamInclFracD.dfc = dfc;
			BeamInclFracD.dfct = dfct;
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			CON_INCLINE_FRACTURE InclFracD;
			InclFracD = con_incline_fracture_elem.dataBase[j];
			InclFracD.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j / 2, 0);//check or not..
			BeamInclFracD.bAddForce = m_DataCtrl.m_ForceCtrl->Is_LcomForMainAdd(InclFracD.LcomK);//add				

			if (!InclFracD.bCHK)continue;
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			T_DETAIL_STR DetailStr;
			DetailStr.Initialize();
			//!/
			BOOL bVersionOK = FALSE;
			//==================================================================================================
			BOOL bCheck = (*pDgnCalc)(&BeamInclFracD, &InclFracD, &DetailStr, sizeIn, sizeRes, bVersionOK);
			if (!bVersionOK) continue;
			//==================================================================================================
			con_incline_fracture_elem.dataBase[j] = InclFracD;

			if (bPrintTitle && bPrinti)
			{
				file.WriteString(_T("**BEGIN PSC梁－运营阶段斜截面抗裂验算: [TB 10002.3--2005]\n"));
				bPrintTitle = FALSE;
			}
			if (bPrinti)
			{
				for (int p = 0; p < DetailStr.arString.GetSize(); p++)
				{
					file.WriteString(DetailStr.CW2A(p));
				}
			}

			int iPos = (j == 0 || j == 1 ? 1 : 2);
			int iTyp = (j == 0 || j == 2 ? 1 : 2);
			//change unit
			InclFracD.dSig_P1 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P1);
			InclFracD.dSig_P2 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P2);
			InclFracD.dSig_P3 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P3);
			InclFracD.dSig_P4 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P4);
			InclFracD.dSig_P5 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P5);
			InclFracD.dSig_P6 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P6);
			InclFracD.dSig_P7 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P7);
			InclFracD.dSig_P8 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P8);
			InclFracD.dSig_P9 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P9);
			InclFracD.dSig_P10 = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_P10);
			InclFracD.dSig_MAX = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_MAX);
			InclFracD.dSig_AP = m_DataCtrl.Cng_StressFromCodeToCurr(InclFracD.dSig_AP);

			Write_Command(_T("BEIF"));//验算名称
			Write_Int(m_elemlist.GetAt(i));  //单元号
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(InclFracD.LcomK);
			Write_Int(InclFracD.itype);
			Write_Double(InclFracD.dSig_P1);
			Write_Double(InclFracD.dSig_P2);
			Write_Double(InclFracD.dSig_P3);
			Write_Double(InclFracD.dSig_P4);
			Write_Double(InclFracD.dSig_P5);
			Write_Double(InclFracD.dSig_P6);
			Write_Double(InclFracD.dSig_P7);
			Write_Double(InclFracD.dSig_P8);
			Write_Double(InclFracD.dSig_P9);
			Write_Double(InclFracD.dSig_P10);
			Write_Double(InclFracD.dSig_MAX);
			Write_Double(InclFracD.dSig_AP);
			Write_Int(InclFracD.bOK);
			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0) m_Beam_incline_fracture_KList.Add(elemK);
	}
	if (!bPrintTitle)file.WriteString(_T("**END\n"));
}



BOOL CPSCrailway::get_con_tp(int ElemK, T_BEAM_CON_TP_CH& con_tp_elem)
{
	con_tp_elem.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	// Add by ZINU.('04.08.25). Check Calculating Item.
	//if(!Is_CalcPscItem(ElemK,FALSE))  return FALSE;  
	// Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
	int iDgnStatus = pDoc->GetDesignStatus();
	pDoc->SetDesignStatus(5);

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	_DGN_SHRS_CRC ShrsMax; ShrsMax.Initialize();
	_DGN_SHRS_CRC ShrsMin; ShrsMin.Initialize();
	if (!m_ForceCtrl.Get_ShrsByLcom(EPairK, ShrsMax, ShrsMin, 4, FALSE))	return FALSE;
	// Set Data.
	for (int i = 0; i < 4; i++)
	{
		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		_DGN_SHRS_CRC ShrsJtg = (i % 2 == 0 ? ShrsMax : ShrsMin);
		if (ShrsJtg.dSspAbs[i / 2] == 0.0)	continue;
		int iDgnLcomNo = ShrsJtg.iRefNo[i / 2];
		CString strOrgLcomNa = _T("");
		int iMaxMinType = 0;
		if (!m_ForceCtrl.Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		con_tp_elem.dataBase[i].bCHK = 1;//Get_ChkFlag(ElemK, i/2, FALSE);
		con_tp_elem.dataBase[i].LcomK = iDgnLcomNo;
		con_tp_elem.dataBase[i].itype = iMaxMinType;
		con_tp_elem.dataBase[i].dSig_P1 = ShrsJtg.dSsp[i / 2][0];
		con_tp_elem.dataBase[i].dSig_P2 = ShrsJtg.dSsp[i / 2][1];
		con_tp_elem.dataBase[i].dSig_P3 = ShrsJtg.dSsp[i / 2][2];
		con_tp_elem.dataBase[i].dSig_P4 = ShrsJtg.dSsp[i / 2][3];
		con_tp_elem.dataBase[i].dSig_P5 = ShrsJtg.dSsp[i / 2][4];
		con_tp_elem.dataBase[i].dSig_P6 = ShrsJtg.dSsp[i / 2][5];
		con_tp_elem.dataBase[i].dSig_P7 = ShrsJtg.dSsp[i / 2][6];
		con_tp_elem.dataBase[i].dSig_P8 = ShrsJtg.dSsp[i / 2][7];
		con_tp_elem.dataBase[i].dSig_P9 = ShrsJtg.dSsp[i / 2][8];
		con_tp_elem.dataBase[i].dSig_P10 = ShrsJtg.dSsp[i / 2][9];
		con_tp_elem.dataBase[i].dSig_MAX = ShrsJtg.dSspAbs[i / 2];
		con_tp_elem.dataBase[i].dSig_AP = 0.0;
	}
	return TRUE;
}

void CPSCrailway::Con_c_SS(BOOL isBeam)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Con_c_SS"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Con_c_SS_rail_W);
	sizeRes = sizeof(CON_SS);

	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		UINT elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		int iCount = 0;

		//get force
		_DGN_FORC_CRC aForce[12];
		if (!m_DataCtrl.Get_ForceFordgn(elemK, m_iDimType, aForce)) continue;

		//get dfc
		double dalpha = .0, dfc = .0, dfct = .0;
		if (!m_DataCtrl.Get_TBConMatl(elemK, dalpha, dfc, dfct)) continue;

		for (int j = 0; j < 2; j++)
		{
			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK, j, isBeam, section)) continue;

			//get Np Npey Npez; 
			double dNp = 0.0, dNpey = 0.0, dNpez = 0.0;
			if (!m_DataCtrl.Get_PscNpe(elemK, j, dNp, dNpey, dNpez)) continue;

			T_Con_c_SS_rail_W ConcSSD;
			ConcSSD.Initialize();
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			ConcSSD.bIsBeam = isBeam;
			ConcSSD.nElemK = elemK;
			ConcSSD.nPart = j;
			ConcSSD.dalpha = dalpha;
			ConcSSD.dfc = dfc;
			ConcSSD.dfct = dfct;
			ConcSSD.dNp = dNp;
			ConcSSD.dNpey = dNpey;
			ConcSSD.dNpez = dNpez;
			ConcSSD.PscSect = WA_Convert::Convert_A2W(section);

			_DGN_STRS_CRC aTSelfStress[12];

			for (int jj = 0; jj < 12; jj++)
			{
				ConcSSD.aForce[jj] = WA_Convert::Convert_A2W(aForce[jj]);

				if (!m_DataCtrl.Get_TSelfStressFordgn(elemK, aForce[jj].iRefNo[j], m_iDimType, isBeam, aTSelfStress[jj])) continue;

			}

			for (int k = 0; k < 6; k++) //_T("TL"),_T("TR"),_T("BR"),_T("BL"),_T("T"),_T("B")
			{
				for (int ii = 0; ii < 12; ii++)
				{
					if (k == 0)
					{
						ConcSSD.dTSelfStress[ii] = aTSelfStress[ii].dStl[j];
					}
					else if (k == 1)
					{
						ConcSSD.dTSelfStress[ii] = aTSelfStress[ii].dStr[j];
					}
					else if (k == 2)
					{
						ConcSSD.dTSelfStress[ii] = aTSelfStress[ii].dSbr[j];
					}
					else if (k == 3)
					{
						ConcSSD.dTSelfStress[ii] = aTSelfStress[ii].dSbl[j];
					}
					else if (k == 4)
					{
						ConcSSD.dTSelfStress[ii] = aTSelfStress[ii].dStop[j];
					}
					else if (k == 5)
					{
						ConcSSD.dTSelfStress[ii] = aTSelfStress[ii].dSbot[j];
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				CON_SS Con_ss;
				Con_ss.Initialize();
				Con_ss.itype = k;
				Con_ss.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j, 0);//check or not..
				if (!Con_ss.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ConcSSD, &Con_ss, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================				
				//print or not print to report-data
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PSCPrintOption(elemK, j, 3);//3:PSC
				if (bPrintTitle && bPrinti)
				{
					if (isBeam) file.WriteString(_T("**BEGIN PSC梁－运营阶段混凝土压应力验算: [TB 10002.3--2005]\n"));
					else  file.WriteString(_T("**BEGIN PSC柱－运营阶段混凝土压应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

				//output the result
				if (isBeam) Write_Command(_T("BCSS"));//验算名称
				else Write_Command(_T("CCSS"));//验算名称
				Write_Int(elemK);//单元号
				Write_Int(j);//截面位置    				        
				Write_Int(k);//验算位置
				Write_Int(Con_ss.Lcomk);//组合名称
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Con_ss.dSig));//σc 1-6
				Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Con_ss.dSig_ALW));//0.5fc 1-6

				m_iTotLineNum++;
				iCount++;
			}

		}
		if (iCount > 0)
		{
			if (isBeam) m_Beam_con_c_SS_KList.Add(elemK);
			else m_Column_con_c_SS_KList.Add(elemK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CPSCrailway::Tendon_SS(BOOL isBeam)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Tendon_SS"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Tendon_SS_rail_W);
	sizeRes = sizeof(TENDON);

	BOOL bPrintTitle = TRUE;	BOOL bPrinti = TRUE;
	CArray<T_TDNA_K, T_TDNA_K> arTdnaK;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
	for (int i = 0; i < arTdnaK.GetSize(); i++)//tendon
	{
		int iCount = 0;  BOOL btendon = TRUE;
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		int iLoadType = 0; // 0=Pre-tension, 1=Post-tension. 
		double dfpk = .0;
		T_BEAM_TENDON_CH T_Tendon;
		T_Tendon.Initialize();
		if (!m_DataCtrl.Get_TendonMatl(TdnaK, iLoadType, dfpk)) continue;

		CTendonGrup TendonGrup;
		CArray<T_ELEM_K, T_ELEM_K> raElemK;
		TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
		BOOL bcala = FALSE;
		for (int k = 0; k < raElemK.GetSize(); k++)//elemk
		{
			int ElemK = raElemK.GetAt(k);
			//
			if (!m_DataCtrl.Is_CalcPscItem(ElemK, 0))  continue;
			bcala = TRUE;
			//get force
			_DGN_FORC_CRC aForce[12];
			if (!m_DataCtrl.Get_ForceFordgn(ElemK, m_iDimType, aForce)) continue;
			//get Tp,Ts
			//			_DGN_FORC_CRC ForceTP,ForceTS;
			//			m_DataCtrl.Get_ForceOfTpTs(ElemK,m_iDimType,ForceTP,ForceTS);
			//Get CS(sum)
			_DGN_FORC_CRC ForceCSALL;
			m_DataCtrl.Get_ForceOfCSALL(ElemK, m_iDimType, ForceCSALL);

			//get dn
			double dn;
			//			if (!m_DataCtrl.Get_TBdn(ElemK,isBeam,dn)) continue;

			TENDON TendonD[2];
			TendonD[0].Initialize();
			TendonD[1].Initialize();

			for (int m = 0; m < 2; m++)
			{//section
				double dYp, dZp;
				if (!m_DataCtrl.Get_YpZp(TdnaK, ElemK, m, dYp, dZp)) continue;
				_TB_PSC_SECT section;//get section
				if (!m_DataCtrl.Get_PscSect(ElemK, m, isBeam, section)) continue;
				BOOL bStage = FALSE;
				T_STAG_K StagK = m_pDoc->m_pPostCtrl->GetStageInfo()->GetFinalStage();
				T_TENDON_LOSS data;
				double dTendonLoss[6];
				int nPart = (m == 0 ? 0 : 7);
				if (!TendonGrup.GetTendonLoss_Tdna(TdnaK, StagK, ElemK, nPart, 2, 1, data)) continue;//
				for (int il = 0; il < 6; il++)
				{
					if (il == 2 || il == 5) dTendonLoss[il] = data.dTendonLoss[il];//比值
					else dTendonLoss[il] = m_DataCtrl.Cng_StressFromCurrToCode(data.dTendonLoss[il]);
				}

				//add by maxiao
				if (!m_DataCtrl.Get_TBdn(ElemK, TdnaK, dn)) continue;
				T_TDNA_D TdnaD;
				if (!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;

				T_Tendon_SS_rail_W TendonSSD; // Input Data
				TendonSSD.Initialize();
				TendonD[m].Initialize();  // Output Data
				TendonD[m].bCHK = m_DataCtrl.Get_ChkPscFlag(ElemK, m, 0);//check or not..
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				TendonSSD.bIsBeam = isBeam;
				TendonSSD.TendonName = TdnaD.TendonName;
				TendonSSD.nElemK = ElemK;
				TendonSSD.nPart = m;
				TendonSSD.dfpk = dfpk;
				TendonSSD.dn = dn;
				TendonSSD.dYp = dYp;
				TendonSSD.dZp = dZp;
				TendonSSD.PscSect = WA_Convert::Convert_A2W(section);
				for (int jj = 0; jj < 12; jj++)
					TendonSSD.aForce[jj] = WA_Convert::Convert_A2W(aForce[jj]);
				TendonSSD.ForceCSALL = WA_Convert::Convert_A2W(ForceCSALL);
				for (int jj = 0; jj < 6; jj++)
					TendonSSD.dTendonLoss[jj] = dTendonLoss[jj];
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&TendonSSD, &TendonD[m], &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================
				//report-data
				if (bPrintTitle && bPrinti)
				{
					if (isBeam)	file.WriteString(_T("**BEGIN PSC梁－运营阶段预应力钢筋拉应力验算: [TB 10002.3--2005]\n"));
					else 	file.WriteString(_T("**BEGIN PSC柱－运营阶段预应力钢筋拉应力验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

			}//end section				
			if (TendonD[0].dSigP > T_Tendon.dataBase[0].dSigP || TendonD[1].dSigP > T_Tendon.dataBase[1].dSigP)
			{
				T_Tendon.dataBase[0] = TendonD[0];
				T_Tendon.dataBase[1] = TendonD[1];
			}
		}//end elem
		//output
		if (!bcala)  continue;
		for (int m = 0; m < 2; m++)
		{
			if (isBeam)	Write_Command(_T("BETS"));//验算名称
			else 	Write_Command(_T("CLTS"));//验算名称
			Write_Int(TdnaK);//钢束号
			Write_Int(T_Tendon.dataBase[m].iElem);//单元号
			Write_Int(m);//截面位置                 
			Write_Int(T_Tendon.dataBase[m].Lcomk);//组合名称          
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(T_Tendon.dataBase[m].dSigP));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(T_Tendon.dataBase[m].dSig_ALW));
			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0)
		{
			if (isBeam) m_Beam_tendon_SS_KList.Add(TdnaK);
			else m_Column_tendon_SS_KList.Add(TdnaK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

//预应力幅验算…
void CPSCrailway::Delta_tendon(int iType, BOOL isBeam)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Delta_tendon"));
	//==================================================================================================
	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Delta_tendon_rail_W);
	sizeRes = sizeof(TENDON);

	BOOL bPrintTitle = TRUE;	BOOL bPrinti = TRUE;
	CArray<T_TDNA_K, T_TDNA_K> arTdnaK;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

	T_BEAM_TENDON_CH T_Tendon;
	T_Tendon.Initialize();

	for (int i = 0; i < arTdnaK.GetSize(); i++)//tendon
	{
		BOOL bCHK = FALSE;
		int iCount = 0;  BOOL btendon = TRUE;
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		int iLoadType = 0; // 0=Pre-tension, 1=Post-tension. 
		double dfpk = .0;
		if (!m_DataCtrl.Get_TendonMatl(TdnaK, iLoadType, dfpk)) continue;
		for (int k = 0; k < m_elemlist.GetSize(); k++)//elemk
		{
			int ElemK = m_elemlist.GetAt(k);
			//get MV force
			_DGN_FORC_CRC aForce[12];
			if (!m_DataCtrl.Get_MVForceFordgn(ElemK, m_iDimType, aForce)) continue;

			//get Tp,Ts
			_DGN_FORC_CRC ForceTP, ForceTS;
			m_DataCtrl.Get_ForceOfTpTs(ElemK, m_iDimType, ForceTP, ForceTS);

			double dn;
			//			if (!m_DataCtrl.Get_TBdn(ElemK,isBeam,dn)) continue;
			if (!m_DataCtrl.Get_TBdn(ElemK, TdnaK, dn)) continue;
			T_TDNA_D TdnaD;
			if (!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;

			TENDON TendonD[2];
			TendonD[0].Initialize();
			TendonD[1].Initialize();

			T_Delta_tendon_rail_W DeltaTendonD; // Input Data        
			for (int m = 0; m < 2; m++)//section
			{
				DeltaTendonD.Initialize();
				double dYp, dZp;
				if (!m_DataCtrl.Get_YpZp(TdnaK, ElemK, m, dYp, dZp)) continue;

				_TB_PSC_SECT section;//get section
				if (!m_DataCtrl.Get_PscSect(ElemK, m, isBeam, section)) continue;


				double dMy[2], dMy1[2], dMy2[2];
				UINT iMy[2];
				get_beam_M(ElemK, m, dMy, iMy, dMy1, dMy2);
				//double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];
				T_Tendon_Stress_D TndnStressD;
				Get_Tendon_Stress(ElemK, m, TndnStressD);
				TendonD[m].Initialize();// Output Data
				TendonD[m].itype = iType;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				DeltaTendonD.bIsBeam = isBeam;
				DeltaTendonD.TendonName = CT2W(TdnaD.TendonName);
				DeltaTendonD.nElemK = ElemK;
				DeltaTendonD.nPart = m;
				DeltaTendonD.dfpk = dfpk;
				DeltaTendonD.dn = dn;
				DeltaTendonD.dYp = dYp;
				DeltaTendonD.dZp = dZp;
				DeltaTendonD.iLoadType = iLoadType;
				DeltaTendonD.PscSect = WA_Convert::Convert_A2W(section);
				for (int ik = 0; ik < 12; ik++)
					DeltaTendonD.aForce[ik] = WA_Convert::Convert_A2W(aForce[ik]);
				DeltaTendonD.ForceTP = WA_Convert::Convert_A2W(ForceTP);
				DeltaTendonD.ForceTS = WA_Convert::Convert_A2W(ForceTS);

				for (int ik = 0; ik < 2; ik++)
				{
					DeltaTendonD.dMy[ik] = dMy[ik];
					DeltaTendonD.dMy1[ik] = dMy1[ik];
					DeltaTendonD.dMy2[ik] = dMy2[ik];
					DeltaTendonD.iMy[ik] = iMy[ik];
				}
				for (int ik = 0; ik < 3; ik++)
				{
					DeltaTendonD.dstressp[ik] = TndnStressD.dstressp[ik];
					DeltaTendonD.dstress_p[ik] = TndnStressD.dstress_p[ik];
					for (int jk = 0; jk < 3; jk++)
					{
						DeltaTendonD.dstresslossp[ik][jk] = TndnStressD.dstresslossp[ik][jk];
						DeltaTendonD.dstressloss_p[ik][jk] = TndnStressD.dstressloss_p[ik][jk];
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK = FALSE;
				//==================================================================================================
				bCHK = (*pDgnCalc)(&DeltaTendonD, &TendonD[m], &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================
				//report-data
				if (bPrintTitle && bPrinti) {
					if (isBeam)	file.WriteString(_T("**BEGIN PSC梁－运营阶段预应力钢筋应力幅验算: [TB 10002.3--2005]\n"));
					else 	file.WriteString(_T("**BEGIN PSC柱－运营阶段预应力钢筋应力幅验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}
			}	//end section		
			if (fabs(TendonD[0].dSigP) >= T_Tendon.dataBase[0].dSigP || fabs(TendonD[1].dSigP) >= T_Tendon.dataBase[1].dSigP)
			{
				if (TendonD[0].iElem <= 0 || TendonD[0].Lcomk <= 0) continue;
				if (TendonD[1].iElem <= 0 || TendonD[1].Lcomk <= 0) continue;
				T_Tendon.dataBase[0] = TendonD[0];
				T_Tendon.dataBase[1] = TendonD[1];
			}
		}//end elem
		//output
		if (!bCHK) continue;
		for (int m = 0; m < 2; m++) {
			if (isBeam)	Write_Command(_T("BEDT"));//验算名称
			else 	Write_Command(_T("CLDT"));//验算名称
			Write_Int(TdnaK);//钢束号
			Write_Int(T_Tendon.dataBase[m].iElem);//单元号
			Write_Int(m);//截面位置                 
			Write_Int(T_Tendon.dataBase[m].Lcomk);//组合名称          
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(T_Tendon.dataBase[m].dSigP));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(T_Tendon.dataBase[m].dSig_ALW));

			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0) {
			if (isBeam) m_Beam_delta_tendon_KList.Add(TdnaK);
			else m_Column_delta_tendon_KList.Add(TdnaK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CPSCrailway::Shear(BOOL isBeam)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Shear"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Shear_rail_W);
	sizeRes = sizeof(SHEAR);

	BOOL bPrintTitle = TRUE;
	for (int k = 0; k < m_elemlist.GetSize(); k++)
	{
		UINT elemK = m_elemlist.GetAt(k);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 1))  continue;
		int iCount = 0;

		//get dfct
		double dalpha = .0, dfc = .0, dfct = .0;
		if (!m_DataCtrl.Get_TBConMatl(elemK, dalpha, dfc, dfct)) continue;

		//get stress. 5-10点
		double dSig_shear[6]; double dSig_tor[6]; double dSig_zz[6]; double dSig[6];
		for (int j = 0; j < 2; j++)
		{
			//////////////////////////////////////////////////////////////////////////
			T_STSS_D stss;  stss.Initialize();//beam
			T_STRB_D strb;  strb.Initialize();//column
			double dSig_Max = .0; 			int iLcomk = 0;
			CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
			T_LCOM_D LcomData;
			for (int il = 0; il < arLcomKey.GetSize(); il++)
			{
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey.GetAt(il), LcomData);
				m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData, 0, 0);
				if (isBeam) //梁				/***/
				{
					T_STSS_D t_stss;
					m_pDoc->m_pPostCtrl->GetStssNew(elemK, 0, 0, &t_stss, 2);//2:主应力
					for (int kj = 0; kj < 6; kj++)
					{
						if (fabs(t_stss.dblStress[4 * j][kj + 4][6] + t_stss.dblStress[4 * j][kj + 4][7] - 0.55*t_stss.dblStress[4 * j][kj + 4][5]) > fabs(dSig_Max))
						{
							dSig_Max = -(t_stss.dblStress[4 * j][kj + 4][6] + t_stss.dblStress[4 * j][kj + 4][7] - 0.55*t_stss.dblStress[4 * j][kj + 4][5]);
							iLcomk = il + 1; stss = t_stss;
						}
						dSig_zz[kj] = m_DataCtrl.Cng_StressFromCurrToCode(-stss.dblStress[4 * j][kj + 4][5]);//change
						dSig_shear[kj] = m_DataCtrl.Cng_StressFromCurrToCode(-stss.dblStress[4 * j][kj + 4][6]);
						dSig_tor[kj] = m_DataCtrl.Cng_StressFromCurrToCode(-stss.dblStress[4 * j][kj + 4][7]);
						dSig[kj] = dSig_shear[kj] + dSig_tor[kj] - 0.55*dSig_zz[kj];
					}
				}
				else       //柱				/***/
				{
					T_STRB_D t_strb;
					m_pDoc->m_pPostCtrl->GetStrbNew(elemK, 0, 0, &t_strb, FALSE);
					for (int kj = 1; kj < 3; kj++)
					{
						if (fabs(t_strb.dblStress[3 * j][kj]) >= fabs(dSig_Max))
						{
							dSig_Max = t_strb.dblStress[3 * j][kj];
							iLcomk = il + 1; strb = t_strb;
						}
					}
					dSig[0] = strb.dblStress[3 * j][1];
					dSig[1] = strb.dblStress[3 * j][2];
					for (int i = 2; i < 6; i++) dSig[i] = 0.0;
				}
			}//
			dSig_Max = m_DataCtrl.Cng_StressFromCurrToCode(dSig_Max);

			T_Shear_rail_W ShearRailD; // Input Data
			ShearRailD.Initialize();
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			ShearRailD.bIsBeam = isBeam;
			ShearRailD.nElemK = elemK;
			ShearRailD.nPart = j;
			ShearRailD.dalpha = dalpha;
			ShearRailD.dfc = dfc;
			ShearRailD.dfct = dfct;
			for (int il = 0; il < 6; il++)
			{
				ShearRailD.dSig_shear[il] = dSig_shear[il];
				ShearRailD.dSig_tor[il] = dSig_tor[il];
				ShearRailD.dSig_zz[il] = dSig_zz[il];
				ShearRailD.dSig[il] = dSig[il];
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			SHEAR ShearD;  // Output Data
			ShearD.Initialize();
			ShearD.Lcomk = iLcomk;
			ShearD.dshearc = dSig_Max;
			ShearD.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j, 1);//check or not..
			if (!ShearD.bCHK)continue;

			T_DETAIL_STR DetailStr;
			DetailStr.Initialize();
			//!/
			BOOL bVersionOK = FALSE;
			//==================================================================================================
			BOOL bCheck = (*pDgnCalc)(&ShearRailD, &ShearD, &DetailStr, sizeIn, sizeRes, bVersionOK);
			if (!bVersionOK) continue;
			//==================================================================================================

			//report-data
			BOOL bPrinti = FALSE;
			if (m_DataCtrl.Get_PSCPrintOption(elemK, j, 2)) bPrinti = TRUE;

			if (bPrintTitle && bPrinti) {
				if (isBeam)	file.WriteString(_T("**BEGIN PSC梁－运营阶段混凝土剪应力验算: [TB 10002.3--2005]\n"));
				else file.WriteString(_T("**BEGIN PSC柱－运营阶段混凝土剪应力验算: [TB 10002.3--2005]\n"));
				bPrintTitle = FALSE;
			}
			if (bPrinti)
			{
				for (int p = 0; p < DetailStr.arString.GetSize(); p++)
				{
					file.WriteString(DetailStr.CW2A(p));
				}
			}

			if (isBeam)		Write_Command(_T("BESH"));//验算名称
			else  	Write_Command(_T("CLSH"));//验算名称
			Write_Int(elemK);//单元号
			Write_Int(j);//截面位置               
			Write_Int(iLcomk);//组合名称  
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear5));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear6));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear7));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear8));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear9));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear10));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshearc));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(ShearD.dshear_ALW));

			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0) {
			if (isBeam)	m_Beam_shear_KList.Add(elemK);
			else m_Column_shear_KList.Add(elemK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CPSCrailway::beam_crack()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Beam_crack"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Beam_crack_rail_W);
	sizeRes = sizeof(PSCCRACK);


	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		int iElemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(iElemK, 0))  continue;

		int iCount = 0;
		for (int j = 0; j < 2; j++)
		{
			double dMy[2], dMy1[2], dMy2[2];
			UINT iMy[2];
			get_beam_M(iElemK, j, dMy, iMy, dMy1, dMy2, TRUE);

			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(iElemK, j, TRUE, section)) continue;

			double dfc, dfct, dfs, df_s, dfp, df_p, dfpk, dEc, dEs, dEp;
			//double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];
			T_Tendon_Stress_D TndnStressD;
			Get_Tendon_Stress(iElemK, j, TndnStressD);
			Get_Metl(iElemK, j, dfc, dfct, dfs, df_s, dfp, df_p, dfpk, dEc, dEs, dEp);

			int iLoadType = 0; // 0=Pre-tension, 1=Post-tension.
			CArray<T_TDNA_K, T_TDNA_K> arTdnaK; BOOL isElemK = FALSE;
			m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
			for (int iL = 0; iL < arTdnaK.GetSize(); iL++)//tendon
			{
				T_TDNA_K TdnaK = arTdnaK.GetAt(iL);
				int iLoadType = 0; // 0=Pre-tension, 1=Post-tension. 
				CTendonGrup TendonGrup;
				CArray<T_ELEM_K, T_ELEM_K> raElemK;
				TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
				for (int k = 0; k < raElemK.GetSize(); k++)//elemk
				{
					if (raElemK.GetAt(k) == iElemK) isElemK = TRUE;
				}
				if (isElemK) m_DataCtrl.Get_TendonMatl(TdnaK, iLoadType, dfpk);
			}
			if (!isElemK) continue;

			for (int t = 0; t < 2; t++)
			{
				Isaddforce(t);
				T_Beam_crack_rail_W BeamCrackD; // Input Data
				BeamCrackD.Initialize();
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				BeamCrackD.nElemK = iElemK;
				BeamCrackD.nPart = j;
				BeamCrackD.t = t;
				BeamCrackD.iLoadType = iLoadType;
				BeamCrackD.PscSect = WA_Convert::Convert_A2W(section);
				BeamCrackD.bIsBeam = TRUE;
				BeamCrackD.iMy = iMy[t];

				for (int jj = 0; jj < 3; jj++)
				{
					BeamCrackD.dstressp[jj] = TndnStressD.dstressp[jj];
					BeamCrackD.dstress_p[jj] = TndnStressD.dstress_p[jj];
					for (int kk = 0; kk < 3; kk++)
					{
						BeamCrackD.dstresslossp[jj][kk] = TndnStressD.dstresslossp[jj][kk];
						BeamCrackD.dstressloss_p[jj][kk] = TndnStressD.dstressloss_p[jj][kk];
					}
				}
				BeamCrackD.dfc = dfc;
				BeamCrackD.dfct = dfct;
				BeamCrackD.dfs = dfs;
				BeamCrackD.df_s = df_s;
				BeamCrackD.dfp = dfp;
				BeamCrackD.df_p = df_p;
				BeamCrackD.dfpk = dfpk;
				BeamCrackD.dEc = dEc;
				BeamCrackD.dEs = dEs;
				BeamCrackD.dEp = dEp;
				BeamCrackD.dMy = dMy[t];
				BeamCrackD.dMy1 = dMy1[t];
				BeamCrackD.dMy2 = dMy2[t];
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				PSCCRACK crack;
				crack.Initialize();
				crack.Lcomk = iMy[t];
				crack.dwfk_ALW = m_dWfk;
				crack.bCHK = m_DataCtrl.Get_ChkPscFlag(iElemK, j, 0);//check or not..
				if (!crack.bCHK) continue;
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&BeamCrackD, &crack, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================

				//report-data
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PSCPrintOption(m_elemlist.GetAt(i), j, t);//true:print to report-data;
				//Check Calculating Item.
				if (!m_DataCtrl.Is_CalcPscItem(m_elemlist.GetAt(i), 0))  continue;
				if (bPrintTitle  && bPrinti) {
					file.WriteString(_T("**BEGIN PSC梁－运营阶段混凝土裂缝验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

				//change unit
				crack.dwfk = m_DataCtrl.Cng_LengthFromCodeToCurr(crack.dwfk, 1);
				crack.dwfk_ALW = m_DataCtrl.Cng_LengthFromCodeToCurr(crack.dwfk_ALW, 1);
				Write_Command(_T("BECR"));//验算名称
				Write_Int(m_elemlist.GetAt(i));//单元号
				Write_Int(j + 1);//截面位置              
				Write_Int(t + 1);//截面位置              
				Write_Int(crack.Lcomk);//组合名称
				Write_Int(crack.itype);//类型                
				Write_Double(crack.dwfk);
				Write_Double(crack.dwfk_ALW);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if (iCount > 0) m_Beam_crack_KList.Add(m_elemlist.GetAt(i));
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}


void CPSCrailway::Tendon_CS(BOOL isBeam)
{
	BOOL bPrintTitle = TRUE;
	CArray<T_TDNA_K, T_TDNA_K> arTdnaK;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Tendon_CS"));
	//==================================================================================================
	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Tendon_CS_rail_W);
	sizeRes = sizeof(TENDON_CS);

	for (int i = 0; i < arTdnaK.GetSize(); i++)//tendon
	{
		int iCount = 0;
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		int iLoadType = 0; // 0=Pre-tension, 1=Post-tension. 
		double dfpk = .0;
		if (!m_DataCtrl.Get_TendonMatl(TdnaK, iLoadType, dfpk)) continue;

		CTendonGrup TendonGrup;
		CArray<T_ELEM_K, T_ELEM_K> raElemK; raElemK.RemoveAll();
		//modified by maxiao
		T_TDNA_D TdnaD;
		if (!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;
		//		raElemK.Add(TdnaD.aElemList.GetAt(0));
		//		raElemK.Add((TdnaD.aElemList.GetAt(TdnaD.aElemList.GetSize() -1)));
				//2010-4-13 siyang'a requirement
		for (int itd = 0; itd < TdnaD.aElemList.GetSize(); itd++)
		{
			raElemK.Add(TdnaD.aElemList.GetAt(itd));
		}
		//Max and min
		T_TENDON_CS_CH TendonOut;
		TendonOut.Initialize();
		for (int k = 0; k < raElemK.GetSize(); k++)//elemk
		{
			int ElemK = raElemK.GetAt(k);
			CArray<T_TENDON_LOSS, T_TENDON_LOSS> aTendonLoss;
			CArray<INT, INT> aStag;
			CArray<T_STAG_K, T_STAG_K> aStagK;
			CArray<INT, INT> aPart;
			m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
			if (aStagK.GetSize() == 0)	continue;
			for (int j = 0; j < aStagK.GetSize(); j++)//stage
			{
				T_TENDON_LOSS data; data.Initialize();
				for (int iPart = 0; iPart < 2; iPart++)
				{
					int nPart = (iPart == 0 ? 0 : 7);
					if (!TendonGrup.GetTendonLoss_Tdna(TdnaK, aStagK.GetAt(j), ElemK, nPart, 2, 1, data)) continue;//
					////
					for (int iL = 0; iL < 6; iL++)
					{
						if (iL == 2 || iL == 5) continue;//比值
						data.dTendonLoss[iL] = m_DataCtrl.Cng_StressFromCurrToCode(data.dTendonLoss[iL]);
					}
					aTendonLoss.Add(data);
					aStag.Add(aStagK.GetAt(j));
					aPart.Add(iPart);
				}
			}

			TENDON_CS TendonCS;
			TendonCS.Initialize();
			T_Tendon_CS_rail_W TendonCSD;
			TendonCSD.Initialize();
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			TendonCSD.bIsBeam = isBeam;
			TendonCSD.nTdnaK = TdnaK;
			TendonCSD.nElemK = ElemK;
			TendonCSD.dfpk = dfpk;
			TendonCSD.TendonName = CT2W(TdnaD.TendonName);
			TendonCSD.iLoadType = iLoadType;
			int iNum = min(min(aPart.GetSize(), aStag.GetSize()), aTendonLoss.GetSize());
			if (iNum <= 0) continue;
			for (int ii = 0; ii < iNum; ii++)
			{
				TendonCSD.aTendonLoss.Add(WA_Convert::Convert_A2W(aTendonLoss.GetAt(ii)));
				TendonCSD.aStag.Add(aStag.GetAt(ii));
				TendonCSD.aPart.Add(aPart.GetAt(ii));
			}
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			T_DETAIL_STR DetailStr;
			DetailStr.Initialize();
			//!/
			BOOL bVersionOK = FALSE;
			//==================================================================================================
			BOOL bCheck = (*pDgnCalc)(&TendonCSD, &TendonCS, &DetailStr, sizeIn, sizeRes, bVersionOK);
			if (!bVersionOK) continue;
			//==================================================================================================

			//report-data
			BOOL bPrinti = TRUE;
			if (bPrintTitle && bPrinti)
			{
				if (isBeam)	file.WriteString(_T("**BEGIN PSC梁－传力锚固阶段预应力钢筋应力验算: [TB 10002.3--2005]\n"));
				else 	file.WriteString(_T("**BEGIN PSC柱－传力锚固阶段预应力钢筋应力验算: [TB 10002.3--2005]\n"));
				bPrintTitle = FALSE;
			}
			if (bPrinti)
			{
				for (int p = 0; p < DetailStr.arString.GetSize(); p++)
				{
					file.WriteString(DetailStr.CW2A(p));
				}
			}

			//output
	//			if (isBeam)	Write_Command(_T("BTCS"));//验算名称  
	//			else 	Write_Command(_T("CLTC"));//验算名称  
	//			Write_Int(TendonCS.iTendon);//tendon number  
	//			Write_Int(TendonCS.iElem);//elem  
	//			Write_Int(TendonCS.iPos);//section  
	//			Write_Int(TendonCS.iStage);//stage  
	//			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(TendonCS.dSigP));
	//			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(TendonCS.dSig_ALW));	
			//Max and MIn
			if (k == 0)
			{
				TendonOut.dataBase[0].iTendon = TendonOut.dataBase[1].iTendon = TendonCS.iTendon;
				TendonOut.dataBase[0].iElem = TendonOut.dataBase[1].iElem = TendonCS.iElem;
				TendonOut.dataBase[0].iPos = TendonOut.dataBase[1].iPos = TendonCS.iPos;
				TendonOut.dataBase[0].iStage = TendonOut.dataBase[1].iStage = TendonCS.iStage;
				TendonOut.dataBase[0].dSigP = TendonOut.dataBase[1].dSigP = TendonCS.dSigP;
				TendonOut.dataBase[0].dSig_ALW = TendonOut.dataBase[1].dSig_ALW = TendonCS.dSig_ALW;
			}
			else
			{
				if (TendonCS.dSigP > TendonOut.dataBase[0].dSigP)//Max
				{
					TendonOut.dataBase[0].iTendon = TendonCS.iTendon;
					TendonOut.dataBase[0].iElem = TendonCS.iElem;
					TendonOut.dataBase[0].iPos = TendonCS.iPos;
					TendonOut.dataBase[0].iStage = TendonCS.iStage;
					TendonOut.dataBase[0].dSigP = TendonCS.dSigP;
					TendonOut.dataBase[0].dSig_ALW = TendonCS.dSig_ALW;
				}
				if (TendonCS.dSigP < TendonOut.dataBase[1].dSigP)//Min
				{
					TendonOut.dataBase[1].iTendon = TendonCS.iTendon;
					TendonOut.dataBase[1].iElem = TendonCS.iElem;
					TendonOut.dataBase[1].iPos = TendonCS.iPos;
					TendonOut.dataBase[1].iStage = TendonCS.iStage;
					TendonOut.dataBase[1].dSigP = TendonCS.dSigP;
					TendonOut.dataBase[1].dSig_ALW = TendonCS.dSig_ALW;
				}
			}

			iCount++;
		}
		//Max and Min
		for (int im = 0; im < 2; im++)
		{
			if (iCount == 0) continue;
			if (isBeam)	Write_Command(_T("BTCS"));//验算名称  
			else 	Write_Command(_T("CLTC"));//验算名称  
			Write_Int(TendonOut.dataBase[im].iTendon);//tendon number  
			Write_Int(TendonOut.dataBase[im].iElem);//elem  
			Write_Int(TendonOut.dataBase[im].iPos);//section  
			Write_Int(TendonOut.dataBase[im].iStage);//stage        
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(TendonOut.dataBase[im].dSigP));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(TendonOut.dataBase[im].dSig_ALW));
			Write_Int(im);//im  
			m_iTotLineNum++;
		}
		if (iCount > 0) {
			if (isBeam) m_Beam_tendon_CS_KList.Add(TdnaK);
			else m_Column_tendon_CS_KList.Add(TdnaK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CPSCrailway::Concrete_CS(BOOL isBeam)
{
	BOOL bPrintTitle = TRUE;
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Concrete_CS"));
	//==================================================================================================

	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Concrete_CS_rail_W);
	sizeRes = sizeof(CON_CS);

	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		int elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		int iCount = 0;

		//get 'CS(sum)' force of every stage. 
		CArray<_DGN_FORC_CRC, _DGN_FORC_CRC> aForceList;
		if (!m_DataCtrl.Get_ForceStageCS(elemK, m_iDimType, aForceList)) continue;

		//get mat  defult fc'=0.8fc, fct'=0.8fct.
		double dalpha = .0, dfc = .0, dfct = .0;
		if (!m_DataCtrl.Get_TBConMatl(elemK, dalpha, dfc, dfct)) continue;

		for (int j = 0; j < 4; j++) //I and  J
		{
			T_Concrete_CS_rail_W ConcCSD;
			ConcCSD.Initialize();
			//
			CON_CS Concs;
			Concs.Initialize();
			Concs.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j / 2, 0);//check or not..
			if (!Concs.bCHK)continue;

			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK, j / 2, isBeam, section)) continue;

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++      
			ConcCSD.bIsBeam = isBeam;
			ConcCSD.nElemK = elemK;
			ConcCSD.nPart = j;
			ConcCSD.dalpha = dalpha;
			ConcCSD.dfc = dfc;
			ConcCSD.dfct = dfct;
			ConcCSD.PscSect = WA_Convert::Convert_A2W(section);
			ConcCSD.aForceCS.RemoveAll();
			ConcCSD.aForceCS.SetSize(aForceList.GetCount());
			for (int kk = 0; kk < aForceList.GetCount(); kk++)
			{
				ConcCSD.aForceCS[kk] = WA_Convert::Convert_A2W(aForceList[kk]);
			}
			//      for(int jj=0; jj<aForceList.GetSize(); jj++)
			//				ConcCSD.aForceCS.Add(aForceList.GetAt(jj));
						//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			T_DETAIL_STR DetailStr;
			DetailStr.Initialize();
			//!/
			BOOL bVersionOK = FALSE;
			//==================================================================================================
			BOOL bCheck = (*pDgnCalc)(&ConcCSD, &Concs, &DetailStr, sizeIn, sizeRes, bVersionOK);
			if (!bVersionOK) continue;
			//==================================================================================================
			//report-data
			BOOL bPrinti = FALSE;
			if (m_DataCtrl.Get_PSCPrintOption(elemK, j / 2, 0) || m_DataCtrl.Get_PSCPrintOption(elemK, j / 2, 1)) bPrinti = TRUE;
			if (bPrintTitle && bPrinti)
			{
				if (isBeam) file.WriteString(_T("**BEGIN PSC梁－传力锚固阶段混凝土法向应力验算: [TB 10002.3--2005]\n"));
				else file.WriteString(_T("**BEGIN PSC柱－传力锚固阶段混凝土法向应力验算: [TB 10002.3--2005]\n"));
				bPrintTitle = FALSE;
			}
			if (bPrinti)
			{
				for (int p = 0; p < DetailStr.arString.GetSize(); p++)
				{
					file.WriteString(DetailStr.CW2A(p));
				}
			}
			if (isBeam) Write_Command(_T("BECS"));//验算名称
			else Write_Command(_T("CLCS"));
			Write_Int(elemK);//单元号
			Write_Int(j / 2);
			Write_Int(j % 2);//0-Max, 1-Min.
			Write_Int(Concs.StagK);//Stage
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_TL));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_TR));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_BR));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_BL));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_T));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_B));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_MAX));
			Write_Double(m_DataCtrl.Cng_StressFromCodeToCurr(Concs.dSig_ALW));
			m_iTotLineNum++;
			iCount++;
		}
		if (iCount > 0)
		{
			if (isBeam)	m_Beam_con_CS_KList.Add(elemK);
			else m_Column_con_CS_KList.Add(elemK);
		}
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CPSCrailway::column_compressive()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_compressive"));
	//==================================================================================================
	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Column_compressive_rail_W);
	sizeRes = sizeof(COLUMN_STRENGTH);
	//////////////////////////////////////////////////////////////////////////
	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		int elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		int iCount = 0;

		//get force 
		_DGN_FORC_CRC aForce[12];
		if (!m_DataCtrl.Get_ForceFordgn(elemK, m_iDimType, aForce, FALSE)) continue;

		//matl
		_DGN_RC_MATL_CH matl;
		if (!m_DataCtrl.Get_Matl(elemK, matl)) continue;

		double dn = .0;
		//		m_DataCtrl.Get_TBdn(elemK,FALSE,dn);

				//get fp
		double dfpk = .0;
		CArray<T_TDNA_K, T_TDNA_K> arTdnaK; BOOL isElemK = FALSE;
		m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
		for (int i = 0; i < arTdnaK.GetSize(); i++)//tendon
		{
			T_TDNA_K TdnaK = arTdnaK.GetAt(i);
			int iLoadType = 0; // 0=Pre-tension, 1=Post-tension. 
			CTendonGrup TendonGrup;
			CArray<T_ELEM_K, T_ELEM_K> raElemK;
			TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
			for (int k = 0; k < raElemK.GetSize(); k++)//elemk
			{
				if (raElemK.GetAt(k) == elemK) isElemK = TRUE;
			}
			if (isElemK) m_DataCtrl.Get_TendonMatl(TdnaK, iLoadType, dfpk);
			//add by maxiao
			if (!m_DataCtrl.Get_TBdn(elemK, TdnaK, dn)) continue;
		}
		if (!isElemK) continue;

		for (int j = 0; j < 2; j++)
		{
			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK, j, FALSE, section)) continue;

			for (int k = 0; k < 6; k++) //每个截面6组值…Fxx(Max\Min),My(Max\Min),Mz(Max\min)..
			{
				_DGN_FORC_CRC Force; int itype;
				if (k == 0) { Force = aForce[0]; itype = 0; }
				if (k == 1) { Force = aForce[6]; itype = 6; }
				if (k == 2) { Force = aForce[4]; itype = 4; }
				if (k == 3) { Force = aForce[10]; itype = 10; }
				if (k == 4) { Force = aForce[5]; itype = 5; }
				if (k == 5) { Force = aForce[11]; itype = 11; }

				double dfpApep, dfp_Ap_ep, dfsAses, dfs_As_es, dfpApZp, dfp_Ap_Zp, dfsAsZs, dfs_As_Zs;
				double dfpApap, dfp_Ap_ap, dfsAsas, dfs_As_as, dfpAp, dfp_Ap, dfsAs, dfs_As;
				m_DataCtrl.Get_fAezData(elemK, j, Force.dMuy[j], Force.dFxx[j], section.dh, dfpApep, dfp_Ap_ep, dfsAses, dfs_As_es, dfpApZp, dfp_Ap_Zp, dfsAsZs, dfs_As_Zs);
				m_DataCtrl.Get_fAaData(elemK, j, section.dCzp, section.dCzm, dfpApap, dfp_Ap_ap, dfsAsas, dfs_As_as, dfpAp, dfp_Ap, dfsAs, dfs_As);

				T_Column_compressive_rail_W ColumnCompD; // Input Data
				ColumnCompD.Initialize();

				COLUMN_STRENGTH ColStreD;  // Output Data
				ColStreD.Initialize();
				ColStreD.itype = itype;
				ColStreD.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j, 0);//check or not..
				if (!ColStreD.bCHK)continue;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ColumnCompD.bIsBeam = FALSE;
				ColumnCompD.nElemK = elemK;
				ColumnCompD.nPart = j;
				ColumnCompD.PscSect = WA_Convert::Convert_A2W(section);
				ColumnCompD.dn = dn;
				ColumnCompD.MatlCH = WA_Convert::Convert_A2W(matl);
				ColumnCompD.Force = WA_Convert::Convert_A2W(Force);
				ColumnCompD.dfpk = dfpk;

				ColumnCompD.dfpApep = dfpApep;				ColumnCompD.dfp_Ap_ep = dfp_Ap_ep;
				ColumnCompD.dfsAses = dfsAses;				ColumnCompD.dfs_As_es = dfs_As_es;
				ColumnCompD.dfpApZp = dfpApZp;				ColumnCompD.dfp_Ap_Zp = dfp_Ap_Zp;
				ColumnCompD.dfsAsZs = dfsAsZs;				ColumnCompD.dfs_As_Zs = dfs_As_Zs;

				ColumnCompD.dfpApap = dfpApap;				ColumnCompD.dfp_Ap_ap = dfp_Ap_ap;
				ColumnCompD.dfsAsas = dfsAsas;				ColumnCompD.dfs_As_as = dfs_As_as;
				ColumnCompD.dfpAp = dfpAp;			  	ColumnCompD.dfp_Ap = dfp_Ap;
				ColumnCompD.dfsAs = dfsAs;			  	ColumnCompD.dfs_As = dfs_As;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ColumnCompD, &ColStreD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================
				//print or not print to report-data
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PSCPrintOption(elemK, j, 3);//3:PSC
				if (bPrintTitle && bPrinti)
				{
					file.WriteString(_T("**BEGIN PSC柱－正截面抗压强度验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

				//输出结果
				Write_Command(_T("CLCP")); //验算名称
				Write_Int(elemK);  //单元号
				Write_Int(j);                  //截面位置
				Write_Int(k);                  //验算位置
				Write_Int(ColStreD.Lcomk);//组合名称  
				Write_Int(ColStreD.itype);      //类型             
				Write_Double(m_DataCtrl.Cng_ForceFromCodeToCurr(ColStreD.dKN));
				Write_Double(m_DataCtrl.Cng_ForceFromCodeToCurr(ColStreD.dN));
				m_iTotLineNum++;
				iCount++;
			}
		}
		if (iCount > 0) m_Column_compressive_KList.Add(elemK);
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}

void CPSCrailway::column_tension()
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Column_tension"));
	//==================================================================================================
	size_t sizeIn, sizeRes;
	sizeIn = sizeof(T_Column_tension_rail_W);
	sizeRes = sizeof(COLUMN_STRENGTH);
	//////////////////////////////////////////////////////////////////////////
	BOOL bPrintTitle = TRUE;
	for (int i = 0; i < m_elemlist.GetSize(); i++)
	{
		int elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if (!m_DataCtrl.Is_CalcPscItem(elemK, 0))  continue;
		int iCount = 0;

		//get force 
		_DGN_FORC_CRC aForce[12];
		if (!m_DataCtrl.Get_ForceFordgn(elemK, m_iDimType, aForce, FALSE)) continue;

		//matl
		_DGN_RC_MATL_CH matl;
		if (!m_DataCtrl.Get_Matl(elemK, matl)) continue;

		double dn = .0;
		//get fp
		double dfpk = .0;
		CArray<T_TDNA_K, T_TDNA_K> arTdnaK; BOOL isElemK = FALSE;
		m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
		for (int i = 0; i < arTdnaK.GetSize(); i++)//tendon
		{
			T_TDNA_K TdnaK = arTdnaK.GetAt(i);
			int iLoadType = 0; // 0=Pre-tension, 1=Post-tension. 
			CTendonGrup TendonGrup;
			CArray<T_ELEM_K, T_ELEM_K> raElemK;
			TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
			for (int k = 0; k < raElemK.GetSize(); k++)//elemk
			{
				if (raElemK.GetAt(k) == elemK) isElemK = TRUE;
			}
			if (isElemK) m_DataCtrl.Get_TendonMatl(TdnaK, iLoadType, dfpk);
			//add by maxiao
			if (!m_DataCtrl.Get_TBdn(elemK, TdnaK, dn)) continue;
		}
		if (!isElemK) continue;

		for (int j = 0; j < 2; j++)
		{
			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK, j, FALSE, section)) continue;

			//			m_DataCtrl.Get_TBdn(elemK,FALSE,dn);

						//get section info.
			double dfpApap = .0, dfsAsas = .0, dfpAp = .0, dfsAs = .0;
			double dfp_Ap_ap = .0, dfs_As_as = .0, dfp_Ap = .0, dfs_As = .0;
			m_DataCtrl.Get_fAaData(elemK, j, section.dCzp, section.dCzm, dfpApap, dfp_Ap_ap, dfsAsas, dfs_As_as, dfpAp, dfp_Ap, dfsAs, dfs_As);

			for (int k = 0; k < 6; k++)
			{
				T_Column_tension_rail_W ColumnTensD; // Input Data
				ColumnTensD.Initialize();

				COLUMN_STRENGTH ColStreD;  // Output Data
				ColStreD.Initialize();
				ColStreD.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK, j, 0);//check or not..
				if (!ColStreD.bCHK)continue;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ColumnTensD.nElemK = elemK;
				ColumnTensD.nPart = j;
				ColumnTensD.k = k;//Fx(max/min),My(max/min),Mz(max/min);
				ColumnTensD.dn = dn;
				ColumnTensD.dfpk = dfpk;
				ColumnTensD.dfpApap = dfpApap; 	ColumnTensD.dfsAsas = dfsAsas;
				ColumnTensD.dfpAp = dfpAp; 	ColumnTensD.dfsAs = dfsAs;
				ColumnTensD.dfp_Ap_ap = dfp_Ap_ap;	  ColumnTensD.dfs_As_as = dfs_As_as;
				ColumnTensD.dfp_Ap = dfp_Ap;		ColumnTensD.dfs_As = dfs_As;
				ColumnTensD.PscSect = WA_Convert::Convert_A2W(section);
				ColumnTensD.MatlCH = WA_Convert::Convert_A2W(matl);

				for (int ii = 0; ii < 12; ii++)
					ColumnTensD.aForce[ii] = WA_Convert::Convert_A2W(aForce[ii]);

				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK = FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&ColumnTensD, &ColStreD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if (!bVersionOK) continue;
				//==================================================================================================

				//print or not print to report-data
				BOOL bPrinti = FALSE;
				bPrinti = m_DataCtrl.Get_PSCPrintOption(elemK, j, 3);//3:PSC
				if (bPrintTitle && bPrinti)
				{
					file.WriteString(_T("**BEGIN PSC柱－正截面抗拉强度验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p = 0; p < DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.CW2A(p));
					}
				}

				//输出结果
				Write_Command(_T("CLTN")); //验算名称
				Write_Int(elemK);  //单元号
				Write_Int(j);                  //截面位置
				Write_Int(k);                  //验算位置
				Write_Int(ColStreD.Lcomk);//组合名称  
				Write_Int(ColStreD.itype);      //类型             
				Write_Double(m_DataCtrl.Cng_ForceFromCodeToCurr(ColStreD.dKN));
				Write_Double(m_DataCtrl.Cng_ForceFromCodeToCurr(ColStreD.dN));

				m_iTotLineNum++;
				iCount++;
			}
		}
		if (iCount > 0) m_Column_tension_KList.Add(elemK);
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}
/*
void CPSCrailway::column_cross_fracture()
{
	//==================================================================================================
	// Explicit Linkage for DLL :
	typedef BOOL (PDGNCALC)(void*, void*,void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_column_cross_fracture"));
	//==================================================================================================
	size_t sizeIn, sizeRes;
	sizeIn  = sizeof(T_column_cross_fracture_rail);
	sizeRes = sizeof(CON_CROSS_FRACTURE);
	//////////////////////////////////////////////////////////////////////////
	double dMy[4],dNx[4];
	UINT 	iMy[4],iNx[4];
	BOOL  bPrintTitle = TRUE;
	for(int i=0;i<m_elemlist.GetSize();i++)
	{
		int elemK = m_elemlist.GetAt(i);
		//Check Calculating Item.
		if(!m_DataCtrl.Is_CalcPscItem(elemK,0))  continue ;
		int iCount = 0;
		for(int j=0;j<2;j++)
		{
			get_coloumn_MN(elemK,j,dMy,iMy,dNx,iNx);

			_TB_PSC_SECT section;//get section
			if (!m_DataCtrl.Get_PscSect(elemK,j,FALSE,section)) continue;

			int iLoadType = 0; // 0=Pre-tension, 1=Post-tension.
			//double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];
			T_Tendon_Stress_D TndnStressD;
			double dfc,dfct,dfs,df_s,dfp,df_p,dfpk,dEc,dEs,dEp;
			CArray<T_TDNA_K,T_TDNA_K> arTdnaK; BOOL isElemK = FALSE;
			m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
			for(int iL=0; iL<arTdnaK.GetSize(); iL++)//tendon
			{
				T_TDNA_K TdnaK = arTdnaK.GetAt(iL);
				int iLoadType = 0; // 0=Pre-tension, 1=Post-tension.
				CTendonGrup TendonGrup;
				CArray<T_ELEM_K, T_ELEM_K> raElemK;
				TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
				for(int k=0;k<raElemK.GetSize();k++)//elemk
				{
					if (raElemK.GetAt(k) == elemK) isElemK = TRUE;
				}
				if(isElemK) m_DataCtrl.Get_TendonMatl(TdnaK,iLoadType ,dfpk);
			}
			if (!isElemK) continue;

			Get_Tendon_Stress(elemK, j, TndnStressD);
			Get_Metl(elemK,j,dfc, dfct, dfs, df_s, dfp, df_p, dfpk,dEc,dEs,dEp);

			for(int t=0;t<4;t++)
			{
				Isaddforce(t);
				T_column_cross_fracture_rail cross_fracture; // Input Data
				cross_fracture.Initialize();

				CON_CROSS_FRACTURE fractureD;  // Output Data
				fractureD.Initialize();
				fractureD.bCHK = m_DataCtrl.Get_ChkPscFlag(elemK,j,0);//check or not..
				if (!fractureD.bCHK)continue;
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				cross_fracture.nElemK   = elemK;
				cross_fracture.nPart    = j;
				cross_fracture.t        = t;
				cross_fracture.dMy      = dMy[t];
				cross_fracture.dNx      = dNx[t];
				cross_fracture.iMy      = iMy[t];
				cross_fracture.iNx      = iNx[t];
				cross_fracture.PscSect  = section;
				cross_fracture.iLoadType= iLoadType;
				cross_fracture.dKf      = m_dKf;
				cross_fracture.dfc      = dfc;				cross_fracture.dfct      = dfct;
				cross_fracture.dfs      = dfs;				cross_fracture.df_s      = df_s;
				cross_fracture.dfp      = dfp;				cross_fracture.df_p      = df_p;
				cross_fracture.dfpk     = dfpk;				cross_fracture.dEc       = dEc;
				cross_fracture.dEs      = dEs;				cross_fracture.dEp       = dEp;
				for(int ik=0;ik<3;ik++)
				{
					cross_fracture.dstressp[ik]  = TndnStressD.dstressp[ik];
					cross_fracture.dstress_p[ik] = TndnStressD.dstress_p[ik];
					for(int jk=0;jk<3;jk++)
					{
						cross_fracture.dstresslossp[ik][jk]  = TndnStressD.dstresslossp[ik][jk];
						cross_fracture.dstressloss_p[ik][jk] = TndnStressD.dstressloss_p[ik][jk];
					}
				}
				//
				T_DETAIL_STR DetailStr;
				DetailStr.Initialize();
				//!/
				BOOL bVersionOK=FALSE;
				//==================================================================================================
				BOOL bCheck = (*pDgnCalc)(&cross_fracture, &fractureD, &DetailStr, sizeIn, sizeRes, bVersionOK);
				if(!bVersionOK) continue;
				//==================================================================================================

				//report-data
				BOOL bPrinti = FALSE;
				if (m_DataCtrl.Get_PSCPrintOption(elemK,j,0)||
					m_DataCtrl.Get_PSCPrintOption(elemK,j,1))   bPrinti = TRUE;
				if (bPrintTitle && bPrinti)
				{
					file.WriteString(_T("**BEGIN PSC柱－运营阶段正截面抗裂验算: [TB 10002.3--2005]\n"));
					bPrintTitle = FALSE;
				}
				if (bPrinti)
				{
					for (int p=0; p<DetailStr.arString.GetSize(); p++)
					{
						file.WriteString(DetailStr.arString.GetAt(p));
					}
				}

				//change unit
				fractureD.dSigct    =m_DataCtrl.Cng_StressFromCodeToCurr(fractureD.dSigct    );
				fractureD.dSig_ALW  =m_DataCtrl.Cng_StressFromCodeToCurr(fractureD.dSigct_ALW  );
				Write_Command(_T("CLCF"));//验算名称
				Write_Int(elemK);//单元号
				Write_Int(j);//截面位置
				Write_Int(t);//验算位置
				Write_Int(fractureD.Lcomk);//组合名称
				Write_Int(fractureD.itype);//类型
				Write_Double(fractureD.dSigct);
				Write_Double(fractureD.dSigct_ALW);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_Column_cross_fracture_KList.Add(elemK);
	}
	if (!bPrintTitle) file.WriteString(_T("**END\n"));
}
*/

BOOL CPSCrailway::Get_Metl(int Elemi, int iPosi, double& dfc, double& dfct, double& dfs, double& df_s, double& dfp, double& df_p, double& dfpk, double& dEc, double& dEs, double& dEp)
{
	ElemPairK EPairK(Elemi, EN_EL_BEAM);

	T_ELEM_D ElemD;  ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Elemi, ElemD)) { ASSERT(0); return FALSE; }
	T_MATD_D MatdD;	MatdD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { ASSERT(0); return FALSE; }
	CString  s_concrete, s_rebar, s_tendon;
	if (MatdD.Data1.CodeName == MATLCODE_CON_TB05)
	{
		s_concrete = MatdD.Data1.CodeMatlName;  //混凝土的标号 
		s_rebar = MatdD.MainRebar_RebarName; //钢筋的标号               
		if (s_concrete == _T("C20")) { dfc = 13.5; dfct = 1.70; dEc = 28000.0; }
		else if (s_concrete == _T("C25")) { dfc = 17.0; dfct = 2.00; dEc = 30000.0; }
		else if (s_concrete == _T("C30")) { dfc = 20.0; dfct = 2.20; dEc = 32000.0; }
		else if (s_concrete == _T("C35")) { dfc = 23.5; dfct = 2.50; dEc = 33000.0; }
		else if (s_concrete == _T("C40")) { dfc = 27.0; dfct = 2.70; dEc = 34000.0; }
		else if (s_concrete == _T("C45")) { dfc = 30.0; dfct = 2.90; dEc = 34500.0; }
		else if (s_concrete == _T("C50")) { dfc = 33.5; dfct = 3.10; dEc = 35500.0; }
		else if (s_concrete == _T("C55")) { dfc = 37.0; dfct = 3.30; dEc = 36000.0; }
		else if (s_concrete == _T("C60")) { dfc = 40.0; dfct = 3.50; dEc = 36500.0; }
		if (s_rebar == _T("Q235")) { dfs = 235;  df_s = 235; dEs = 210000.0; }
		else if (s_rebar == _T("HRB335")) { dfs = 335;  df_s = 335; dEs = 200000.0; }
	}
	else { ASSERT(0);	return FALSE; }

	/////////读取预应力钢筋的预应力,预应力损失,面积,保护层厚度以及钢筋总的周边长度//////
	iPosi == 0 ? TRUE : FALSE;                                                       //
	BOOL bIsGrup = TRUE;	// Grup (NOT Sngl).                                       //
	double dMuyMax = 0.0, dMuyMin = 0.0;                                               //
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;                               //
	m_DataCtrl.m_DataCtrl.Get_ConTndn(EPairK, iPosi, bIsGrup, dMuyMax, dMuyMin, arTndnPosi);     //
	for (int k = 0; k < arTndnPosi.GetSize(); k++)                                          //
	{                                                                              //
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(k);                                   //
		if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(Tndn.MatlK, MatdD)) { ASSERT(0); return FALSE; }
		if (MatdD.Data1.CodeName == MATLCODE_STL_TB05)
		{
			s_tendon = MatdD.Data1.CodeMatlName;  //预应力的标号 
			if (s_tendon == _T("PSB830")) { dfpk = 830;  dfp = 0.9*dfpk; df_p = 380; dEp = 200000.0; }
			else if (s_tendon == _T("Wire1470")) { dfpk = 1470; dfp = 0.9*dfpk; df_p = 380; dEp = 205000.0; }
			else if (s_tendon == _T("Wire1570")) { dfpk = 1570; dfp = 0.9*dfpk; df_p = 380; dEp = 205000.0; }
			else if (s_tendon == _T("Wire1670")) { dfpk = 1670; dfp = 0.9*dfpk; df_p = 380; dEp = 205000.0; }
			else if (s_tendon == _T("Wire1770")) { dfpk = 1770; dfp = 0.9*dfpk; df_p = 380; dEp = 205000.0; }
			else if (s_tendon == _T("Wire1860")) { dfpk = 1860; dfp = 0.9*dfpk; df_p = 380; dEp = 205000.0; }
			else if (s_tendon == _T("Strand1470")) { dfpk = 1470; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
			else if (s_tendon == _T("Strand1570")) { dfpk = 1570; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
			else if (s_tendon == _T("Strand1670")) { dfpk = 1670; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
			else if (s_tendon == _T("Strand1720")) { dfpk = 1720; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
			else if (s_tendon == _T("Strand1770")) { dfpk = 1770; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
			else if (s_tendon == _T("Strand1820")) { dfpk = 1820; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
			else if (s_tendon == _T("Strand1860")) { dfpk = 1860; dfp = 0.9*dfpk; df_p = 380; dEp = 195000.0; }
		}
		else {/*ASSERT(0);*/	return FALSE; }
	}
	return TRUE;
}

BOOL CPSCrailway::Get_SubRebar(int Elemi, int iPosi, double& dfsv)
{
	T_ELEM_D ElemD;  ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Elemi, ElemD)) { ASSERT(0); return FALSE; }
	T_MATD_D MatdD;	MatdD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { ASSERT(0); return FALSE; }
	CString  s_rebar;
	if (MatdD.Data1.CodeName == MATLCODE_CON_TB05)
	{
		s_rebar = MatdD.SubRebar_RebarName; //钢筋的标号                
		if (s_rebar == _T("Q235")) { dfsv = 235; /* df_s=235; dEs=210000.0;*/ }
		else if (s_rebar == _T("HRB335")) { dfsv = 335; /* df_s=335; dEs=200000.0;*/ }
	}
	else { ASSERT(0);	return FALSE; }
	return TRUE;
}

BOOL CPSCrailway::Get_Tendon_Stress(int Elemi, int iPosi, T_Tendon_Stress_D& TndnStress)
{
	ElemPairK EPairK(Elemi, EN_EL_BEAM);

	TndnStress.Initialize();

	iPosi == 0 ? TRUE : FALSE;                                                       //
	BOOL bIsGrup = TRUE;	// Grup (NOT Sngl).                                       //
	double dMuyMax = 0.0, dMuyMin = 0.0;                                               //
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;                               //
	m_DataCtrl.m_DataCtrl.Get_ConTndn(EPairK, iPosi, bIsGrup, dMuyMax, dMuyMin, arTndnPosi);     //
		//by maxiao 2010-12-16 考虑应力的平均，而不是叠加
	double dAp[3], dA_p[3];
	double dlossAp[3][3], dlossA_p[3][3];
	for (int lm = 0; lm < 3; lm++)
	{
		dA_p[lm] = dAp[lm] = .0;
		for (int n = 0; n < 3; n++)                                                    //
		{                                                                         //
			dlossA_p[lm][n] = dlossAp[lm][n] = .0;
		}
	}

	for (int k = 0; k < arTndnPosi.GetSize(); k++)                                          //
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(k);
		for (int m = 0; m < 3; m++)
		{
			if (Tndn.dz > 0.0)
			{
				TndnStress.dstress_p[m] += Tndn.dStress[m] * Tndn.dArea;
				if (Tndn.dStress[m] * Tndn.dArea > m_dZero)
				{
					dA_p[m] += Tndn.dArea;
				}
			}
			else
			{
				TndnStress.dstressp[m] += Tndn.dStress[m] * Tndn.dArea;
				if (Tndn.dStress[m] * Tndn.dArea > m_dZero)
				{
					dAp[m] += Tndn.dArea;
				}
			}
			for (int n = 0; n < 3; n++)                                                    //
			{                                                                         //
				if (Tndn.dz > 0.0)
				{
					TndnStress.dstressloss_p[m][n] += Tndn.dStrLoss[m][n] * Tndn.dArea;
					if (Tndn.dStrLoss[m][n] * Tndn.dArea > m_dZero)dlossA_p[m][n] += Tndn.dArea;
				}
				else
				{
					TndnStress.dstresslossp[m][n] += Tndn.dStrLoss[m][n] * Tndn.dArea;
					if (Tndn.dStrLoss[m][n] * Tndn.dArea > m_dZero)dlossAp[m][n] += Tndn.dArea;
				}
			}
		}
	}

	for (int m = 0; m < 3; m++)
	{
		TndnStress.dstress_p[m] = dA_p[m] >= m_dZero ? TndnStress.dstress_p[m] / dA_p[m] : 0;
		TndnStress.dstressp[m] = dAp[m] >= m_dZero ? TndnStress.dstressp[m] / dAp[m] : 0;
		for (int n = 0; n < 3; n++)                                                    //
		{                                                                         //
			TndnStress.dstressloss_p[m][n] = dlossA_p[m][n] >= m_dZero ? TndnStress.dstressloss_p[m][n] / dlossA_p[m][n] : 0;
			TndnStress.dstresslossp[m][n] = dlossAp[m][n] >= m_dZero ? TndnStress.dstresslossp[m][n] / dlossAp[m][n] : 0;
		}
	}
	return TRUE;
}

double CPSCrailway::Get_Sigma(BOOL bZ, double Np, double A, double Npez, double Npey, double z, double y, double Iyy, double Izz)
{
	if (fabs(A) < 1.0E-07 || fabs(Iyy) < 1.0E-07 || (bZ && fabs(Izz) < 1.0E-07)) return 0.0;
	if (bZ) return Np / A + Npez * z / Iyy + Npey * y / Izz;
	else    return Np / A + Npez * z / Iyy;
}