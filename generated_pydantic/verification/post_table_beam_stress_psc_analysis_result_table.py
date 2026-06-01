from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Beam Stress (PSC) - Analysis Result Table (POST /post/TABLE)

class BeamStressPSCAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class BeamStressPSCAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class BeamStressPSCAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class BeamStressPSCAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["BEAMSTRESSPSC", "BEAMSTRESS7DOFPSC"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: BeamStressPSCAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: BeamStressPSCAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: BeamStressPSCAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    parts: list[Literal["Part1", "Part1/4", "Part2/4", "Part3/4", "PartJ"]] | None = Field(None, alias="PARTS")
    section_position: list[Literal["Pos-1", "Pos-2", "Pos-3", "Pos-4", "Pos-5", "Pos-6", "Pos-7", "Pos-8", "Pos-9", "Pos-10", "Pos-11", "Pos-12", "Pos-13", "Pos-14", "Pos-15", "Pos-16", "Max", "Min", "All"]] | None = Field(None, alias="SECTION_POSITION")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "Part", "SectionPosition", "Sig-xx(Axial)", "Sig-xx(Moment-y)", "Sig-xx(Moment-z)", "Sig-xx(Bar)", "Sig-xx(Summation)", "Sig-zz", "Sig-xz(shear)", "Sig-xz(torsion)", "Sig-xz(bar)", "Sig-Is(shear)", "Sig-Is(shear+torsion)", "Sig-Ps(Max)", "Sig-Ps(Min)", "Stage", "Step", "Sax(Warping)", "Ssy(Mt)", "Ssy(Mw)", "Ssz(Mt)", "Ssz(Mw)", "Combined(Ssy)", "Combined(Ssz)"]] | None = Field(None, alias="COMPONENTS")

class BeamStressPSCAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: BeamStressPSCAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-beam-stress-psc-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["BeamStressPSCAnalysisResultTableRequest"]
