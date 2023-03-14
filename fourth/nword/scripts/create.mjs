#!/usr/bin/env zx
import "zx/globals";
import { CONFIG_D, GNS3_KEY } from "../constants.mjs";
import { get_nodes, format_node } from "../nodes/index.mjs";
import { close_project } from "../projects/index.mjs";
import { open_project } from "../project/index.mjs";
import { get_project } from "../project/index.mjs";

const { project: project_name } = argv;

if (!project_name) {
  echo("project name is required");
  exit(1);
}

const project = await get_project(project_name);

echo(`ensuring ${CONFIG_D} exists`);
await fs.mkdirp(CONFIG_D);

echo(`ensuring ${GNS3_KEY} exists`);
await $`chmod 400 gns3.rsa`;
await fs.copy("gns3.rsa", GNS3_KEY, { overwrite: true });

const { name, status, project_id } = project;

const isOpen = status === "opened";
const wasOpen = isOpen;

if (!isOpen) {
  echo(`opening ${name}`);
  await open_project(project_id);
}

const nodes = await get_nodes(project_id);
const output = nodes.map(format_node).join("\n\n");
const path = `${CONFIG_D}/gns3-current-project`;

fs.writeFile(path, output);
echo(`wrote ${path}`);

if (!wasOpen) {
  echo(`closing ${name}`);
  await close_project(project_id);
}

echo(output);
