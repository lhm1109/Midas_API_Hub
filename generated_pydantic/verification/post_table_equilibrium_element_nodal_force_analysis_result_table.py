from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Equilibrium Element Nodal Force - Analysis Result Table (POST /post/TABLE)

class EquilibriumElementNodalForceAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class EquilibriumElementNodalForceAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class EquilibriumElementNodalForceAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["EQUILIBRIUM_ELEM_FORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: EquilibriumElementNodalForceAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: EquilibriumElementNodalForceAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Type", "ID", "ElementNodalForce-i/Fx", "ElementNodalForce-i/Fy", "ElementNodalForce-i/Fz", "ElementNodalForce-i/Mx", "ElementNodalForce-i/My", "ElementNodalForce-i/Mz", "ElementNodalForce-j/Fx", "ElementNodalForce-j/Fy", "ElementNodalForce-j/Fz", "ElementNodalForce-j/Mx", "ElementNodalForce-j/My", "ElementNodalForce-j/Mz"]] | None = Field(None, alias="COMPONENTS")

class EquilibriumElementNodalForceAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: EquilibriumElementNodalForceAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-equilibrium-element-nodal-force-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["EquilibriumElementNodalForceAnalysisResultTableRequest"]
