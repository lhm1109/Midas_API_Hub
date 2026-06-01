from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Time History Text - Node Results (POST /post/TEXT)

class TimeHistoryTextNodeResultsRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TimeHistoryTextNodeResultsRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TimeHistoryTextNodeResultsRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TimeHistoryTextNodeResultsRequestArgumentSTEP(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    from_: float | None = Field(None, alias="FROM")
    to: str | None = Field(None, alias="TO")
    steps: int | None = Field(None, alias="STEPS")
    keys: list[int] | None = Field(None, alias="KEYS")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TimeHistoryTextNodeResultsRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TH_DISP", "TH_VELOCITY", "TH_ACCEL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TimeHistoryTextNodeResultsRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TimeHistoryTextNodeResultsRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    text_type: str | None = Field(None, alias="TEXT_TYPE")
    node_elems: TimeHistoryTextNodeResultsRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    th_case_name: list[str] | None = Field(None, alias="TH_CASE_NAME")
    step: TimeHistoryTextNodeResultsRequestArgumentSTEP | None = Field(None, alias="STEP")
    ref_pt: str | None = Field(None, alias="REF_PT")
    components: list[Literal["Node", "Load", "Time/Step", "Dx", "Dy", "Dz", "Rx", "Ry", "Rz"]] | None = Field(None, alias="COMPONENTS")

class TimeHistoryTextNodeResultsRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TimeHistoryTextNodeResultsRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-text-time-history-text-node-results"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TimeHistoryTextNodeResultsRequest"]
