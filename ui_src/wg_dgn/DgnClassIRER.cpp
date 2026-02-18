// DgnClassIRED.cpp : implementation file
// IDEA RS EXPORT : Read Class

#include "stdafx.h"
#include "DgnClassIRED.h"
#include "DgnClassIRER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnClassIRER::CDgnClassIRER()
{
	nVersion = 0;
}

CDgnClassIRER::~CDgnClassIRER()
{
	
}

int CDgnClassIRER::GetVersion()
{
	return nVersion;
}

void CDgnClassIRER::SetVersion(int nVer)
{
	nVersion = nVer;
}

BOOL CDgnClassIRER::ReadFile(CArchive&  ar, CDgnClassIRED&  cData)
{
	cData.Initialize();
	int nVer(0);
	int nMapNum[S_IDEA_NUM];
	memset(nMapNum, 0, sizeof(nMapNum));
	CString szMCTPath(_T(""));

	TRY
	{    
		
		if (ReadBASC(ar, &nMapNum[0], nVer, szMCTPath) == FALSE) throw new CArchiveException();
		SetVersion(nVer);
		cData.SetVersion(nVer);
		cData.SetMCTPath(szMCTPath);
		cData.SetMapSize(nMapNum);
		
		S_UNIT_D  UnitD;
		if (ReadUNIT(ar, UnitD) == FALSE) throw new CArchiveException();
		cData.SetUnit(UnitD);

		int i(0), nSize(0);

		S_NODE_K  NodeK;
		S_NODE_D  NodeD;
		nSize = nMapNum[S_IDEA_NODE];
		for (i=0; i<nSize; i++)
		{
			if (ReadNODE(ar, NodeK, NodeD) == FALSE) throw new CArchiveException();
			if (cData.AddNode(NodeK, NodeD) == FALSE) throw new CArchiveException();
		}

		S_ELEM_K  ElemK;
		S_ELEM_D  ElemD;
		nSize = nMapNum[S_IDEA_ELEM];
		for (i=0; i<nSize; i++)
		{
			if (ReadELEM(ar, ElemK, ElemD) == FALSE) throw new CArchiveException();
			if (cData.AddElem(ElemK, ElemD) == FALSE) throw new CArchiveException();
		}

		S_MEMB_K  MembK;
		S_MEMB_D  MembD;
		nSize = nMapNum[S_IDEA_MEMB];
		for (i=0; i<nSize; i++)
		{
			if (ReadMEMB(ar, MembK, MembD) == FALSE) throw new CArchiveException();
			if (cData.AddMemb(MembK, MembD) == FALSE) throw new CArchiveException();
		}
		
		S_MATL_STEEL_K  MtstK;
		S_MATL_STEEL_D  MtstD;
		nSize = nMapNum[S_IDEA_MTST];
		for (i=0; i<nSize; i++)
		{
			if (ReadMTST(ar, MtstK, MtstD) == FALSE) throw new CArchiveException();
			if (cData.AddMatlST(MtstK, MtstD) == FALSE) throw new CArchiveException();
		}
		
		S_MATL_CONCRETE_K  MtccK;
		S_MATL_CONCRETE_D  MtccD;
		nSize = nMapNum[S_IDEA_MTCN];
		for (i=0; i<nSize; i++)
		{
			if (ReadMTCC(ar, MtccK, MtccD) == FALSE) throw new CArchiveException();
			if (cData.AddMatlCN(MtccK, MtccD) == FALSE) throw new CArchiveException();
		}
		
		S_MATL_REIF_K      RebrK;
		S_MATL_REBAR_INFO  RebrD;
		nSize = nMapNum[S_IDEA_MTRB];
		for (i=0; i<nSize; i++)
		{
			if (ReadMTRB(ar, RebrK, RebrD) == FALSE) throw new CArchiveException();
			if (cData.AddMatlRB(RebrK, RebrD) == FALSE) throw new CArchiveException();
		}
		
		S_MATL_ADIF_K  MtadK;
		S_MATL_ADIF_D  MtadD;
		nSize = nMapNum[S_IDEA_MATL];
		for (i=0; i<nSize; i++)
		{
			if (ReadMTAD(ar, MtadK, MtadD) == FALSE) throw new CArchiveException();
			if (cData.AddMatlAD(MtadK, MtadD) == FALSE) throw new CArchiveException();
		}
			
		S_SECT_K  SectK;
		S_SECT_D  SectD;
		nSize = nMapNum[S_IDEA_SECT];
		for (i=0; i<nSize; i++)
		{
			if (ReadSECT(ar, SectK, SectD) == FALSE) throw new CArchiveException();
			if (cData.AddSect(SectK, SectD) == FALSE) throw new CArchiveException();
		}
		
		S_ELST_D  ElstD;
		nSize = nMapNum[S_IDEA_ELST];
		for (i=0; i<nSize; i++)
		{
			if (ReadELST(ar, ElemK, SectK, ElstD) == FALSE) throw new CArchiveException();
			if (cData.AddElst(ElemK, SectK, ElstD) == FALSE) throw new CArchiveException();
		}

		if (cData.GetVersion() > D_IDEARSFILE_110)
		{
			S_PART_K  PartK;
			S_CSET_D  CsetD;
			nSize = nMapNum[S_IDEA_CSET];
			for (i=0; i<nSize; i++)
			{
				if (ReadCSET(ar, ElemK, SectK, PartK, CsetD) == FALSE) throw new CArchiveException();
				if (cData.AddCset(ElemK, SectK, PartK, CsetD) == FALSE) throw new CArchiveException();
			}
		}

		// for 2D Element
		// 101 버전보가 클 경우에만 2D 요소와 관련된 정보를 읽는다.
		if (cData.GetVersion() > D_IDEARSFILE_101)
		{
			S_THIK_K  ThikK;
			S_THIK_D  ThikD;
			nSize = nMapNum[S_IDEA_THIK];
			for (i=0; i<nSize; i++)
			{        
				if (ReadTHIK(ar, ThikK, ThikD) == FALSE) throw new CArchiveException();
				if (cData.AddThik(ThikK, ThikD) == FALSE) throw new CArchiveException();
			}    
		}        
				 
		S_CONS_K  ConsK;
		S_CONS_D  ConsD;
		nSize = nMapNum[S_IDEA_CONS];
		for (i=0; i<nSize; i++)
		{
			if (ReadCONS(ar, ConsK, ConsD) == FALSE) throw new CArchiveException();
			if (cData.AddCons(ConsK, ConsD) == FALSE) throw new CArchiveException();
		}
		
		S_STLD_K  StldK;
		S_STLD_D  StldD;
		nSize = nMapNum[S_IDEA_STLD];
		for (i=0; i<nSize; i++)
		{
			if (ReadSTLD(ar, StldK, StldD) == FALSE) throw new CArchiveException();
			if (cData.AddStld(StldK, StldD) == FALSE) throw new CArchiveException();
		}
		
		S_LCOM_T  LcomT;
		S_LCOM_K  LcomK;
		S_LCOM_D  LcomD;
		nSize = nMapNum[S_IDEA_LCOM];
		for (i=0; i<nSize; i++)
		{
			if (ReadLCOM(ar, LcomT, LcomK, LcomD) == FALSE) throw new CArchiveException();
			if (cData.AddLcom(LcomT, LcomK, LcomD) == FALSE) throw new CArchiveException();
		}
			
		S_LENG_K  LengK;
		S_LENG_D  LengD;
		nSize = nMapNum[S_IDEA_LENG];
		for (i=0; i<nSize; i++)
		{
			if (ReadLENG(ar, LengK, LengD) == FALSE) throw new CArchiveException();
			if (cData.AddLeng(LengK, LengD) == FALSE) throw new CArchiveException();
		}
	
		S_KFAC_K  KfacK;
		S_KFAC_D  KfacD;
		nSize = nMapNum[S_IDEA_KFAC];
		for (i=0; i<nSize; i++)
		{
			if (ReadKFAC(ar, KfacK, KfacD) == FALSE) throw new CArchiveException();
			if (cData.AddKfac(KfacK, KfacD) == FALSE) throw new CArchiveException();
		}
		
		S_LTSR_K  LtsrK;
		S_LTSR_D  LtsrD;
		nSize = nMapNum[S_IDEA_LTSR];
		for (i=0; i<nSize; i++)
		{
			if (ReadLTSR(ar, LtsrK, LtsrD) == FALSE) throw new CArchiveException();
			if (cData.AddLtsr(LtsrK, LtsrD) == FALSE) throw new CArchiveException();
		}
		
		S_CMFT_K  CmftK;
		S_CMFT_D  CmftD;
		nSize = nMapNum[S_IDEA_CMFT];
		for (i=0; i<nSize; i++)
		{
			if (ReadCMFT(ar, CmftK, CmftD) == FALSE) throw new CArchiveException();
			if (cData.AddCmft(CmftK, CmftD) == FALSE) throw new CArchiveException();
		}
		
		S_FMAG_K  FmagK;
		S_FMAG_D  FmagD;
		nSize = nMapNum[S_IDEA_FMAG];
		for (i=0; i<nSize; i++)
		{
			if (ReadFMAG(ar, FmagK, FmagD) == FALSE) throw new CArchiveException();
			if (cData.AddFmag(FmagK, FmagD) == FALSE) throw new CArchiveException();
		}
	
		S_DCON_D  DconD;  
		if (ReadDCON(ar, DconD) == FALSE) throw new CArchiveException();
		if (cData.SetDcon(DconD) == FALSE) throw new CArchiveException();
	
		S_RDBM_K    RdbmK;
		S_RDGN_BEAM RdbmD;
		nSize = nMapNum[S_IDEA_RDBM];
		for (i=0; i<nSize; i++)
		{
			if (ReadRDBM(ar, RdbmK, RdbmD) == FALSE) throw new CArchiveException();
			if (cData.AddRdbm(RdbmK, RdbmD) == FALSE) throw new CArchiveException();
		}
		
		S_RDCN_K    RdcnK;
		S_RDGN_COLM RdcnD;
		nSize = nMapNum[S_IDEA_RDCN];
		for (i=0; i<nSize; i++)
		{
			if (ReadRDCN(ar, RdcnK, RdcnD) == FALSE) throw new CArchiveException();
			if (cData.AddRdcn(RdcnK, RdcnD) == FALSE) throw new CArchiveException();
		}
		
		S_RCBM_K     RcbmK;
		S_RCHK_BEAM  RcbmD;
		nSize = nMapNum[S_IDEA_RCBM];
		for (i=0; i<nSize; i++)
		{
			if (ReadRCBM(ar, RcbmK, RcbmD) == FALSE) throw new CArchiveException();
			if (cData.AddRcbm(RcbmK, RcbmD) == FALSE) throw new CArchiveException();
		}
		
		S_RCCN_K     RccnK;
		S_RCHK_COLM  RccnD;
		nSize = nMapNum[S_IDEA_RCCN];
		for (i=0; i<nSize; i++)
		{
			if (ReadRCCN(ar, RccnK, RccnD) == FALSE) throw new CArchiveException();
			if (cData.AddRccn(RccnK, RccnD) == FALSE) throw new CArchiveException();
		}
			
		S_REBT_K  RebtK;
		S_REBT_D  RebtD;
		nSize = nMapNum[S_IDEA_REBT];
		for (i=0; i<nSize; i++)
		{
			if (ReadREBT(ar, RebtK, RebtD) == FALSE) throw new CArchiveException();
			if (cData.AddRebt(RebtK, RebtD) == FALSE) throw new CArchiveException();
		}
		
		S_RPSC_K      RpscK;
		S_RPSC_SSM_D  RpscD;
		nSize = nMapNum[S_IDEA_RPSC];
		for (i=0; i<nSize; i++)
		{
			if (ReadRPSC(ar, RpscK, RpscD) == FALSE) throw new CArchiveException();
			if (cData.AddRpsc(RpscK, RpscD) == FALSE) throw new CArchiveException();
		}
		
		S_TDIF_K  TdifK;
		S_TDIJ_K  TdIJ;
		S_TDIF_D  TdifD;
		nSize = nMapNum[S_IDEA_TDIF];
		for (i=0; i<nSize; i++)
		{
			if (ReadTDIF(ar, TdifK, TdIJ, TdifD) == FALSE) throw new CArchiveException();
			if (cData.AddTdif(TdifK, TdIJ, TdifD) == FALSE) throw new CArchiveException();
		}
		
		// for Construction Stage
		// 110 버전엔 2D 요소만 추가한다. 따라서 110보다 클 경우에만 시공단게결과를 읽는다.
		if (cData.GetVersion() > D_IDEARSFILE_110)
		{
			// LoadCase      
			nSize = nMapNum[S_IDEA_CSLC];
			for (i=0; i<nSize; i++)
			{
				if (ReadCSLC(ar, StldK, StldD) == FALSE) throw new CArchiveException();
				if (cData.AddCslc(StldK, StldD) == FALSE) throw new CArchiveException();        
			}
			// Auto Load Case
			nSize = nMapNum[S_IDEA_CLAT];
			for (i=0; i<nSize; i++)
			{
				if (ReadCLAT(ar, StldK, StldD) == FALSE) throw new CArchiveException();
				if (cData.AddClat(StldK, StldD) == FALSE) throw new CArchiveException();        
			}
			// Stage Information
			S_STAG_K  StagK;
			S_STAG_D  StagD;
			nSize = nMapNum[S_IDEA_STAG];
			for (i=0; i<nSize; i++)
			{
				if (ReadSTAG(ar, StagK, StagD) == FALSE) throw new CArchiveException();
				if (cData.AddStag(StagK, StagD) == FALSE) throw new CArchiveException();        
			}
			// Composite Section For Construction Stage
			S_CSCS_K  CscsK;
			S_CSCS_D  CscsD;      
			nSize = nMapNum[S_IDEA_CSCS];
			for (i=0; i<nSize; i++)
			{
				if (ReadCSCS(ar, CscsK, CscsD) == FALSE) throw new CArchiveException();
				if (cData.AddCscs(CscsK, CscsD) == FALSE) throw new CArchiveException();        
			}
			// Structure Group
			S_GRUP_K  GrupK;
			S_GRUP_D  GrupD;
			nSize = nMapNum[S_IDEA_EGRP];
			for (i=0; i<nSize; i++)
			{
				if (ReadGRUP(ar, GrupK, GrupD) == FALSE) throw new CArchiveException();
				if (cData.AddGrup(GrupK, GrupD) == FALSE) throw new CArchiveException();        
			}
			// Boundary Group
			S_BNGR_K  BngrK;
			S_BNGR_D  BngrD;
			nSize = nMapNum[S_IDEA_BGRP];
			for (i=0; i<nSize; i++)
			{
				if (ReadBNGR(ar, BngrK, BngrD) == FALSE) throw new CArchiveException();
				if (cData.AddBngr(BngrK, BngrD) == FALSE) throw new CArchiveException();        
			}
			// Load Group
			S_LDGR_K  LdgrK;
			S_LDGR_D  LdgrD;
			nSize = nMapNum[S_IDEA_LGRP];
			for (i=0; i<nSize; i++)
			{
				if (ReadLDGR(ar, LdgrK, LdgrD) == FALSE) throw new CArchiveException();
				if (cData.AddLdgr(LdgrK, LdgrD) == FALSE) throw new CArchiveException();        
			}

			S_TDPF_K  TdpfK;
			S_STEP_K  StepK;
			S_TDLS_D  TdlsD;
			nSize = nMapNum[S_IDEA_TDLS];
			for (i=0; i<nSize; i++)
			{
				if (ReadTDLS(ar, TdpfK, StagK, StepK, TdlsD) == FALSE) throw new CArchiveException();
				if (cData.AddTdls(TdpfK, StagK, StepK, TdlsD) == FALSE) throw new CArchiveException();
			}
		}

		S_REAC_D  ReacD;
		S_LDTP_K  LdtpK;
		nSize = nMapNum[S_IDEA_REAC];
		for (i=0; i<nSize; i++)
		{
			if (ReadREAC(ar, NodeK, StldK, LdtpK, ReacD) == FALSE) throw new CArchiveException();
			if (cData.AddReac(NodeK, StldK, LdtpK, ReacD) == FALSE) throw new CArchiveException();
		}
	
		S_DISP_D  DispD;
		nSize = nMapNum[S_IDEA_DISP];
		for (i=0; i<nSize; i++)
		{
			if (ReadDISP(ar, NodeK, StldK, LdtpK, DispD) == FALSE) throw new CArchiveException();
			if (cData.AddDisp(NodeK, StldK, LdtpK, DispD) == FALSE) throw new CArchiveException();
		}
	
		S_TFOR_D  TforD;
		nSize = nMapNum[S_IDEA_TFOR];
		for (i=0; i<nSize; i++)
		{
			if (ReadTFOR(ar, ElemK, StldK, LdtpK, TforD) == FALSE) throw new CArchiveException();
			if (cData.AddTfor(ElemK, StldK, LdtpK, TforD) == FALSE) throw new CArchiveException();
		}
	
		S_BFOR_D  BforD;
		nSize = nMapNum[S_IDEA_BFOR];
		for (i=0; i<nSize; i++)
		{
			if (ReadBFOR(ar, ElemK, StldK, LdtpK, BforD) == FALSE) throw new CArchiveException();
			if (cData.AddBfor(ElemK, StldK, LdtpK, BforD) == FALSE) throw new CArchiveException();
		}
		
		S_STRP_DL  StrpD;
		if (cData.GetVersion() > D_IDEARSFILE_101)
		{      
			nSize = nMapNum[S_IDEA_PFOR];
			for (i=0; i<nSize; i++)
			{
				if (ReadSTRP(ar, ElemK, StldK, LdtpK, StrpD) == FALSE) throw new CArchiveException();
				if (cData.AddStrp(ElemK, StldK, LdtpK, StrpD) == FALSE) throw new CArchiveException();
			}
		}

		S_RECC_D  ReccD;
		nSize = nMapNum[S_IDEA_RECC];
		for (i=0; i<nSize; i++)
		{
			if (ReadRECC(ar, NodeK, StldK, LdtpK, ReccD) == FALSE) throw new CArchiveException();
			if (cData.AddRecc(NodeK, StldK, LdtpK, ReccD) == FALSE) throw new CArchiveException();
		}
		
		S_DSPC_D  DspcD;
		nSize = nMapNum[S_IDEA_DSPC];
		for (i=0; i<nSize; i++)
		{
			if (ReadDSPC(ar, NodeK, StldK, LdtpK, DspcD) == FALSE) throw new CArchiveException();
			if (cData.AddDspc(NodeK, StldK, LdtpK, DspcD) == FALSE) throw new CArchiveException();
		}
		
		S_BCFO_B  BcfoD;
		nSize = nMapNum[S_IDEA_BCFO];
		for (i=0; i<nSize; i++)
		{
			if (ReadBCFO(ar, ElemK, StldK, LdtpK, BcfoD) == FALSE) throw new CArchiveException();
			if (cData.AddBcfo(ElemK, StldK, LdtpK, BcfoD) == FALSE) throw new CArchiveException();
		}  

		// 110 버전엔 2D 요소만 추가한다. 따라서 110보다 클 경우에만 시공단게결과를 읽는다.
		if (cData.GetVersion() > D_IDEARSFILE_110)
		{
			nSize = nMapNum[S_IDEA_CSDP];
			S_DISP_CSD  DispCSD;
			for (i=0; i<nSize; i++)
			{
				if (cData.GetVersion() < D_IDEARSFILE_201)
				{
					DispCSD.aDisp.SetSize(1);
					if (ReadDISP(ar, NodeK, StldK, LdtpK, DispD) == FALSE) throw new CArchiveException();
					DispCSD.aDisp[0] = DispD;
				}
				else
				{
					if (ReadDISPCS(ar, NodeK, StldK, LdtpK, DispCSD) == FALSE) throw new CArchiveException();
				}
				if (cData.AddDispCS(NodeK, StldK, LdtpK, DispCSD) == FALSE) throw new CArchiveException();
			}
			
			nSize = nMapNum[S_IDEA_CSTF];
			S_TFOR_CSD  TForCSD;
			for (i=0; i<nSize; i++)
			{        
				if (cData.GetVersion() < D_IDEARSFILE_201)
				{
					TForCSD.aTFOR.SetSize(1);
					if (ReadTFOR(ar, ElemK, StldK, LdtpK, TforD) == FALSE) throw new CArchiveException();
					TForCSD.aTFOR[0] = TforD;
				}
				else
				{
					if (ReadTFORCS(ar, ElemK, StldK, LdtpK, TForCSD) == FALSE) throw new CArchiveException();
				}
				if (cData.AddTforCS(ElemK, StldK, LdtpK, TForCSD) == FALSE) throw new CArchiveException();
			}
			
			nSize = nMapNum[S_IDEA_CSBF];
			S_BFOR_CSD  BForCSD;
			for (i=0; i<nSize; i++)
			{        
				if (cData.GetVersion() < D_IDEARSFILE_201)
				{
					BForCSD.aBFOR.SetSize(1);
					if (ReadBFOR(ar, ElemK, StldK, LdtpK, BforD) == FALSE) throw new CArchiveException();
					BForCSD.aBFOR[0] = BforD;
				}
				else
				{
					if (ReadBFORCS(ar, ElemK, StldK, LdtpK, BForCSD) == FALSE) throw new CArchiveException();
				}

				if (cData.AddBforCS(ElemK, StldK, LdtpK, BForCSD) == FALSE) throw new CArchiveException();
			}
						
			S_STRP_CSDL StrpCSD;
			nSize = nMapNum[S_IDEA_CSPF];
			for (i=0; i<nSize; i++)
			{
				if (cData.GetVersion() < D_IDEARSFILE_201)
				{
					StrpCSD.aSTRP.SetSize(1);
					if (ReadSTRP(ar, ElemK, StldK, LdtpK, StrpD) == FALSE) throw new CArchiveException();
					StrpCSD.aSTRP[0] = StrpD;
				}
				else
				{
					if (ReadSTRPCS(ar, ElemK, StldK, LdtpK, StrpCSD) == FALSE) throw new CArchiveException();
				}
				
				if (cData.AddStrpCS(ElemK, StldK, LdtpK, StrpCSD) == FALSE) throw new CArchiveException();
			}      
		}

		if (cData.GetVersion() >= D_IDEARSFILE_210)
		{
			S_BFOR_PT BForPT;
			nSize = nMapNum[S_IDEA_EPTB];
			for (i=0; i<nSize; i++)
			{
				if (ReadBFORPT(ar, ElemK, StldK, LdtpK, BForPT) == FALSE) throw new CArchiveException();        
				if (cData.AddBforPT(ElemK, StldK, LdtpK, BForPT) == FALSE) throw new CArchiveException();
			}      

			S_BFOR_PTCC BForPTCC;
			nSize = nMapNum[S_IDEA_EPTC];
			for (i=0; i<nSize; i++)
			{
				if (ReadBFORPTCC(ar, ElemK, StldK, LdtpK, BForPTCC) == FALSE) throw new CArchiveException();        
				if (cData.AddBforPTCC(ElemK, StldK, LdtpK, BForPTCC) == FALSE) throw new CArchiveException();
			}      

			S_BFOR_PTCS BForPTCS;
			nSize = nMapNum[S_IDEA_EPCS];
			for (i=0; i<nSize; i++)
			{
				if (ReadBFORPTCS(ar, ElemK, StldK, LdtpK, BForPTCS) == FALSE) throw new CArchiveException();        
				if (cData.AddBforPTCS(ElemK, StldK, LdtpK, BForPTCS) == FALSE) throw new CArchiveException();
			}
		}

		ASSERT(ar.GetFile()->GetLength() == ar.GetFile()->GetPosition());
	}
	CATCH (CArchiveException, e)
	{
		ASSERT(FALSE);
		e->ReportError();
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CDgnClassIRER::ReadBASC(CArchive&  ar, int* nASize, int& nVer, CString& szMCTPath)
{
	// Version  
	ar >> nVer; 
	if (nVer < D_IDEARSFILE_110)
	{
		ar >> nASize[S_IDEA_NODE]; // Numbder of Node
		ar >> nASize[S_IDEA_ELEM]; // Numbder of Element
		ar >> nASize[S_IDEA_MEMB]; // Numbder of Member
		ar >> nASize[S_IDEA_MATL]; // Material - Steel
		ar >> nASize[S_IDEA_MTST]; // Material - Concrete
		ar >> nASize[S_IDEA_MTCN]; // Material - Rebar
		ar >> nASize[S_IDEA_MTRB]; // Material - Added Information
		ar >> nASize[S_IDEA_SECT]; // SectD
		ar >> nASize[S_IDEA_ELST]; // Sect-Part
		ar >> nASize[S_IDEA_CONS]; // Constraint - Boundary
		ar >> nASize[S_IDEA_STLD]; // Load Case
		ar >> nASize[S_IDEA_LCOM]; // Load Combination
		ar >> nASize[S_IDEA_LENG]; // Unbraced Length
		ar >> nASize[S_IDEA_KFAC]; // Effective Length Factor(K)
		ar >> nASize[S_IDEA_LTSR]; // Limiting Slenderness Ratio
		ar >> nASize[S_IDEA_CMFT]; // Equivalent Moment Correction Factor(Cm)
		ar >> nASize[S_IDEA_FMAG]; // Moment Magnifier
		ar >> nASize[S_IDEA_RDBM]; // Beam Section Data for Design
		ar >> nASize[S_IDEA_RDCN]; // Column Section Data for Design
		ar >> nASize[S_IDEA_RCBM]; // Beam Section Data for Checking
		ar >> nASize[S_IDEA_RCCN]; // Column Section Data for Checking
		ar >> nASize[S_IDEA_REBT]; // Rebar Input for General Section
		ar >> nASize[S_IDEA_RPSC]; // Rebar for PSC
		ar >> nASize[S_IDEA_TDIF]; // endon of Cross section
		ar >> nASize[S_IDEA_REAC]; // Reaction NodeK + LoadCaseK
		ar >> nASize[S_IDEA_DISP]; // Displacement NodeK + LoadCaseK
		ar >> nASize[S_IDEA_TFOR]; // Truss Force ElemK + LoadCaseK 
		ar >> nASize[S_IDEA_BFOR]; // Beam Force ElemK + LoadCaseK  
		ar >> nASize[S_IDEA_RECC]; // Reaction NodeK * Loadcase ( Moving, Settlement )
		ar >> nASize[S_IDEA_DSPC]; // Displacment NodeK * Loadcase ( Moving, Settlement )
		ar >> nASize[S_IDEA_BCFO]; // Beam concurrent Force ElemK + LoadCaseK  
	}
	else
	{
		ar >> nASize[S_IDEA_NODE]; // Numbder of Node
		ar >> nASize[S_IDEA_ELEM]; // Numbder of Element
		ar >> nASize[S_IDEA_MEMB]; // Numbder of Member
		ar >> nASize[S_IDEA_MATL]; // Material - Steel
		ar >> nASize[S_IDEA_MTST]; // Material - Concrete
		ar >> nASize[S_IDEA_MTCN]; // Material - Rebar
		ar >> nASize[S_IDEA_MTRB]; // Material - Added Information
		ar >> nASize[S_IDEA_SECT]; // SectD
		ar >> nASize[S_IDEA_ELST]; // Sect-Part
		ar >> nASize[S_IDEA_CONS]; // Constraint - Boundary
		ar >> nASize[S_IDEA_STLD]; // Load Case
		ar >> nASize[S_IDEA_LCOM]; // Load Combination
		ar >> nASize[S_IDEA_LENG]; // Unbraced Length
		ar >> nASize[S_IDEA_KFAC]; // Effective Length Factor(K)
		ar >> nASize[S_IDEA_LTSR]; // Limiting Slenderness Ratio
		ar >> nASize[S_IDEA_CMFT]; // Equivalent Moment Correction Factor(Cm)
		ar >> nASize[S_IDEA_FMAG]; // Moment Magnifier
		ar >> nASize[S_IDEA_RDBM]; // Beam Section Data for Design
		ar >> nASize[S_IDEA_RDCN]; // Column Section Data for Design
		ar >> nASize[S_IDEA_RCBM]; // Beam Section Data for Checking
		ar >> nASize[S_IDEA_RCCN]; // Column Section Data for Checking
		ar >> nASize[S_IDEA_REBT]; // Rebar Input for General Section
		ar >> nASize[S_IDEA_RPSC]; // Rebar for PSC
		ar >> nASize[S_IDEA_TDIF]; // endon of Cross section
		ar >> nASize[S_IDEA_REAC]; // Reaction NodeK + LoadCaseK
		if (nASize[S_IDEA_REAC] != 0) nASize[S_IDEA_REAC] = 0;    // 110버전부터 반력은 삭제한다.
		ar >> nASize[S_IDEA_DISP]; // Displacement NodeK + LoadCaseK
		ar >> nASize[S_IDEA_TFOR]; // Truss Force ElemK + LoadCaseK 
		ar >> nASize[S_IDEA_BFOR]; // Beam Force ElemK + LoadCaseK  
		ar >> nASize[S_IDEA_RECC]; // Reaction NodeK * Loadcase ( Moving, Settlement )
		if (nASize[S_IDEA_RECC] != 0) nASize[S_IDEA_RECC] = 0;    // 110버전부터 반력은 삭제한다.
		ar >> nASize[S_IDEA_DSPC]; // Displacment NodeK * Loadcase ( Moving, Settlement )
		ar >> nASize[S_IDEA_BCFO]; // Beam concurrent Force ElemK + LoadCaseK  
		ar >> nASize[S_IDEA_THIK]; // 2D element Thickness
		ar >> nASize[S_IDEA_PFOR]; // 2D Element Result
		ar >> nASize[S_IDEA_CSLC]; // Construction Stage Load Case  
		ar >> nASize[S_IDEA_CLAT]; // Auto Generate LoadCase By Construction Stage Analaysis
		ar >> nASize[S_IDEA_STAG]; // Stage Information
		ar >> nASize[S_IDEA_CSCS]; // Composite Section for Construction Stage
		ar >> nASize[S_IDEA_EGRP]; // Element Group
		ar >> nASize[S_IDEA_BGRP]; // Boundary Group
		ar >> nASize[S_IDEA_LGRP]; // Load Group    
		ar >> nASize[S_IDEA_CSDP]; // Displacement Construction Stage
		ar >> nASize[S_IDEA_CSTF]; // Truss Force Construction Stgae
		ar >> nASize[S_IDEA_CSBF]; // Beam Force Constrution Stage
		ar >> nASize[S_IDEA_CSPF]; // Beam Force Constrution Stage

		if (nVer >= D_IDEARSFILE_200)
		{
			ar >> nASize[S_IDEA_CSET]; // Composite Section For Tapared
			ar >> nASize[S_IDEA_TDLS]; // Beam Force Constrution Stage      
		}
		if (nVer >= D_IDEARSFILE_210)
		{
			ar >> nASize[S_IDEA_EPTB];
			ar >> nASize[S_IDEA_EPTC];
			ar >> nASize[S_IDEA_EPCS];
		}
	}
	ar >> szMCTPath;

	return TRUE;
}

BOOL CDgnClassIRER::ReadUNIT(CArchive&  ar, S_UNIT_D&  UnitD)
{
	UnitD.Initialize();

	ar >> UnitD.nBase_Length;
	ar >> UnitD.nBase_Force;
	return TRUE;
}

BOOL CDgnClassIRER::ReadNODE(CArchive&  ar, UINT& nKey, S_NODE_D& NodeD)
{
	NodeD.Initialize();

	ar >> nKey;
	ar >> NodeD.dx;
	ar >> NodeD.dy;
	ar >> NodeD.dz;
	return TRUE;
}

BOOL CDgnClassIRER::ReadELEM(CArchive&ar, UINT& nKey, S_ELEM_D& ElemD)
{
	ElemD.Initialize();

	ar >> nKey;
	ar >> ElemD.nEltype;
	ar >> ElemD.MatlK;
	ar >> ElemD.SectK;
	ar >> ElemD.dAngle;
	for (int i=0; i<DT_ELEM_MAXNOD; i++) ar >> ElemD.NodeK[i];

	return TRUE;
}

BOOL CDgnClassIRER::ReadMEMB(CArchive&ar, UINT& nKey, S_MEMB_D& MembD)
{
	MembD.Initialize();
	
	ar >> nKey;
	ar >> MembD.nMbType;
	int i(0), nSize(0);    
	ar >> nSize;
	MembD.aElemK.SetSize(nSize);  
	for (i=0; i<nSize; i++) ar >> MembD.aElemK[i];
	ar >> MembD.bReverseDir;

	return TRUE;
}

BOOL CDgnClassIRER::ReadMtPr(CArchive&ar, S_MATL_PROP& MtPrD)
{
	MtPrD.Initialize();
	ar >> MtPrD.dElast;
	ar >> MtPrD.dPoisson;
	ar >> MtPrD.dThermal;
	ar >> MtPrD.dDensity;
	ar >> MtPrD.dMassDensity;
	ar >> MtPrD.dShearModulus;

	return TRUE;
}

BOOL CDgnClassIRER::ReadMTST(CArchive& ar, UINT& nKey, S_MATL_STEEL_D& MtstD)
{
	MtstD.Initialize();

	ar >> nKey;
	ar >> MtstD.szCode;
	ar >> MtstD.szMatName;
	ReadMtPr(ar, MtstD.PropD);  
	ar >> MtstD.dS_Fu;
	ar >> MtstD.dS_Fy;
	ar >> MtstD.dS_Fy1;
	ar >> MtstD.dS_Fy2;
	ar >> MtstD.dS_Fy3;
	ar >> MtstD.dS_Fy4;
	ar >> MtstD.dS_Fy5;
	ar >> MtstD.dS_Fy6;

	return TRUE;
}

BOOL CDgnClassIRER::ReadMTCC(CArchive& ar, UINT& nKey, S_MATL_CONCRETE_D& MtccD)
{
	MtccD.Initialize();

	ar >> nKey;
	ar >> MtccD.szCode;
	ar >> MtccD.szMatName;
	ReadMtPr(ar, MtccD.PropD);  
	ar >> MtccD.dC_fc;
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadMTRB(CArchive& ar, UINT& nKey, S_MATL_REBAR_INFO& MtrbD)
{
	MtrbD.Initialize();

	ar >> nKey;
	ar >> MtrbD.szRebar_CodeName;
	ar >> MtrbD.szMainRebar_RebarName;
	ar >> MtrbD.szSubRebar_RebarName;
	ar >> MtrbD.MainRebarData.dB_fy;
	ar >> MtrbD.MainRebarData.dB_Elast;
	ar >> MtrbD.SubRebarData.dB_fy;
	ar >> MtrbD.SubRebarData.dB_Elast;

	return TRUE;
}

BOOL CDgnClassIRER::ReadMTAD(CArchive& ar, UINT& nKey, S_MATL_ADIF_D& MtadD)
{
	MtadD.Initialize();

	ar >> nKey;
	ar >> MtadD.szType;
	ar >> MtadD.szName;
	ar >> MtadD.dSpecificHeat;
	ar >> MtadD.dHeatConduction;
	ar >> MtadD.nThermalUnit;

	return TRUE;
}

BOOL CDgnClassIRER::ReadSTSF(CArchive& ar, S_SECT_STIFFNESS& StsfD)
{
	StsfD.Initialize();

	ar >> StsfD.dArea;
	ar >> StsfD.dAsy;
	ar >> StsfD.dAsz;
	ar >> StsfD.dRxx;
	ar >> StsfD.dRyy;
	ar >> StsfD.dRzz;
	ar >> StsfD.dCyp;
	ar >> StsfD.dCym;
	ar >> StsfD.dCzp;
	ar >> StsfD.dCzm;
	ar >> StsfD.dQyb;
	ar >> StsfD.dQzb;
	ar >> StsfD.dWArea;
	ar >> StsfD.dx1;
	ar >> StsfD.dy1;
	ar >> StsfD.dx2;
	ar >> StsfD.dy2;
	ar >> StsfD.dx3;
	ar >> StsfD.dy3;
	ar >> StsfD.dx4;
	ar >> StsfD.dy4;

	return TRUE;
}

BOOL CDgnClassIRER::ReadSTPG(CArchive& ar, S_GSEC_POLYGON& StpgD)
{
	StpgD.Initialize();

	int i(0), nSize(0);
	ar >> nSize;
	StpgD.aVertex.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> StpgD.aVertex[i].dX;
		ar >> StpgD.aVertex[i].dY;
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadSECT(CArchive& ar, UINT& nKey, S_SECT_D& SectD)
{
	SectD.Initialize();

	ar >> nKey;

	for (int nIJ=0; nIJ<S_SECT_NUM; nIJ++)
	{
		S_SECT_SECTBASE_D&  Sect = (nIJ==0) ? SectD.SectI : SectD.SectJ;
		
		ar >> Sect.nStype;
		ar >> Sect.szShape ;
		ar >> Sect.szDBName;
		ar >> Sect.szName  ;
		ar >> Sect.nPartBaseK;
		int i(0), nSize=60;
		if (GetVersion() >= D_IDEARSFILE_210)
		{
			for (i=0; i<nSize; i++) ar >> Sect.dSize[i];
		}    

		ReadSTCS(ar, nKey, Sect.PartProp);
		ReadSTSF(ar, Sect.Stiffness);
				
		ar >> nSize;
		Sect.aOuterPolygon.SetSize(nSize);
		for (i=0; i<nSize; i++) ReadSTPG(ar, Sect.aOuterPolygon[i]);
		
		ar >> nSize;
		Sect.aInnerPolygon.SetSize(nSize);
		for (i=0; i<nSize; i++) ReadSTPG(ar, Sect.aInnerPolygon[i]);
		
		UINT nDummy(0);
		ar >> nSize;
		Sect.aSectPart.SetSize(nSize);
		for (i=0; i<nSize; i++) ReadCSPT(ar, nDummy, nDummy, Sect.aSectPart[i], FALSE);

		if (GetVersion() >= D_IDEARSFILE_220)
		{
			ar >> nSize;
			Sect.aLine.SetSize(nSize);
			for (i=0; i<nSize; i++) ReadLINE(ar, Sect.aLine[i]);

			ar >> nSize;
			Sect.aLineLoop.SetSize(nSize);
			for (i=0; i<nSize; i++) ar >> Sect.aLineLoop[i];
		}
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadELST(CArchive& ar, UINT& nEKey, UINT& nSKey, S_ELST_D& ElstD)
{
	ElstD.Initialize();
	
	ar >> nEKey;
	ar >> nSKey;
	
	S_SECT_SECTBASE_D*  SectDP = NULL;
	for (int nIJ=0; nIJ<S_ELEMSECT_NUM; nIJ++)
	{    
		switch(nIJ)
		{
		case 0: SectDP = &ElstD.SectI;  break;
		case 1: SectDP = &ElstD.SectQ;  break;
		case 2: SectDP = &ElstD.SectH;  break;
		case 3: SectDP = &ElstD.SectT;  break;
		case 4: SectDP = &ElstD.SectJ;  break;
		}
		
		ar >> SectDP->nStype;
		ar >> SectDP->szShape;
		ar >> SectDP->szDBName;
		ar >> SectDP->szName;
		ar >> SectDP->nPartBaseK;
		int i(0), nSize=60;
		if (GetVersion() >= D_IDEARSFILE_210)
		{
			for (i=0; i<nSize; i++) ar >> SectDP->dSize[i];
		}    
		ReadSTCS(ar, nSKey, SectDP->PartProp);
		ReadSTSF(ar, SectDP->Stiffness);
		
		ar >> nSize;
		SectDP->aOuterPolygon.SetSize(nSize);
		for (i=0; i<nSize; i++) ReadSTPG(ar, SectDP->aOuterPolygon[i]);
		
		ar >> nSize;
		SectDP->aInnerPolygon.SetSize(nSize);
		for (i=0; i<nSize; i++) ReadSTPG(ar, SectDP->aInnerPolygon[i]);
		
		UINT nDummy(0);
		ar >> nSize;
		SectDP->aSectPart.SetSize(nSize);
		for (i=0; i<nSize; i++) ReadCSPT(ar, nDummy, nDummy, SectDP->aSectPart[i], FALSE);
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadCSET(CArchive&  ar, UINT& nEKey, UINT& nSKey, UINT& nPartK, S_CSET_D& CsetD)
{
	CsetD.Initialize();

	ar >> nEKey;
	ar >> nSKey;
	ar >> nPartK;

	ar >> CsetD.nActiveStage;

	int i=0, nSize=0;

	ar >> nSize;
	CsetD.aOPolyI.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aOPolyI[i]);

	ar >> nSize;
	CsetD.aIPolyI.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aIPolyI[i]);

	ar >> nSize;
	CsetD.aOPolyQ.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aOPolyQ[i]);

	ar >> nSize;
	CsetD.aIPolyQ.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aIPolyQ[i]);

	ar >> nSize;
	CsetD.aOPolyH.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aOPolyH[i]);

	ar >> nSize;
	CsetD.aIPolyH.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aIPolyH[i]);

	ar >> nSize;
	CsetD.aOPolyT.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aOPolyT[i]);

	ar >> nSize;
	CsetD.aIPolyT.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aIPolyT[i]);

	ar >> nSize;
	CsetD.aOPolyJ.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aOPolyJ[i]);

	ar >> nSize;
	CsetD.aIPolyJ.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadSTPG(ar, CsetD.aIPolyJ[i]);

	return TRUE;
}


BOOL CDgnClassIRER::ReadSTCS(CArchive& ar, UINT& nKey, S_PROP_D& StcsD)
{
	StcsD.Initialize();

	ar >> nKey;
	int i(0), nSize(0);
	ar >> nSize;
	StcsD.aPropD.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		StcsD.aPropD[i].Initialize();

		ar >> StcsD.aPropD[i].SectK;
		ar >> StcsD.aPropD[i].SectPartK;
		ar >> StcsD.aPropD[i].SectIJK;    
		ar >> StcsD.aPropD[i].MatlK;
	}
	return TRUE;
}

BOOL CDgnClassIRER::ReadGSPO(CArchive& ar, S_GSPO_D& GspoD)
{
	GspoD.Initialize();

	int i(0), nSize(0);
	ar >> nSize;
	GspoD.aPoint.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> GspoD.aPoint[i].dX;
		ar >> GspoD.aPoint[i].dY;
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadGSPI(CArchive& ar, S_GSPI_D& GspiD)
{ 
	GspiD.Initialize();

	int i(0), nSize(0);
	ar >> nSize;
	GspiD.aPoint.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> GspiD.aPoint[i].dX;
		ar >> GspiD.aPoint[i].dY;
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadCSPT(CArchive& ar, UINT& nSKey, UINT& nPKey, S_GSEC_PART& CsptD, BOOL bKey)
{
	CsptD.Initialize();

	if (bKey == TRUE)
	{
		ar >> nSKey;
		ar >> nPKey;  
	}
	ar >> CsptD.szShape;
	ar >> CsptD.szDBName;
	int i(0), nSize = DT_SECT_SIZE_NUM_MAX;
	for (i=0; i<nSize; i++) ar >> CsptD.dSize[i];
	ar >> CsptD.nBuiltUpFlag;

	ReadSTSF(ar, CsptD.Stiffness);
	ReadSTSF(ar, CsptD.StiffnessCom);

	ar >> CsptD.dPeriIn;
	ar >> CsptD.dPeriOut;
	ar >> CsptD.dPeriInCom;
	ar >> CsptD.dPeriOutCom;
	
	ar >> nSize;  
	CsptD.aOuterPolygon.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadGSPO(ar, CsptD.aOuterPolygon[i]);  
	ar >> nSize;
	CsptD.aInnerPolygon.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadGSPI(ar, CsptD.aInnerPolygon[i]);

	// IdxStart, IdxEnd
	if (GetVersion() >= D_IDEARSFILE_220)
	{
		ar >> CsptD.IdxStart[0] >> CsptD.IdxStart[1] >> CsptD.IdxStart[2] >> CsptD.IdxStart[3] >> CsptD.IdxStart[4];
		ar >> CsptD.IdxEnd  [0] >> CsptD.IdxEnd  [1] >> CsptD.IdxEnd  [2] >> CsptD.IdxEnd  [3] >> CsptD.IdxEnd  [4];
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadLINE(CArchive& ar, S_GSEC_LINE& Line)
{ 
	Line.Initialize();
	ar >> Line.v1 >> Line.v2 >> Line.dThik >> Line.nAlign;
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadCONS(CArchive& ar, UINT& nKey, S_CONS_D& ConsD)
{
	ConsD.Initialize();

	ar >> nKey;
	for (int i=0; i<6; i++) ar >> ConsD.sConstraint[i];

	return TRUE;
}

BOOL CDgnClassIRER::ReadSTLD(CArchive& ar, UINT& nKey, S_STLD_D& StldD)
{
	StldD.Initialize();

	ar >> nKey;
	ar >> StldD.szLoadCaseName;
	ar >> StldD.szLoadCaseType;
	ar >> StldD.szDescription;
	ar >> StldD.nLoadCaseId;

	return TRUE;
}

BOOL CDgnClassIRER::ReadLCBS(CArchive& ar, S_LCOM_BASE& LcbsD)
{
	LcbsD.Initialize();
	ar >> LcbsD.nAnalType;
	ar >> LcbsD.nLoadCaseKey;
	ar >> LcbsD.dFactor;

	return TRUE;
}


BOOL CDgnClassIRER::ReadLCOM(CArchive& ar, UINT& nType, UINT& nKey, S_LCOM_D& LcomD)
{
	LcomD.Initialize();

	ar >> nType;
	ar >> nKey;
	ar >> LcomD.szLoadCombName;
	ar >> LcomD.nLoadCombType;
	ar >> LcomD.szDescription;
	ar >> LcomD.nActive;
	ar >> LcomD.nLoadCombId;
	for (int i=0; i<DT_LCOM_NUMCOMB; i++) ReadLCBS(ar, LcomD.Combination[i]);
	ar >> LcomD.nServLcomType;

	return TRUE;
}

BOOL CDgnClassIRER::ReadLENG(CArchive& ar, UINT& nKey, S_LENG_D& LengD)
{
	LengD.Initialize();

	ar >> nKey;
	ar >> LengD.dBLy;
	ar >> LengD.dBLz;
	ar >> LengD.bNotUseBLu;
	ar >> LengD.dBLu;
	ar >> LengD.bAutoCalcLe;  

	return TRUE;
}

BOOL CDgnClassIRER::ReadKFAC(CArchive& ar, UINT& nKey, S_KFAC_D& KfacD)
{
	KfacD.Initialize();

	ar >> nKey;
	ar >> KfacD.dBKy;
	ar >> KfacD.dBKz;

	return TRUE;
}

BOOL CDgnClassIRER::ReadLTSR(CArchive& ar, UINT& nKey, S_LTSR_D& LtsrD)
{
	LtsrD.Initialize();

	ar >> nKey;
	ar >> LtsrD.bNotCheck;
	ar >> LtsrD.dCLMT;
	ar >> LtsrD.dTLMT;

	return TRUE;
}

BOOL CDgnClassIRER::ReadCMFT(CArchive& ar, UINT& nKey, S_CMFT_D& CmftD)
{
	CmftD.Initialize();

	ar >> nKey;
	ar >> CmftD.bAuto;
	ar >> CmftD.dCmy;
	ar >> CmftD.dCmz;

	return TRUE;
}

BOOL CDgnClassIRER::ReadFMAG(CArchive& ar, UINT& nKey, S_FMAG_D& FmagD)
{
	FmagD.Initialize();

	ar >> nKey;
	ar >> FmagD.dB1y;
	ar >> FmagD.dB1z;
	ar >> FmagD.dB2y;
	ar >> FmagD.dB2z;
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadDCON(CArchive& ar, S_DCON_D& DConD)
{
	DConD.Initialize();

	ar >> DConD.szDesignCode;
	for (int i=0; i<6; i++) ar >> DConD.dPhi[i];
	ar >> DConD.dRhoc;
	ar >> DConD.dRhor;
	ar >> DConD.bSpecialSeismic;
	ar >> DConD.dMrft;
	ar >> DConD.nSubCode;
	ar >> DConD.dAlphacc;
	ar >> DConD.dMinRhoc;
	ar >> DConD.dStrutAngle;

	return TRUE;
}

BOOL CDgnClassIRER::ReadRDBM(CArchive& ar, UINT& nKey, S_RDGN_BEAM& RdbmD)
{
	RdbmD.Initialize();

	ar >> nKey;
	ar >> RdbmD.dDt;
	ar >> RdbmD.dDb;
	ar >> RdbmD.dSubBarNum;
	ar >> RdbmD.szSubBarNa;

	return TRUE;
}

BOOL CDgnClassIRER::ReadRDCN(CArchive& ar, UINT& nKey, S_RDGN_COLM& RbcnD)
{
	RbcnD.Initialize();

	ar >> nKey;
	ar >> RbcnD.dDc;
	ar >> RbcnD.dSubBarNum;
	ar >> RbcnD.szSubBarNa;  

	return TRUE;
}

BOOL CDgnClassIRER::ReadRCBM(CArchive& ar, UINT& nKey, S_RCHK_BEAM& RcbmD)
{
	RcbmD.Initialize();

	ar >> nKey;
	int i(0), j(0);
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			ar >> RcbmD.dDt[i][j];
			ar >> RcbmD.dDb[i][j];
			ar >> RcbmD.nTopBarNum[i][j];
			ar >> RcbmD.nBotBarNum[i][j];
			ar >> RcbmD.szTopBarNa1[i][j];
			ar >> RcbmD.szTopBarNa2[i][j];
			ar >> RcbmD.szBotBarNa1[i][j];
			ar >> RcbmD.szBotBarNa2[i][j];
		}
		ar >> RcbmD.dSubBarNum[i];
		ar >> RcbmD.dSubBarDist[i];
		ar >> RcbmD.szSubBarNa[i];
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadRCCN(CArchive& ar, UINT& nKey, S_RCHK_COLM& RccnD)
{
	RccnD.Initialize();

	ar >> nKey;
	int i(0), j(0);
	for (i=0; i<5; i++) ar >> RccnD.dDc[i];
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{     
			ar >> RccnD.nBarNum[i][j];
			ar >> RccnD.szBarNa1[i][j];
			ar >> RccnD.szBarNa2[i][j];
		}
	}
	ar >> RccnD.dSubBarNum;
	ar >> RccnD.dSubBarDist;
	ar >> RccnD.szSubBarNa;

	return TRUE;
}

BOOL CDgnClassIRER::ReadREBT(CArchive& ar, UINT& nKey, S_REBT_D& RebtD)
{
	RebtD.Initialize();

	ar >> nKey;
	ar >> RebtD.nCenterType;
	int i(0), nSize = 0;
	ar >> nSize;
	RebtD.aMainRebar.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> RebtD.aMainRebar[i].nLabel;
		ar >> RebtD.aMainRebar[i].dCenterY;
		ar >> RebtD.aMainRebar[i].dCenterZ;
		ar >> RebtD.aMainRebar[i].szRebarName;
	}

	ar >> RebtD.ShearRebar.dSubRebarSpace;
	ar >> RebtD.ShearRebar.dSubRebarNum[0];
	ar >> RebtD.ShearRebar.dSubRebarNum[1];
	ar >> RebtD.ShearRebar.szSubRebarName;

	return TRUE;
}

BOOL CDgnClassIRER::ReadRPMS(CArchive& ar, S_RPSC_MBAR_SSM& RpmsD)
{
	RpmsD.Initialize();

	ar >> RpmsD.nType;
	ar >> RpmsD.nRefY;
	ar >> RpmsD.nRefZ;
	ar >> RpmsD.Start.dPointY;
	ar >> RpmsD.Start.dPointZ;
	ar >> RpmsD.Center.dPointY;
	ar >> RpmsD.Center.dPointZ;
	int i(0), nSize(0);
	ar >> nSize;
	RpmsD.aPassage.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		RpmsD.aPassage[i].Initialize();

		ar >> RpmsD.aPassage[i].dPointY;
		ar >> RpmsD.aPassage[i].dPointZ;
	}
	ar >> RpmsD.End.dPointY;
	ar >> RpmsD.End.dPointZ;
	ar >> RpmsD.dRadius;
	ar >> RpmsD.nNumorCTC;
	ar >> RpmsD.nNum;
	ar >> RpmsD.dCTC;
	ar >> RpmsD.bEdgeBar;
	ar >> RpmsD.szBar;
	ar >> RpmsD.nStandRef;
	ar >> RpmsD.nPart;
	ar >> RpmsD.nRefY_A;
	ar >> RpmsD.nRefZ_A;
	ar >> RpmsD.dRefY_A;
	ar >> RpmsD.dRefZ_A;
	ar >> RpmsD.dSpacing;

	return TRUE;
}

BOOL CDgnClassIRER::ReadRPSS(CArchive& ar, S_RPSC_SBAR_SSM& RpssD)
{
	RpssD.Initialize();

	ar >> RpssD.bDiagonal;
	ar >> RpssD.dDiagonalPitch;
	ar >> RpssD.dDiagonalAngle;
	ar >> RpssD.dDiagonalAw;
	ar >> RpssD.bSteelBar;
	ar >> RpssD.dSteelBarPitch;
	ar >> RpssD.dSteelBarAngle;
	ar >> RpssD.dSteelBarAp;
	ar >> RpssD.dSteelBarPe;
	ar >> RpssD.bTorsional;
	ar >> RpssD.dTorsionalPitch;
	ar >> RpssD.dTorsionalAwt;
	ar >> RpssD.dTorsionalAlt;
	ar >> RpssD.bLbarFlg;
	ar >> RpssD.dLbarThick;
	ar >> RpssD.bIncFC;

	return TRUE;
}

BOOL CDgnClassIRER::ReadRPSC(CArchive& ar, UINT& nKey, S_RPSC_SSM_D& RpscD)
{
	RpscD.Initialize();

	ar >> nKey;
	int i(0), nSize(0);
	ar >> nSize;
	RpscD.aMbar.SetSize(nSize);
	for (i=0; i<nSize; i++) ReadRPMS(ar, RpscD.aMbar[i]);
	ReadRPSS(ar, RpscD.Sbar);
	ar >> RpscD.bCracked;

	return TRUE;
}

BOOL CDgnClassIRER::ReadTDIF(CArchive& ar, UINT& nKey, UINT& nIJ, S_TDIF_D& TdifD)
{
	ar >> nKey;
	ar >> nIJ;
	int i(0), nSize(0);
	ar >> nSize;
	TdifD.aTndn.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		TdifD.aTndn[i].Initialize();

		ar >> TdifD.aTndn[i].bPrestressed;
		ar >> TdifD.aTndn[i].nTendonType;
		ar >> TdifD.aTndn[i].nLoadType;
		ar >> TdifD.aTndn[i].nBondType;
		ar >> TdifD.aTndn[i].dDia;
		ar >> TdifD.aTndn[i].dDuctDia;
		ar >> TdifD.aTndn[i].dAp;
		ar >> TdifD.aTndn[i].dyz[0];
		ar >> TdifD.aTndn[i].dyz[1];
		ar >> TdifD.aTndn[i].dEp;
		ar >> TdifD.aTndn[i].dDp;
		ar >> TdifD.aTndn[i].dPp;
		if (GetVersion() > D_IDEARSFILE_100) ar >> TdifD.aTndn[i].szTendonName;
		if (GetVersion() > D_IDEARSFILE_200) ar >> TdifD.aTndn[i].nTendonKey;
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadTHIK(CArchive&  ar, UINT& nKey, S_THIK_D& ThikD)
{
	ThikD.Initialize();
	
	ar >> nKey;
	ar >> ThikD.nType;
	ar >> ThikD.nThickType;
	ar >> ThikD.ThickIn;
	ar >> ThikD.ThickOut;
	ar >> ThikD.bOffset;
	ar >> ThikD.nOffsetType;
	ar >> ThikD.dValue;
	ar >> ThikD.dRatio;

	ar >> ThikD.nStifType;
	ar >> ThikD.szDBName;
	ar >> ThikD.dPlateThik;
	ar >> ThikD.dPlateThik;
	ar >> ThikD.nRibPos;
	
	int i(0), j(0);
	for (i=0; i<2; i++)
	{
		// Value
		ar >> ThikD.szStVShape[i];
		ar >> ThikD.dThickIn[i];
		ar >> ThikD.dThickOut[i];
		ar >> ThikD.dHu[i];
		ar >> ThikD.dHl[i];
	}
	for (i=0; i<2; i++)
	{
		// Sect
		ar >> ThikD.bRibAttach[i];
		ar >> ThikD.szStSShape[i];
		ar >> ThikD.szName[i];
		ar >> ThikD.dDist[i];
		for (j=0; j<10; j++) ar >> ThikD.dSize[i][j];
		ar >> ThikD.dPeriIn[i];
		ar >> ThikD.dPeriOut[i];
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadSTRP(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_STRP_DL& StrpD)
{
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<8; j++)
		{
			ar >> StrpD.dblStress[i][j];
		}    
	} 

	return TRUE;
}

BOOL CDgnClassIRER::ReadSTRPCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_STRP_CSDL& StrpD)
{
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	
	int nSize = 0;
	ar >> nSize;
	StrpD.aSTRP.SetSize(nSize);

	for (int i=0; i<nSize; i++)
	{
		for (int j=0; j<5; j++)
		{
			for (int k=0; k<8; k++)
			{
				ar >> StrpD.aSTRP[i].dblStress[j][k];
			}    
		} 
	}
	
	return TRUE;
}
	
// Construnction Stage
BOOL CDgnClassIRER::ReadCSLC(CArchive&  ar, UINT& nKey, S_STLD_D& iCslc)
{
	ar >> nKey;
	ar >> iCslc.szLoadCaseName;
	ar >> iCslc.szLoadCaseType;
	ar >> iCslc.szDescription;
	ar >> iCslc.nLoadCaseId;
		
	return TRUE;
}

BOOL CDgnClassIRER::ReadCLAT(CArchive&  ar, UINT& nKey, S_STLD_D& iClat)
{  
	return ReadCSLC(ar, nKey, iClat);
}

BOOL CDgnClassIRER::ReadSTAG(CArchive&  ar, UINT& nKey, S_STAG_D& iStag)
{
	int i(0), nSize(0);
	ar >> nKey;
	ar >> iStag.szStageName;
	ar >> iStag.dDuration;
		
	ar >> nSize;
	iStag.aUserDefStep.SetSize(nSize);
	for (i=0; i<nSize; i++) ar >> iStag.aUserDefStep[i];
	
	ar >> nSize;
	iStag.aActiveElem.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> iStag.aActiveElem[i].GrupKey;
		ar >> iStag.aActiveElem[i].dAge;
	}
		
	ar >> nSize;
	iStag.aDeactiveElem.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> iStag.aDeactiveElem[i].GrupKey;
		ar >> iStag.aDeactiveElem[i].dRedistribution;
	}
	
	ar >> nSize;
	iStag.aActiveBndr.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> iStag.aActiveBndr[i].BngrKey;
		ar >> iStag.aActiveBndr[i].nPosition;
	}
	ar >> nSize;
	iStag.aDeactiveBndr.SetSize(nSize);
	for (i=0; i<nSize; i++) ar >> iStag.aDeactiveBndr[i];
	
	
	ar >> nSize;
	iStag.aActiveLoad.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> iStag.aActiveLoad[i].LdgrKey;
		ar >> iStag.aActiveLoad[i].dDay;
	}
	
	ar >> nSize;
	iStag.aDeactiveLoad.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> iStag.aDeactiveLoad[i].LdgrKey;
		ar >> iStag.aDeactiveLoad[i].dDay;
	}
	
	ar >> iStag.nStageId;

	return TRUE;
}

BOOL CDgnClassIRER::ReadCSCS(CArchive&  ar, UINT& nKey, S_CSCS_D& iCscs)
{
	int i(0), nSize(0);
	ar >> nKey;
	ar >> iCscs.nSectKey;
	ar >> iCscs.nActStagKey;
	ar >> iCscs.nCompType;
		
	ar >> nSize;
	iCscs.aPart.SetSize(nSize);
	for (i=0; i<nSize; i++)
	{
		ar >> iCscs.aPart[i].nPart;
		ar >> iCscs.aPart[i].nMatlKey;
		ar >> iCscs.aPart[i].nCompStagKey;
		ar >> iCscs.aPart[i].dAge;
		ReadSTSF(ar, iCscs.aPart[i].StiffScale);
		ReadSTSF(ar, iCscs.aPart[i].Stiffness);
		ReadSTSF(ar, iCscs.aPart[i].StiffnessJ);    
		ar >> iCscs.aPart[i].YBar;
		ar >> iCscs.aPart[i].ZBar;
		ar >> iCscs.aPart[i].YBarJ;
		ar >> iCscs.aPart[i].ZBarJ;
		ar >> iCscs.aPart[i].dH;
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadGRUP(CArchive&  ar, UINT& nKey, S_GRUP_D& iGrup)
{
	int i(0), nSize(0);
	ar >> nKey;
	ar >> iGrup.szGroupName;  
	
	ar >> nSize;
	iGrup.arKeyNode.SetSize(nSize);
	for (i=0; i<nSize; i++) ar >> iGrup.arKeyNode[i];
		
	ar >> nSize;
	iGrup.arKeyElem.SetSize(nSize);
	for (i=0; i<nSize; i++) ar >> iGrup.arKeyElem[i];
	
	ar >> iGrup.nGroupId;
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadBNGR(CArchive&  ar, UINT& nKey, S_BNGR_D& iBngr)
{
	ar >> nKey;
	ar >> iBngr.szGroupName;
	ar >> iBngr.nGroupId;
		
	return TRUE;
}

BOOL CDgnClassIRER::ReadLDGR(CArchive&  ar, UINT& nKey, S_LDGR_D& iLdgr)
{
	ar >> nKey;
	ar >> iLdgr.szGroupName;
	ar >> iLdgr.nGroupId;
		
	return TRUE;
}

BOOL CDgnClassIRER::ReadTDLS(CArchive&  ar, UINT& nKey, UINT& nStag, UINT& nStep, S_TDLS_D& TdlsD)
{
	ar >> nKey;
	ar >> nStag;
	ar >> nStep;
	int i(0), nSize(0);
	ar >> nSize;
	TdlsD.aTendonFrc.SetSize(nSize);
	TdlsD.aTendonPos.SetSize(nSize);
	
	for (i=0; i<nSize; i++)
	{
		ar >> TdlsD.aTendonPos[i];
		ar >> TdlsD.aTendonFrc[i];    
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadREAC(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_REAC_D& ReacD)
{
	ReacD.Initialize();

	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	for (int i=0; i<6; i++) ar >> ReacD.dblReac[i];

	return TRUE;
}

BOOL CDgnClassIRER::ReadDISP(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_DISP_D& DispD)
{
	DispD.Initialize();

	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	for (int i=0; i<6; i++) ar >> DispD.dblDisp[i];

	return TRUE;
}

BOOL CDgnClassIRER::ReadDISPCS(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_DISP_CSD& DispD)
{
	DispD.Initialize();
	
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;

	int nSize;
	ar >> nSize;
	DispD.aDisp.SetSize(nSize);
	for (int i=0; i<nSize; i++)
	{
		for (int j=0; j<6; j++) ar >> DispD.aDisp[i].dblDisp[j];
	}  
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadTFOR(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_TFOR_D& TforD)
{
	TforD.Initialize();
	
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	ar >> TforD.dblForce[0];
	ar >> TforD.dblForce[1];
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadTFORCS(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_TFOR_CSD& TforD)
{
	TforD.Initialize();
	
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;

	int nSize = 0;
	ar >> nSize;

	TforD.aTFOR.SetSize(nSize);
	for (int i=0; i<nSize; i++)
	{
		ar >> TforD.aTFOR[i].dblForce[0];
		ar >> TforD.aTFOR[i].dblForce[1];
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadBFOR(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_D& BforD)
{
	BforD.Initialize();

	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	int i(0), j(0), k(0);
	for (i=0; i<4; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<6; k++)
			{
				ar >> BforD.dblForce[i][j][k];
			}
		}
	}

	return TRUE;
}

BOOL CDgnClassIRER::ReadBFORCS(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_CSD& BforD)
{
	BforD.Initialize();
	
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;

	int nSize = 0;
	ar >> nSize;
	BforD.aBFOR.SetSize(nSize);

	int i(0), j(0), k(0);
	for (int nCS = 0; nCS < nSize; nCS++)
	{
		for (i=0; i<4; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<6; k++)
				{
					ar >> BforD.aBFOR[nCS].dblForce[i][j][k];
				}
			}
		}
	}
	
	return TRUE;
}

BOOL CDgnClassIRER::ReadBFORPT(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_PT& BForPT)
{
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	int i(0), j(0), k(0);

	int nPart = 0;
	ar >> nPart;
	
	BForPT.aBFOR.SetSize(nPart);
	for (int i_part = 0; i_part < nPart; i_part++)
	{
		for (i=0; i<4; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<6; k++)
				{
					ar >> BForPT.aBFOR[i_part].dblForce[i][j][k];
				}
			}
		}
	}
		
	return TRUE;
}

BOOL CDgnClassIRER::ReadBFORPTCC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_PTCC& BForPTCC)
{
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;  
	int i(0), j(0), k(0), l(0);
	
	int nPart = 0;
	ar >> nPart;
	BForPTCC.aBFOR.SetSize(nPart);
	
	for (int i_part = 0; i_part < nPart; i_part++)
	{
		for (i=0; i<4; i++)
		{
			for (j=0; j<2; j++)
			{
				for (k=0; k<12; k++)
				{
					for (l=0; l<6; l++)
					{
						ar >> BForPTCC.aBFOR[i_part].dblForce[i][j][k][l];
					}                
				}
			}
		}
	}
		
	return TRUE;
}

BOOL CDgnClassIRER::ReadBFORPTCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_PTCS& BForPTCS)
{
	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	
	int nSize = 0;
	ar >> nSize;
	BForPTCS.aBFORPT.SetSize(nSize);  
	
	int i(0), j(0), k(0);
	for (int nCS = 0; nCS < nSize; nCS++)
	{
		int nPSize = 0;
		ar >> nPSize;
		BForPTCS.aBFORPT[nCS].aBFOR.SetSize(nPSize);

		for (int i_part = 0; i_part < nPSize; ++i_part)
		{
			for (i=0; i<4; i++)
			{
				for (j=0; j<2; j++)
				{
					for (k=0; k<6; k++)
					{
						ar >> BForPTCS.aBFORPT[nCS].aBFOR[i_part].dblForce[i][j][k];
					}
				}
			}
		}
	}
		
	return TRUE;
}

BOOL CDgnClassIRER::ReadRECC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_RECC_D& ReccD)
{
	ReccD.Initialize();

	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	for (int i=0; i<12; i++) ar >> ReccD.dblReac[i];

	return TRUE;
}
BOOL CDgnClassIRER::ReadDSPC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_DSPC_D& DspcD)
{
	DspcD.Initialize();

	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	for (int i=0; i<12; i++) ar >> DspcD.dblDisp[i];

	return TRUE;
}

BOOL CDgnClassIRER::ReadBCFO(CArchive& ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BCFO_B& BcfoD)
{
	BcfoD.Initialize();

	ar >> nEKey;
	ar >> nLKey;
	ar >> nType;
	int i(0), j(0), k(0), l(0);
	for (i=0; i<4; i++)
	{
		for (j=0; j<2; j++)
		{
			for (k=0; k<12; k++)
			{
				for (l=0; l<6; l++)
				{
					ar >> BcfoD.dblForce[i][j][k][l];
				}        
			}
		}
	}

	return TRUE;
}
