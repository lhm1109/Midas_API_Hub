from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Drift(X,Y,Combine) - Story Result Table (POST /post/TABLE)

class StoryDriftXYCombineStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryDriftXYCombineStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONALSETSTORYDRIFTPARAMSLCOMSItem(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    name: str | None = Field(None, alias="NAME")
    factor: int | None = Field(None, alias="FACTOR")

class StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONALSETSTORYDRIFTPARAMSBETA(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    fix_user_check: str | None = Field(None, alias="FIX_USER_CHECK")
    name_from: str | None = Field(None, alias="NAME_FROM")
    name_to: str | None = Field(None, alias="NAME_TO")
    value: int | None = Field(None, alias="VALUE")

class StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONALSETSTORYDRIFTPARAMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    response_mod_factor_check: bool | None = Field(None, alias="RESPONSE_MOD_FACTOR_CHECK")
    deflection_ampl_factor_value: int | None = Field(None, alias="DEFLECTION_AMPL_FACTOR_VALUE")
    importance_factor_value: float | None = Field(None, alias="IMPORTANCE_FACTOR_VALUE")
    scale_factor_value: int | None = Field(None, alias="SCALE_FACTOR_VALUE")
    allowable_ratio: float | None = Field(None, alias="ALLOWABLE_RATIO")
    lcoms: list[StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONALSETSTORYDRIFTPARAMSLCOMSItem] | None = Field(None, alias="LCOMS")
    beta: StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONALSETSTORYDRIFTPARAMSBETA | None = Field(None, alias="BETA")

class StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_story_drift_params: StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONALSETSTORYDRIFTPARAMS | None = Field(None, alias="SET_STORY_DRIFT_PARAMS")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class StoryDriftXYCombineStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_DRIFT_X", "STORY_DRIFT_Y", "STORY_DRIFT_COMB"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryDriftXYCombineStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryDriftXYCombineStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    additional: StoryDriftXYCombineStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["LoadCase", "Story", "StoryHeight", "P-DeltaIncrementalFactor", "AllowableStoryDriftRatio", "MaximumDriftOfAllVerticalElements/Node", "MaximumDriftOfAllVerticalElements/StoryDrift", "MaximumDriftOfAllVerticalElements/ModifiedDrift", "MaximumDriftOfAllVerticalElements/StoryDriftRatio", "MaximumDriftOfAllVerticalElements/Remark", "DriftAtTheCenterOfMass/StoryDrift", "DriftAtTheCenterOfMass/ModifiedDrift", "DriftAtTheCenterOfMass/DriftFactor", "DriftAtTheCenterOfMass/StoryDriftRatio", "DriftAtTheCenterOfMass/Remark", "MaximumDriftofAllVerticalElements/Node", "MaximumDriftofAllVerticalElements/StoryDrift", "MaximumDriftofAllVerticalElements/ModifiedDrift", "MaximumDriftofAllVerticalElements/StoryDriftRatio", "MaximumDriftofAllVerticalElements/Remark", "DriftattheCenterofMass/StoryDrift", "DriftattheCenterofMass/ModifiedDrift", "DriftattheCenterofMass/DriftFactor", "DriftattheCenterofMass/StoryDriftRatio", "DriftattheCenterofMass/Remark", "MaximumDriftOfAllVerticalElements/Shear-WeightedAverageDriftOfVerticalElements", "AverageDriftOfVerticalElements/Remark", "AverageDriftOfVerticalElements/StoryDrift", "AverageDriftOfVerticalElements/ModifiedDrift", "AverageDriftOfVerticalElements/DriftFactor", "AverageDriftOfVerticalElements/StoryDriftRatio", "DriftOfAVerticalLineOnSelectedNode/Remark", "DriftOfAVerticalLineOnSelectedNode/StoryDrift", "DriftOfAVerticalLineOnSelectedNode/ModifiedDrift", "DriftOfAVerticalLineOnSelectedNode/DriftFactor", "DriftOfAVerticalLineOnSelectedNode/StoryDriftRatio", "AverageDriftOfVerticalLinesOnSelectedNodes/Remark", "AverageDriftOfVerticalLinesOnSelectedNodes/StoryDrift", "AverageDriftOfVerticalLinesOnSelectedNodes/ModifiedDrift", "AverageDriftOfVerticalLinesOnSelectedNodes/DriftFactor", "AverageDriftOfVerticalLinesOnSelectedNodes/StoryDriftRatio"]] | None = Field(None, alias="COMPONENTS")

class StoryDriftXYCombineStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryDriftXYCombineStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-drift-x-y-combine-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryDriftXYCombineStoryResultTableRequest"]
