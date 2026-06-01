from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Load Summary Table (POST /post/TABLE)

class LoadSummaryTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class LoadSummaryTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class LoadSummaryTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["LOAD_SUMMARY_X", "LOAD_SUMMARY_Y", "LOAD_SUMMARY_Z"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: LoadSummaryTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: LoadSummaryTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Load", "Concent", "Beam", "Floor", "Pressure", "SelfWeight", "Sum"]] | None = Field(None, alias="COMPONENTS")

class LoadSummaryTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: LoadSummaryTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-load-summary-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["LoadSummaryTableRequest"]
