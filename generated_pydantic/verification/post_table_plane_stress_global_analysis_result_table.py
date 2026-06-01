from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Plane Stress (Global) - Analysis Result Table (POST /post/TABLE)

class PlaneStressGlobalAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PlaneStressGlobalAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PlaneStressGlobalAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlaneStressGlobalAnalysisResultTableRequestArgumentNODEFLAG(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    center: bool | None = Field(None, alias="CENTER")
    nodes: bool | None = Field(None, alias="NODES")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlaneStressGlobalAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PLANESTRESSSG"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PlaneStressGlobalAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PlaneStressGlobalAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: PlaneStressGlobalAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    average_nodal_result: bool | None = Field(None, alias="AVERAGE_NODAL_RESULT")
    node_flag: PlaneStressGlobalAnalysisResultTableRequestArgumentNODEFLAG | None = Field(None, alias="NODE_FLAG")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "Node", "Sig-XX", "Sig-YY", "Sig-ZZ", "Sig-XY", "Sig-YZ", "Sig-XZ", "Sig-Max", "Sig-Min", "Angle", "Sig-EFF", "Max-Shear", "Stage", "Step", "Sig-xx", "Sig-yy", "Sig-xy"]] | None = Field(None, alias="COMPONENTS")

class PlaneStressGlobalAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PlaneStressGlobalAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-plane-stress-global-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PlaneStressGlobalAnalysisResultTableRequest"]
