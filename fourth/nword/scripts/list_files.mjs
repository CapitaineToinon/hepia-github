#!/usr/bin/env zx

import "zx/globals";
import { CONFIG_D } from "../constants.mjs";

const files = await glob(`${CONFIG_D}/gns3-*`);

for (const file of files) {
  echo(file);
}
