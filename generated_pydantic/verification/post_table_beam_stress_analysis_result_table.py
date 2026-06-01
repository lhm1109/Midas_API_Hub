from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Beam Stress - Analysis Result Table (POST /post/TABLE)

class BeamStressAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class BeamStressAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class BeamStressAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class BeamStressAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["BEAMSTRESS", "BEAMSTRESS7DOF", "BEAMSTRESSVBM"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: BeamStressAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: BeamStressAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: BeamStressAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    parts: list[Literal["Part1", "Part1/4", "Part2/4", "Part3/4", "PartJ"]] | None = Field(None, alias="PARTS")
    section_position: list[Literal["Pos-1", "Pos-2", "Pos-3", "Pos-4", "Max"]] | None = Field(None, alias="SECTION_POSITION")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    item_to_display: list[Literal["Axial", "Shear-y", "Shear-z", "Bend(+y)", "Bend(-y)", "Bend(+z)", "Bend(-z)"]] | None = Field(None, alias="ITEM_TO_DISPLAY")
    components: list[Literal["Elem", "Load", "Part", "Axial", "Shear-y", "Shear-z", "Bend(+y)", "Bend(-y)", "Bend(+z)", "Bend(-z)", "Cb(min/max)", "Cb1(-y+z)", "Cb2(+y+z)", "Cb3(+y-z)", "Cb4(-y-z)", "Stage", "Step", "Component", "SectionPosition", "Sax(Warping)", "Ssy(Mt)", "Ssy(Mw)", "Ssz(Mt)", "Ssz(Mw)", "Cb(Ssy)", "Cb(Ssz)"]] | None = Field(None, alias="COMPONENTS")

class BeamStressAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: BeamStressAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-beam-stress-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["BeamStressAnalysisResultTableRequest"]
