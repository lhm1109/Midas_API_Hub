#ifndef __DGN_IDEA_RS_EXPORT_READ_H__
#define __DGN_IDEA_RS_EXPORT_READ_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DGN_IDEA_ST.H"
class  CDgnClassIRED;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnClassIRER
{
	// Construction
public:
	CDgnClassIRER();
	~CDgnClassIRER();

	BOOL ReadFile(CArchive& ar, CDgnClassIRED&  cData);

protected:
	int  nVersion;
	int  GetVersion();
	void SetVersion(int nVer);

	// Read Data
	BOOL ReadBASC(CArchive&  ar, int* nASize, int& nVer, CString& szMCTPath);
	BOOL ReadUNIT(CArchive&  ar, S_UNIT_D&  UnitD);
	BOOL ReadNODE(CArchive&  ar, UINT& nKey, S_NODE_D& NodeD);
	BOOL ReadELEM(CArchive&  ar, UINT& nKey, S_ELEM_D& ElemD);
	BOOL ReadMEMB(CArchive&  ar, UINT& nKey, S_MEMB_D& MembD);
	BOOL ReadMtPr(CArchive&  ar, S_MATL_PROP& MtPrD);
	BOOL ReadMTST(CArchive&  ar, UINT& nKey, S_MATL_STEEL_D&    MtstD);
	BOOL ReadMTCC(CArchive&  ar, UINT& nKey, S_MATL_CONCRETE_D& MtccD);
	BOOL ReadMTRB(CArchive&  ar, UINT& nKey, S_MATL_REBAR_INFO& MtrbD);
	BOOL ReadMTAD(CArchive&  ar, UINT& nKey, S_MATL_ADIF_D&     MtadD);
	
	BOOL ReadSTSF(CArchive&  ar, S_SECT_STIFFNESS& StsfD);
	BOOL ReadSTPG(CArchive&  ar, S_GSEC_POLYGON& StpgD);
	BOOL ReadSECT(CArchive&  ar, UINT& nKey, S_SECT_D& SectD);
	BOOL ReadELST(CArchive&  ar, UINT& nEKey, UINT& nSKey, S_ELST_D& ElstD); 
	BOOL ReadCSET(CArchive&  ar, UINT& nEKey, UINT& nSKey, UINT& nPartK, S_CSET_D& CsetD); 
	
	BOOL ReadSTCS(CArchive&  ar, UINT& nKey, S_PROP_D& StcsD);
	BOOL ReadGSPO(CArchive&  ar, S_GSPO_D& GspoD);
	BOOL ReadGSPI(CArchive&  ar, S_GSPI_D& GspiD);
	BOOL ReadCSPT(CArchive&  ar, UINT& nSKey,  UINT& nPKey, S_GSEC_PART& CsptD, BOOL bKey=TRUE);
	BOOL ReadLINE(CArchive&  ar, S_GSEC_LINE& Line);
	
	BOOL ReadCONS(CArchive&  ar, UINT& nKey, S_CONS_D& ConsD);
	BOOL ReadSTLD(CArchive&  ar, UINT& nKey, S_STLD_D& StldD);
	BOOL ReadLCBS(CArchive&  ar, S_LCOM_BASE& LcomD);
	BOOL ReadLCOM(CArchive&  ar, UINT& nType, UINT& nKey, S_LCOM_D& LcomD);
	
	BOOL ReadLENG(CArchive&  ar, UINT& nKey, S_LENG_D& LengD);
	BOOL ReadKFAC(CArchive&  ar, UINT& nKey, S_KFAC_D& KfacD);
	BOOL ReadLTSR(CArchive&  ar, UINT& nKey, S_LTSR_D& LtsrD);
	BOOL ReadCMFT(CArchive&  ar, UINT& nKey, S_CMFT_D& CmftD);
	BOOL ReadFMAG(CArchive&  ar, UINT& nKey, S_FMAG_D& FmagD);
	
	BOOL ReadDCON(CArchive&  ar, S_DCON_D& DConD);
	BOOL ReadRDBM(CArchive&  ar, UINT& nKey, S_RDGN_BEAM& RdbmD);
	BOOL ReadRDCN(CArchive&  ar, UINT& nKey, S_RDGN_COLM& RbcnD);
	BOOL ReadRCBM(CArchive&  ar, UINT& nKey, S_RCHK_BEAM& RcbmD);
	BOOL ReadRCCN(CArchive&  ar, UINT& nKey, S_RCHK_COLM& RccnD);
	
	BOOL ReadREBT(CArchive&  ar, UINT& nKey, S_REBT_D&     RebtD);
	BOOL ReadRPMS(CArchive&  ar, S_RPSC_MBAR_SSM& RpmsD);
	BOOL ReadRPSS(CArchive&  ar, S_RPSC_SBAR_SSM& RpssD);
	BOOL ReadRPSC(CArchive&  ar, UINT& nKey, S_RPSC_SSM_D& RpscD);
	BOOL ReadTDIF(CArchive&  ar, UINT& nKey, UINT& nIJ, S_TDIF_D& TdifD);

	// for 2D element
	BOOL ReadTHIK(CArchive&  ar, UINT& nKey, S_THIK_D& ThikD);
	BOOL ReadSTRP(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_STRP_DL& StrpD);
	BOOL ReadSTRPCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_STRP_CSDL& StrpD);

	// for Construction Stage
	BOOL ReadCSLC(CArchive&  ar, UINT& nKey, S_STLD_D& iCslc);
	BOOL ReadCLAT(CArchive&  ar, UINT& nKey, S_STLD_D& iCslc);
	BOOL ReadSTAG(CArchive&  ar, UINT& nKey, S_STAG_D& iStag);
	BOOL ReadCSCS(CArchive&  ar, UINT& nKey, S_CSCS_D& iCscs);
	BOOL ReadGRUP(CArchive&  ar, UINT& nKey, S_GRUP_D& iGrup);
	BOOL ReadBNGR(CArchive&  ar, UINT& nKey, S_BNGR_D& iBngr);
	BOOL ReadLDGR(CArchive&  ar, UINT& nKey, S_LDGR_D& iLdgr);
	BOOL ReadTDLS(CArchive&  ar, UINT& nKey, UINT& nStag, UINT& nStep, S_TDLS_D& TdlsD);
	
	BOOL ReadREAC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_REAC_D& ReacD);
	BOOL ReadDISP(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_DISP_D& DispD);  
	BOOL ReadTFOR(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_TFOR_D& TforD);  
	BOOL ReadBFOR(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_D& BforD);
	BOOL ReadRECC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_RECC_D& ReacD);
	BOOL ReadDSPC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_DSPC_D& DspcD);
	BOOL ReadBCFO(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BCFO_B& BcfoD);

	BOOL ReadDISPCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_DISP_CSD& DispD);
	BOOL ReadTFORCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_TFOR_CSD& TforD);
	BOOL ReadBFORCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_CSD& BforD);
	
	BOOL ReadBFORPT(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_PT& DispD);
	BOOL ReadBFORPTCC(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_PTCC& TforD);
	BOOL ReadBFORPTCS(CArchive&  ar, UINT& nEKey, UINT& nLKey, UINT& nType, S_BFOR_PTCS& BforD);
};

#include "HeaderPost.h"
#endif  // __DGN_IDEA_RS_EXPORT_READ_H__
