#!/usr/bin/env zx
import "zx/globals";
import { reload_nodes } from "../nodes/index.mjs";
import { get_project, open_project } from "../project/index.mjs";

const { project: project_name } = argv;

if (!project_name) {
  echo("project name is required");
  exit(1);
}

const { name, project_id, status } = await get_project(project_name);
const isOpen = status === "opened";

if (!isOpen) {
  echo(`opening ${name}`);
  await open_project(project_id);
}

await reload_nodes(project_id);
