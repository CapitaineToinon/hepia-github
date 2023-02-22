#!/usr/bin/env zx
import "zx/globals";
import { BASE_URL, CONFIG_D, GNS3_KEY } from "./constants.mjs";

async function get_projects() {
  const response = await fetch(`${BASE_URL}/projects`);
  return await response.json();
}

function open_project(project_id) {
  return fetch(`${BASE_URL}/projects/${project_id}/open`, {
    method: "POST",
  });
}

function close_project(project_id) {
  return fetch(`${BASE_URL}/projects/${project_id}/close`, {
    method: "POST",
  });
}

async function get_nodes(project_id) {
  const response = await fetch(`${BASE_URL}/projects/${project_id}/nodes`);
  return await response.json();
}

function format_node(node) {
  return `Host ${node.name}
  User root
  Hostname ${node.console_host}
  Port ${node.console + 1}
  StrictHostKeychecking no
  UserKnownHostsFile /dev/null
  IdentityFile ~/.ssh/gns3.rsa`;
}

echo(`ensuring ${CONFIG_D} exists`);
await fs.mkdirp(CONFIG_D);

echo(`ensuring ${GNS3_KEY} exists`);
await $`chmod 400 gns3.rsa`;
await fs.copy("gns3.rsa", GNS3_KEY, { overwrite: true });

const projects = await get_projects();

const promises = projects.map(async ({ name, status, project_id }) => {
  const isOpen = status === "opened";
  const wasOpen = isOpen;

  if (!isOpen) {
    echo(`opening ${name}`);
    await open_project(project_id);
  }

  const nodes = await get_nodes(project_id);
  const output = nodes.map(format_node).join("\n\n");
  const path = `${CONFIG_D}/gns3-${name}`;

  fs.writeFile(path, output);
  echo(`wrote ${path}`);

  if (!wasOpen) {
    echo(`closing ${name}`);
    await close_project(project_id);
  }
});

await Promise.all(promises);
