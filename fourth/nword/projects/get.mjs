#!/usr/bin/env zx
import "zx/globals";
import { BASE_URL } from "../constants.mjs";

export async function get_projects() {
  const response = await fetch(`${BASE_URL}/projects`);

  if (!response.ok) {
    throw response;
  }

  return await response.json();
}
