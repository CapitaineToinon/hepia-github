#!/usr/bin/env zx
import "zx/globals";
import { get_projects, delete_project } from "../projects/index.mjs";

const projects = await get_projects();

const response = await question(
  `You're about to delete ${projects.length} projects. Are you sure?\n`
);

if (!["yes", "y"].includes(response.toLowerCase())) {
  echo("Aborting");
  exit(1);
}

const promises = projects.map((project) => delete_project(project.project_id));
await Promise.all(promises);
