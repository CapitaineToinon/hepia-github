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
const hosts = nodes.filter((node) => node.name.toLowerCase().startsWith("h"));

if (hosts.length === 0) {
  echo("no nodes found");
  exit(1);
}

const map = new Map();

const ipAddress = hosts
  .map((host, i) => {
    const index = i + 1;
    map.set(host.name, index);
    return `ssh ${host.name} ip address add 192.168.0.${index}/24 dev eth0
ssh ${host.name} ip link set eth0 addr 00:00:00:00:00:${index}
ssh ${host.name} ip link set dev eth0 up`;
  })
  .join(`\n\n`);

const master = hosts[0];

const pings = hosts
  .filter((host) => host.name !== master.name)
  .map((host) => {
    const index = map.get(host.name);
    return `ssh ${master.name} ping 192.168.0.${index} -c 1`;
  })
  .join(`\n`);

const output = `#!/bin/bash
  
echo "adding ip addresses"
${ipAddress}

echo "pinging"
${pings}`;

await $`touch ipv4.sh`;
await $`echo ${output} > ipv4.sh`;
await $`chmod +x ipv4.sh`;
