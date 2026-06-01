from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Material Table (POST /post/TABLE)

class MaterialTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class MaterialTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class MaterialTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["MATERIAL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: MaterialTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: MaterialTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["ID", "Name", "Type", "Standard", "Code", "DB", "UseMassDensity", "Elasticity", "Poisson", "Thermal", "Density", "MassDensity", "Standard2", "Code2", "DB2", "Elasticity2", "Poisson2", "Thermal2", "Density2", "MassDensity2", "PlasticMatl.", "Sp.Heat", "HeatCo.", "MaterialType", "ShearMod._xy", "Elasticity_y", "Thermal_y", "ShearMod._xz", "Poisson_xz", "Elasticity_z", "Thermal_z", "ShearMod._yz", "Poisson_yz"]] | None = Field(None, alias="COMPONENTS")

class MaterialTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: MaterialTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-material-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["MaterialTableRequest"]
