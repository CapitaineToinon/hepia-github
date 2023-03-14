#!/usr/bin/env zx
import "zx/globals";
import { get_nodes } from "../nodes/index.mjs";
import { get_project } from "../project/index.mjs";

const { project: project_name } = argv;

if (!project_name) {
  echo("project name is required");
  exit(1);
}

const { project_id } = await get_project(project_name);
const nodes = await get_nodes(project_id);

const commands = nodes
  .map((node) => `ssh ${node.name} hostname ${node.name}`)
  .join(`\n`);

const output = `#!/bin/bash
  
${commands}`;

await $`touch hostnames.sh`;
await $`echo ${output} > hostnames.sh`;
await $`chmod +x hostnames.sh`;
