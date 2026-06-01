from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Overturning Moment - Story Result Table (POST /POST/TABLE)

class OverturningMomentStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class OverturningMomentStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class OverturningMomentStoryResultTableRequestArgumentADDITIONALSETANGLE(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    angle: float | None = Field(None, alias="ANGLE")

class OverturningMomentStoryResultTableRequestArgumentADDITIONALSETOVERTURNINGMOMENTPARAMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    sf_for_rs: int | None = Field(None, alias="SF_FOR_RS")
    define_rf: str | None = Field(None, alias="DEFINE_RF")

class OverturningMomentStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_angle: OverturningMomentStoryResultTableRequestArgumentADDITIONALSETANGLE | None = Field(None, alias="SET_ANGLE")
    set_overturning_moment_params: OverturningMomentStoryResultTableRequestArgumentADDITIONALSETOVERTURNINGMOMENTPARAMS | None = Field(None, alias="SET_OVERTURNING_MOMENT_PARAMS")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class OverturningMomentStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["OVERTURNING_MOMENT"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: OverturningMomentStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: OverturningMomentStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    additional: OverturningMomentStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["LoadCase", "Story", "Level", "StoryHeight", "ReductionFactor", "Angle1", "OverturningMomentByVerticalMemberTypes/Frame/Value", "OverturningMomentByVerticalMemberTypes/Frame/Ratio", "OverturningMomentByVerticalMemberTypes/Wall/Value", "OverturningMomentByVerticalMemberTypes/Wall/Ratio", "SumOfStoryForce1*Distance", "OverturningMoment1", "Angle2", "SumOfStoryForce2*Distance", "OverturningMoment2"]] | None = Field(None, alias="COMPONENTS")

class OverturningMomentStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: OverturningMomentStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-overturning-moment-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["OverturningMomentStoryResultTableRequest"]
