from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Mass Summary Table (POST /post/TABLE)

class MassSummaryTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class MassSummaryTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class MassSummaryTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["MASS_SUMMARY_X", "MASS_SUMMARY_Y", "MASS_SUMMARY_Z"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: MassSummaryTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: MassSummaryTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Node", "NodalMass", "LoadToMass", "StructureMass", "Sum"]] | None = Field(None, alias="COMPONENTS")

class MassSummaryTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: MassSummaryTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-mass-summary-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["MassSummaryTableRequest"]
