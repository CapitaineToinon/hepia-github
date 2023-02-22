#!/usr/bin/env zx

import "zx/globals";
import { CONFIG_D, GNS3_KEY } from "./constants.mjs";

async function removeKey() {
  const need = await fs.pathExists(GNS3_KEY);

  if (need) {
    await fs.remove(GNS3_KEY);
    echo(`removed ${GNS3_KEY}`);
  }
}

async function removeConfig() {
  const files = await glob(`${CONFIG_D}/gns3-*`);

  const promises = files.map(async (file) => {
    await fs.remove(file);
    echo(`removed ${file}`);
  });

  await Promise.all(promises);
}

await Promise.all([removeKey(), removeConfig()]);
