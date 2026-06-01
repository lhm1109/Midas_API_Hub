from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from General Link - TH Result Table (POST /post/TABLE)

class GeneralLinkTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class GeneralLinkTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class GeneralLinkTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class GeneralLinkTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["THISGLINKFORCE", "THISGLINKDEFORMATION"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: GeneralLinkTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: GeneralLinkTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: GeneralLinkTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["No.", "Load", "Node", "Axial/Force", "Axial/Time/Step", "Shear-y/Force", "Shear-y/Time/Step", "Shear-z/Force", "Shear-z/Time/Step", "Torsion/Force", "Torsion/Time/Step", "Moment-y/Force", "Moment-y/Time/Step", "Moment-z/Force", "Moment-z/Time/Step", "Node1", "Node2", "Dx/Deformation", "Dx/Time/Step", "Dy/Deformation", "Dy/Time/Step", "Dz/Deformation", "Dz/Time/Step", "Rx/Deformation", "Rx/Time/Step", "Ry/Deformation", "Ry/Time/Step", "Rz/Deformation", "Rz/Time/Step"]] | None = Field(None, alias="COMPONENTS")

class GeneralLinkTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: GeneralLinkTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-general-link-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["GeneralLinkTHResultTableRequest"]
