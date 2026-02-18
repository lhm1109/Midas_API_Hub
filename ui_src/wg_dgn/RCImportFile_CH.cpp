// RCImportFile_CH.cpp: implementation of the CRCImportFile_CH class.
// add by maxiao(2006.09.08) for CIVIL's  RC Design.(china)[JTG D62--2004] 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RCImportFile_CH.h"
#include "RCDataCtrl_CH.h"
//#include "..\wg_xl\FileBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_FileCtrl.h"
//#include "..\wg_db\wg_db_AttrCtrl.h"
#include <math.h>
//#include "..\MIT_Lib\rptostream.h"
#include "..\wg_db\wg_db_StagCtrl.h"
//#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"

#include "DgnStruct.h"
#include "..\..\dgnengine\src/CVL_DgnCalc_CH/DgnCalc_CH_RC_Struct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double pi =  4*atan(1.0);
//#define _PROFILING ?????????????????????????????????????????????????

// #include "profilewrite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRCImportFile_CH::CRCImportFile_CH()
{
	m_hDllCalc = NULL;
	m_ElemKList.RemoveAll();

    m_iTotLineNum=0;
    m_CbcsBeamKList.RemoveAll();
    m_CsisBeamKList.RemoveAll();
    m_CtssBeamKList.RemoveAll();
    m_CcwsBeamKList.RemoveAll();
    m_CcwsColumnKList.RemoveAll();
    m_CsccBeamKList.RemoveAll();
    m_CtstBeamKList.RemoveAll();
    m_CsncBeamKList.RemoveAll();
    m_CtcsColumnKList.RemoveAll();
    m_CscsColumnKList.RemoveAll();
    m_BrlrKList.RemoveAll();
    m_CrlrKList.RemoveAll();
    m_BrcrKList.RemoveAll();
    
    m_rcD.Initialize();
	m_matl.Initialize();
	m_pDoc = CDBDoc::GetDocPoint();
 	ASSERT(m_pDoc);
}

CRCImportFile_CH::~CRCImportFile_CH()
{
}

//RC design:TRUE-beam; FALSE-column
BOOL CRCImportFile_CH::RcDesign_CH(BOOL isBeam){
    if (CDBDoc::GetDocPoint()->IsLimitedTrialVersion())
    {
        //AfxMessageBox(_LS(IDS_TRIAL_NO_LICENSE));
        CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
        return FALSE;
    }
    int iStageNum = m_pDoc->m_pAttrCtrl->GetCountStag();
    T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
    int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;
    if (iStageNum>0)
        m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
    if(!MakeData(isBeam)) 
    {
        if(iStageNum>0)
            m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
        return FALSE;
    }

    CFileException FileException;
    CString strPathName=m_pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strFilePathName,outFilePathName,ReportPathNamme;
    if (isBeam) {
        strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chBeam.jpo");
        outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chBeam.mrpt");
    }else{
        strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chColumn.jpo");
        outFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_chColumn.mrpt");
    }
    Set_FileName(strFilePathName);
    TRY
    {
        if(!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
    		AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
        if(!m_outfile.Open(outFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
    		AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
        if(!Write_ImportData(isBeam)) 
        {
            m_cfile.Close();
            m_outfile.Close();
            CFileStatus status;
            if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
            if(iStageNum>0)
                m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
            return FALSE;
        }
        m_cfile.Close();
        m_outfile.Close();
    }
    CATCH(CFileException, e)
    {
        e->ReportError();
        m_cfile.Abort();
        m_outfile.Abort();
        CFileStatus status;
        return FALSE;
    }
    END_CATCH 
    return TRUE;
}

CString CRCImportFile_CH::Get_DllFileName(int iCode)
{
	BOOL bDebug = FALSE;
    CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

	if(iCode==RC_JTG_D62_04)
	{
		strDllName = bDebug ? _T("CVL_DgnCalc_CHD.dgne") : _T("CVL_DgnCalc_CH.dgne");      
	}
	else 
		ASSERT(0);

    return strDllName;
}

//run rc design and write process data to .jpo file.
//TRUE(beam design); FALSE(column design)
BOOL CRCImportFile_CH::Write_ImportData(BOOL isBeam){
    if (!m_DataCtrl.Get_Rcd(m_rcD)) { 
        return FALSE;
    }
	/*
    m_ElemKList.RemoveAll();
    if (!m_DataCtrl.Get_CrcElemListForDgn(m_ElemKList,isBeam)) { 
        return FALSE;
    }
    */
    if(!(m_rcD.iDgnCode == RC_JTG_D62_04 || m_rcD.iDgnCode == RC_CJJ_11_2011)){
        ASSERT(0);
        return TRUE;
    }
    
    CString strCalcDll = Get_DllFileName(RC_JTG_D62_04);  
    
	{// AFX_MANAGE_STATE(AfxGetModuleState())篮 _T("{"),_T("}") 郴何俊 沥狼
		// LoadLibrary()俊辑 府家胶 眉牢阑 噶啊哆府绰 版快啊 乐绢辑
		// 捞甫 汗盔窍扁 困秦 AfxGetModuleState() 窃荐甫 荤侩窃.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if(m_hDllCalc == NULL)
		{		
			if((m_hDllCalc = LoadLibrary(strCalcDll)) == NULL)
			{
				CString strMsg = _T("Can't Find ")+strCalcDll;
				AfxMessageBox(strMsg);
			}
		}
	}

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    m_DataCtrl.Set_UnitIndex(CodeUnit);	

    //for beam
    if (isBeam) 
	{
        if (m_rcD.bDesignPrint[5]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的抗弯验算***"));
			if (!Write_Cbcs()) return FALSE;
        }
        if (m_rcD.bDesignPrint[6]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的抗剪验算***"));
			if (!Write_Csis()) return FALSE;
        }
        if (m_rcD.bDesignPrint[7]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的抗扭验算***"));
			if (!Write_Ctss()) return FALSE;
        }
        if (m_rcD.bDesignPrint[8]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的裂缝宽度验算***"));
            if (!Write_Ccws(isBeam)) return FALSE;
        }
        if (m_rcD.bDesignPrint[3]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的截面纵向钢筋用量估算***"));
            if (!Write_Brlr()) return FALSE;
        }
        if (m_rcD.bDesignPrint[4]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的截面普通箍筋用量估算***"));
            if (!Write_Brcr()) return FALSE;
        }
        if (m_rcD.bDesignPrint[2]) 
		{
            GSaveHistoryFormatNF(_T("开始进行梁的施工阶段法向压应力验算***"));
			if (!Write_Cscc()) return FALSE;
            GSaveHistoryFormatNF(_T("开始进行梁的施工阶段中性轴主拉应力验算***"));
            if (!Write_Csnc()) return FALSE;
        }
    }
	else
	{//for column
        if (m_rcD.bDesignPrint[8]) 
		{
            GSaveHistoryFormatNF(_T("开始进行柱的裂缝宽度验算***"));
            if (!Write_ColumnCcws()) return FALSE;
        }
        if (m_rcD.bDesignPrint[10]) 
		{
            GSaveHistoryFormatNF(_T("开始进行柱的使用阶段正截面轴心/偏心抗压承载能力验算***"));
            if (!Write_Cscs()) return FALSE;
        }
        if (m_rcD.bDesignPrint[11]) 
		{
            GSaveHistoryFormatNF(_T("开始进行柱的使用阶段正截面轴心/偏心抗拉承载能力验算***"));
            if (!Write_Ctcs()) return FALSE;
        }
        if (m_rcD.bDesignPrint[3]) 
		{
            GSaveHistoryFormatNF(_T("开始进行柱的截面纵向钢筋用量估算***"));
            if (!Write_Crlr()) return FALSE;
        }
    }
	return TRUE;
}

//Check bending for cross section at service loads: see [JTG D62--2004] 5.2.2  5.2.3 P25
BOOL CRCImportFile_CH::Write_Cbcs()
{
    BOOL bPrintTitle = TRUE;
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_bending_cross_section")); 
	//==================================================================================================
    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CBCS_D_W);
    sizeResu = sizeof(T_CBCS_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
 		//1:get design material of checking ELEM
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl))	continue;
        //2:get force data
        T_CBCS_CH cbcs;	
		if (!m_DataCtrl.Get_CHRcCbcs(ElemK,cbcs)) continue;		
        int iCount = 0;
        for (int j=0; j<4; j++) 
		{
            if (!cbcs.cbcsBase[j].bCHK) continue;
			//3:get section of checking ELEM
			_DGN_RC_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_BeamSection(m_ElemKList.GetAt(i),j/2,sectioni))	continue;

            _DGN_CBCS_D_W DgnCbcsD;
            DgnCbcsD.Initialize();
            DgnCbcsD.nElemK = ElemK;
			DgnCbcsD.nPart = j;
            DgnCbcsD.RcMatlCH = m_matl;
            DgnCbcsD.SectionCH = sectioni;
			//
			cbcs.cbcsBase[j].drMuy = m_rcD.iSafeType * cbcs.cbcsBase[j].dMuy;
            //
            T_DETAIL_STR DetailStr; DetailStr.Initialize();
            //!/
            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCbcsD, &cbcs.cbcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================

			//output report data...
            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/2,j%2);//true:print to report-data;
            if (bPrintTitle && bPrinti) 
			{
                m_outfile.WriteString(_T("**BEGIN使用阶段正截面抗弯验算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

		}//endof for(j)
		for (int j=0;j<4;j++) 
		{
            m_DataCtrl.Get_CHRCCbcsBaseCodeToCurr(cbcs.cbcsBase[j]);  //change UNIT      
            if (!cbcs.cbcsBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iTyp = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("CBCS_JT"));                      
			Write_Int(m_ElemKList.GetAt(i));
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(cbcs.cbcsBase[j].LcomK);
			Write_Int(cbcs.cbcsBase[j].iMax);
			Write_Double(cbcs.cbcsBase[j].dMuy);
			Write_Double(cbcs.cbcsBase[j].drMuy);	   
			Write_Double(cbcs.cbcsBase[j].dMny);		   
			Write_Double(cbcs.cbcsBase[j].dRatY); 

            Write_Double(cbcs.cbcsBase[j].dx);
            Write_Double(cbcs.cbcsBase[j].dEb); 
            Write_Double(cbcs.cbcsBase[j].d2a); 
            Write_Double(cbcs.cbcsBase[j].dReBarRatio); 
            Write_Double(cbcs.cbcsBase[j].dMinReBarRatio); 
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_CbcsBeamKList.Add(ElemK);
	}//endof for(i)	
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

// Check shear for inclined section at service loads: see [JTG D62--2004] 5.2.7;5.2.9;5.2.10 P28
BOOL CRCImportFile_CH::Write_Csis()
{
    BOOL bPrintTitle = TRUE;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_shear_inclined_section")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CSIS_D_W);
    sizeResu = sizeof(T_CSIS_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design material of checking ELEM
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue;

		T_CSIS_CH csis;
        if (!m_DataCtrl.Get_CHRcCsis(ElemK,csis)) continue;
        int iCount = 0;
		for (int j=0; j<4; j++) 
		{
            if (!csis.CsisBase[j].bCHK) continue;
			//1:get section of checking ELEM
			_DGN_RC_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_BeamSection(m_ElemKList.GetAt(i),j/2,sectioni)) continue;

            //!/
            _DGN_CSIS_D_W DgnCsisD;
            DgnCsisD.Initialize();
			DgnCsisD.nElemK = ElemK;
			DgnCsisD.nPart = j;
			DgnCsisD.RcMatlCH = m_matl;
            DgnCsisD.SectionCH = sectioni;
			//
			csis.CsisBase[j].drVu = m_rcD.iSafeType * csis.CsisBase[j].dVu;
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCsisD, &csis.CsisBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================

            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/2,2);//true:print to report-data;
            if (bPrintTitle && bPrinti) {
                m_outfile.WriteString(_T("**BEGIN使用阶段斜截面抗剪验算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

		}//endof for(j)
		for (int j=0;j<4;j++) 
		{
            m_DataCtrl.Get_CHRCCsisBaseCodeToCurr(csis.CsisBase[j]);  //change UNIT

            if (!csis.CsisBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iTyp = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("CSIS_JT"));
			Write_Int(m_ElemKList.GetAt(i));
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(csis.CsisBase[j].LcomK);
			Write_Int(csis.CsisBase[j].iMax);
			Write_Double(csis.CsisBase[j].dVu);
			Write_Double(csis.CsisBase[j].drVu);
			Write_Double(csis.CsisBase[j].dVn);
			Write_Double(csis.CsisBase[j].dRatV);
			Write_Double(csis.CsisBase[j].dShrSect);
			Write_Double(csis.CsisBase[j].dShrLoad);
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_CsisBeamKList.Add(ElemK);
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}


// Check torsion at service loads: see [JTG D62--2004] 5.5.2;5.5.3;5.5.4;5.5.5 P42
BOOL CRCImportFile_CH::Write_Ctss()
{
    BOOL bPrintTitle = TRUE;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_torsion")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CTSS_D_W);
    sizeResu = sizeof(T_CTSS_CH_BASE);

	for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design matl of checking ELEM    
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue; 
		T_CTSS_CH ctss;	 
        if (!m_DataCtrl.Get_CHRcCtss(ElemK,ctss)) continue;
        int iCount = 0;
		for (int j=0; j<6; j++) 
		{
            if (!ctss.CtssBase[j].bCHK) continue;
            //1:get section of checking ELEM
			_DGN_RC_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_BeamSection(m_ElemKList.GetAt(i),j/3,sectioni)) continue;

            //!/
            _DGN_CTSS_D_W DgnCtssD;
            DgnCtssD.Initialize();
			DgnCtssD.nElemK = ElemK;
			DgnCtssD.nPart = j;
			DgnCtssD.RcMatlCH = m_matl;
            DgnCtssD.SectionCH = sectioni;
			//
 			ctss.CtssBase[j].drVu = m_rcD.iSafeType * ctss.CtssBase[j].dVu;
			ctss.CtssBase[j].drTu = m_rcD.iSafeType * ctss.CtssBase[j].dTu;	
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCtssD, &ctss.CtssBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================

            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/2,3);//true:print to report-data;
            if (bPrintTitle && bPrinti) {
                m_outfile.WriteString(_T("**BEGIN使用阶段抗扭验算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

		}//endof for(j)
		for(int j=0; j<6; j++)
		{ 
            m_DataCtrl.Get_CHRCCtssBaseCodeToCurr(ctss.CtssBase[j]);  //change UNIT

            if (!ctss.CtssBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 || j==2 ? 1 : 2);
			// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
			int iTyp = 0;
			if( j==0 || j==3)      iTyp = 1;
			else if( j==1 || j==4) iTyp = 2;
			else                   iTyp = 3;
			Write_Command(_T("CTSS_JT"));
			Write_Int(m_ElemKList.GetAt(i));
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(ctss.CtssBase[j].LcomK);
			Write_Int(ctss.CtssBase[j].iMax);
			Write_Double(ctss.CtssBase[j].dTu);		 
			Write_Double(ctss.CtssBase[j].drTu);   //1/4Tcr
			Write_Double(ctss.CtssBase[j].dTn);	 
			Write_Double(ctss.CtssBase[j].dMu);	 
			Write_Double(ctss.CtssBase[j].dVu);
			Write_Double(ctss.CtssBase[j].drVu);
			Write_Double(ctss.CtssBase[j].dVn);		 
			Write_Double(ctss.CtssBase[j].dRatT);
			Write_Double(ctss.CtssBase[j].dShrTorVal);
			Write_Double(ctss.CtssBase[j].dTorSect);
			Write_Double(ctss.CtssBase[j].dTorLoad);
            m_iTotLineNum++;
            iCount++;
		}//endof for(j)
        if(iCount > 0) m_CtssBeamKList.Add(ElemK);
	}//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//Check crack width at service loads: see [JTG D62--2004] 6.4.3;6.4.5; P60
BOOL CRCImportFile_CH::Write_Ccws(BOOL itype)//TRUE(beam);FALSE(column)
{
    BOOL bPrintTitle = TRUE;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_crack_width")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CCWS_D_W);
    sizeResu  = sizeof(T_CCWS_CH_BASE);

	for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design matl of checking ELEM
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue; 
		T_CCWS_CH ccws; 
        if (!m_DataCtrl.Get_CHRcCcws(ElemK,itype,ccws)) continue;
		//get C1,C2,C3
        T_CWRC_D CwrcD;	CwrcD.Initialize();
		double dC1= 0.0, dC2= 0.0, dC3=0.0;
		BOOL bUserInputPscw = m_pDoc->m_pAttrCtrl->GetCwrc(ElemK, CwrcD);
		if(bUserInputPscw )
		{
			dC1 = CwrcD.dC1; dC2 = CwrcD.dC2;dC3 = CwrcD.dC3;
		}
        int iCount = 0;
		for (int j=0; j<4; j++) 
		{
            if (!ccws.ccwsBase[j].bCHK) continue;
			//1:get section of checking ELEM//@todo还需要判断 截面是否需要验算,验算可以在 方法Get_BeamSection中实现

	  ccws.ccwsBase[j].iKind = m_rcD.iCrackType;
			_DGN_RC_SECTION_CH_W sectioni; sectioni.Initialize();
            _DGN_RC_COLUMN_SECTION_CH_W sectionk; sectionk.Initialize();
            if (itype) 
			{
                if (!m_DataCtrl.Get_BeamSection(ElemK,j/2,sectioni)) 	continue;
            }
			else
			{
     			if (!m_DataCtrl.Get_ColumnSection(ElemK,j/2,sectionk))   continue;
			}
    if(!bUserInputPscw)    // add by juhonghua(MITC) 2012.03.05
            {
                dC2 = (fabs(ccws.ccwsBase[j].dFT) > 1.0e-6) ? 1.0+0.5*fabs(ccws.ccwsBase[j].dFB/ccws.ccwsBase[j].dFT) : 1.0;
            }
            if(!bUserInputPscw && itype == TRUE)
            {
                //
                if((fabs(sectioni.dh) > 1.0e-6) && sectioni.db / sectioni.dh > 2.0) //slab
                {
                    dC3 = 1.15;
                }
                else
                    dC3 = 1.0;
            }
            else if(!bUserInputPscw && itype == FALSE)
            {
                if(ccws.ccwsBase[j].iKind == 0)       //(0:轴心受拉, 1:偏心受拉, 2:偏心受压, 3:(圆形)偏心受压)
                {
                    dC3 = 1.2;
                }
                else if(ccws.ccwsBase[j].iKind == 1)
                {
                    dC3 = 1.1;
                }
                else if(ccws.ccwsBase[j].iKind == 2 || ccws.ccwsBase[j].iKind == 3)
                {
                    dC3 = 0.9;
                }
                else
                    dC3 = 1.0;

            }
			//!/
            _DGN_CCWS_D_W DgnCcwsD;
            DgnCcwsD.Initialize();
			DgnCcwsD.nElemK = ElemK;
			DgnCcwsD.nPart = j;
			DgnCcwsD.bBeam = itype;
			DgnCcwsD.RcMatlCH = m_matl;
			DgnCcwsD.BeamSectionCH = sectioni;
			DgnCcwsD.ColumnSectionCH = sectionk;
			DgnCcwsD.dC1 = dC1; DgnCcwsD.dC2 = dC2; DgnCcwsD.dC3 = dC3;
			DgnCcwsD.dGamma0 = m_rcD.iSafeType;//double
			//
			ccws.ccwsBase[j].dAWC = m_rcD.iCrackCode;  //double  			
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            BOOL bCheck=FALSE;
			//==================================================================================================
			bCheck = (*pDgnCalc)(&DgnCcwsD, &ccws.ccwsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			//==================================================================================================
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }

            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/2,j%2);//true:print to report-data;
            if (bPrintTitle && bPrinti) 
			{
                if (itype) m_outfile.WriteString(_T("**BEGIN梁的裂缝宽度验算:\n"));
                else m_outfile.WriteString(_T("**BEGIN柱的裂缝宽度验算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

        }//endof for(j)
		for (int j=0;j<4;j++) //output result of crack check...
		{		
            m_DataCtrl.Get_CHRCCcwsBaseCodeToCurr(ccws.ccwsBase[j],itype);  //change UNIT

            if (!ccws.ccwsBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			// 0=I-顶/反弯矩, 1=I-底/正弯矩, 2=J-顶/反弯矩, 3=J-底/正弯矩.
			int iTyp = 0;
			if( j==0 || j==2)      iTyp = 1;
			else if( j==1 || j==3) iTyp = 2;
            if (itype)  Write_Command(_T("BCWS_JT"));
			else        Write_Command(_T("CCWS_JT"));
			Write_Int(m_ElemKList.GetAt(i));
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(ccws.ccwsBase[j].LcomK);
			Write_Int(ccws.ccwsBase[j].iMax);
			Write_Double(ccws.ccwsBase[j].dFRT);
			Write_Double(ccws.ccwsBase[j].dAWC);
			Write_Double(ccws.ccwsBase[j].dWC);
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0 && itype) m_CcwsBeamKList.Add(ElemK);
        if(iCount > 0 && !itype) m_CcwsColumnKList.Add(ElemK);
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//Check crack width at service loads: see [JTG D62--2004] 6.4.3;6.4.5; P60
BOOL CRCImportFile_CH::Write_ColumnCcws()//(column)
{
    BOOL bPrintTitle = TRUE;
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_crack_width")); 
	//==================================================================================================
    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CCWS_D_W);
    sizeResu  = sizeof(T_CCWS_CH_BASE);

	for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design matl of checking ELEM
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue; 

		T_CCWS_CH_BASE ccwsBase[6];		
		if (!m_DataCtrl.Get_CHRcColumnCcws(ElemK,ccwsBase)) continue;

		//get C1,C2,C3
        T_CWRC_D CwrcD;	CwrcD.Initialize();
		double dC1= 0.0, dC2= 0.0, dC3=0.0;
		BOOL bUserInputPscw = m_pDoc->m_pAttrCtrl->GetCwrc(ElemK, CwrcD);
		if(bUserInputPscw)
		{
			dC1 = CwrcD.dC1; dC2 = CwrcD.dC2;dC3 = CwrcD.dC3;
		}
        int iCount = 0;
		for (int j=0; j<6; j++) 
		{
            if (!ccwsBase[j].bCHK) continue;
			//1:get section of checking ELEM//@todo还需要判断 截面是否需要验算,验算可以在 方法Get_BeamSection中实现
	  ccwsBase[j].iKind = m_rcD.iCrackType;

            _DGN_RC_COLUMN_SECTION_CH_W sectionk; sectionk.Initialize();
 			if (!m_DataCtrl.Get_ColumnSection(ElemK,j/3,sectionk))   continue;

			if(!bUserInputPscw)    // add by juhonghua(MITC) 2012.03.05
			{
				dC2 = (fabs(ccwsBase[j].dFT) > 1.0e-6) ? 1.0+0.5*fabs(ccwsBase[j].dFB/ccwsBase[j].dFT) : 1.0;
			}

			if(!bUserInputPscw )
			{
				if(ccwsBase[j].iKind == 0)       //(0:轴心受拉, 1:偏心受拉, 2:偏心受压, 3:(圆形)偏心受压)
				{
					dC3 = 1.2;
				}
				else if(ccwsBase[j].iKind == 1)
				{
					dC3 = 1.1;
				}
				else if(ccwsBase[j].iKind == 2 ||ccwsBase[j].iKind == 3)
				{
					dC3 = 0.9;
				}
				else
					dC3 = 1.0;
				
			}
			//!/
            _DGN_CCWS_D_W DgnCcwsD;
            DgnCcwsD.Initialize();
			DgnCcwsD.nElemK = ElemK;
			DgnCcwsD.nPart = j;
			DgnCcwsD.bBeam = FALSE;
			DgnCcwsD.RcMatlCH = m_matl;
			DgnCcwsD.ColumnSectionCH = sectionk;
			DgnCcwsD.dC1 = dC1; DgnCcwsD.dC2 = dC2; DgnCcwsD.dC3 = dC3;
			DgnCcwsD.dGamma0 = m_rcD.iSafeType;//double
			//
			ccwsBase[j].dAWC = m_rcD.iCrackCode;  //double  			
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            BOOL bCheck=FALSE;
			//==================================================================================================
			bCheck = (*pDgnCalc)(&DgnCcwsD, &ccwsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			//==================================================================================================
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }

            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/3,j%2);//true:print to report-data;
            if (bPrintTitle && bPrinti) 
			{
                m_outfile.WriteString(_T("**BEGIN柱的裂缝宽度验算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}
        }//endof for(j)
		
		T_CCWS_CH ccws;
		ccws.Initialize();
		ccws.ccwsBase[0] = ccwsBase[0]; 		ccws.ccwsBase[2] = ccwsBase[3]; 
		ccws.ccwsBase[1] = ccwsBase[0]; 		ccws.ccwsBase[3] = ccwsBase[3]; 
		for (int j=0; j<3; j++) 
		{
			if (ccwsBase[j].dWC >= ccws.ccwsBase[0].dWC) 	  ccws.ccwsBase[0] = ccwsBase[j];
			if (ccwsBase[j].dWC <= ccws.ccwsBase[1].dWC) 	  ccws.ccwsBase[1] = ccwsBase[j];
			if (ccwsBase[j+3].dWC >= ccws.ccwsBase[2].dWC) 	ccws.ccwsBase[2] = ccwsBase[j+3];
			if (ccwsBase[j+3].dWC <= ccws.ccwsBase[3].dWC) 	ccws.ccwsBase[3] = ccwsBase[j+3];
		}

		for (int j=0;j<4;j++) //output result of crack check...
		{		
            m_DataCtrl.Get_CHRCCcwsBaseCodeToCurr(ccws.ccwsBase[j],FALSE);  //change UNIT

            if (!ccws.ccwsBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			// 0=I-顶/反弯矩, 1=I-底/正弯矩, 2=J-顶/反弯矩, 3=J-底/正弯矩.
			int iTyp = 0;
			if( j==0 || j==2)      iTyp = 1;
			else if( j==1 || j==3) iTyp = 2;
            Write_Command(_T("CCWS_JT"));
			Write_Int(m_ElemKList.GetAt(i));
			Write_Int(iPos);
			Write_Int(iTyp);
			Write_Int(ccws.ccwsBase[j].iKind);
			Write_Int(ccws.ccwsBase[j].LcomK);
			Write_Int(ccws.ccwsBase[j].iMax);
			Write_Double(ccws.ccwsBase[j].dFRT);
			Write_Double(ccws.ccwsBase[j].dAWC);
			Write_Double(ccws.ccwsBase[j].dWC);
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_CcwsColumnKList.Add(ElemK);
	}//endof for(i)
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

// 施工阶段正截面法向应力验算: see [JTG D62--2004] 7.2.4-1;7.2.4-2 P69
BOOL CRCImportFile_CH::Write_Cscc()
{
    BOOL bPrintTitle = TRUE; int rbarid = 0;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_Cscc")); 
	//==================================================================================================
    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CSCC_D_W);
    sizeResu = sizeof(T_CSCC_CH_BASE);

	for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
        //0.get design matl of checking ELEM
		m_matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue; 
		//1.get stress of checking ELEM.
		T_CSCC_CH cscc; cscc.Initialize();
//by maxiao++++++++++++++ 
//		if (!m_DataCtrl.Get_CrcCscc(ElemK, cscc))	continue;//法向应力
		CArray<T_CTST_CH,T_CTST_CH> ctstList;
         if (!m_DataCtrl.Get_CrcCscc(ElemK,m_matl.dfsd,m_matl.dfsd1,m_matl.dfcd,m_matl.dEs,m_matl.dEc,
			 cscc,ctstList)) continue;
//+++++++++++++++++++++++
        CString ctext,cdata;
        int iCount = 0;
		for (int j=0; j<4; j++) 
		{
            if (!cscc.CsccBase[j].bCHK) continue;
            //!/
            _DGN_CSCC_D_W DgnCsccD;
            DgnCsccD.Initialize();
			DgnCsccD.nElemK = ElemK;
			DgnCsccD.nPart = j;
			DgnCsccD.RcMatlCH = m_matl;
			//
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCsccD, &cscc.CsccBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================

            BOOL bPrinti = FALSE;//m_DataCtrl.Get_PrintOption(ElemK,j/2,j%2);//true:print to report-data;
            if (bPrintTitle && bPrinti) 
			{
                m_outfile.WriteString(_T("**BEGIN施工阶段法向压应力验算:\n"));
                bPrintTitle = FALSE;
            } 
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

		}
		for (int j = 0; j<4;j++) 
		{
            m_DataCtrl.Get_CHRCCsccBaseCodeToCurr(cscc.CsccBase[j]);  //change UNIT
            if (!cscc.CsccBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iMax = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("CSCC_JT"));
			Write_Int(m_ElemKList.GetAt(i));
			Write_Int(iPos);
			Write_Int(iMax);
			Write_Int(cscc.CsccBase[j].StagK);
			Write_Int(cscc.CsccBase[j].iMax);
			Write_Double(cscc.CsccBase[j].dFMAX);
			Write_Double(cscc.CsccBase[j].dALW); 
            m_iTotLineNum++;
            iCount++;
		}
		int icountrbar = 0;
		for (int j=0;j < ctstList.GetSize();j++) 
		{
			T_CTST_CH ctst ; ctst.Initialize();
			ctst = ctstList.GetAt(j);
			ctst.dADLL = ctst.dAFLL = 0.75* m_matl.dfsk;
         		if (ctst.dFLL <= ctst.dAFLL && ctst.dDLL <= ctst.dADLL) 
				{
	  			ctst.bOK = TRUE;
	  		}
				else
				{
	  			ctst.bOK = FALSE;
	  		}
		//+++++++++++++++++++++++++++++++
                m_DataCtrl.Get_CHRCCtstBaseCodeToCurr(ctst);// change UNIT
	  		Write_Command(_T("CTST_JT"));
	  		Write_Int(ElemK);
                Write_Int(ctst.iposi); //i.j
                Write_Int(rbarid);
                Write_Int(ctst.iRbarNa);
	  		Write_Double(ctst.dDLL);
	  		Write_Double(ctst.dFLL);
		  	Write_Double(ctst.dADLL);
		  	Write_Double(ctst.dAFLL);
                m_iTotLineNum++;
                icountrbar++;
                if (icountrbar >0) m_CtstBeamKList.Add(rbarid);
                rbarid++;
     }//endof 受拉钢筋应力验算
     if(iCount > 0) m_CsccBeamKList.Add(ElemK);
	}//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

// 施工阶段中性轴处的主拉应力(剪应力)验算:Check stress for Neutral axis at a construction stage…
//add by maxiao. see [JTG D62--2004] 7.2.5 P69
BOOL CRCImportFile_CH::Write_Csnc()
{
    BOOL bPrintTitle = TRUE;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_stress_Neutral_axis_cs")); 
	//==================================================================================================
    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CSNC_D_W);
    sizeResu = sizeof(T_CSNC_CH_BASE);

	for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design matl of checking ELEM
		m_matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue; 
		//1.get stress of checking ELEM.
		T_CSNC_CH csnc; csnc.Initialize();
		if (!m_DataCtrl.Get_CrcCsnc(ElemK, csnc)) continue;
		CString ctext,cdata;
        int iCount = 0;
		for (int j=0; j<4; j++) 
		{
            if (!csnc.CsncBase[j].bCHK) continue;
            //!/
            _DGN_CSNC_D_W DgnCsncD;
            DgnCsncD.Initialize();
			DgnCsncD.nElemK = ElemK;
			DgnCsncD.nPart = j;
			DgnCsncD.RcMatlCH = m_matl;
			//
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCsncD, &csnc.CsncBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================

            BOOL bPrinti = FALSE;//m_DataCtrl.Get_PrintOption(ElemK,j/2,j%2);//true:print to report-data;
            if (bPrintTitle && bPrinti) 
			{
                m_outfile.WriteString(_T("**BEGIN施工阶段中性轴处的主拉应力(剪应力)验算:\n"));
                bPrintTitle = FALSE;
            } 
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

		}
		for (int j = 0; j<4;j++) 
		{
            m_DataCtrl.Get_CHRCCsncBaseCodeToCurr(csnc.CsncBase[j]);  //change UNIT

            if (!csnc.CsncBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iMax = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("CSNC_JT"));
			Write_Int(ElemK);
			Write_Int(iPos);
			Write_Int(iMax);
			Write_Int(csnc.CsncBase[j].StagK);
			Write_Double(csnc.CsncBase[j].dFM);
			Write_Double(csnc.CsncBase[j].dFML);
			Write_Double(csnc.CsncBase[j].dFMR);
			Write_Double(csnc.CsncBase[j].dFMAX);
			Write_Double(csnc.CsncBase[j].dALW); 
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_CsncBeamKList.Add(ElemK);
	}//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
	return TRUE;
}

//(正截面轴/偏心抗压验算)Axes/eccentricity compressive stress check of orthogonal section at service stage
//add by maxiao. see [JTG D62--2004] 5.3.1~12 P32 
BOOL CRCImportFile_CH::Write_Cscs()
{
    BOOL bPrintTitle = TRUE;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_axes_eccentricity_compressive_stress")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CSCS_D_W);
    sizeResu = sizeof(T_CSTCS_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design matl of checking ELEM
		m_matl.Initialize();
        if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) continue;
        int iCount = 0;
        //get force
		T_CSTCS_CH cscs;	cscs.Initialize();
        if (!m_DataCtrl.Get_CHRcCstcs(ElemK,TRUE,cscs)) continue;
        for (int j=0;j<10;j++) 
        {
            if (!cscs.cstcsBase[j].bCHK) continue;
            //1:get section of checking ELEM
			_DGN_RC_COLUMN_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_ColumnSection(ElemK,j/5,sectioni)) continue;      
            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/5,5);//true:print to report-data;5-抗压

	  cscs.cstcsBase[j].iMembType = m_rcD.iCrackType;
            //!/
            _DGN_CSCS_D_W DgnCscsD;
            DgnCscsD.Initialize();
			DgnCscsD.nElemK = ElemK;
			DgnCscsD.nPart = j;
			DgnCscsD.RcMatlCH = m_matl;
			DgnCscsD.SectionCH = sectioni;
			//
            cscs.cstcsBase[j].drNd = m_rcD.iSafeType * cscs.cstcsBase[j].dNd;
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCscsD, &cscs.cstcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================
            if (bPrintTitle && bPrinti) 
			{
                m_outfile.WriteString(_T("**BEGIN正截面轴/偏心抗压验算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

        }//endof for(j) check compressive stress of eccentricity
        for (int j =0;j<10;j++) 
		{
            m_DataCtrl.Get_CHRCCstcsBaseCodeToCurr(cscs.cstcsBase[j]);  //change UNIT
            
            if (!cscs.cstcsBase[j].bCHK) continue;
            Write_Command(_T("CSCS_JT"));
            Write_Int(ElemK);
            Write_Int(j/5);
            Write_Int(j%5);//位置标示
            Write_Int(cscs.cstcsBase[j].LcomK);
            Write_Double(cscs.cstcsBase[j].dx);
            Write_Double(cscs.cstcsBase[j].dNd);
            Write_Double(cscs.cstcsBase[j].drNd);
            Write_Double(cscs.cstcsBase[j].drNde);
            Write_Double(cscs.cstcsBase[j].drNde1);
            Write_Double(cscs.cstcsBase[j].dNn);
            Write_Double(cscs.cstcsBase[j].dNne);
            Write_Double(cscs.cstcsBase[j].dNne1);

            Write_Double(cscs.cstcsBase[j].dRa);
            Write_Double(cscs.cstcsBase[j].dRs);
            Write_Double(cscs.cstcsBase[j].dMinRa);
            Write_Double(cscs.cstcsBase[j].dMinRs);

            m_iTotLineNum++;
            iCount++;
        }//endof for(j) section
        if(iCount > 0) m_CscsColumnKList.Add(ElemK);
    }//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//(正截面轴/偏心抗拉验算)Axes/eccentricity tensile stress check of orthogonal section at service stage
//add by maxiao. see [JTG D62--2004] 5.4.1~5.4.2 P40  传入的拉力为正,在输出时转换符号
BOOL CRCImportFile_CH::Write_Ctcs()
{
    BOOL bPrintTitle = TRUE;  
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_axes_eccentricity_tensile_stress")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CTCS_D_W);
    sizeResu = sizeof(T_CSTCS_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++)
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		//0.get design matl of checking ELEM
		m_matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl))	continue;
        int iCount = 0;
        //get force
		T_CSTCS_CH ctcs;	ctcs.Initialize();
        if (!m_DataCtrl.Get_CHRcCstcs(ElemK,FALSE,ctcs)) continue;
        for (int j=0;j<10;j++) 
		{
            if (!ctcs.cstcsBase[j].bCHK) continue;
			//1:get section of checking ELEM
			_DGN_RC_COLUMN_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_ColumnSection(ElemK,j/5,sectioni)) continue;

			ctcs.cstcsBase[j].iMembType = m_rcD.iCrackType;
            //!/
            _DGN_CTCS_D_W DgnCtcsD;
            DgnCtcsD.Initialize();
			DgnCtcsD.nElemK = ElemK;
			DgnCtcsD.nPart = j;
			DgnCtcsD.RcMatlCH = m_matl;
			DgnCtcsD.SectionCH = sectioni;
			//
            ctcs.cstcsBase[j].drNd = m_rcD.iSafeType * ctcs.cstcsBase[j].dNd;
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCtcsD, &ctcs.cstcsBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================
            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/5,4);//true:print to report-data;4-抗拉
            if (bPrintTitle && bPrinti) 
			{
                m_outfile.WriteString(_T("**BEGIN正截面轴/偏心抗拉验算:\n"));
                bPrintTitle = FALSE;
            } 
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

        }//end for(j) 
        for (int j =0;j<10;j++) 
		{
            m_DataCtrl.Get_CHRCCstcsBaseCodeToCurr(ctcs.cstcsBase[j]);  //change UNIT

            if (!ctcs.cstcsBase[j].bCHK) continue;
            Write_Command(_T("CTCS_JT"));
            Write_Int(ElemK);
            Write_Int(j/5);//截面
            Write_Int(j%5);//位置标示
            Write_Int(ctcs.cstcsBase[j].LcomK);
            Write_Double(-ctcs.cstcsBase[j].dNd);
            Write_Double(-ctcs.cstcsBase[j].drNd);
            Write_Double(-ctcs.cstcsBase[j].drNde);
            Write_Double(-ctcs.cstcsBase[j].drNde1);
            Write_Double(ctcs.cstcsBase[j].dNn);
            Write_Double(ctcs.cstcsBase[j].dNne);
            Write_Double(ctcs.cstcsBase[j].dNne1);

            Write_Double(ctcs.cstcsBase[j].dRs);
            Write_Double(ctcs.cstcsBase[j].dMinRs);
            m_iTotLineNum++;
            iCount++;
        }//endof for(j) out-data   
        if(iCount > 0) m_CtcsColumnKList.Add(ElemK);
    }//endof for(i) ELEM
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//Beam Required Longitudinal Reinforcement: see 设计原理 p82 P89 P99
BOOL CRCImportFile_CH::Write_Brlr()
{
    BOOL bPrintTitle = TRUE;
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_required_longitudinal_reinforcement")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_BRLR_D_W);
    sizeResu = sizeof(T_BRLR_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
        m_matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl))  continue;
        T_BRLR_CH brlr; brlr.Initialize();
        if (!m_DataCtrl.Get_CHRcBrlr(ElemK,brlr)) continue;
        int iCount = 0;
        for (int j=0;j<4;j++) 
		{
            if (!brlr.BrlrBase[j].bCHK) continue;
			//1:get section of checking ELEM
			_DGN_RC_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_BeamSection(ElemK,j/2,sectioni)) continue;
            if (sectioni.dh0-sectioni.das1 <=0.0 ) continue;
            if (m_matl.dfsd<=0.0 || m_matl.dfsd1<=0.0 )continue;

            //!/
            _DGN_BRLR_D_W DgnBrlrD;
            DgnBrlrD.Initialize();
			DgnBrlrD.nElemK = ElemK;
			DgnBrlrD.nPart = j;
			DgnBrlrD.RcMatlCH = m_matl;
			DgnBrlrD.SectionCH = sectioni;
			DgnBrlrD.dGamma0  = m_rcD.iSafeType;//double
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnBrlrD, &brlr.BrlrBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================

            BOOL bPrinti = m_DataCtrl.Get_PrintOption(ElemK,j/2,j%2==0 ? 6:7);//true:print to report-data;
            if (bPrintTitle && bPrinti) {
                m_outfile.WriteString(_T("**BEGIN梁的纵向钢筋用量（截面面积）估算:\n"));
                bPrintTitle = FALSE;
            }
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

        }//endof for(j) section 
		for (int j = 0; j<4;j++) 
		{
            m_DataCtrl.Get_CHRCBrlrBaseCodeToCurr(brlr.BrlrBase[j]);  //change UNIT
            if (!brlr.BrlrBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iMax = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("BRLR_JT"));
			Write_Int(ElemK);
			Write_Int(iPos);
			Write_Int(iMax);
			Write_Int(brlr.BrlrBase[j].LcomK);
			Write_Int(brlr.BrlrBase[j].iMax);
			Write_Double(brlr.BrlrBase[j].dMd);
			Write_Double(brlr.BrlrBase[j].dREQTop);
			Write_Double(brlr.BrlrBase[j].dREQBot);
			Write_Double(brlr.BrlrBase[j].dUSETop);
			Write_Double(brlr.BrlrBase[j].dUSEBot);
			Write_Double(brlr.BrlrBase[j].dRho); 
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_BrlrKList.Add(ElemK);
    }//endof for(i) ELEM  
    if (!bPrintTitle) m_outfile.WriteString(_T("**END\n"));
    return TRUE;
}

//Beam Required comm rebar: see [JTG D62-2004] 5.2.7-2  P28..let Vcs = 0.6*γ0*Vd
BOOL CRCImportFile_CH::Write_Brcr()
{
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_required_comm_rebar")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_BRCR_D_W);
    sizeResu = sizeof(T_BRCR_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++)
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) 	continue;
        T_BRCR_CH brcr;
		if (!m_DataCtrl.Get_CHRcBrcr(ElemK,brcr)) 	continue;
        int iCount = 0;
        for (int j=0;j<4;j++) 
		{
            if (!brcr.BrcrBase[j].bCHK) continue;
			//1:get section of checking ELEM
			_DGN_RC_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_BeamSection(ElemK,j/2,sectioni))	continue;

            //!/
            _DGN_BRCR_D_W DgnBrcrD;
            DgnBrcrD.Initialize();
			DgnBrcrD.nElemK = ElemK;
			DgnBrcrD.nPart = j;
			DgnBrcrD.RcMatlCH = m_matl;
			DgnBrcrD.SectionCH = sectioni;
			DgnBrcrD.dGamma0  = m_rcD.iSafeType;//double
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnBrcrD, &brcr.BrcrBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================
			BOOL bPrinti = FALSE;
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

        }//endof for(j)
		for (int j = 0; j<4;j++) 
		{
            m_DataCtrl.Get_CHRCBrcrBaseCodeToCurr(brcr.BrcrBase[j]);  //change UNIT

            if (!brcr.BrcrBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iMax = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("BRCR_JT"));
			Write_Int(ElemK);
			Write_Int(iPos);
			Write_Int(iMax);
			Write_Int(brcr.BrcrBase[j].LcomK);
			Write_Int(brcr.BrcrBase[j].iMax);
			Write_Double(brcr.BrcrBase[j].dVd);
			Write_Double(brcr.BrcrBase[j].dREQ);
			Write_Double(brcr.BrcrBase[j].dUSE);
            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_BrcrKList.Add(ElemK);
    }//endof for(i) ELEM
    return TRUE;
}

//(柱的纵向钢筋用量)Column Required Longitudinal Reinforcement
//see [设计原理]  P162  P198
BOOL CRCImportFile_CH::Write_Crlr()
{
    //!/
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Check_column_required_longitudinal_rebar")); 
	//==================================================================================================

    size_t sizeIn, sizeResu;
    sizeIn  = sizeof(_DGN_CRLR_D_W);
    sizeResu = sizeof(T_CRLR_CH_BASE);

    for (int i=0; i<m_ElemKList.GetSize(); i++) 
	{
        T_ELEM_K ElemK = m_ElemKList.GetAt(i); 
        //0.get design matl of checking ELEM
		m_matl.Initialize();
		if (!m_DataCtrl.Get_Matl(ElemK,m_matl)) 
		{
			continue;
		}
		CString Elemi =  _T("");
		Elemi.Format(_T("%d"), ElemK);
        T_CRLR_CH crlr; crlr.Initialize();
        if (!m_DataCtrl.Get_CHRcCrlr(ElemK,crlr)) 	continue;

		int iCount = 0;
        for (int j=0;j<4;j++) 
		{
			//1:get section of checking ELEM
			_DGN_RC_COLUMN_SECTION_CH_W sectioni; sectioni.Initialize();
			if (!m_DataCtrl.Get_ColumnSection(ElemK,j/2,sectioni))		continue;
            //!/
            _DGN_CRLR_D_W DgnCrlrD;
            DgnCrlrD.Initialize();
			DgnCrlrD.nElemK = ElemK;
			DgnCrlrD.nPart = j;
			DgnCrlrD.RcMatlCH = m_matl;
			DgnCrlrD.SectionCH = sectioni;
			DgnCrlrD.dGamma0   = m_rcD.iSafeType;//double
            //
            T_DETAIL_STR DetailStr;
            DetailStr.Initialize();

            BOOL bVersionOK=FALSE;
            //==================================================================================================
	    BOOL bCheck = (*pDgnCalc)(&DgnCrlrD, &crlr.CrlrBase[j], &DetailStr, sizeIn, sizeResu, bVersionOK);
	    if(!bVersionOK)
	    {
		    AfxMessageBox(_T("Conversion of Design data failed"));
		    continue;
	    }
            //==================================================================================================
			BOOL bPrinti = FALSE;
			if (bPrinti) 
			{
				for (int p=0; p<DetailStr.arString.GetSize(); p++)
				{
					m_outfile.WriteString(DetailStr.CW2A(p));
				}
			}

        }//endof for(j)
		for (int j = 0; j<4;j++)
		{
            m_DataCtrl.Get_CHRCCrlrBaseCodeToCurr(crlr.CrlrBase[j]);  //change UNIT
            if (!crlr.CrlrBase[j].bCHK) continue;
			int iPos = (j==0 || j==1 ? 1 : 2);
			int iMax = (j==0 || j==2 ? 1 : 2);
			Write_Command(_T("CRLR_JT"));
			Write_Int(ElemK);
			Write_Int(iPos);
			Write_Int(iMax);
			Write_Int(crlr.CrlrBase[j].LcomK);
			Write_Int(crlr.CrlrBase[j].iMax);
			Write_Double(crlr.CrlrBase[j].dNd);
			Write_Double(crlr.CrlrBase[j].dMyd);
			Write_Double(crlr.CrlrBase[j].dMzd);
			Write_Double(crlr.CrlrBase[j].dREQTop);
			Write_Double(crlr.CrlrBase[j].dREQLeft);
			Write_Double(crlr.CrlrBase[j].dUSETop);
			Write_Double(crlr.CrlrBase[j].dUSELeft);

            m_iTotLineNum++;
            iCount++;
		}
        if(iCount > 0) m_CrlrKList.Add(ElemK);
    }//endof for(i) ELEM
    return TRUE;
}

BOOL CRCImportFile_CH::MakeData( BOOL isBeam)
{	
    m_DataCtrl.Initialize();
    if (!m_DataCtrl.Get_Rcd(m_rcD))
	{ 
        return FALSE;
    }
    if(!(m_rcD.iDgnCode == RC_JTG_D62_04 || m_rcD.iDgnCode == RC_CJJ_11_2011))
	{
        GSaveHistoryFormatNF(_T("[错误] RC设计参数中设计规范选择的不是JTG D62 -2004."));
        return FALSE;
    }
    if (isBeam)
	{
        if (!(m_rcD.bDesignPrint[2] || m_rcD.bDesignPrint[3] || m_rcD.bDesignPrint[4] ||
            m_rcD.bDesignPrint[5] || m_rcD.bDesignPrint[6] || m_rcD.bDesignPrint[7] || m_rcD.bDesignPrint[8])) 
		{
            GSaveHistoryFormatNF(_T("[错误] RC设计参数中没有选择梁的设计项."));
            return FALSE;
        }
    }
	else
	{
        if (!(m_rcD.bDesignPrint[3] || m_rcD.bDesignPrint[8] || m_rcD.bDesignPrint[10] || m_rcD.bDesignPrint[11]))
		{
            GSaveHistoryFormatNF(_T("[错误] RC设计参数中没有选择柱的设计项."));
            return FALSE;
        }
    }  
    m_ElemKList.RemoveAll();
    if (!m_DataCtrl.Get_CrcElemListForDgn(FALSE,m_ElemKList,isBeam)) 
	{
        return FALSE;
    }
    for (int i=0; i<m_ElemKList.GetSize();i++ ) 
	{
        if (!m_DataCtrl.Get_Matl(m_ElemKList.GetAt(i),m_matl))
		{
            GSaveHistoryFormatNF(_T("[错误] RC设计单元材料选择不正确."));
            return FALSE;
        }
        for (int j=0;j<2;j++) 
		{
            if (isBeam) {
                _DGN_RC_SECTION_CH sectioni; sectioni.Initialize();    
                if (!m_DataCtrl.Get_BeamSection(m_ElemKList.GetAt(i),j,sectioni))
				{
                    GSaveHistoryFormatNF(_T("[错误] RC设计梁单元 %d 截面或配筋数据错误."),m_ElemKList.GetAt(i));
                    return FALSE;
                }
            }
			else
			{
                _DGN_RC_COLUMN_SECTION_CH sectioni; sectioni.Initialize();  
                if (!m_DataCtrl.Get_ColumnSection(m_ElemKList.GetAt(i),j,sectioni))
				{
                    return FALSE;
                }
            }
        }//end for (j)
    }
    //+++++++++++++++++++++
    if(!m_DataCtrl.Get_LcomDataMap(m_DgnLcomKMap))	return FALSE;
//	// Get Forces for Design.
	if(!m_DataCtrl.Get_ForcDataMap(FALSE,m_ComfDMap))	return FALSE;
    return TRUE;
}
