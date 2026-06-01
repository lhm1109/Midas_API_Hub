from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Displacement(X,Y,Combined) - Story Result Table (POST /POST/TABLE)

class StoryDisplacementXYCombinedStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryDisplacementXYCombinedStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryDisplacementXYCombinedStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_DISPLACEMENT_X", "STORY_DISPLACEMENT_Y", "STORY_DISPLACEMENT_COMB"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryDisplacementXYCombinedStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryDisplacementXYCombinedStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["LoadCase", "Node", "Story", "Level", "StoryHeight", "MaximumDisplacement", "AverageDisplacement", "Maximum/Average"]] | None = Field(None, alias="COMPONENTS")

class StoryDisplacementXYCombinedStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryDisplacementXYCombinedStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-displacement-x-y-combined-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryDisplacementXYCombinedStoryResultTableRequest"]
