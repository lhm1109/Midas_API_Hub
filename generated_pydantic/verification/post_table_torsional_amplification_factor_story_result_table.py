from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Torsional Amplification Factor - Story Result Table (POST /POST/TABLE)

class TorsionalAmplificationFactorStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TorsionalAmplificationFactorStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TorsionalAmplificationFactorStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TORSIONAL_AMPLIFICATION_FACTOR_X", "TORSIONAL_AMPLIFICATION_FACTOR_Y"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TorsionalAmplificationFactorStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TorsionalAmplificationFactorStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["LoadCase", "Story", "Level", "StoryHeight", "AverageDisplacementOfExtremePoints", "MaximumDisplacement/Node", "MaximumDisplacement/Displacement", "TorsionalAmplificationFactor", "Note"]] | None = Field(None, alias="COMPONENTS")

class TorsionalAmplificationFactorStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TorsionalAmplificationFactorStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-torsional-amplification-factor-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TorsionalAmplificationFactorStoryResultTableRequest"]
