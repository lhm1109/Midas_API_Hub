from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Ultimate Story Shear For Check - Story Result Table (POST /POST/TABLE)

class UltimateStoryShearForCheckStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class UltimateStoryShearForCheckStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class UltimateStoryShearForCheckStoryResultTableRequestArgumentADDITIONALSETANGLE(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    angle: int | None = Field(None, alias="ANGLE")

class UltimateStoryShearForCheckStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_angle: UltimateStoryShearForCheckStoryResultTableRequestArgumentADDITIONALSETANGLE | None = Field(None, alias="SET_ANGLE")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class UltimateStoryShearForCheckStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["ULTIMATE_STORY_SHEAR_FORCE_CHECK"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: UltimateStoryShearForCheckStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: UltimateStoryShearForCheckStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    additional: UltimateStoryShearForCheckStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["Story", "LoadCase", "Angle", "AppliedShearForce(Ve)", "Clockwise/UltimateShearForce1(Vp)/Column", "Clockwise/UltimateShearForce1(Vp)/Wall", "Clockwise/UltimateShearForce1(Vp)/SUM", "Clockwise/Ratio1", "Clockwise/Beta1", "Counter-Clockwise/UltimateShearForce2(Vp)/Column", "Counter-Clockwise/UltimateShearForce2(Vp)/Wall", "Counter-Clockwise/UltimateShearForce2(Vp)/SUM", "Counter-Clockwise/Ratio2", "Counter-Clockwise/Beta2", "MIN", "Remark"]] | None = Field(None, alias="COMPONENTS")

class UltimateStoryShearForCheckStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: UltimateStoryShearForCheckStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-ultimate-story-shear-for-check-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["UltimateStoryShearForCheckStoryResultTableRequest"]
