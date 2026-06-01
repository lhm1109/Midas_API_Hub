from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Stiffness Irregularity Check (Soft Story) - Story Result Table (POST /POST/TABLE)

class StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentADDITIONALSETCALCULATIONMETHOD(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    story_drift_method: str | None = Field(None, alias="STORY_DRIFT_METHOD")
    story_stiffness_method: str | None = Field(None, alias="STORY_STIFFNESS_METHOD")

class StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_calculation_method: StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentADDITIONALSETCALCULATIONMETHOD | None = Field(None, alias="SET_CALCULATION_METHOD")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STIFFNESS_IRREGULARITY_X", "STIFFNESS_IRREGULARITY_Y"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    additional: StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["LoadCase", "Story", "Level", "StoryHeight", "StoryDrift", "StoryShearForce", "StoryStiffness", "UpperStoryStiffness/0.7Ku1", "UpperStoryStiffness/0.8Ku123", "StoryStiffnessRatio", "StoryDriftAngleRatio", "Remark"]] | None = Field(None, alias="COMPONENTS")

class StiffnessIrregularityCheckSoftStoryStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StiffnessIrregularityCheckSoftStoryStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-stiffness-irregularity-check-soft-story-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StiffnessIrregularityCheckSoftStoryStoryResultTableRequest"]
