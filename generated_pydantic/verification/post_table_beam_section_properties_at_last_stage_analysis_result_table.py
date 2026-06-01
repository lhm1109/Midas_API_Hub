from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Beam Section Properties at Last Stage - Analysis Result Table (POST /post/TABLE)

class BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["SECT_PROP_LAST_STAGE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    part: list[Literal["I", "J"]] | None = Field(None, alias="PART")
    node_elems: BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    components: list[Literal["Elem", "Part", "Area", "Ixx", "Iyy", "Izz", "Cyp", "Cym", "Czp", "Czm", "WArea", "TranslationalDistance/Local-y", "TranslationalDistance/Local-z"]] | None = Field(None, alias="COMPONENTS")

class BeamSectionPropertiesAtLastStageAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: BeamSectionPropertiesAtLastStageAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-beam-section-properties-at-last-stage-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["BeamSectionPropertiesAtLastStageAnalysisResultTableRequest"]
