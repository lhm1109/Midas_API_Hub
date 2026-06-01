from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Plate Force (Unit Length) - Analysis Result Table (POST /post/TABLE)

class PlateForceUnitLengthAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PlateForceUnitLengthAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PlateForceUnitLengthAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlateForceUnitLengthAnalysisResultTableRequestArgumentNODEFLAG(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    center: bool | None = Field(None, alias="CENTER")
    nodes: bool | None = Field(None, alias="NODES")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlateForceUnitLengthAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PLATEFORCEUCL", "PLATEFORCEG", "PLATEFORCEUCG", "PLATEFORCEUGVBM", "PLATEFORCEWA", "PLATEFORCEUL", "PLATEFORCEULVBM", "PLATEFORCEUG"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PlateForceUnitLengthAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PlateForceUnitLengthAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: PlateForceUnitLengthAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    average_nodal_result: bool | None = Field(None, alias="AVERAGE_NODAL_RESULT")
    node_flag: PlateForceUnitLengthAnalysisResultTableRequestArgumentNODEFLAG | None = Field(None, alias="NODE_FLAG")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    item_to_display: list[Literal["Fxx", "Fyy", "Fxy", "Mxx", "Myy", "Mxy", "Vxx", "Vyy"]] | None = Field(None, alias="ITEM_TO_DISPLAY")
    components: list[Literal["Elem", "Load", "Node", "Fxx", "Fyy", "Fxy", "Fmax", "Fmin", "Angle", "Mxx", "Myy", "Mxy", "Mmax", "Mmin", "Vxx", "Vyy", "Component", "Ma", "Mb", "Mab", "W-AMomentTopDir.1", "W-AMomentTopDir.2", "W-AMomentBotDir.1", "W-AMomentBotDir.2", "Stage", "Step"]] | None = Field(None, alias="COMPONENTS")

class PlateForceUnitLengthAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PlateForceUnitLengthAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-plate-force-unit-length-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PlateForceUnitLengthAnalysisResultTableRequest"]
