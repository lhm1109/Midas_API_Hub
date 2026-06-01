from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Inelastic Hinge Event Time - TH Result Table (POST /post/TABLE)

class InelasticHingeEventTimeTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InelasticHingeEventTimeTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InelasticHingeEventTimeTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["IEHG_EVENT_TIME_LUMPED", "IEHG_EVENT_TIME_DIST", "IEHG_EVENT_TIME_WALL", "IEHG_EVENT_TIME_TRUSS", "IEHG_EVENT_TIME_SPRING"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InelasticHingeEventTimeTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InelasticHingeEventTimeTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["Elem", "HingeLocation", "Load", "1stYield/Dx", "1stYield/Dy", "1stYield/Dz", "1stYield/Rx", "1stYield/Ry", "1stYield/Rz", "2ndYield/Dx", "2ndYield/Dy", "2ndYield/Dz", "2ndYield/Rx", "2ndYield/Ry", "2ndYield/Rz", "3rdYield/Dx", "3rdYield/Dy", "3rdYield/Dz", "3rdYield/Rx", "3rdYield/Ry", "3rdYield/Rz", "WallID", "Story", "InelasticHingeProp.", "GeneralLink/No", "GeneralLink/Prop.", "GeneralLink/Node1", "GeneralLink/Node2"]] | None = Field(None, alias="COMPONENTS")

class InelasticHingeEventTimeTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InelasticHingeEventTimeTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-inelastic-hinge-event-time-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InelasticHingeEventTimeTHResultTableRequest"]
