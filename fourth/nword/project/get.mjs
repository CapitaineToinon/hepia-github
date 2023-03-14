#!/usr/bin/env zx
import "zx/globals";
import { get_projects } from "../projects/index.mjs";

export async function get_project(project_name) {
  const projects = await get_projects();
  const project = projects.find(({ name }) => name === project_name);

  if (!project) {
    throw new Error(`project ${project_name} not found`);
  }

  return project;
}
