from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Temperature - HY Result Table (POST /post/TABLE)

class TemperatureHYResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TemperatureHYResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TemperatureHYResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TemperatureHYResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["HEAT_HYDR_TEMPERATURE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TemperatureHYResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TemperatureHYResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: TemperatureHYResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Node", "Stage", "Step", "Time", "Temperature"]] | None = Field(None, alias="COMPONENTS")

class TemperatureHYResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TemperatureHYResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-temperature-hy-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TemperatureHYResultTableRequest"]
