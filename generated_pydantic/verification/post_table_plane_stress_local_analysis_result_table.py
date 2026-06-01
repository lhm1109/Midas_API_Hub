from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Plane Stress (Local) - Analysis Result Table (POST /post/TABLE)

class PlaneStressLocalAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PlaneStressLocalAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PlaneStressLocalAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlaneStressLocalAnalysisResultTableRequestArgumentNODEFLAG(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    center: bool | None = Field(None, alias="CENTER")
    nodes: bool | None = Field(None, alias="NODES")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlaneStressLocalAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PLANESTRESSSL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PlaneStressLocalAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PlaneStressLocalAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: PlaneStressLocalAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    average_nodal_result: bool | None = Field(None, alias="AVERAGE_NODAL_RESULT")
    node_flag: PlaneStressLocalAnalysisResultTableRequestArgumentNODEFLAG | None = Field(None, alias="NODE_FLAG")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "Node", "Sig-xx", "Sig-yy", "Sig-xy", "Sig-Max", "Sig-Min", "Angle", "Sig-EFF", "Max-Shear", "Stage", "Step"]] | None = Field(None, alias="COMPONENTS")

class PlaneStressLocalAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PlaneStressLocalAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-plane-stress-local-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PlaneStressLocalAnalysisResultTableRequest"]
