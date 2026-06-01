from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Composite Section for C.S. (Force and Stress) - Analysis Result Table (POST /post/TABLE)

class CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["COMPSECTBEAMFORCE", "COMPSECTBEAMSTRESS"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    parts: list[str] | None = Field(None, alias="PARTS")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "SectionPart", "Part", "Axial", "Moment-y", "Moment-z", "DOF", "Bend(+y)", "Bend(-y)", "Bend(+z)", "Bend(-z)", "Cb(min/max)", "Cb1(-y+z)", "Cb2(+y+z)", "Cb3(+y-z)", "Cb4(-y-z)", "Sax(Warping)1", "Sax(Warping)2", "Sax(Warping)3", "Sax(Warping)4"]] | None = Field(None, alias="COMPONENTS")

class CompositeSectionForCSForceAndStressAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: CompositeSectionForCSForceAndStressAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-composite-section-for-c-s-force-and-stress-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["CompositeSectionForCSForceAndStressAnalysisResultTableRequest"]
