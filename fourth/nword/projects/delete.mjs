#!/usr/bin/env zx
import "zx/globals";
import { BASE_URL } from "../constants.mjs";

export async function delete_project(project_id) {
  const response = await fetch(`${BASE_URL}/projects/${project_id}`, {
    method: "DELETE",
  });

  if (!response.ok) {
    throw response;
  }
}
