from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Nodal Results of RS - Analysis Result Table (POST /post/TABLE)

class NodalResultsOfRSAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class NodalResultsOfRSAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class NodalResultsOfRSAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class NodalResultsOfRSAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["RS_NODAL_INERTIA", "RS_NODAL_ACCEL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: NodalResultsOfRSAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: NodalResultsOfRSAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: NodalResultsOfRSAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    modes: list[str] | None = Field(None, alias="MODES")
    components: list[Literal["LoadCase", "Mode", "Node", "FX", "FY", "FZ", "MX", "MY", "MZ", "DX", "DY", "DZ", "RX", "RY", "RZ"]] | None = Field(None, alias="COMPONENTS")

class NodalResultsOfRSAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: NodalResultsOfRSAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-nodal-results-of-rs-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["NodalResultsOfRSAnalysisResultTableRequest"]
