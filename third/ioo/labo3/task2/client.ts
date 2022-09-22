import {
  CuissonMenuFactory,
  PotageMenuFactory,
  SaladeMenuFactory,
  VapeurMenuFactory,
} from "./factories";

{
  const f = new CuissonMenuFactory();
  console.log(f.createLegume());
  console.log(f.createHuile());
  console.log(f.createEpice());
}

{
  const f = new PotageMenuFactory();
  console.log(f.createLegume());
  console.log(f.createHuile());
  console.log(f.createEpice());
}

{
  const f = new SaladeMenuFactory();
  console.log(f.createLegume());
  console.log(f.createHuile());
  console.log(f.createEpice());
}

{
  const f = new VapeurMenuFactory();
  console.log(f.createLegume());
  console.log(f.createHuile());
  console.log(f.createEpice());
}
