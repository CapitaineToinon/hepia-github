export class Legume {}
export class Salade extends Legume {}
export class Potage extends Legume {}
export class Vapeur extends Legume {}
export class Cuisson extends Legume {}

export enum LegumeType {
  PourSalade,
  PourPotage,
  PourVapeur,
  PourCuisson,
}

export class LegumeFactory {
  static buildLegume(type: LegumeType): Legume {
    switch (type) {
      case LegumeType.PourSalade:
        return new Salade();
      case LegumeType.PourPotage:
        return new Potage();
      case LegumeType.PourVapeur:
        return new Vapeur();
      case LegumeType.PourCuisson:
        return new Cuisson();
      default:
        throw new Error("unsupported");
    }
  }
}
