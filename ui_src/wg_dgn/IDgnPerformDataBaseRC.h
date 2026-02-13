#pragma once

#include "IDgnPerformDataBase.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ IDgnPerformDataBaseRC : public IDgnPerformDataBase
{
public:
    IDgnPerformDataBaseRC();
    virtual ~IDgnPerformDataBaseRC();

protected:
    virtual dgn::def::enDgn GetDgnType() override;

public:
    virtual bool IsPrint1waySlabDgn() { ASSERT(0); return false; }

    virtual bool GetRebbFromResult(int MembKey, T_REBB_D& rRebbD) { ASSERT(0); return false; }
    virtual bool GetRebcFromResult(int MembKey, T_REBC_D& rRebcD) { ASSERT(0); return false; }
    virtual bool GetRebrFromResult(int MembKey, T_REBR_D& rRebrD) { ASSERT(0); return false; }
    virtual bool GetRebwFromResult(int MembKey, T_REBW_D& rRebrD, int& rnBEH1, int& rnBEH2) { ASSERT(0); return false; }
    virtual bool GetRbmsFromResult(int ElemKey, T_RBMS_D rRbmsD[4][4]) { ASSERT(0); return false; }
    virtual bool GetRbslFromResult(int ElemKey, T_RBSL_D rRbslD[4]) { ASSERT(0); return false; }

	virtual bool GetRebarFromResult(dgn::def::enMemb MembType, int MembKey, dgn::def::IInput* prData) { ASSERT(0); return false; }

	virtual bool GetRdgnDefaultValue(dgn::def::enMemb MembType, int MembKey, T_RDGN_D& Rdgn) { ASSERT(0); return false; }

public:
    virtual bool IsMeshed2D() const { ASSERT(0); return false; }
    virtual bool IsSlabAverageNodal() const { ASSERT(0); return false; }
    virtual bool IsSlabFlexural() const { ASSERT(0); return false; }
    virtual bool IsSlab1WayFlexural() const { ASSERT(0); return false; }
    virtual bool IsSlabPunchingShear() const { ASSERT(0); return false; }
    virtual bool IsSlab1WayShear() const { ASSERT(0); return false; }
    virtual bool IsSlabServiceability() const { ASSERT(0); return false; }

    virtual bool IsShellFlexureCheck() { ASSERT(0); return false; }
	virtual bool IsShellShearCheck() { ASSERT(0); return false; }

public:
	virtual double Get2DMemberLength(T_SBDO_K SbdoK) { ASSERT(0); return 0.0; }

public:
    virtual int GetSlabSelectedLcomK() const { ASSERT(0); return 0; }
    virtual int GetSlabFlexuralDir() const { ASSERT(0); return 0; }
    virtual int GetSlabCellOption() const { ASSERT(0); return 0; }
    virtual int GetSlabPunchingType() const { ASSERT(0); return 0; }
    virtual int GetSlabPunchingStressType() const { ASSERT(0); return 0; }
    virtual int GetCuttingDiagramLeftRight() const { ASSERT(0); return 0; }
    virtual int GetDeflectionCheckType() const { ASSERT(0); return 0; }
    virtual double GetSlabCellSize() const { ASSERT(0); return 0.0; }
    virtual double GetDeflectionCreepCoeff() const { ASSERT(0); return 0.0; }
    virtual int GetLcomKeyListMeshedMemb(T_LCOM_K_LIST& raLcomK) { ASSERT(0); return 0; }
    virtual bool IsLongTermDef() const { ASSERT(0); return false; }

public:
    virtual bool IsDesigning() override;
};

#include "HeaderPost.h"
