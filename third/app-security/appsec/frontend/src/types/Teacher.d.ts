import { GormModel } from "./GormModel";

export type Teacher = GormModel & {
  lastname: string;
  name: string;
  class: string;
};
