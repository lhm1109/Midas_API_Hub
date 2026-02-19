// DgnClassIRED.cpp : implementation file
// IDEA RS EXPORT : DATA Class

#include "stdafx.h"
#include "DgnClassIRED.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnClassIRED::CDgnClassIRED()
{

}

CDgnClassIRED::~CDgnClassIRED()
{
 
}

void CDgnClassIRED::Initialize()
{
	m_unit.Initialize();
	m_nVersion = 0;
	m_szMCTPath = _T("");
	m_Dcon.Initialize();

	RemoveMap();
}

void CDgnClassIRED::SetMapSize(int* nSize)
{
	RemoveMap();
	
	if (nSize[S_IDEA_NODE]>0) m_mNode.InitHashTable(nSize[S_IDEA_NODE]);
	if (nSize[S_IDEA_ELEM]>0) m_mElem.InitHashTable(nSize[S_IDEA_ELEM]);
	if (nSize[S_IDEA_MEMB]>0) m_mMemb.InitHashTable(nSize[S_IDEA_MEMB]);

	if (nSize[S_IDEA_MATL]>0) m_mMatlST.InitHashTable(nSize[S_IDEA_MATL]);  
	if (nSize[S_IDEA_MTST]>0) m_mMatlCN.InitHashTable(nSize[S_IDEA_MTST]);  
	if (nSize[S_IDEA_MTCN]>0) m_mMatlRB.InitHashTable(nSize[S_IDEA_MTCN]);  
	if (nSize[S_IDEA_MTRB]>0) m_mMatlAD.InitHashTable(nSize[S_IDEA_MTRB]); 

	if (nSize[S_IDEA_SECT]>0) m_mSect.InitHashTable(nSize[S_IDEA_SECT]);  
	if (nSize[S_IDEA_ELST]>0) m_mElst.InitHashTable(nSize[S_IDEA_ELST]); 
	if (nSize[S_IDEA_CSET]>0) m_mCset.InitHashTable(nSize[S_IDEA_CSET]); 
	
	if (nSize[S_IDEA_CONS]>0) m_mCons.InitHashTable(nSize[S_IDEA_CONS]); 
	if (nSize[S_IDEA_STLD]>0) m_mStld.InitHashTable(nSize[S_IDEA_STLD]);  
	if (nSize[S_IDEA_LCOM]>0) m_mLcom.InitHashTable(nSize[S_IDEA_LCOM]); 

	if (nSize[S_IDEA_LENG]>0) m_mLeng.InitHashTable(nSize[S_IDEA_LENG]);  
	if (nSize[S_IDEA_KFAC]>0) m_mKfac.InitHashTable(nSize[S_IDEA_KFAC]);  
	if (nSize[S_IDEA_LTSR]>0) m_mLtsr.InitHashTable(nSize[S_IDEA_LTSR]);  
	if (nSize[S_IDEA_CMFT]>0) m_mCmft.InitHashTable(nSize[S_IDEA_CMFT]);  
	if (nSize[S_IDEA_FMAG]>0) m_mFmag.InitHashTable(nSize[S_IDEA_FMAG]);  

	if (nSize[S_IDEA_RDBM]>0) m_mRdbm.InitHashTable(nSize[S_IDEA_RDBM]);  
	if (nSize[S_IDEA_RDCN]>0) m_mRdcn.InitHashTable(nSize[S_IDEA_RDCN]);  
	if (nSize[S_IDEA_RCBM]>0) m_mRcbm.InitHashTable(nSize[S_IDEA_RCBM]);  
	if (nSize[S_IDEA_RCCN]>0) m_mRccn.InitHashTable(nSize[S_IDEA_RCCN]);  
	if (nSize[S_IDEA_REBT]>0) m_mRebt.InitHashTable(nSize[S_IDEA_REBT]);  
	if (nSize[S_IDEA_RPSC]>0) m_mRpsc.InitHashTable(nSize[S_IDEA_RPSC]);  
	if (nSize[S_IDEA_TDIF]>0) m_mTdif.InitHashTable(nSize[S_IDEA_TDIF]);    

	//if (nSize[S_IDEA_REAC]>0) m_mReac.InitHashTable(nSize[S_IDEA_REAC]);  
	if (nSize[S_IDEA_DISP]>0) m_mDisp.InitHashTable(nSize[S_IDEA_DISP]);  
	if (nSize[S_IDEA_TFOR]>0) m_mTfor.InitHashTable(nSize[S_IDEA_TFOR]);  
	if (nSize[S_IDEA_BFOR]>0) m_mBfor.InitHashTable(nSize[S_IDEA_BFOR]);  

	//if (nSize[S_IDEA_RECC]>0) m_mRecc.InitHashTable(nSize[S_IDEA_RECC]);  
	if (nSize[S_IDEA_DSPC]>0) m_mDspc.InitHashTable(nSize[S_IDEA_DSPC]);  
	if (nSize[S_IDEA_BCFO]>0) m_mBcfo.InitHashTable(nSize[S_IDEA_BCFO]); 
	
	// for 2D element
	if (nSize[S_IDEA_THIK]>0) m_mThik.InitHashTable(nSize[S_IDEA_THIK]);
	if (nSize[S_IDEA_PFOR]>0) m_mSTRP.InitHashTable(nSize[S_IDEA_PFOR]);
	// for Construction Stage
	if (nSize[S_IDEA_CSLC]>0) m_mCslc.InitHashTable(nSize[S_IDEA_CSLC]);  
	if (nSize[S_IDEA_CLAT]>0) m_mClat.InitHashTable(nSize[S_IDEA_CLAT]);  
	if (nSize[S_IDEA_STAG]>0) m_mStag.InitHashTable(nSize[S_IDEA_STAG]);  
	if (nSize[S_IDEA_CSCS]>0) m_mCscs.InitHashTable(nSize[S_IDEA_CSCS]);  
	if (nSize[S_IDEA_EGRP]>0) m_mGrup.InitHashTable(nSize[S_IDEA_EGRP]);  
	if (nSize[S_IDEA_BGRP]>0) m_mBngr.InitHashTable(nSize[S_IDEA_BGRP]);  
	if (nSize[S_IDEA_LGRP]>0) m_mLdgr.InitHashTable(nSize[S_IDEA_LGRP]); 
	if (nSize[S_IDEA_TDLS]>0) m_mTdls.InitHashTable(nSize[S_IDEA_TDLS]);  
	if (nSize[S_IDEA_CSDP]>0) m_mCsds.InitHashTable(nSize[S_IDEA_CSDP]);  
	if (nSize[S_IDEA_CSTF]>0) m_mCstf.InitHashTable(nSize[S_IDEA_CSTF]);  
	if (nSize[S_IDEA_CSBF]>0) m_mCsbf.InitHashTable(nSize[S_IDEA_CSBF]);  
	if (nSize[S_IDEA_CSPF]>0) m_mCspf.InitHashTable(nSize[S_IDEA_CSPF]);      

	if (nSize[S_IDEA_EPTB]>0) m_mBforPT.InitHashTable(nSize[S_IDEA_EPTB]);      
	if (nSize[S_IDEA_EPTC]>0) m_mBforPTCC.InitHashTable(nSize[S_IDEA_EPTC]);      
	if (nSize[S_IDEA_EPCS]>0) m_mBforPTCS.InitHashTable(nSize[S_IDEA_EPCS]);        
}

void CDgnClassIRED::RemoveMap()
{
	if (m_mNode.GetCount()>0) m_mNode.RemoveAll();
	if (m_mElem.GetCount()>0) m_mElem.RemoveAll();
	if (m_mMemb.GetCount()>0) m_mMemb.RemoveAll();
	
	if (m_mMatlST.GetCount()>0) m_mMatlST.RemoveAll();
	if (m_mMatlCN.GetCount()>0) m_mMatlCN.RemoveAll();
	if (m_mMatlRB.GetCount()>0) m_mMatlRB.RemoveAll();
	if (m_mMatlAD.GetCount()>0) m_mMatlAD.RemoveAll();
	
	if (m_mSect.GetCount()>0) m_mSect.RemoveAll();    
	if (m_mElst.GetCount()>0) m_mElst.RemoveAll();    
	if (m_mCset.GetCount()>0) m_mCset.RemoveAll();

	if (m_mCons.GetCount()>0) m_mCons.RemoveAll();
	
	if (m_mStld.GetCount()>0) m_mStld.RemoveAll();
	if (m_mLcom.GetCount()>0) m_mLcom.RemoveAll();
	
	if (m_mLeng.GetCount()>0) m_mLeng.RemoveAll();
	if (m_mKfac.GetCount()>0) m_mKfac.RemoveAll();
	if (m_mLtsr.GetCount()>0) m_mLtsr.RemoveAll();
	if (m_mCmft.GetCount()>0) m_mCmft.RemoveAll();
	if (m_mFmag.GetCount()>0) m_mFmag.RemoveAll();
	
	if (m_mRdbm.GetCount()>0) m_mRdbm.RemoveAll();
	if (m_mRdcn.GetCount()>0) m_mRdcn.RemoveAll();
	if (m_mRcbm.GetCount()>0) m_mRcbm.RemoveAll();
	if (m_mRccn.GetCount()>0) m_mRccn.RemoveAll();
	
	if (m_mRebt.GetCount()>0) m_mRebt.RemoveAll();
	if (m_mRpsc.GetCount()>0) m_mRpsc.RemoveAll();
	if (m_mTdif.GetCount()>0) m_mTdif.RemoveAll();
	if (m_mTdls.GetCount()>0) m_mTdls.RemoveAll();

	if (m_mThik.GetCount()>0) m_mThik.RemoveAll();
	if (m_mSTRP.GetCount()>0) m_mSTRP.RemoveAll();
	
	if (m_mCslc.GetCount()>0) m_mCslc.RemoveAll();
	if (m_mClat.GetCount()>0) m_mClat.RemoveAll();
	if (m_mStag.GetCount()>0) m_mStag.RemoveAll();
	if (m_mCscs.GetCount()>0) m_mCscs.RemoveAll();
	if (m_mGrup.GetCount()>0) m_mGrup.RemoveAll();
	if (m_mBngr.GetCount()>0) m_mBngr.RemoveAll();
	if (m_mLdgr.GetCount()>0) m_mLdgr.RemoveAll();

	if (m_mReac.GetCount()>0) m_mReac.RemoveAll();
	if (m_mDisp.GetCount()>0) m_mDisp.RemoveAll();
	if (m_mTfor.GetCount()>0) m_mTfor.RemoveAll();
	if (m_mBfor.GetCount()>0) m_mBfor.RemoveAll();
	if (m_mRecc.GetCount()>0) m_mRecc.RemoveAll();
	if (m_mDspc.GetCount()>0) m_mDspc.RemoveAll();
	if (m_mBcfo.GetCount()>0) m_mBcfo.RemoveAll();

	if (m_mCsds.GetCount()>0) m_mCsds.RemoveAll();    
	if (m_mCstf.GetCount()>0) m_mCstf.RemoveAll();    
	if (m_mCsbf.GetCount()>0) m_mCsbf.RemoveAll();    
	if (m_mCspf.GetCount()>0) m_mCspf.RemoveAll();  
	
	if (m_mBforPT.GetCount()>0)   m_mBforPT.RemoveAll();    
	if (m_mBforPTCC.GetCount()>0) m_mBforPTCC.RemoveAll();    
	if (m_mBforPTCS.GetCount()>0) m_mBforPTCS.RemoveAll();   
}

// About Unit
void CDgnClassIRED::SetUnit(S_UNIT_D unit)
{
	m_unit.nBase_Length = unit.nBase_Length;
	m_unit.nBase_Force  = unit.nBase_Force;
}

void CDgnClassIRED::GetUnit(S_UNIT_D&  unit)
{
	unit.nBase_Length = m_unit.nBase_Length;
	unit.nBase_Force  = m_unit.nBase_Force;
}

// About Node
BOOL CDgnClassIRED::GetNode(S_NODE_K nKey, S_NODE_D&  NodeD)
{
	return m_mNode.Lookup(nKey, NodeD);
}

int CDgnClassIRED::GetNode(S_NODE_K_LIST& aKey, S_NODE_D_LIST&  aNodeD)
{
	aKey.RemoveAll();
	aNodeD.RemoveAll();
	
	int nSize = GetNodeKeyList(aKey);
	if (nSize < 1) return 0;
	
	aNodeD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetNode(aKey[i], aNodeD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize;
}

int CDgnClassIRED::GetNodeKeyList(S_NODE_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mNode.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetNodeCount()
{
	return m_mNode.GetCount();
}

BOOL CDgnClassIRED::AddNode(S_NODE_K nKey, S_NODE_D&  NodeD)
{
	if (m_mNode.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mNode.SetAt(nKey, NodeD);
	return TRUE;
}


// About Element
BOOL CDgnClassIRED::GetElem(S_ELEM_K nKey, S_ELEM_D&  ElemD)
{
	return m_mElem.Lookup(nKey, ElemD);
}

int CDgnClassIRED::GetElem(S_ELEM_K_LIST& aKey, S_ELEM_D_LIST&  aElemD)
{
	aKey.RemoveAll();
	aElemD.RemoveAll();

	int nSize = GetElemKeyList(aKey);
	if (nSize < 1) return 0;

	aElemD.SetSize(nSize);

	for (int i=0; i<nSize; i++)
	{
		if (GetElem(aKey[i], aElemD[i]) == FALSE)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	return nSize;
}

int CDgnClassIRED::GetElemKeyList(S_ELEM_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mElem.GetKeyList(aKey);
	if (nSize<1) return 0;

	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetElemCount()
{
	return m_mElem.GetCount();
}

BOOL CDgnClassIRED::AddElem(S_ELEM_K nKey, S_ELEM_D&  ElemD)
{
	if (m_mElem.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_mElem.SetAt(nKey, ElemD);
	return TRUE;
}

// About Member
BOOL CDgnClassIRED::GetMemb(S_MEMB_K nKey, S_MEMB_D&  MembD)
{
	return m_mMemb.Lookup(nKey, MembD);
}

int CDgnClassIRED::GetMemb(S_MEMB_K_LIST& aKey, S_MEMB_D_LIST&  aMembD)
{
	aKey.RemoveAll();
	aMembD.RemoveAll();
	
	int nSize = GetMembKeyList(aKey);
	if (nSize < 1) return 0;
	
	aMembD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetMemb(aKey[i], aMembD[i]) == FALSE)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	
	return nSize;
}

int CDgnClassIRED::GetMembKeyList(S_MEMB_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mMemb.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetMembCount()
{
	return m_mMemb.GetCount();
}

BOOL CDgnClassIRED::AddMemb(S_MEMB_K nKey, S_MEMB_D&  MembD)
{
	if (m_mMemb.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mMemb.SetAt(nKey, MembD);
	return TRUE;
}

// About Material - Steel  
BOOL CDgnClassIRED::GetMatlST(S_MATL_STEEL_K nKey, S_MATL_STEEL_D&  MatlSTD)
{
	return m_mMatlST.Lookup(nKey, MatlSTD);  
}

int CDgnClassIRED::GetMatlST(S_MATL_STEEL_K_LIST& aKey, S_MATL_STEEL_D_LIST&  aMatlSTD)
{
	aKey.RemoveAll();
	aMatlSTD.RemoveAll();
	
	int nSize = GetMatlSTKeyList(aKey);
	if (nSize < 1) return 0;
	
	aMatlSTD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetMatlST(aKey[i], aMatlSTD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetMatlSTKeyList(S_MATL_STEEL_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mMatlST.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetMatlSTCount()
{
	return m_mMatlST.GetCount();
}

BOOL CDgnClassIRED::AddMatlST(S_MATL_STEEL_K nKey, S_MATL_STEEL_D&  MatlSTD)
{
	if (m_mMatlST.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mMatlST.SetAt(nKey, MatlSTD);
	return TRUE;
}

// About Material - Concrete  
BOOL CDgnClassIRED::GetMatlCN(S_MATL_CONCRETE_K nKey, S_MATL_CONCRETE_D&  MatlCND)
{
	return m_mMatlCN.Lookup(nKey, MatlCND);
}

int CDgnClassIRED::GetMatlCN(S_MATL_CONCRETE_K_LIST& aKey, S_MATL_CONCRETE_D_LIST&  aMatlCND)
{
	aKey.RemoveAll();
	aMatlCND.RemoveAll();
	
	int nSize = GetMatlCNKeyList(aKey);
	if (nSize < 1) return 0;
	
	aMatlCND.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetMatlCN(aKey[i], aMatlCND[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize;
}

int CDgnClassIRED::GetMatlCNKeyList(S_MATL_CONCRETE_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mMatlCN.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
	
}

int CDgnClassIRED::GetMatlCNCount()
{
	return m_mMatlCN.GetCount();
}

BOOL CDgnClassIRED::AddMatlCN(S_MATL_CONCRETE_K nKey, S_MATL_CONCRETE_D&  MatlCND)
{
	if (m_mMatlCN.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mMatlCN.SetAt(nKey, MatlCND);
	return TRUE; 
}

// About Material - Rebar
BOOL CDgnClassIRED::GetMatlRB(S_MATL_REIF_K nKey, S_MATL_REBAR_INFO&  MatlRBD)
{
	return m_mMatlRB.Lookup(nKey, MatlRBD);
}

int CDgnClassIRED::GetMatlRB(S_MATL_REIF_K_LIST& aKey, S_MATL_REIF_D_LIST&  aMatlRBD)
{
	aKey.RemoveAll();
	aMatlRBD.RemoveAll();
	
	int nSize = GetMatlRBKeyList(aKey);
	if (nSize < 1) return 0;
	
	aMatlRBD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetMatlRB(aKey[i], aMatlRBD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetMatlRBKeyList(S_MATL_REIF_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mMatlRB.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetMatlRBCount()
{
	return m_mMatlRB.GetCount();
}

BOOL CDgnClassIRED::AddMatlRB(S_MATL_REIF_K nKey, S_MATL_REBAR_INFO&  MatlRBD)
{
	if (m_mMatlRB.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mMatlRB.SetAt(nKey, MatlRBD);
	return TRUE;
}

// About Material - Add Inform
BOOL CDgnClassIRED::GetMatlAD(S_MATL_ADIF_K nKey, S_MATL_ADIF_D&  MatlADD)
{
	return m_mMatlAD.Lookup(nKey, MatlADD);
}

int CDgnClassIRED::GetMatlAD(S_MATL_ADIF_K_LIST& aKey, S_MATL_ADIF_D_LIST&  aMatlADD)
{
	aKey.RemoveAll();
	aMatlADD.RemoveAll();
	
	int nSize = GetMatlADKeyList(aKey);
	if (nSize < 1) return 0;
	
	aMatlADD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetMatlAD(aKey[i], aMatlADD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetMatlADKeyList(S_MATL_ADIF_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mMatlAD.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetMatlADCount()
{
	return m_mMatlAD.GetCount();
}

BOOL CDgnClassIRED::AddMatlAD(S_MATL_ADIF_K nKey, S_MATL_ADIF_D&  MatlADD)
{
	if (m_mMatlAD.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mMatlAD.SetAt(nKey, MatlADD);
	return TRUE;
}

BOOL CDgnClassIRED::GetSect(S_SECT_K nKey, S_SECT_D&  SectD)
{
	return m_mSect.Lookup(nKey, SectD);
}

int CDgnClassIRED::GetSect(S_SECT_K_LIST& aKey, S_SECT_D_LIST&  aSectD)
{
	aKey.RemoveAll();
	aSectD.RemoveAll();
	
	int nSize = GetSectKeyList(aKey);
	if (nSize < 1) return 0;
	
	aSectD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetSect(aKey[i], aSectD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetSectKeyList(S_SECT_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mSect.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetSectCount()
{
	return m_mSect.GetCount();
}

BOOL CDgnClassIRED::AddSect(S_SECT_K nKey, S_SECT_D&  SectD)
{
	if (m_mSect.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mSect.SetAt(nKey, SectD);
	return TRUE;
}

// About Element Section
BOOL CDgnClassIRED::GetElst(S_ELEM_K ElemK, S_SECT_K SectK, S_ELST_D& ElstD)
{
	T_IDEA_K<UINT>  MKey;
	MKey.i1 = ElemK;
	MKey.i2 = SectK;  
	
	return m_mElst.Lookup(MKey, ElstD);
}

int CDgnClassIRED::GetElst(S_ELEM_K_LIST& aElemK, S_SECT_K_LIST& aSectK, S_ELST_D_LIST&  aElstD)
{
	T_IDEA_K<UINT>  MKey;
	S_ELST_D   ElstD;
	
	aElemK.RemoveAll();
	aSectK.RemoveAll();
	aElstD.RemoveAll();
	
	int nCur(0), nCount = m_mElst.GetCount();
	aElemK.SetSize(nCount);
	aSectK.SetSize(nCount);
	aElstD.SetSize(nCount);

	POSITION pos = m_mElst.GetStartPosition();
	while(pos)
	{
		m_mElst.GetNextAssoc(pos, MKey, ElstD);
		
		aElemK[nCur] = MKey.i1;
		aSectK[nCur] = MKey.i2;
		aElstD[nCur] = ElstD;

		nCur++;
	}        
	
	return aElemK.GetSize();
}

int CDgnClassIRED::GetElstCount()
{
	return m_mElst.GetCount();
}

BOOL CDgnClassIRED::AddElst(S_ELEM_K ElemK, S_SECT_K SectK, S_ELST_D& ElstD)
{
	T_IDEA_K<UINT>  MKey;
	MKey.i1 = ElemK;
	MKey.i2 = SectK;
		
	m_mElst.SetAt(MKey, ElstD); 
	return TRUE;
}

// About Element Section
BOOL CDgnClassIRED::GetCset(S_ELEM_K ElemK, S_SECT_K SectK, S_PART_K PartK, S_CSET_D& CSetD)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = ElemK;
	MKey.i2 = SectK;  
	MKey.i3 = PartK;  
	
	return m_mCset.Lookup(MKey, CSetD);
}

int CDgnClassIRED::GetCset(S_ELEM_K_LIST& aElemK, S_SECT_K_LIST& aSectK, S_PART_K_LIST& aPartK, S_CSET_D_LIST&  aCsetD)
{
	T_IDEA_K3<UINT>  MKey;
	S_CSET_D   CsetD;
	
	aElemK.RemoveAll();
	aSectK.RemoveAll();
	aPartK.RemoveAll();
	aCsetD.RemoveAll();
	
	int nCur(0), nCount = m_mCset.GetCount();
	aElemK.SetSize(nCount);
	aSectK.SetSize(nCount);
	aPartK.SetSize(nCount);
	aCsetD.SetSize(nCount);
	
	POSITION pos = m_mCset.GetStartPosition();
	while(pos)
	{
		m_mCset.GetNextAssoc(pos, MKey, CsetD);
		
		aElemK[nCur] = MKey.i1;
		aSectK[nCur] = MKey.i2;
		aPartK[nCur] = MKey.i3;
		aCsetD[nCur] = CsetD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();
}

int CDgnClassIRED::GetCsetCount()
{
	return m_mCset.GetCount();
	
}

BOOL CDgnClassIRED::AddCset(S_ELEM_K ElemK, S_SECT_K SectK, S_PART_K PartK, S_CSET_D& CSetD)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = ElemK;
	MKey.i2 = SectK;
	MKey.i3 = PartK;
	
	m_mCset.SetAt(MKey, CSetD); 
	return TRUE;
}

// About Thickness
BOOL CDgnClassIRED::GetThik(S_THIK_K nKey, S_THIK_D&  ThikD)
{
	return m_mThik.Lookup(nKey, ThikD);
}

int CDgnClassIRED::GetThik(S_THIK_K_LIST& aKey, S_THIK_D_LIST&  aThikD)
{
	aKey.RemoveAll();
	aThikD.RemoveAll();
	
	int nSize = GetThikKeyList(aKey);
	if (nSize < 1) return 0;
	
	aThikD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetThik(aKey[i], aThikD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetThikKeyList(S_THIK_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mThik.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetThikCount()
{
	return m_mThik.GetCount();
}

BOOL CDgnClassIRED::AddThik(S_THIK_K nKey, S_THIK_D&  ThikD)
{
	if (m_mThik.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mThik.SetAt(nKey, ThikD);
	return TRUE;
}

// About Support
BOOL CDgnClassIRED::GetCons(S_CONS_K nKey, S_CONS_D&  ConsD)
{
	return m_mCons.Lookup(nKey, ConsD);
}

int CDgnClassIRED::GetCons(S_CONS_K_LIST& aKey, S_CONS_D_LIST&  aConsD)
{
	aKey.RemoveAll();
	aConsD.RemoveAll();
	
	int nSize = GetConsKeyList(aKey);
	if (nSize < 1) return 0;
	
	aConsD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetCons(aKey[i], aConsD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetConsKeyList(S_CONS_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mCons.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetConsCount()
{
	return m_mCons.GetCount();
}

BOOL CDgnClassIRED::AddCons(S_CONS_K nKey, S_CONS_D&  ConsD)
{
	if (m_mCons.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mCons.SetAt(nKey, ConsD);
	return TRUE;
}

// About Load Case
BOOL CDgnClassIRED::GetStld(S_STLD_K nKey, S_STLD_D&  StldD)
{
	return m_mStld.Lookup(nKey, StldD);
}

int CDgnClassIRED::GetStld(S_STLD_K_LIST& aKey, S_STLD_D_LIST&  aStldD)
{
	aKey.RemoveAll();
	aStldD.RemoveAll();
	
	int nSize = GetStldKeyList(aKey);
	if (nSize < 1) return 0;
	
	aStldD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetStld(aKey[i], aStldD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetStldKeyList(S_STLD_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mStld.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetStldCount()
{
	return m_mStld.GetCount();
}

BOOL CDgnClassIRED::AddStld(S_STLD_K nKey, S_STLD_D&  StldD)
{
	if (m_mStld.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mStld.SetAt(nKey, StldD);
	return TRUE;
}

// About Load Combination
BOOL CDgnClassIRED::GetLcom(S_LCOM_T nType, S_LCOM_K nKey, S_LCOM_D&  LcomD)
{
	T_IDEA_K<UINT>  MKey;
	MKey.i1 = nType;
	MKey.i2 = nKey;  

	return m_mLcom.Lookup(MKey, LcomD);
}

int CDgnClassIRED::GetLcom(S_LCOM_T_LIST& aType, S_LCOM_K_LIST& aKey, S_LCOM_D_LIST&  aLcomD)
{
	T_IDEA_K<UINT>  MKey;
	S_LCOM_D   LComD;
	
	aType.RemoveAll();
	aKey.RemoveAll();
	aLcomD.RemoveAll();
	
	int nCur(0), nCount = m_mLcom.GetCount();
	aType.SetSize(nCount);
	aKey.SetSize(nCount);
	aLcomD.SetSize(nCount);

	POSITION pos = m_mLcom.GetStartPosition();
	while(pos)
	{
		m_mLcom.GetNextAssoc(pos, MKey, LComD);
		
		aType[nCur]  = MKey.i1;
		aKey[nCur]   = MKey.i2;
		aLcomD[nCur] = LComD;

		nCur++;
	}        
	
	return aType.GetSize();
}

int CDgnClassIRED::GetLcomCount()
{
	return m_mLcom.GetCount();
}

BOOL CDgnClassIRED::AddLcom(S_LCOM_T nType, S_LCOM_K nKey, S_LCOM_D&  LcomD)
{
	T_IDEA_K<UINT>  MKey;
	MKey.i1 = nType;
	MKey.i2 = nKey;
		
	m_mLcom.SetAt(MKey, LcomD); 
	return TRUE;
}

// About Unbraced Length(L,Lb)
BOOL CDgnClassIRED::GetLeng(S_LENG_K nKey, S_LENG_D&  LengD)
{
	return m_mLeng.Lookup(nKey, LengD);
}

int CDgnClassIRED::GetLeng(S_LENG_K_LIST& aKey, S_LENG_D_LIST&  aLengD)
{
	aKey.RemoveAll();
	aLengD.RemoveAll();
	
	int nSize = GetLengKeyList(aKey);
	if (nSize < 1) return 0;
	
	aLengD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetLeng(aKey[i], aLengD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetLengKeyList(S_LENG_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mLeng.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetLengCount()
{
	return m_mLeng.GetCount();
}

BOOL CDgnClassIRED::AddLeng(S_LENG_K nKey, S_LENG_D&  LengD)
{
	if (m_mLeng.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mLeng.SetAt(nKey, LengD);
	return TRUE;
}

// About Effective Length Factor(K)
BOOL CDgnClassIRED::GetKfac(S_KFAC_K nKey, S_KFAC_D&  KfacD)
{
	return m_mKfac.Lookup(nKey, KfacD);
}

int CDgnClassIRED::GetKfac(S_KFAC_K_LIST& aKey, S_KFAC_D_LIST&  aKfacD)
{
	aKey.RemoveAll();
	aKfacD.RemoveAll();
	
	int nSize = GetKfacKeyList(aKey);
	if (nSize < 1) return 0;
	
	aKfacD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetKfac(aKey[i], aKfacD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetKfacKeyList(S_KFAC_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mKfac.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetKfacCount()
{
	return m_mKfac.GetCount();
}

BOOL CDgnClassIRED::AddKfac(S_KFAC_K nKey, S_KFAC_D&  KfacD)
{
	if (m_mKfac.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mKfac.SetAt(nKey, KfacD);
	return TRUE;
}

// About Limiting Slenderness Ratio
BOOL CDgnClassIRED::GetLtsr(S_LTSR_K nKey, S_LTSR_D&  LtsrD)
{
	return m_mLtsr.Lookup(nKey, LtsrD);
}

int CDgnClassIRED::GetLtsr(S_LTSR_K_LIST& aKey, S_LTSR_D_LIST&  aLtsrD)
{
	aKey.RemoveAll();
	aLtsrD.RemoveAll();
	
	int nSize = GetLtsrKeyList(aKey);
	if (nSize < 1) return 0;
	
	aLtsrD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetLtsr(aKey[i], aLtsrD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetLtsrKeyList(S_LTSR_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mLtsr.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetLtsrCount()
{
	return m_mLtsr.GetCount();
}

BOOL CDgnClassIRED::AddLtsr(S_LTSR_K nKey, S_LTSR_D&  LtsrD)
{
	if (m_mLtsr.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mLtsr.SetAt(nKey, LtsrD);
	return TRUE;
}

// About Equivalent Moment Correction Factor(Cm)
BOOL CDgnClassIRED::GetCmft(S_CMFT_K nKey, S_CMFT_D&  CmftD)
{
	return m_mCmft.Lookup(nKey, CmftD);
}

int CDgnClassIRED::GetCmft(S_CMFT_K_LIST& aKey, S_CMFT_D_LIST&  aCmftD)
{
	aKey.RemoveAll();
	aCmftD.RemoveAll();
	
	int nSize = GetCmftKeyList(aKey);
	if (nSize < 1) return 0;
	
	aCmftD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetCmft(aKey[i], aCmftD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetCmftKeyList(S_CMFT_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mCmft.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetCmftCount()
{
	return m_mCmft.GetCount();
}

BOOL CDgnClassIRED::AddCmft(S_CMFT_K nKey, S_CMFT_D&  CmftD)
{
	if (m_mCmft.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mCmft.SetAt(nKey, CmftD);
	return TRUE;
}

// About Moment Magnifier
BOOL CDgnClassIRED::GetFmag(S_FMAG_K nKey, S_FMAG_D&  FmagD)
{
	return m_mFmag.Lookup(nKey, FmagD);
}

int CDgnClassIRED::GetFmag(S_FMAG_K_LIST& aKey, S_FMAG_D_LIST&  aFmagD)
{
	aKey.RemoveAll();
	aFmagD.RemoveAll();
	
	int nSize = GetFmagKeyList(aKey);
	if (nSize < 1) return 0;
	
	aFmagD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetFmag(aKey[i], aFmagD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetFmagKeyList(S_FMAG_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mFmag.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetFmagCount()
{
	return m_mFmag.GetCount();
}

BOOL CDgnClassIRED::AddFmag(S_FMAG_K nKey, S_FMAG_D&  FmagD)
{
	if (m_mFmag.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mFmag.SetAt(nKey, FmagD);
	return TRUE;
}

BOOL CDgnClassIRED::SetDcon(S_DCON_D dcon)
{
	m_Dcon = dcon;
	return TRUE;
}

void CDgnClassIRED::GetDcon(S_DCON_D&  dcon)
{
	dcon = m_Dcon;
}

// Beam Section Data for Design
BOOL CDgnClassIRED::GetRdbm(S_RDBM_K nKey, S_RDGN_BEAM&  RdbmD)
{
	return m_mRdbm.Lookup(nKey, RdbmD); 
}

int CDgnClassIRED::GetRdbm(S_RDBM_K_LIST& aKey, S_RDBM_D_LIST&  aRdbmD)
{
	aKey.RemoveAll();
	aRdbmD.RemoveAll();
	
	int nSize = GetRdbmKeyList(aKey);
	if (nSize < 1) return 0;
	
	aRdbmD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetRdbm(aKey[i], aRdbmD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetRdbmKeyList(S_RDBM_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mRdbm.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;

}

int CDgnClassIRED::GetRdbmCount()
{
	return m_mRdbm.GetCount();
}

BOOL CDgnClassIRED::AddRdbm(S_RDBM_K nKey, S_RDGN_BEAM&  RdbmD)
{
	if (m_mRdbm.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mRdbm.SetAt(nKey, RdbmD);
	return TRUE;

}

// Column Section Data for Design
BOOL CDgnClassIRED::GetRdcn(S_RDCN_K nKey, S_RDGN_COLM&  RdcnD)
{
	return m_mRdcn.Lookup(nKey, RdcnD); 
}

int CDgnClassIRED::GetRdcn(S_RDCN_K_LIST& aKey, S_RDCN_D_LIST&  aRdcnD)
{
	aKey.RemoveAll();
	aRdcnD.RemoveAll();
	
	int nSize = GetRdcnKeyList(aKey);
	if (nSize < 1) return 0;
	
	aRdcnD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetRdcn(aKey[i], aRdcnD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetRdcnKeyList(S_RDCN_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mRdcn.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetRdcnCount()
{
	return m_mRdcn.GetCount();
}

BOOL CDgnClassIRED::AddRdcn(S_RDCN_K nKey, S_RDGN_COLM&  RdcnD)
{
	if (m_mRdcn.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mRdcn.SetAt(nKey, RdcnD);
	return TRUE;

}

// Beam Section Data for Checking
BOOL CDgnClassIRED::GetRcbm(S_RCBM_K nKey, S_RCHK_BEAM&  RcbmD)
{
	return m_mRcbm.Lookup(nKey, RcbmD);
}

int CDgnClassIRED::GetRcbm(S_RCBM_K_LIST& aKey, S_RCBM_D_LIST&  aRcbmD)
{
	aKey.RemoveAll();
	aRcbmD.RemoveAll();
	
	int nSize = GetRcbmKeyList(aKey);
	if (nSize < 1) return 0;
	
	aRcbmD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetRcbm(aKey[i], aRcbmD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 

}

int CDgnClassIRED::GetRcbmKeyList(S_RCBM_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mRcbm.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;

}

int CDgnClassIRED::GetRcbmCount()
{
	return m_mRcbm.GetCount();
}

BOOL CDgnClassIRED::AddRcbm(S_RCBM_K nKey, S_RCHK_BEAM&  RcbmD)
{
	if (m_mRcbm.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mRcbm.SetAt(nKey, RcbmD);
	return TRUE;

}

// Column Section Data for Checking
BOOL CDgnClassIRED::GetRccn(S_RCCN_K nKey, S_RCHK_COLM&  RccnD)
{
	return m_mRccn.Lookup(nKey, RccnD);
}
int CDgnClassIRED::GetRccn(S_RCCN_K_LIST& aKey, S_RCCN_D_LIST&  aRccnD)
{
	aKey.RemoveAll();
	aRccnD.RemoveAll();
	
	int nSize = GetRccnKeyList(aKey);
	if (nSize < 1) return 0;
	
	aRccnD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetRccn(aKey[i], aRccnD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetRccnKeyList(S_RCCN_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mRccn.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;

}

int CDgnClassIRED::GetRccnCount()
{
	return m_mRccn.GetCount();
}

BOOL CDgnClassIRED::AddRccn(S_RCCN_K nKey, S_RCHK_COLM&  RccnD)
{
	if (m_mRccn.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mRccn.SetAt(nKey, RccnD);
	return TRUE;
}

BOOL CDgnClassIRED::GetRebt(S_REBT_K nKey, S_REBT_D&  RebtD)
{
	return m_mRebt.Lookup(nKey, RebtD);
}

int CDgnClassIRED::GetRebt(S_REBT_K_LIST& aKey, S_REBT_D_LIST&  aRebtD)
{
	aKey.RemoveAll();
	aRebtD.RemoveAll();
	
	int nSize = GetRebtKeyList(aKey);
	if (nSize < 1) return 0;
	
	aRebtD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetRebt(aKey[i], aRebtD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}
int CDgnClassIRED::GetRebtKeyList(S_REBT_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mRebt.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetRebtCount()
{
	return m_mRebt.GetCount();
}

BOOL CDgnClassIRED::AddRebt(S_REBT_K nKey, S_REBT_D&  RebtD)
{
	if (m_mRebt.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mRebt.SetAt(nKey, RebtD);
	return TRUE;
}

// Rebar for PSC
BOOL CDgnClassIRED::GetRpsc(S_RPSC_K nKey, S_RPSC_SSM_D&  RpscD)
{
	return m_mRpsc.Lookup(nKey, RpscD);
}

int CDgnClassIRED::GetRpsc(S_RPSC_K_LIST& aKey, S_RPSC_D_LIST&  aRpscD)
{
	aKey.RemoveAll();
	aRpscD.RemoveAll();
	
	int nSize = GetRpscKeyList(aKey);
	if (nSize < 1) return 0;
	
	aRpscD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetRpsc(aKey[i], aRpscD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetRpscKeyList(S_RPSC_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mRpsc.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetRpscCount()
{
	return m_mRpsc.GetCount();
}

BOOL CDgnClassIRED::AddRpsc(S_RPSC_K nKey, S_RPSC_SSM_D&  RpscD)
{
	if (m_mRpsc.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mRpsc.SetAt(nKey, RpscD);
	return TRUE;
}

// Tendon of Cross section
BOOL CDgnClassIRED::GetTdif(S_TDIF_K nKey, S_TDIJ_K nIJ, S_TDIF_D&  TdifD)
{
	T_IDEA_K<UINT>  MKey;
	MKey.i1 = nKey;
	MKey.i2 = nIJ;

	return m_mTdif.Lookup(MKey, TdifD);
}

int CDgnClassIRED::GetTdif(S_TDIF_K_LIST& aKey, S_TDIJ_K_LIST& aIJ, S_TDIF_D_LIST&  aTdifD)
{
	T_IDEA_K<UINT>  MKey;
	S_TDIF_D        TdifD;
	
	aKey.RemoveAll();
	aIJ.RemoveAll();
	aTdifD.RemoveAll();
	
	POSITION pos = m_mTdif.GetStartPosition();

	int nCur(0), nCount = m_mTdif.GetCount();
	aKey.SetSize(nCount);
	aIJ.SetSize(nCount);
	aTdifD.SetSize(nCount);

	while(pos)
	{
		m_mTdif.GetNextAssoc(pos, MKey, TdifD);
		
		aKey[nCur]   = MKey.i1;
		aIJ[nCur]    = MKey.i2;    
		aTdifD[nCur] = TdifD;

		nCur++;
	}        
	
	return aKey.GetSize();      
}

int CDgnClassIRED::GetTdifCount()
{
	return m_mTdif.GetCount();
}

BOOL CDgnClassIRED::AddTdif(S_TDIF_K nKey, S_TDIJ_K nIJ, S_TDIF_D&  TdifD)
{
	T_IDEA_K<UINT>  MKey;
	MKey.i1 = nKey;
	MKey.i2 = nIJ;  
	
	m_mTdif.SetAt(MKey, TdifD);
	return TRUE;
}

// Tendon Time-Dependent Loss Graph
BOOL CDgnClassIRED::GetTdls(S_TDPF_K nKey, S_STAG_K nStag, S_STEP_K nStep, S_TDLS_D& TdlsD)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nKey;
	MKey.i2 = nStag;
	MKey.i3 = nStep;
	
	return m_mTdls.Lookup(MKey, TdlsD);
}

int CDgnClassIRED::GetTdls(S_TDPF_K_LIST& aKey, S_STAG_K_LIST& aStag, S_STEP_K_LIST& aStep, S_TDLS_D_LIST&  aTdlsD)
{
	T_IDEA_K3<UINT>  MKey;
	S_TDLS_D        TdlsD;
	
	aKey.RemoveAll();
	aStag.RemoveAll();
	aStep.RemoveAll();
	aTdlsD.RemoveAll();
	
	POSITION pos = m_mTdls.GetStartPosition();
	
	int nCur(0), nCount = m_mTdls.GetCount();
	aKey.SetSize(nCount);
	aStag.SetSize(nCount);
	aStep.SetSize(nCount);
	aTdlsD.SetSize(nCount);
	
	while(pos)
	{
		m_mTdls.GetNextAssoc(pos, MKey, TdlsD);
		
		aKey[nCur]   = MKey.i1;
		aStag[nCur]  = MKey.i2;    
		aStep[nCur]  = MKey.i3;    
		aTdlsD[nCur] = TdlsD;
		
		nCur++;
	}        
	
	return aKey.GetSize();      

}

int CDgnClassIRED::GetTdlsCount()
{
	return m_mTdls.GetCount();

}

BOOL CDgnClassIRED::AddTdls(S_TDPF_K nKey, S_STAG_K nStag, S_STEP_K nStep, S_TDLS_D&  TdlsD)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nKey;
	MKey.i2 = nStag;
	MKey.i3 = nStep;
	
	m_mTdls.SetAt(MKey, TdlsD);
	return TRUE;
}

// Reaction
BOOL CDgnClassIRED::GetReac(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_REAC_D& reac)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;

	return m_mReac.Lookup(MKey, reac);
}

int CDgnClassIRED::GetReac(S_NODE_K_LIST& aNodeK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_REAC_D_LIST& aReacD)
{
	T_IDEA_K3<UINT>  MKey;
	S_REAC_D        ReacD;
	
	aNodeK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aReacD.RemoveAll();

	int nCur(0), nCount = m_mReac.GetCount();
	aNodeK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aReacD.SetSize(nCount);
	
	POSITION pos = m_mReac.GetStartPosition();
	while(pos)
	{
		m_mReac.GetNextAssoc(pos, MKey, ReacD);
		
		aNodeK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aReacD[nCur] = ReacD;

		nCur++;
	}        
	
	return aNodeK.GetSize();  
}

int CDgnClassIRED::GetReacCount()
{
	return m_mReac.GetCount();
}

BOOL CDgnClassIRED::AddReac(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_REAC_D& reac)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;

	m_mReac.SetAt(MKey, reac);

	return TRUE;
}

// Displacement
BOOL CDgnClassIRED::GetDisp(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_DISP_D& disp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nLdcsK;
	
	return m_mDisp.Lookup(MKey, disp);
}

int CDgnClassIRED::GetDisp(S_NODE_K_LIST& aNodeK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_DISP_D_LIST& aDispD)
{
	T_IDEA_K3<UINT>  MKey;
	S_DISP_D        DispD;
	
	aNodeK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aDispD.RemoveAll();
	
	int nCur(0), nCount = m_mDisp.GetCount();
	aNodeK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aDispD.SetSize(nCount);

	POSITION pos = m_mDisp.GetStartPosition();
	while(pos)
	{
		m_mDisp.GetNextAssoc(pos, MKey, DispD);
		
		aNodeK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aDispD[nCur] = DispD;

		nCur++;
	}        
	
	return aNodeK.GetSize();  
}

int CDgnClassIRED::GetDispCount()
{
	return m_mDisp.GetCount();
}

BOOL CDgnClassIRED::AddDisp(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_DISP_D& disp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;    

	m_mDisp.SetAt(MKey, disp);
	return TRUE;
}

// Truss Force
BOOL CDgnClassIRED::GetTfor(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_TFOR_D& tfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	return m_mTfor.Lookup(MKey, tfor);
}

int CDgnClassIRED::GetTfor(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_TFOR_D_LIST& aTForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_TFOR_D        TForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aTForD.RemoveAll();
	
	POSITION pos = m_mTfor.GetStartPosition();
	int nCur(0), nCount = m_mTfor.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aTForD.SetSize(nCount);

	while(pos)
	{
		m_mTfor.GetNextAssoc(pos, MKey, TForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aTForD[nCur] = TForD;

		nCur++;
	}        
	
	return aElemK.GetSize();
}

int CDgnClassIRED::GetTforCount()
{
	return m_mTfor.GetCount();
}

BOOL CDgnClassIRED::AddTfor(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_TFOR_D& tfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mTfor.SetAt(MKey, tfor);

	return TRUE;
}

// Beam Force
BOOL CDgnClassIRED::GetBfor(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_D& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	return m_mBfor.Lookup(MKey, bfor);
}

int CDgnClassIRED::GetBfor(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BFOR_D_LIST& aBForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_BFOR_D        BForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aBForD.RemoveAll();
	
	POSITION pos = m_mBfor.GetStartPosition();
	int nCur(0), nCount = m_mBfor.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aBForD.SetSize(nCount);

	while(pos)
	{
		m_mBfor.GetNextAssoc(pos, MKey, BForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aBForD[nCur] = BForD;

		nCur++;
	}        
	
	return aElemK.GetSize();
}

int CDgnClassIRED::GetBforCount()
{
	return m_mBfor.GetCount();
}

BOOL CDgnClassIRED::AddBfor(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_D& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mBfor.SetAt(MKey, bfor);

	return TRUE;
}

// Plate force
BOOL CDgnClassIRED::GetStrp(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_STRP_DL& strp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	return m_mSTRP.Lookup(MKey, strp);  
}

int CDgnClassIRED::GetStrp(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_STRP_DL_LIST& aStrlD)
{
	T_IDEA_K3<UINT>  MKey;
	S_STRP_DL        StrpDL;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aStrlD.RemoveAll();
	
	POSITION pos = m_mSTRP.GetStartPosition();
	int nCur(0), nCount = m_mSTRP.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aStrlD.SetSize(nCount);
	
	while(pos)
	{
		m_mSTRP.GetNextAssoc(pos, MKey, StrpDL);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aStrlD[nCur] = StrpDL;
		
		nCur++;
	}        
	
	return aElemK.GetSize();

}

int CDgnClassIRED::GetStrpCount()
{
	return m_mSTRP.GetCount();
}

BOOL CDgnClassIRED::AddStrp(S_ELEM_K  nElemK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_STRP_DL& strp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mSTRP.SetAt(MKey, strp);

	return TRUE;
}

// Reaction
BOOL CDgnClassIRED::GetRecc(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_RECC_D& reac)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;

	return m_mRecc.Lookup(MKey, reac);
}

int CDgnClassIRED::GetRecc(S_NODE_K_LIST& aNodeK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_RECC_D_LIST& aReacD)
{
	T_IDEA_K3<UINT>  MKey;
	S_RECC_D        ReacD;
	
	aNodeK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aReacD.RemoveAll();
	
	POSITION pos = m_mRecc.GetStartPosition();
	int nCur(0), nCount = m_mRecc.GetCount();
	aNodeK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aReacD.SetSize(nCount);

	while(pos)
	{
		m_mRecc.GetNextAssoc(pos, MKey, ReacD);
		
		aNodeK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aReacD[nCur] = ReacD;

		nCur++;
	}        
	
	return aNodeK.GetSize();  
}

int CDgnClassIRED::GetReccCount()
{
	return m_mRecc.GetCount();
}

BOOL CDgnClassIRED::AddRecc(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_RECC_D& reac)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;

	m_mRecc.SetAt(MKey, reac);

	return TRUE;
}

// Displacement
BOOL CDgnClassIRED::GetDspc(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_DSPC_D& disp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	
	return m_mDspc.Lookup(MKey, disp);
}

int CDgnClassIRED::GetDspc(S_NODE_K_LIST& aNodeK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_DSPC_D_LIST& aDscpD)
{
	T_IDEA_K3<UINT>  MKey;
	S_DSPC_D        DspcD;
	
	aNodeK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aDscpD.RemoveAll();
	
	POSITION pos = m_mDspc.GetStartPosition();
	int nCur(0), nCount = m_mDspc.GetCount();
	aNodeK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aDscpD.SetSize(nCount);

	while(pos)
	{
		m_mDspc.GetNextAssoc(pos, MKey, DspcD);
		
		aNodeK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aDscpD[nCur] = DspcD;

		nCur++;
	}        
	
	return aNodeK.GetSize();  
}

int CDgnClassIRED::GetDspcCount()
{
	return m_mDspc.GetCount();
}

BOOL CDgnClassIRED::AddDspc(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_DSPC_D& disp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mDspc.SetAt(MKey, disp);

	return TRUE;
}

// Beam Concurrent Force
BOOL CDgnClassIRED::GetBcfo(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BCFO_B& bcfo)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	return m_mBcfo.Lookup(MKey, bcfo);
}

int CDgnClassIRED::GetBcfo(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BCFO_B_LIST& aBcfoD)
{
	T_IDEA_K3<UINT>  MKey;
	S_BCFO_B   BcfoD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aBcfoD.RemoveAll();
	
	POSITION pos = m_mBcfo.GetStartPosition();
	int nCur(0), nCount = m_mBcfo.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aBcfoD.SetSize(nCount);

	while(pos)
	{
		m_mBcfo.GetNextAssoc(pos, MKey, BcfoD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aBcfoD[nCur] = BcfoD;

		nCur++;
	}        

	return aElemK.GetSize();
}

int CDgnClassIRED::GetBcfoCount()
{
	return m_mBcfo.GetCount();
}

BOOL CDgnClassIRED::AddBcfo(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BCFO_B& bcfo)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mBcfo.SetAt(MKey, bcfo);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// About Construction Stage Load Case
BOOL CDgnClassIRED::GetCslc(S_STLD_K nKey, S_STLD_D&  StldD)
{
	return m_mCslc.Lookup(nKey, StldD);
}

int CDgnClassIRED::GetCslc(S_STLD_K_LIST& aKey, S_STLD_D_LIST&  aStldD)
{
	aKey.RemoveAll();
	aStldD.RemoveAll();
	
	int nSize = GetCslcKeyList(aKey);
	if (nSize < 1) return 0;
	
	aStldD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetCslc(aKey[i], aStldD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetCslcKeyList(S_STLD_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mCslc.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetCslcCount()
{
	return m_mCslc.GetCount();
}

BOOL CDgnClassIRED::AddCslc(S_STLD_K nKey, S_STLD_D&  StldD) 
{
	if (m_mCslc.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mCslc.SetAt(nKey, StldD);
	return TRUE;
}

// Auto Generated Construction Stage LoadCase
BOOL CDgnClassIRED::GetClat(S_STLD_K nKey, S_STLD_D&  StldD)
{
	return m_mClat.Lookup(nKey, StldD);
}

int CDgnClassIRED::GetClat(S_STLD_K_LIST& aKey, S_STLD_D_LIST&  aStldD)
{
	aKey.RemoveAll();
	aStldD.RemoveAll();
	
	int nSize = GetClatKeyList(aKey);
	if (nSize < 1) return 0;
	
	aStldD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetClat(aKey[i], aStldD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetClatKeyList(S_STLD_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mClat.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetClatCount()
{
	return m_mClat.GetCount();
}

BOOL CDgnClassIRED::AddClat(S_STLD_K nKey, S_STLD_D&  StldD) 
{
	if (m_mClat.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mClat.SetAt(nKey, StldD);
	return TRUE;
}

// Construction Stage
BOOL CDgnClassIRED::GetStag(S_STAG_K nKey, S_STAG_D&  StagD)
{
	return m_mStag.Lookup(nKey, StagD);
}

int CDgnClassIRED::GetStag(S_STAG_K_LIST& aKey, S_STAG_D_LIST&  aStagD)
{
	aKey.RemoveAll();
	aStagD.RemoveAll();
	
	int nSize = GetStagKeyList(aKey);
	if (nSize < 1) return 0;
	
	aStagD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetStag(aKey[i], aStagD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetStagKeyList(S_STAG_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mStag.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetStagCount()
{
	return m_mStag.GetCount();
}

BOOL CDgnClassIRED::AddStag(S_STAG_K nKey, S_STAG_D&  StagD)   
{
	if (m_mStag.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mStag.SetAt(nKey, StagD);
	return TRUE;
}

// Composite Section for Construction Stage
BOOL CDgnClassIRED::GetCscs(S_CSCS_K nKey, S_CSCS_D&  CscsD)
{
	return m_mCscs.Lookup(nKey, CscsD);
}

int CDgnClassIRED::GetCscs(S_CSCS_K_LIST& aKey, S_CSCS_D_LIST&  aCscsD)
{
	aKey.RemoveAll();
	aCscsD.RemoveAll();
	
	int nSize = GetCscsKeyList(aKey);
	if (nSize < 1) return 0;
	
	aCscsD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetCscs(aKey[i], aCscsD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetCscsKeyList(S_CSCS_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mCscs.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetCscsCount()
{
	return m_mCscs.GetCount();
}

BOOL CDgnClassIRED::AddCscs(S_CSCS_K nKey, S_CSCS_D&  CscsD)   
{
	if (m_mCscs.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mCscs.SetAt(nKey, CscsD);
	return TRUE;
}
// Element Group
BOOL CDgnClassIRED::GetGrup(S_GRUP_K nKey, S_GRUP_D&  GrupD)
{
	return m_mGrup.Lookup(nKey, GrupD);
}

int CDgnClassIRED::GetGrup(S_GRUP_K_LIST& aKey, S_GRUP_D_LIST&  aGrupD)
{
	aKey.RemoveAll();
	aGrupD.RemoveAll();
	
	int nSize = GetGrupKeyList(aKey);
	if (nSize < 1) return 0;
	
	aGrupD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetGrup(aKey[i], aGrupD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetGrupKeyList(S_GRUP_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mGrup.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetGrupCount()
{
	return m_mGrup.GetCount();
}

BOOL CDgnClassIRED::AddGrup(S_GRUP_K nKey, S_GRUP_D&  GrupD)   
{
	if (m_mGrup.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mGrup.SetAt(nKey, GrupD);
	return TRUE;
}

// Boundary Group
BOOL CDgnClassIRED::GetBngr(S_BNGR_K nKey, S_BNGR_D&  BngrD)
{
	return m_mBngr.Lookup(nKey, BngrD);
}

int CDgnClassIRED::GetBngr(S_BNGR_K_LIST& aKey, S_BNGR_D_LIST&  aBngrD)
{
	aKey.RemoveAll();
	aBngrD.RemoveAll();
	
	int nSize = GetBngrKeyList(aKey);
	if (nSize < 1) return 0;
	
	aBngrD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetBngr(aKey[i], aBngrD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetBngrKeyList(S_BNGR_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mBngr.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetBngrCount()
{
	return m_mBngr.GetCount();
}

BOOL CDgnClassIRED::AddBngr(S_BNGR_K nKey, S_BNGR_D&  BngrD)   
{
	if (m_mBngr.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mBngr.SetAt(nKey, BngrD);
	return TRUE;
}

// Load Group
BOOL CDgnClassIRED::GetLdgr(S_RPSC_K nKey, S_LDGR_D&  LdgrD)
{
	return m_mLdgr.Lookup(nKey, LdgrD);
}

int CDgnClassIRED::GetLdgr(S_LDGR_K_LIST& aKey, S_LDGR_D_LIST&  aLdgrD)
{
	aKey.RemoveAll();
	aLdgrD.RemoveAll();
	
	int nSize = GetLdgrKeyList(aKey);
	if (nSize < 1) return 0;
	
	aLdgrD.SetSize(nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (GetLdgr(aKey[i], aLdgrD[i]) == FALSE)
		{
			ASSERT(FALSE);
			continue;
		}
	}
	
	return nSize; 
}

int CDgnClassIRED::GetLdgrKeyList(S_LDGR_K_LIST& aKey)
{
	aKey.RemoveAll();  
	int nSize = m_mLdgr.GetKeyList(aKey);
	if (nSize<1) return 0;
	
	qsort((void*)aKey.GetData(), nSize, sizeof(int), Compare_UINTAsc);
	return nSize;
}

int CDgnClassIRED::GetLdgrCount()
{
	return m_mLdgr.GetCount();
}

BOOL CDgnClassIRED::AddLdgr(S_LDGR_K nKey, S_LDGR_D&  LdgrD)   
{
	if (m_mLdgr.Exist(nKey) == TRUE) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	m_mLdgr.SetAt(nKey, LdgrD);
	return TRUE;
}

BOOL CDgnClassIRED::GetDispCS(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_DISP_CSD& disp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	return m_mCsds.Lookup(MKey, disp);
}

int CDgnClassIRED::GetDispCS(S_NODE_K_LIST& aNodeK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_DISP_CSD_LIST& aDispD)
{
	T_IDEA_K3<UINT>  MKey;
	S_DISP_CSD       DispD;
	
	aNodeK.RemoveAll();
	aLdcsK.RemoveAll();
	aStagK.RemoveAll();
	aDispD.RemoveAll();
	
	int nCur(0), nCount = m_mCsds.GetCount();
	aNodeK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aStagK.SetSize(nCount);
	aDispD.SetSize(nCount);
	
	POSITION pos = m_mCsds.GetStartPosition();
	while(pos)
	{
		m_mCsds.GetNextAssoc(pos, MKey, DispD);
		
		aNodeK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aStagK[nCur] = MKey.i3;
		aDispD[nCur] = DispD;
		
		nCur++;
	}        
	
	return aNodeK.GetSize();  
}

int CDgnClassIRED::GetDispCountCS()
{
	return m_mCsds.GetCount();
}

BOOL CDgnClassIRED::AddDispCS(S_NODE_K  nNodeK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_DISP_CSD& disp)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nNodeK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;    
	
	m_mCsds.SetAt(MKey, disp);
	return TRUE;
}

// Truss Force
BOOL CDgnClassIRED::GetTforCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_TFOR_CSD& tfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	return m_mCstf.Lookup(MKey, tfor);  
}

int CDgnClassIRED::GetTforCS(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_TFOR_CSD_LIST& aTForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_TFOR_CSD       TForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aStagK.RemoveAll();
	aTForD.RemoveAll();
	
	POSITION pos = m_mCstf.GetStartPosition();
	int nCur(0), nCount = m_mCstf.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aStagK.SetSize(nCount);
	aTForD.SetSize(nCount);
	
	while(pos)
	{
		m_mCstf.GetNextAssoc(pos, MKey, TForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aStagK[nCur] = MKey.i3;
		aTForD[nCur] = TForD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();  
}

int CDgnClassIRED::GetTforCountCS()
{
	return m_mCstf.GetCount();  
}

BOOL CDgnClassIRED::AddTforCS(S_ELEM_K  nElemK, S_STLD_K  nStldK, S_STAG_K nStag, S_TFOR_CSD& tfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nStldK;
	MKey.i3 = nStag;
	
	m_mCstf.SetAt(MKey, tfor);
	
	return TRUE;  
}

// Beam Force
BOOL CDgnClassIRED::GetBforCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_CSD& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	return m_mCsbf.Lookup(MKey, bfor);  
}

int CDgnClassIRED::GetBforCS(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_BFOR_CSD_LIST& aBForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_BFOR_CSD       BForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aStagK.RemoveAll();
	aBForD.RemoveAll();
	
	POSITION pos = m_mCsbf.GetStartPosition();
	int nCur(0), nCount = m_mCsbf.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aStagK.SetSize(nCount);
	aBForD.SetSize(nCount);
	
	while(pos)
	{
		m_mCsbf.GetNextAssoc(pos, MKey, BForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aStagK[nCur] = MKey.i3;
		aBForD[nCur] = BForD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();
	
}

int CDgnClassIRED::GetBforCountCS()
{
	return m_mCsbf.GetCount();
}

BOOL CDgnClassIRED::AddBforCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_CSD& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	m_mCsbf.SetAt(MKey, bfor);
	
	return TRUE;

}

// Plate Force
BOOL CDgnClassIRED::GetStrpCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_STRP_CSDL& pfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	return m_mCspf.Lookup(MKey, pfor);  
}

int CDgnClassIRED::GetStrpCS(S_ELEM_K_LIST& aElemK, S_STLD_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_STRP_CSDL_LIST& aPForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_STRP_CSDL      PForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aStagK.RemoveAll();
	aPForD.RemoveAll();
	
	POSITION pos = m_mCspf.GetStartPosition();
	int nCur(0), nCount = m_mCspf.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aStagK.SetSize(nCount);
	aPForD.SetSize(nCount);
	
	while(pos)
	{
		m_mCspf.GetNextAssoc(pos, MKey, PForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aStagK[nCur] = MKey.i3;
		aPForD[nCur] = PForD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();
}

int CDgnClassIRED::GetStrpCountCS()
{
	return m_mCspf.GetCount();
}

BOOL CDgnClassIRED::AddStrpCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_STRP_CSDL& pfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	m_mCspf.SetAt(MKey, pfor);
	
	return TRUE;
}

// Beam Part Force 
BOOL CDgnClassIRED::GetBforPT(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PT& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	return m_mBforPT.Lookup(MKey, bfor);
}

int  CDgnClassIRED::GetBforPT(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BFOR_PT_LIST& aBForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_BFOR_PT        BForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aBForD.RemoveAll();
	
	POSITION pos = m_mBforPT.GetStartPosition();
	int nCur(0), nCount = m_mBforPT.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aBForD.SetSize(nCount);
	
	while(pos)
	{
		m_mBforPT.GetNextAssoc(pos, MKey, BForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aBForD[nCur] = BForD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();
}

int  CDgnClassIRED::GetBforPTCount()
{
	return m_mBforPT.GetCount();
}

BOOL CDgnClassIRED::AddBforPT(S_ELEM_K  nElemK, S_LDCS_K  nLdcsK, S_LDTP_K nType, S_BFOR_PT& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mBforPT.SetAt(MKey, bfor);
	
	return TRUE;
}

// Beam Part Force CC
BOOL CDgnClassIRED::GetBforPTCC(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PTCC& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	return m_mBforPTCC.Lookup(MKey, bfor);
}
int CDgnClassIRED::GetBforPTCC(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_LDTP_K_LIST& aTypeK, S_BFOR_PTCC_LIST& aBForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_BFOR_PTCC      BForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aTypeK.RemoveAll();
	aBForD.RemoveAll();
	
	POSITION pos = m_mBforPTCC.GetStartPosition();
	int nCur(0), nCount = m_mBforPTCC.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aTypeK.SetSize(nCount);
	aBForD.SetSize(nCount);
	
	while(pos)
	{
		m_mBforPTCC.GetNextAssoc(pos, MKey, BForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aTypeK[nCur] = MKey.i3;
		aBForD[nCur] = BForD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();
}

int CDgnClassIRED::GetBforPTCCCount()
{
	return m_mBforPTCC.GetCount();
}

BOOL CDgnClassIRED::AddBforPTCC(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_LDTP_K nType, S_BFOR_PTCC& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nType;
	
	m_mBforPTCC.SetAt(MKey, bfor);
	
	return TRUE;
}


// Beam Part Force 
BOOL CDgnClassIRED::GetBforPTCS(S_ELEM_K  nElemK, S_STLD_K  nLdcsK, S_STAG_K nStag, S_BFOR_PTCS& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	return m_mBforPTCS.Lookup(MKey, bfor);
}

int  CDgnClassIRED::GetBforPTCS(S_ELEM_K_LIST& aElemK, S_LDCS_K_LIST& aLdcsK, S_STAG_K_LIST& aStagK, S_BFOR_PTCS_LIST& aBForD)
{
	T_IDEA_K3<UINT>  MKey;
	S_BFOR_PTCS      BForD;
	
	aElemK.RemoveAll();
	aLdcsK.RemoveAll();
	aStagK.RemoveAll();
	aBForD.RemoveAll();
	
	POSITION pos = m_mBforPTCS.GetStartPosition();
	int nCur(0), nCount = m_mBforPTCS.GetCount();
	aElemK.SetSize(nCount);
	aLdcsK.SetSize(nCount);
	aStagK.SetSize(nCount);
	aBForD.SetSize(nCount);
	
	while(pos)
	{
		m_mBforPTCS.GetNextAssoc(pos, MKey, BForD);
		
		aElemK[nCur] = MKey.i1;
		aLdcsK[nCur] = MKey.i2;
		aStagK[nCur] = MKey.i3;
		aBForD[nCur] = BForD;
		
		nCur++;
	}        
	
	return aElemK.GetSize();
}

int  CDgnClassIRED::GetBforPTCSCount()
{
	return m_mBforPTCS.GetCount();
}

BOOL CDgnClassIRED::AddBforPTCS(S_ELEM_K  nElemK, S_LDCS_K  nLdcsK, S_STAG_K nStag, S_BFOR_PTCS& bfor)
{
	T_IDEA_K3<UINT>  MKey;
	MKey.i1 = nElemK;
	MKey.i2 = nLdcsK;
	MKey.i3 = nStag;
	
	m_mBforPTCS.SetAt(MKey, bfor);
	
	return TRUE;
}

