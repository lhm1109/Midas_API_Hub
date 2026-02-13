// Dgn_SeismicManager.cpp: implementation of the CDgn_SeismicManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_SeismicManager.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "DgnEngineProgressDlg.h"

#pragma comment(lib, "DGN_def.lib")
#pragma comment(lib, "DGN_lib.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_SeismicManager::CDgn_SeismicManager()
{

}

CDgn_SeismicManager::~CDgn_SeismicManager()
{

}

// �������� m-phi�� ���ķ� ����.
// ������ ���� CDgn_SeismicDataPool�� Data�� �����������.
bool CDgn_SeismicManager::Parallel_Calc_MPhiCurve()
{
    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(_LS(IDS_DGN_MPHI_CURVE_MAKE_MOMENT_CURVATURE_RESULT), _LS(IDS_DGN_MPHI_CURVE_CALC_MOMENT_CURVATURE), TRUE);	// �������� �������� ProgressDlg �� Titile�� ���⼭ �����Ͽ���. �������� �ܱ�� �ȵǰ� ����..
    D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
    pProInte->bUpdatePercent = true;

    bool bChk = CDgn_SeismicManager::Parallel_Calc_MPhiCurve(pProInte);

    return bChk;
}

bool CDgn_SeismicManager::Parallel_Calc_MPhiCurve(void *pProInte)
{
    CDgn_SeismicDataPool* pDB = CDgn_SeismicDataPool::Instance();

    std::vector<dgn::lib::T_DGN_MPHI_D> vecData;
    int nData = pDB->GetDataVector(vecData);
    if (nData ==0) return false;

    dgn::lib::IDgnMPhiUtil* pMPhiLib = dgn::lib::IDgnMPhiUtil::Instance();
    bool bChk = pMPhiLib->Run_MPhi(vecData, pProInte, (nData>1 ? true : false));

    return bChk;
}

// dgn::lib�� ���� m-phi ����, implicit linkage
int CDgn_SeismicManager::LCalc_MPhiCurve(double dAngle, int nPointNum, DGN_CALC_MPHI_SECT& InData, DGN_CALC_MPHI_RESULT& OutData)
{
    dgn::lib::IDgnMPhiUtil* pMPhiLib = dgn::lib::IDgnMPhiUtil::Instance();
    return pMPhiLib->Run_MPhi(dAngle, nPointNum, &InData, &OutData);
}

int CDgn_SeismicManager::GCalc_MPhiCurve(double dAngle, int nPointNum, DGN_CALC_MPHI_SECT& InData, DGN_CALC_MPHI_RESULT& OutData)
{
	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(DGNENGINE_KR_DLL);
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function P-�� Chart").
	typedef int (PSEISCALC)(double, int, void*, void*, size_t, size_t, BOOL&);
	PSEISCALC *pSeisCalc;
	pSeisCalc = (PSEISCALC*)GetProcAddress(hDllCalc, "GCalc_MPhiCurve");
	//==================================================================================================
	size_t TSize_In= sizeof(DGN_CALC_MPHI_SECT);
	size_t TSize_Res = sizeof(DGN_CALC_MPHI_RESULT);

	BOOL bVersionOK = FALSE;
	if(fabs(fabs(dAngle)-360.0) < 1.0E-7) dAngle = 0.0; // // MQC:7575 Seungjun-20140522 360�� ��� 0����.
	int nChk = (*pSeisCalc)(dAngle, nPointNum, &InData, &OutData, TSize_In, TSize_Res, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_DESIGN_FAIL));
		return FALSE;
	}

	return nChk;	
}

BOOL CDgn_SeismicManager::GCalc_ConfinedConcretePolygonData_Column(DGN_GSEC_SHAPE_D& rSection, DGN_GSEC_REBAR_COLUMN& rRebar, DGN_GSEC_POLYGON_LIST& arOutConcPolyData, DGN_GSEC_POLYGON_LIST& arInConcPolyData, double dOffsetX, double dOffsetY)
{
#if defined(_MGEN)
		return FALSE; // ����� �ʿ��Ҷ� GEN_DgnCalc_KR�� GCalc_ConfinedConcretePolygonData_Column�Լ� �߰��� ��
#endif
		HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(DGNENGINE_KR_DLL);
		if(hDllCalc == NULL) return FALSE;

		//==================================================================================================
		// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function P-�� Chart").
		typedef BOOL (PSEISCALC)(void*, void*, void*, void*, double, double, size_t, size_t, size_t, BOOL&);
		PSEISCALC *pSeisCalc;
		pSeisCalc = (PSEISCALC*)GetProcAddress(hDllCalc, "GCalc_ConfinedConcretePolygonData_Column");
		//==================================================================================================
		size_t TSize_Section  = sizeof(DGN_GSEC_SHAPE_D);
		size_t TSize_Rebar    = sizeof(DGN_GSEC_REBAR_COLUMN);
		size_t TSize_PolyData = sizeof(DGN_GSEC_POLYGON_LIST);

		BOOL bVersionOK = FALSE;
		BOOL bChk = (*pSeisCalc)(&rSection, &rRebar, &arOutConcPolyData, &arInConcPolyData, dOffsetX, dOffsetY, TSize_Section, TSize_Rebar, TSize_PolyData, bVersionOK);

		if(!bVersionOK)
		{
				AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_DESIGN_FAIL));
				return FALSE;
		}

		return bChk;	
}

BOOL CDgn_SeismicManager::GPrint_MPhiCurveReport(DGN_REPORT_OPTION PrintOption, CArray<DGN_REPORT_MPHI_RESULT, DGN_REPORT_MPHI_RESULT&>& arResult)
{
#if defined(_MGEN)
		return FALSE; // ����� �ʿ��Ҷ� GEN_DgnReport_KR�� GPrint_MPhiCurveReport�Լ� �߰��� ��
#endif
		HINSTANCE hDllReport = GetOrLoad_ReportDllHandle(DGNENGINE_KR_DLL);
		if(hDllReport == NULL) return FALSE;

#ifdef _RUS
		//Set Language
		typedef BOOL (PSETLANG)(UINT);
		PSETLANG* pSetLang;
		pSetLang = (PSETLANG*)GetProcAddress(hDllReport, "SetLanguage");
		BOOL bLang = (*pSetLang)(7);
#endif

		CString strDataPath = m_pDoc->GetPathName();
		CString strBasePath = m_pDoc->GetProgramPath();

		CDgnEngineProgressDlg ProDlg;
		ProDlg.Create_ProgressDlg(_LS(IDS_DGN_MPHI_CURVE_MOMENT_CURVATURE), _LS(IDS_DGN_MPHI_CURVE_PRINT), FALSE);	// Stop��ư�� ���������� Progressâ�� ��Ʈ���� �ȵ�

		BOOL bVersionOK = FALSE;	
		D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
		pProInte->bUpdateTitle   = TRUE;
		pProInte->bUpdateMessage = TRUE;
		pProInte->bUpdatePercent = TRUE;
		size_t Tsize_Prog  = sizeof(D_XL_PROGRESS_INTERFACE);
		//==================================================================================================
		// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
		typedef BOOL (PDLLSETTING)(CString, CString, void*, size_t, BOOL&);
		PDLLSETTING *pDllSetting;
		pDllSetting = (PDLLSETTING*)GetProcAddress(hDllReport, ("Set_CallProgramInfo"));
		//==================================================================================================
		(*pDllSetting)(strBasePath, strDataPath, pProInte, Tsize_Prog, bVersionOK);


		//==================================================================================================
		// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
		typedef BOOL (PDGNREPORT)(void*, void*, size_t, size_t, BOOL&);
		PDGNREPORT *pDgnExcel;
		pDgnExcel = (PDGNREPORT*)GetProcAddress(hDllReport, ("GPrint_MPhiCurveReport"));
		//==================================================================================================
		size_t Tsize_Opt = sizeof(DGN_REPORT_OPTION);
		size_t Tsize_Res = sizeof(CArray<DGN_REPORT_MPHI_RESULT, DGN_REPORT_MPHI_RESULT&>);

		BOOL bChk = (*pDgnExcel)(&PrintOption, &arResult, Tsize_Opt, Tsize_Res, bVersionOK);

		ProDlg.Delete_ProgressDlg();

		if(!bVersionOK)
		{
				AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_DESIGN_FAIL));
				return FALSE;
		}	

		if(bChk && PrintOption.bIsView)
		{
				CString strPathTemp(PrintOption.strPath);
				ShellExecute(NULL, _T("open"), strPathTemp, NULL, NULL, SW_SHOWNORMAL);		
		}

		return bChk;	
}

// Convert ////////////////////////////////////////////////////////////////////////

BOOL CDgn_SeismicManager::ConvertMpcc_ToEngine(const T_MPCC_D& MpccD, const T_ECUC_D& EcucD, int nCode_Length, int nCode_Force, DGN_CALC_MPHI_SECT& rData, double& dAxisAngle, int& nNumPoint, BOOL bCreateRebarDB)
{
		rData.Initialize();
		dAxisAngle = 0.0;
		nNumPoint  = 0;

		CDBDoc* pDoc = CDBDoc::GetDocPoint();

		T_SECT_D SectD;
		T_RCHK_D RchkD;
		T_REBT_D RebtD;
		BOOL bGen = FALSE;
		if(!pDoc->m_pAttrCtrl->GetSect(MpccD.SectK, SectD)) return FALSE;
		if(SectD.SectBefore.Shape != D_SECT_SHAPE_REG_GEN)
		{
				if(!pDoc->m_pAttrCtrl->GetRchk(MpccD.SectK, RchkD)) return FALSE;
		}
		else 
		{
				if(!pDoc->m_pAttrCtrl->GetRebt(MpccD.SectK, RebtD)) return FALSE;
				bGen = TRUE;
		}

		if(bCreateRebarDB) pDoc->m_pMatlDB->Create_RebarData(nCode_Length, nCode_Force, TRUE);

		T_FIMP_D FimpConcD1;  FimpConcD1.Initialize();
		T_FIMP_D FimpConcD2;  FimpConcD2.Initialize();
		T_FIMP_D FimpSteelD;  FimpSteelD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1, FimpConcD1)) return FALSE;
		if(!pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlSteelK, FimpSteelD)) return FALSE;

		dAxisAngle = MpccD.dAxisAngle;
		nNumPoint  = MpccD.nNumPoint ;

		rData.strName = MpccD.strName;
		rData.strName += (MpccD.MatlConcK1Trans==0 ? _LS(IDS_DGN_MPHI_CURVE_LONGITUDINAL) : _LS(IDS_DGN_MPHI_CURVE_TRANSVERSE));
		rData.nMembType = DGN_MEMB_COLUMN;
		rData.DataUnit.nBase_Length = nCode_Length;
		rData.DataUnit.nBase_Force  = nCode_Force;
		int nIMJ = MpccD.nPosition;
		// �ܸ� ����
		ConvertToDLL_SectData(SectD, nIMJ, rData.SectD);	
		ConvertToDLL_Stiffness(SectD, nIMJ, rData.ConcStiff);
		// ö�� ����
		if(bGen) ConvertToDLL_RebarData(SectD, nIMJ, RebtD, rData.Rebar_Column);
		else     ConvertToDLL_RebarData(SectD, nIMJ, RchkD.COLM, rData.Rebar_Column);

		if(rData.nMembType == DGN_MEMB_COLUMN && FimpConcD1.nMaterialType == 0 && FimpConcD1.nHysModelType == 6)
        {
            if (pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK2, FimpConcD2))
            {
                if (FimpConcD2.nMaterialType == 0 && FimpConcD2.nHysModelType == 6)
                {
                    if (FimpConcD2.PROP.CON_MANDER.nConcType == 1)
                    {
                        DGN_GSEC_POLYGON_LIST arCoreOutPolyData;  arCoreOutPolyData.RemoveAll();
                        DGN_GSEC_POLYGON_LIST arCoreInPolyData;   arCoreInPolyData.RemoveAll();

                        BOOL bChk = GCalc_ConfinedConcretePolygonData_Column(rData.SectD, rData.Rebar_Column, arCoreOutPolyData, arCoreInPolyData, 0.0, 0.0);
                        if (bChk && arCoreOutPolyData.GetSize() > 0)
                        {
                            int i, nSize;
                            nSize = arCoreOutPolyData.GetSize();
                            rData.SectD.arCoreOutPolyData.SetSize(nSize);
                            for (i = 0; i < nSize; i++)
                            {
                                rData.SectD.arCoreOutPolyData[i] = arCoreOutPolyData[i];
                                // MQC:7575 Seungjun-20140522 : mander model�� core ���� 0,0�� ���ϴܿ� ��ġ�ϵ���.
                                for (int k = 0; k < rData.SectD.arCoreOutPolyData[i].aVertex.GetSize(); ++k)
                                {
                                    rData.SectD.arCoreOutPolyData[i].aVertex[k].dX += rData.SectD.dYBar;
                                    rData.SectD.arCoreOutPolyData[i].aVertex[k].dY += rData.SectD.dZBar;
                                }
                            }
                            nSize = arCoreInPolyData.GetSize();
                            rData.SectD.arCoreInPolyData.SetSize(nSize);
                            for (i = 0; i < nSize; i++)
                            {
                                rData.SectD.arCoreInPolyData[i] = arCoreInPolyData[i];
                                // MQC:7575 Seungjun-20140522 : mander model�� core ���� 0,0�� ���ϴܿ� ��ġ�ϵ���.
                                for (int k = 0; k < rData.SectD.arCoreInPolyData[i].aVertex.GetSize(); ++k)
                                {
                                    rData.SectD.arCoreInPolyData[i].aVertex[k].dX += rData.SectD.dYBar;
                                    rData.SectD.arCoreInPolyData[i].aVertex[k].dY += rData.SectD.dZBar;
                                }
                            }
                        }
                        else
                        {
                            rData.SectD.arCoreOutPolyData.RemoveAll();
                            rData.SectD.arCoreInPolyData.RemoveAll();
                        }
                    }
                }
            }
		}

		rData.nMaxIteration = 50;// Default 20-30���� ����Ǵ� ��찡 ���� ���е��� �������� ���� 50���� �ø�
		rData.nSectionDivision = 100;// Default 50�� ���� ���̴� ������ ���౸���� ���� ������ ���е��� �������� ���� 100���� �ø�
		rData.nResultType = 1;

		rData.bIdealize  = MpccD.bIdealize ; 
		rData.bCurvature = MpccD.bCurvature;
		rData.dCurvature = MpccD.dCurvature;
		rData.nIdealOpt  = MpccD.nIdealOpt;
		// Force Data
		rData.dPu_R = (-1.0)*MpccD.dAxialForce; // dPu_R ������ ����

		// Matl
		rData.MatlD.nCodeType = 0;
		rData.MatlD.nMatlType = DGN_PMCV_MATL_CON;
		if(!ConvertFimp_ToEngine(FimpConcD1, rData.MatlD.FIMP_Conc)) return FALSE;
		if(!ConvertFimp_ToEngine(FimpSteelD, rData.MatlD.FIMP_Rebar)) return FALSE;
		rData.MatlD.FIMP_SubRebar = rData.MatlD.FIMP_Rebar;
		if(FimpConcD1.nMaterialType == 0 && FimpConcD1.nHysModelType == 6)
		{ if(!ConvertFimp_ToEngine(FimpConcD2, rData.MatlD.FIMP_Conc_Sub)) return FALSE; }	

		rData.UltimateOptionD.bUseOption = TRUE;	
		rData.UltimateOptionD.becu      = EcucD.becu;	
		rData.UltimateOptionD.iecuType  = EcucD.necuType ;	
		rData.UltimateOptionD.decu_User = EcucD.decu_User;
		rData.UltimateOptionD.bfck85    = EcucD.bfck85   ;	
		rData.UltimateOptionD.beru_tens = EcucD.beru_tens;	
		rData.UltimateOptionD.beru_comp = EcucD.beru_comp;
		rData.UltimateOptionD.ieruType  = EcucD.neruType ;	
		rData.UltimateOptionD.deru_comp = EcucD.deru_comp;		

		return TRUE;
}

BOOL CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_D& InData, DGN_CALC_FIMP_D& rData)
{
		rData.Initialize();
		rData.strName = InData.strName;	

        if      (InData.nMaterialType == D_FIMP_MATLTYPE_CONC) rData.nMaterialType = DGN_FIMP_MATL_CON;
        else if (InData.nMaterialType == D_FIMP_MATLTYPE_STL)  rData.nMaterialType = DGN_FIMP_MATL_STL;

		rData.nHysModelType = InData.nHysModelType;
		rData.nSeq = InData.nSeq;

        if (rData.nMaterialType == 0)
        {
            switch (InData.nHysModelType)
            {
            case 0: //Kent & Park (���� ��ũ��Ʈ ��)
                ConvertFimp_ToEngine(InData.PROP.CON_KENTPK, rData.PROP.CON_KENTPK);
                break;
            case 1: //�Ϻ� ��ũ��Ʈ ǥ�ؽù漭
                ConvertFimp_ToEngine(InData.PROP.CON_STANJP, rData.PROP.CON_STANJP);
                break;
            case 2: //�Ϻ� ���α� �ù漭
                ConvertFimp_ToEngine(InData.PROP.CON_ROADJP, rData.PROP.CON_ROADJP);
                break;
            case 3: //����� ��Ӱ���(����)
                ConvertFimp_ToEngine(InData.PROP.CON_NAGOYA, rData.PROP.CON_NAGOYA);
                break;
            case 4: //Trilinear Concrete Model
                ConvertFimp_ToEngine(InData.PROP.CON_TRILIN, rData.PROP.CON_TRILIN);
                break;
            case 5: //China (GB50010-02)
                ConvertFimp_ToEngine(InData.PROP.CON_GB1002, rData.PROP.CON_GB1002);
                break;
            case 6: //Mander Model
                ConvertFimp_ToEngine(InData.PROP.CON_MANDER, rData.PROP.CON_MANDER);
                break;
            case 7: // �Ϻ� ���α� �ù漭 H24
            {
                rData.nHysModelType = FIMP_CONC_INELA_JP_ROADWAY_H24;
                ConvertFimp_ToEngine(InData.PROP.CON_ROADJP_H24, rData.PROP.CON_ROADJP_H24);
                break;
            }
            case 8: // China (GB/T50010-10)
            {
                rData.nHysModelType = FIMP_CONC_INELA_CH_GB50010_10;
                ConvertFimp_ToEngine(InData.PROP.CON_GB1010, rData.PROP.CON_GB1010);
                break;
            }
            default:
            {
                ASSERT(0);
                return FALSE;
            }
            }
        }
        else
        {
            rData.dDefaultStlEu = 0.1; // Menegotto-Pinto Model(���� ö�� ��) �� Bilinear Model �϶��� ���ؼ� //�ӽ�ó��
            switch (InData.nHysModelType)
            {
            case 0: //Menegotto-Pinto Model(���� ö�� ��)
                ConvertFimp_ToEngine(InData.PROP.STL_MENEGO, rData.PROP.STL_MENEGO);
                break;
            case 1: //Bilinear Model
                ConvertFimp_ToEngine(InData.PROP.STL_BILINE, rData.PROP.STL_BILINE);
                break;
            case 2: //Unsymmetric Bilinear Steel Model(Frame3D like Model)
                ConvertFimp_ToEngine(InData.PROP.STL_GENBIL, rData.PROP.STL_GENBIL);
                break;
            case 3: //Trilinear Steel Model
                ConvertFimp_ToEngine(InData.PROP.STL_TRILIN, rData.PROP.STL_TRILIN);
                break;
            case 4: // PARK steel model
                ConvertFimp_ToEngine(InData.PROP.STL_PARK, rData.PROP.STL_PAKSHD);
                break;
            case 5: // �Ϻ� ���α� �ù漭
            {
                rData.nHysModelType = FIMP_STL_INELA_ROADJP;
                ConvertFimp_ToEngine(InData.PROP.STL_ROADJP, rData.PROP.STL_ROADJP);
                break;
            }
            case 6: // China (GB50010-2010)
            {
                rData.nHysModelType = FIMP_STL_INELA_GB1010;
                ConvertFimp_ToEngine(InData.PROP.STL_GB2010, rData.PROP.STL_GB1010);
                break;
            }
            default:
            {
                ASSERT(0);
                return FALSE;
            }
            }
        }
        return TRUE;
}


BOOL CDgn_SeismicManager::ConvertResult_ToReportDLL(DGN_CALC_MPHI_SECT& InData, DGN_CALC_MPHI_RESULT& InRes, CString strSectFigFileName, CString strChartFigFileName, DGN_REPORT_MPHI_RESULT& rData)
{
		int i, nSize;
		rData.Initialize();

		rData.strName = InData.strName;
		rData.strSectFigFileName  = strSectFigFileName;
		rData.strChartFigFileName = strChartFigFileName;
		rData.dAxisForce = (-1.0)*InData.dPu_R; // ���� ������ �������� �Ͽ����� ��¹��� ����� �������� ǥ����
		rData.dAxisAngle = InRes.dAxisAngle;
		rData.dArea_Rebar = InRes.dArea_Rebar;
		rData.ConcStiff   = InData.ConcStiff;

		rData.bSymmetry = InRes.bSymmetry;
		rData.bIsIdeal  = InRes.bIsIdeal;

		for(i=0 ; i<2 ; i++)
		{
				if(!ConvertResult_ToReportDLL(InRes.MPhi_c[i] , rData.MPhi_c[i] )) return FALSE;
				if(!ConvertResult_ToReportDLL(InRes.MPhi_y0[i], rData.MPhi_y0[i])) return FALSE;
				if(!ConvertResult_ToReportDLL(InRes.MPhi_y[i] , rData.MPhi_y[i] )) return FALSE;
				if(!ConvertResult_ToReportDLL(InRes.MPhi_yI[i], rData.MPhi_yI[i])) return FALSE;
				if(!ConvertResult_ToReportDLL(InRes.MPhi_u[i] , rData.MPhi_u[i] )) return FALSE;
		}		

		DGN_REPORT_MPHI_UNIT_RESULT ResData;
		nSize = InRes.arCurvePos.GetSize();
		rData.arCurvePos.SetSize(nSize);
		for(i=0 ; i<nSize ; i++)
		{
				if(!ConvertResult_ToReportDLL(InRes.arCurvePos[i], ResData)) return FALSE;
				rData.arCurvePos.SetAt(i, ResData);
		}

		nSize = InRes.arCurveNeg.GetSize();
		rData.arCurveNeg.SetSize(nSize);	
		for(i=0 ; i<nSize ; i++)
		{
				if(!ConvertResult_ToReportDLL(InRes.arCurveNeg[i], ResData)) return FALSE;
				rData.arCurveNeg.SetAt(i, ResData);
		}

		return TRUE;
}

BOOL CDgn_SeismicManager::ConvertResult_ToReportDLL(DGN_CALC_MPHI_UNIT_RESULT& InData, DGN_REPORT_MPHI_UNIT_RESULT& rData)
{	
		rData.Initialize();
		rData.dCurvature   = InData.dCurvature  ;
		rData.dAxisForce   = InData.dAxisForce  ;
		rData.dMoment      = InData.dMoment     ;
		rData.dNeutralAxis = InData.dNeutralAxis;
		rData.dPn_ConcComp    = InData.dPn_ConcComp   ;
		rData.dPn_SteelComp   = InData.dPn_SteelComp  ;
		rData.dPn_SteelTen    = InData.dPn_SteelTen   ;
		rData.dPn_RebarComp   = InData.dPn_RebarComp  ;
		rData.dPn_RebarTen    = InData.dPn_RebarTen   ;
		for(int i=0 ; i<2 ; i++)
		{
				rData.dConcStrain[i]  = InData.dConcStrain[i] ;
				rData.dSteelStrain[i] = InData.dSteelStrain[i];
				rData.dRebarStrain[i] = InData.dRebarStrain[i];
				rData.dTndnStrain[i]  = InData.dTndnStrain[i] ;
		}
		return TRUE;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_KENTPK& inKENTPK, OUT DGN_CALC_FIMP_CON_KENTPK& rKENTPK)
{
    // [GEN-1375] M-phi ����, ����ڰ� ������ partial factor ���.
    rKENTPK.dFc = ( inKENTPK.dPartialFact < 1.0e-7 ) ? inKENTPK.dFc : inKENTPK.dFc / inKENTPK.dPartialFact;
    rKENTPK.dE0 = inKENTPK.dE0;
    rKENTPK.dK  = inKENTPK.dK;
    rKENTPK.dZ  = inKENTPK.dZ;
    rKENTPK.dEu = inKENTPK.dEu;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_STANJP& inSTANJP, OUT DGN_CALC_FIMP_CON_STANJP& rSTANJP)
{
    // [GEN-1375] M-phi ����, ����ڰ� ������ partial factor ���.
    rSTANJP.dFc        = ( inSTANJP.dPartialFact < 1.0e-7 ) ? inSTANJP.dFc : inSTANJP.dFc / inSTANJP.dPartialFact;
    rSTANJP.dEpeak     = inSTANJP.dEpeak;
    rSTANJP.dEpsiloncu = 10.0*inSTANJP.dEpeak; //�ӽ�ó��

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_ROADJP& inROADJP, OUT DGN_CALC_FIMP_CON_ROADJP& rROADJP)
{
    rROADJP.nEarthquakeType = inROADJP.nEarthquakeType;
    rROADJP.dEc             = inROADJP.dEc;
    rROADJP.dSigmack        = inROADJP.dSigmack;
    rROADJP.dSigmasy        = inROADJP.dSigmasy;
    rROADJP.dAlpha          = inROADJP.dAlpha;
    rROADJP.dSigmabt        = inROADJP.dSigmabt;
    rROADJP.dAh   = inROADJP.dAh;
    rROADJP.ds    = inROADJP.ds;
    rROADJP.dd    = inROADJP.dd;
    rROADJP.dBeta = inROADJP.dBeta;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_NAGOYA& inNAGOYA, OUT DGN_CALC_FIMP_CON_NAGOYA& rNAGOYA)
{
    rNAGOYA.dSigmack   = inNAGOYA.dSigmack;
    rNAGOYA.dEpsiloncc = inNAGOYA.dEpsiloncc;
    rNAGOYA.dEpsiloncu = inNAGOYA.dEpsiloncu;
    rNAGOYA.dK         = inNAGOYA.dK;
    rNAGOYA.dEpsilont0 = inNAGOYA.dEpsilont0;
    rNAGOYA.dEpsilont1 = inNAGOYA.dEpsilont1;
    rNAGOYA.dEpsilontu = inNAGOYA.dEpsilontu;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_TRILIN& inTRILIN, OUT DGN_CALC_FIMP_CON_TRILIN& rTRILIN)
{
    rTRILIN.nMethod    = inTRILIN.nMethod;
    rTRILIN.dEpsiloncu = inTRILIN.dEpsiloncu;
    for (int i = 0; i < 3; i++)
    {
        rTRILIN.dSigmay[i]   = inTRILIN.dSigmay[i];
        rTRILIN.dEpsilont[i] = inTRILIN.dEpsilont[i];
        rTRILIN.dEpsilony[i] = inTRILIN.dEpsilony[i];
        rTRILIN.dK[i]        = inTRILIN.dK[i];
    }
    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_GB1002& inGB1002, OUT DGN_CALC_FIMP_CON_GB1002& rGB1002)
{
    rGB1002.dFck = inGB1002.dFck;
    rGB1002.dFtk = inGB1002.dFtk;
    rGB1002.dEc  = inGB1002.dEc;
    rGB1002.dEt  = inGB1002.dEt;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_MANDER& inMANDER, OUT DGN_CALC_FIMP_CON_MANDER& rMANDER)
{
    rMANDER.nConcType    = inMANDER.nConcType;
    rMANDER.dConcDataEc  = inMANDER.dConcDataEc;
    rMANDER.dConcDataFco = inMANDER.dConcDatafco;
    rMANDER.dConcDataeco = inMANDER.dConcDataeco;
    rMANDER.dConcDataecy = inMANDER.dConcDataecy;
    rMANDER.dConfinedConcStrengthFcc = inMANDER.dConfinedConcStrengthfcc;
    rMANDER.dConfinedConcStrainecc = inMANDER.dConfinedConcStrainecc;
    rMANDER.dConfinedConcStrainecy = inMANDER.dConfinedConcStrainecy;
    rMANDER.dTensConcDataft = inMANDER.dTensConcDataft;
    rMANDER.dTensConcDataet = inMANDER.dTensConcDataet;
    rMANDER.dConfinedConcStraineu = inMANDER.dConcDataesp;
    rMANDER.dConfinedConcStrainecu = /*(!InData.PROP.CON_MANDER.bUseUltStrUlt)? 0.02 : */inMANDER.dConfinedConcStrUltEcu;

    if (inMANDER.nConcType == 1 && !inMANDER.bUseUltStrUlt)
    {
        rMANDER.dConfinedConcStrainecu = rMANDER.dConfinedConcStraineu;
    }

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_CON_GB1010& inGB1010, OUT DGN_CALC_FIMP_CON_GB1010& rGB1010)
{
    rGB1010.dFcr = inGB1010.dFcr;
    rGB1010.dFtr = inGB1010.dFtr;
    rGB1010.dEcr = inGB1010.dEcr;
    rGB1010.dEtr = inGB1010.dEtr;
    rGB1010.dEc  = inGB1010.dEc;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_MENEGO& inMENEGO, OUT DGN_CALC_FIMP_STL_MENEGO& rMENEGO)
{
    // [GEN-1375] M-phi ����, ����ڰ� ������ partial factor ���.
    rMENEGO.dFy = ( inMENEGO.dPartialFact < 1.0e-7 ) ? inMENEGO.dFy : inMENEGO.dFy / inMENEGO.dPartialFact;
    rMENEGO.dR0 = inMENEGO.dR0;
    rMENEGO.dE  = inMENEGO.dE;
    rMENEGO.dA1 = inMENEGO.dA1;
    rMENEGO.dA2 = inMENEGO.dA2;
    rMENEGO.dB  = inMENEGO.dB;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_BILINE& inBILINE, OUT DGN_CALC_FIMP_STL_BILINE& rBILINE)
{
    rBILINE.dFy   = ( inBILINE.dPartialFact < 1.0e-7 ) ? inBILINE.dFy :  inBILINE.dFy / inBILINE.dPartialFact;
    rBILINE.dE1   = inBILINE.dE1;
    rBILINE.dE2E1 = inBILINE.dE2E1;
    
    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_GENBIL& inGENBIL, OUT DGN_CALC_FIMP_STL_GENBIL& rGENBIL)
{
    rGENBIL.dSigmay   = inGENBIL.dSigmay;
    rGENBIL.dSigmacy  = inGENBIL.dSigmacy;
    rGENBIL.dEpsilon1 = inGENBIL.dEpsilon1;
    rGENBIL.dEpsilon2 = inGENBIL.dEpsilon2;
    for (int i = 0; i < 5; ++i)
    {
        rGENBIL.dE[i] = inGENBIL.dE[i];
    }

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_TRILIN& inTRILIN, OUT DGN_CALC_FIMP_STL_TRILIN& rTRILIN)
{
    rTRILIN.nMethod = inTRILIN.nMethod;
    for (int i = 0; i < 6; ++i)
    {
        rTRILIN.dSigmay[i]   = inTRILIN.dSigmay[i];
        rTRILIN.dEpsilony[i] = inTRILIN.dEpsilony[i];
        if (i == 5) break;
        rTRILIN.dK[i] = inTRILIN.dK[i];
    }

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_PARK& inPARK, OUT DGN_CALC_FIMP_STL_PAKSHD& rPAKSHD)
{
    rPAKSHD.dFy = inPARK.dFy;
    rPAKSHD.dFu = inPARK.dFu;
    rPAKSHD.dE  = inPARK.dEs;
    rPAKSHD.deps1 = inPARK.dEsh;
    rPAKSHD.depsu = inPARK.dEsu;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_ROADJP& inROADJP, OUT DGN_CALC_FIMP_STL_ROADJP& rROADJP)
{
    rROADJP.dSigsy = inROADJP.dSigsy;
    rROADJP.dEs    = inROADJP.dEs;
    rROADJP.dest   = inROADJP.dest;

    return true;
}

bool CDgn_SeismicManager::ConvertFimp_ToEngine(const T_FIMP_STL_GB2010& inGB2010, OUT DGN_CALC_FIMP_STL_GB1010& rGB1010)
{
    rGB1010.dFyr  = inGB2010.dFyr;
    rGB1010.dFstr = inGB2010.dFstr;
    rGB1010.dEs   = inGB2010.dEs;
    rGB1010.dEy   = inGB2010.dEy;
    rGB1010.dEuy  = inGB2010.dEuy;
    rGB1010.dEu   = inGB2010.dEu;
    rGB1010.dK    = inGB2010.dK;

    return true;
}



//====================================================================================================================================================
void CDgn_SeismicDataPool::InitPool()
{
    m_mapIndex.clear();
    m_vecData.clear();
}

int CDgn_SeismicDataPool::GetSerialIndex(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK)
{
    tupleKey Key = std::make_tuple(ElemK, bWall, LcomK, PosiK, SignK);
    auto Iter = m_mapIndex.find(Key);
    if(Iter == m_mapIndex.end())
    {
        return -1;
    }
    else
    {
        if (Iter->second >= m_vecData.size())
        {
            ASSERT(0);
            m_mapIndex.erase(Key);
            return -1;
        }
    }

    return Iter->second;
}

void CDgn_SeismicDataPool::SetSerialIndex(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, const UINT& iIdx)
{
    tupleKey Key = std::make_tuple(ElemK, bWall, LcomK, PosiK, SignK);
    m_mapIndex.insert(std::make_pair(Key, iIdx));
}

int CDgn_SeismicDataPool::AddInputData(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, const double& dAngel, const int& nPointNum, const DGN_CALC_MPHI_SECT& InData)
{
    int iIdx = GetSerialIndex(ElemK, bWall, LcomK, PosiK, SignK);
    STMPhiData Data(InData);
    Data.dAngle = dAngel;

    if (iIdx<0)
    {   
        m_vecData.push_back(Data);
        iIdx = static_cast<int>(m_vecData.size())-1;
        SetSerialIndex(ElemK, bWall, LcomK, PosiK, SignK, iIdx);
    }
    else
    {
        m_vecData[iIdx] = Data;
    }

    return iIdx;
}

int CDgn_SeismicDataPool::AddResult(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, const int&nError, const DGN_CALC_MPHI_RESULT& ResData)
{
    int iIdx = GetSerialIndex(ElemK, bWall, LcomK, PosiK, SignK);
    if (iIdx<0)
    {
        ASSERT(0);
    }
    else
    {
        m_vecData[iIdx].nError = nError;
        m_vecData[iIdx].ResD   = ResData;
    }

    return iIdx;
}

bool CDgn_SeismicDataPool::GetResult(const UINT& ElemK, const bool bWall, const UINT& LcomK, const UINT& PosiK, const UINT& SignK, int& nError, DGN_CALC_MPHI_RESULT& ResData)
{
    ResData.Initialize();
    nError = 0;
    int iIdx = GetSerialIndex(ElemK, bWall, LcomK, PosiK, SignK);
    if (iIdx < 0)
    {
        return false;
    }

    nError  = m_vecData[iIdx].nError;
    ResData = m_vecData[iIdx].ResD;
    return true;
}

void CDgn_SeismicDataPool::ClearResult(const UINT& ElemK, const bool bWall)
{
    std::vector<tupleKey> vTupleKey;
    for ( auto iter = m_mapIndex.begin(); iter != m_mapIndex.end(); iter++ )
    {
        const tupleKey CurKey = iter->first;
		if ( std::get<0>(CurKey) == ElemK && std::get<1>(CurKey) == bWall )
        {
            vTupleKey.emplace_back(CurKey);
        }
    }

    for ( auto const &cur : vTupleKey )
    {
        m_mapIndex.erase(cur);
    }    
}

int CDgn_SeismicDataPool::GetDataVector(std::vector<dgn::lib::T_DGN_MPHI_D>& vecData)
{
    int nSizeIdx  = static_cast<int>(m_mapIndex.size());
    int nSizeData = static_cast<int>(m_vecData.size());
    if (nSizeData==0 || nSizeData==0)   return 0;
    if (nSizeData != nSizeIdx)
    {
        ASSERT(0);
        return 0;
    }

    vecData.resize(nSizeData);
    for(int i=0; i<nSizeData; ++i)
    {
        vecData[i].dAngle    = m_vecData[i].dAngle;
        vecData[i].nPointNum = m_vecData[i].nPointNum;
        vecData[i].pInData   = &m_vecData[i].InD;
        vecData[i].pOutData  = &m_vecData[i].ResD;
    }

    return nSizeData;
}