#!/usr/bin/env zx
import "zx/globals";
import { get_projects, close_project } from "../projects/index.mjs";

const projects = await get_projects();
const promises = projects.map((project) => close_project(project.project_id));

await Promise.all(promises);
