from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Fiber Section Estimate Yield Strength - TH Result Table (POST /post/TABLE)

class FiberSectionEstimateYieldStrengthTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class FiberSectionEstimateYieldStrengthTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class FiberSectionEstimateYieldStrengthTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class FiberSectionEstimateYieldStrengthTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["FIBR_YILEDSTRENGTH"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: FiberSectionEstimateYieldStrengthTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: FiberSectionEstimateYieldStrengthTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: FiberSectionEstimateYieldStrengthTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["Type", "Definition", "Elem", "HingeLocation", "InelasticHingeProp.", "LoadCase", "Crack/Rc'", "Crack/Mc'", "Crack/Step", "Yield/Ry'", "Yield/My'", "Yield/Step", "Ultimate/Ru", "Ultimate/Mu", "Ultimate/Step", "EstimateYield/Ry", "EstimateYield/My", "EstimateYield/Step", "Mmax", "Ieff", "Ieff/I"]] | None = Field(None, alias="COMPONENTS")

class FiberSectionEstimateYieldStrengthTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: FiberSectionEstimateYieldStrengthTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-fiber-section-estimate-yield-strength-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["FiberSectionEstimateYieldStrengthTHResultTableRequest"]
