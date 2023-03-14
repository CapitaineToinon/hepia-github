#!/usr/bin/env zx
import "zx/globals";
import { BASE_URL } from "../constants.mjs";

export async function close_project(project_id) {
  const response = await fetch(`${BASE_URL}/projects/${project_id}/close`, {
    method: "POST",
  });

  if (!response.ok) {
    throw response;
  }
}
