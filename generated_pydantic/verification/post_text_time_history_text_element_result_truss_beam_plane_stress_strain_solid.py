from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Time History Text - Element Result(Truss, Beam, Plane Stress/Strain, Solid) (POST /post/TEXT)

class TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentSTEP(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    from_: float | None = Field(None, alias="FROM")
    to: str | None = Field(None, alias="TO")
    steps: int | None = Field(None, alias="STEPS")
    keys: list[int] | None = Field(None, alias="KEYS")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TH_BEAMFORCE", "TH_TRUSSFORCE", "TH_PLANE_STRESS_FORCE", "TH_PLANE_STRAIN_FORCE", "TH_SOLIDFORCE", "TH_BEAMSTRESS", "TH_TRUSSSTRESS", "TH_PLANESTRESS", "TH_PLANE_STRAIN_STRESS", "TH_SOLIDSTRESS"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    text_type: str | None = Field(None, alias="TEXT_TYPE")
    node_elems: TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    parts: list[str] | None = Field(None, alias="PARTS")
    th_case_name: list[str] | None = Field(None, alias="TH_CASE_NAME")
    step: TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgumentSTEP | None = Field(None, alias="STEP")
    components: list[Literal["Elem", "Load", "Time/Step", "Part", "Axial", "Shear-y", "Shear-z", "Torsion", "Moment-y", "Moment-z", "Force-I", "Force-J", "Fx", "Fy", "Fz", "Bend(+y)", "Bend(-y)", "Bend(+z)", "Bend(-z)", "Stress-I", "Stress-J", "Sig-xx", "Sig-yy", "Sig-xy", "Sig-zz", "Sig-yz", "Sig-xz"]] | None = Field(None, alias="COMPONENTS")

class TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-text-time-history-text-element-result-truss-beam-plane-stress-strain-solid"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TimeHistoryTextElementResultTrussBeamPlaneStressStrainSolidRequest"]
