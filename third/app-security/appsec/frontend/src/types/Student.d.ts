import { GormModel } from "./GormModel";

export type Student = GormModel & {
  lastname: string;
  name: string;
  filiere: string;
};

export type StudentRegister = {
  lastname: string;
  name: string;
  filiere: string;
};
