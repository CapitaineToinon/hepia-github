#!/usr/bin/env zx
import "zx/globals";
import { BASE_URL } from "../constants.mjs";

export async function get_nodes(project_id) {
  const response = await fetch(`${BASE_URL}/projects/${project_id}/nodes`);

  if (!response.ok) {
    throw response;
  }

  return await response.json();
}
