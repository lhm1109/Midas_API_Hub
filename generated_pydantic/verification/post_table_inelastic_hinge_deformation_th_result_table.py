from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Inelastic Hinge Deformation - TH Result Table (POST /post/TABLE)

class InelasticHingeDeformationTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InelasticHingeDeformationTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InelasticHingeDeformationTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["IEHG_DEFORM_LUMPED", "IEHG_DEFORM_DIST", "IEHG_DEFORM_WALL", "IEHG_DEFORM_TRUSS", "IEHG_DEFORM_SPRING"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InelasticHingeDeformationTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InelasticHingeDeformationTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["Elem", "HingeLocation", "InelasticHingeProp.", "Load", "Dx/Deform", "Dx/Time", "Dy/Deform", "Dy/Time", "Dz/Deform", "Dz/Time", "Rx/Deform", "Rx/Time", "Ry/Deform", "Ry/Time", "Rz/Deform", "Rz/Time", "WallID", "Story", "GeneralLink/No", "GeneralLink/Prop.", "GeneralLink/Node1", "GeneralLink/Node2"]] | None = Field(None, alias="COMPONENTS")

class InelasticHingeDeformationTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InelasticHingeDeformationTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-inelastic-hinge-deformation-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InelasticHingeDeformationTHResultTableRequest"]
