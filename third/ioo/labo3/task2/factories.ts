import {
  Epice,
  EpicePourCuisson,
  EpicePourPotage,
  EpicePourSalade,
  EpicePourVapeur,
} from "./epices";
import {
  Huile,
  HuilePourCuisson,
  HuilePourPotage,
  HuilePourSalade,
  HuilePourVapeur,
} from "./huiles";
import { Cuisson, Legume, Potage, Salade, Vapeur } from "./legumes";

abstract class AbstractMenu {
  abstract createLegume(): Legume;
  abstract createHuile(): Huile;
  abstract createEpice(): Epice;
}

export class SaladeMenuFactory extends AbstractMenu {
  createLegume(): Legume {
    return new Salade();
  }

  createHuile(): Huile {
    return new HuilePourSalade();
  }

  createEpice(): Epice {
    return new EpicePourSalade();
  }
}
export class PotageMenuFactory extends AbstractMenu {
  createLegume(): Legume {
    return new Potage();
  }

  createHuile(): Huile {
    return new HuilePourPotage();
  }

  createEpice(): Epice {
    return new EpicePourPotage();
  }
}
export class VapeurMenuFactory extends AbstractMenu {
  createLegume(): Legume {
    return new Vapeur();
  }

  createHuile(): Huile {
    return new HuilePourVapeur();
  }

  createEpice(): Epice {
    return new EpicePourVapeur();
  }
}
export class CuissonMenuFactory extends AbstractMenu {
  createLegume(): Legume {
    return new Cuisson();
  }

  createHuile(): Huile {
    return new HuilePourCuisson();
  }

  createEpice(): Epice {
    return new EpicePourCuisson();
  }
}
