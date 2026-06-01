from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Capacity Irregularity Check(Weak Story) - Story Result Table (POST /POST/TABLE)

class CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentADDITIONALSETANGLE(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    angle: float | None = Field(None, alias="ANGLE")

class CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_angle: CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentADDITIONALSETANGLE | None = Field(None, alias="SET_ANGLE")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["CAPACITY_IRREGULARITY"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    additional: CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["Story", "Level", "StoryHeight", "Angle1", "StoryShearStrength1", "UpperStoryShearStrength1", "StoryShearStrengthRatio1", "Remark1", "Angle2", "StoryShearStrength2", "UpperStoryShearStrength2", "StoryShearStrengthRatio2", "Remark2"]] | None = Field(None, alias="COMPONENTS")

class CapacityIrregularityCheckWeakStoryStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: CapacityIrregularityCheckWeakStoryStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-capacity-irregularity-check-weak-story-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["CapacityIrregularityCheckWeakStoryStoryResultTableRequest"]
