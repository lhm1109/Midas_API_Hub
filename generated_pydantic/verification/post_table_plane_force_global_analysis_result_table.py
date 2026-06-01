from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Plane Force (Global) - Analysis Result Table (POST /post/TABLE)

class PlaneForceGlobalAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PlaneForceGlobalAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PlaneForceGlobalAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlaneForceGlobalAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PLANESTRESSFG"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PlaneForceGlobalAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PlaneForceGlobalAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: PlaneForceGlobalAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    average_nodal_result: bool | None = Field(None, alias="AVERAGE_NODAL_RESULT")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "Node", "FX", "FY", "FZ", "Stage", "Step"]] | None = Field(None, alias="COMPONENTS")

class PlaneForceGlobalAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PlaneForceGlobalAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-plane-force-global-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PlaneForceGlobalAnalysisResultTableRequest"]
