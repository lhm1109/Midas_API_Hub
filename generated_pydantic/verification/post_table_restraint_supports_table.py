from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Restraint Supports Table (POST /post/TABLE)

class RestraintSupportsTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class RestraintSupportsTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class RestraintSupportsTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["SUPPORTS"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: RestraintSupportsTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: RestraintSupportsTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Node", "Dx", "Dy", "Dz", "Rx", "Ry", "Rz", "Rw", "Group"]] | None = Field(None, alias="COMPONENTS")

class RestraintSupportsTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: RestraintSupportsTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-restraint-supports-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["RestraintSupportsTableRequest"]
