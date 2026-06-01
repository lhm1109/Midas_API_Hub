from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Torsional Irregularity Check - Story Result Table (POST /POST/TABLE)

class TorsionalIrregularityCheckStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TorsionalIrregularityCheckStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TorsionalIrregularityCheckStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TORSIONAL_IRREGULARITY_X", "TORSIONAL_IRREGULARITY_Y"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TorsionalIrregularityCheckStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TorsionalIrregularityCheckStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["LoadCase", "Story", "Level", "StoryHeight", "AverageValueOfExtremePoints/StoryDrift", "AverageValueOfExtremePoints/1.2*StoryDrift", "MaximumValue/Node", "MaximumValue/StoryDrift", "Remark"]] | None = Field(None, alias="COMPONENTS")

class TorsionalIrregularityCheckStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TorsionalIrregularityCheckStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-torsional-irregularity-check-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TorsionalIrregularityCheckStoryResultTableRequest"]
