from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Inelastic Hinge Beam Summary - TH Result Table (POST /post/TABLE)

class InelasticHingeBeamSummaryTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InelasticHingeBeamSummaryTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InelasticHingeBeamSummaryTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class InelasticHingeBeamSummaryTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["IEHG_BEAM_SUM_DX", "IEHG_BEAM_SUM_DY", "IEHG_BEAM_SUM_DZ", "IEHG_BEAM_SUM_RY", "IEHG_BEAM_SUM_RZ"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InelasticHingeBeamSummaryTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InelasticHingeBeamSummaryTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: InelasticHingeBeamSummaryTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["Type", "Elem", "HingeLocation", "InelasticHingeProp.", "Load", "Time/Step", "Deform", "Force", "max(D/D1)", "max(D/D2)", "Status", "Performance", "P1", "P2", "P3", "D1", "D2", "D3"]] | None = Field(None, alias="COMPONENTS")

class InelasticHingeBeamSummaryTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InelasticHingeBeamSummaryTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-inelastic-hinge-beam-summary-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InelasticHingeBeamSummaryTHResultTableRequest"]
