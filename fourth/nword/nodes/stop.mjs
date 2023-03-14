#!/usr/bin/env zx
import "zx/globals";
import { BASE_URL } from "../constants.mjs";

export async function stop_nodes(project_id) {
  const response = await fetch(
    `${BASE_URL}/projects/${project_id}/nodes/stop`,
    {
      method: "POST",
    }
  );

  if (!response.ok) {
    throw response;
  }
}
