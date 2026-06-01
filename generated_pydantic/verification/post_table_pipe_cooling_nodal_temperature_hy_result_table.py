from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Pipe Cooling Nodal Temperature - HY Result Table (POST /post/TABLE)

class PipeCoolingNodalTemperatureHYResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PipeCoolingNodalTemperatureHYResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PipeCoolingNodalTemperatureHYResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["HEAT_HYDR_PIPE_NODE_TEMP"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PipeCoolingNodalTemperatureHYResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PipeCoolingNodalTemperatureHYResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["PipeCooling", "Stage", "Step", "Time", "Node", "Temperature"]] | None = Field(None, alias="COMPONENTS")

class PipeCoolingNodalTemperatureHYResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PipeCoolingNodalTemperatureHYResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-pipe-cooling-nodal-temperature-hy-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PipeCoolingNodalTemperatureHYResultTableRequest"]
