from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Inelastic Hinge Ductility Factor(D/D1) - TH Result Table (POST /post/TABLE)

class InelasticHingeDuctilityFactorDD1THResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InelasticHingeDuctilityFactorDD1THResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InelasticHingeDuctilityFactorDD1THResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["IEHG_DUCT_D1_LUMPED", "IEHG_DUCT_D1_DIST", "IEHG_DUCT_D1_WALL", "IEHG_DUCT_D1_TRUSS", "IEHG_DUCT_D1_SPRING"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InelasticHingeDuctilityFactorDD1THResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InelasticHingeDuctilityFactorDD1THResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["Elem", "HingeLocation", "InelasticHingeProp.", "Load", "Dx/max", "Dx/Time", "Dy/max", "Dy/Time", "Dz/max", "Dz/Time", "Rx/max", "Rx/Time", "Ry/max", "Ry/Time", "Rz/max", "Rz/Time", "WallID", "Story", "GeneralLink/No", "GeneralLink/Prop.", "GeneralLink/Node1", "GeneralLink/Node2"]] | None = Field(None, alias="COMPONENTS")

class InelasticHingeDuctilityFactorDD1THResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InelasticHingeDuctilityFactorDD1THResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-inelastic-hinge-ductility-factor-d-d1-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InelasticHingeDuctilityFactorDD1THResultTableRequest"]
