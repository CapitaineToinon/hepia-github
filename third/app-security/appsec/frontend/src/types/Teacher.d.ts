import { GormModel } from "./GormModel";

export type Teacher = GormModel & {
  lastname: string;
  name: string;
  class: string;
};

export type TeacherRegister = {
  lastname: string;
  name: string;
  class: string;
};
