from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Element Weight Table (POST /post/TABLE)

class ElementWeightTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class ElementWeightTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class ElementWeightTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    to: str | None = Field(None, alias="TO")
    keys: list[int] | None = Field(None, alias="KEYS")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class ElementWeightTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["ELEMENTWEIGHT"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: ElementWeightTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: ElementWeightTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: ElementWeightTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    components: list[Literal["No", "Type", "Name", "Value", "UnitWeight", "TotalWeight"]] | None = Field(None, alias="COMPONENTS")

class ElementWeightTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: ElementWeightTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-element-weight-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["ElementWeightTableRequest"]
