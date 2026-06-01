from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Nodal Body Force Table (POST /post/TABLE)

class NodalBodyForceTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class NodalBodyForceTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class NodalBodyForceTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["NODALBODYFORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: NodalBodyForceTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: NodalBodyForceTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["LoadCase", "Node", "FX", "FY", "FZ"]] | None = Field(None, alias="COMPONENTS")

class NodalBodyForceTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: NodalBodyForceTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-nodal-body-force-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["NodalBodyForceTableRequest"]
