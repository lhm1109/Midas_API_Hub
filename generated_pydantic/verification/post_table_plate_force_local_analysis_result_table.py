from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Plate Force (Local) - Analysis Result Table (POST /post/TABLE)

class PlateForceLocalAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PlateForceLocalAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PlateForceLocalAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlateForceLocalAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PLATEFORCEL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PlateForceLocalAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PlateForceLocalAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: PlateForceLocalAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    average_nodal_result: bool | None = Field(None, alias="AVERAGE_NODAL_RESULT")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "Node", "Fx", "Fy", "Fz", "Mx", "My", "Mz", "Stage", "Step"]] | None = Field(None, alias="COMPONENTS")

class PlateForceLocalAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PlateForceLocalAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-plate-force-local-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PlateForceLocalAnalysisResultTableRequest"]
