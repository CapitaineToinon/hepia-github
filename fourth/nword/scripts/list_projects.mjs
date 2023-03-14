#!/usr/bin/env zx
import "zx/globals";
import { get_projects } from "../projects/index.mjs";

const projects = await get_projects();
const project_names = projects.map((project) => project.name);

for (const project_name of project_names) {
  echo(project_name);
}
