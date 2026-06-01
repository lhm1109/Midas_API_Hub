from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Beam Force (Static Prestress) - Analysis Result Table (POST /post/TABLE)

class BeamForceStaticPrestressAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class BeamForceStaticPrestressAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class BeamForceStaticPrestressAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class BeamForceStaticPrestressAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["BEAMFORCESIP", "BEAMFORCESTP"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: BeamForceStaticPrestressAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: BeamForceStaticPrestressAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: BeamForceStaticPrestressAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    parts: list[Literal["Part1", "Part1/4", "Part2/4", "Part3/4", "PartJ"]] | None = Field(None, alias="PARTS")
    components: list[Literal["Elem", "Load", "Part", "Type", "Axial", "Shear-z", "Moment-y"]] | None = Field(None, alias="COMPONENTS")

class BeamForceStaticPrestressAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: BeamForceStaticPrestressAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-beam-force-static-prestress-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["BeamForceStaticPrestressAnalysisResultTableRequest"]
