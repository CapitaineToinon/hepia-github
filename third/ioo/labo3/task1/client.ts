import {
  LegumeFactory,
  LegumeType,
  Potage,
  Salade,
  Vapeur,
  Cuisson,
} from "../task1/Factory";

const legume = LegumeFactory.buildLegume(LegumeType.PourVapeur);

console.log({
  isPotage: legume instanceof Potage,
  isVapeur: legume instanceof Vapeur,
  isSalade: legume instanceof Salade,
  isCuisson: legume instanceof Cuisson,
});
