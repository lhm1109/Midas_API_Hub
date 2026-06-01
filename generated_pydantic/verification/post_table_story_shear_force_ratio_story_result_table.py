from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Shear Force Ratio - Story Result Table (POST /POST/TABLE)

class StoryShearForceRatioStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryShearForceRatioStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryShearForceRatioStoryResultTableRequestArgumentADDITIONALSETANGLE(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    angle: float | None = Field(None, alias="ANGLE")

class StoryShearForceRatioStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_angle: StoryShearForceRatioStoryResultTableRequestArgumentADDITIONALSETANGLE | None = Field(None, alias="SET_ANGLE")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class StoryShearForceRatioStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_SHEAR_FORCE_RATIO"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryShearForceRatioStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryShearForceRatioStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    story_names: list[str] | None = Field(None, alias="STORY_NAMES")
    additional: StoryShearForceRatioStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["Story", "Level", "Load", "Type", "No", "Angle1", "Force1", "Ratio1", "Angle2", "Force2", "Ratio2"]] | None = Field(None, alias="COMPONENTS")

class StoryShearForceRatioStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryShearForceRatioStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-shear-force-ratio-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryShearForceRatioStoryResultTableRequest"]
