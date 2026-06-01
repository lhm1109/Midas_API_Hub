from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Open Project (POST /doc/OPEN)

class OpenProjectRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    file_path: str = Field(..., alias="FILE_PATH")

class OpenProjectRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: OpenProjectRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "doc/open/open-project"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["OpenProjectRequest"]
