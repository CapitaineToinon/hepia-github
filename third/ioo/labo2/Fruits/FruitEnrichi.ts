import { CaloriesEnrichi } from "../Calories/CaloriesEnrichi";
import { FruitType } from "./FruitType";
import { Fruit } from "./Fruit";

export class FruitEnrichi extends FruitType {
	constructor() {
		super({ nom: "FruitEnrichi" });
	}

	getCalories(fruit: Fruit): number {
		return CaloriesEnrichi.returnCalories(fruit);
	}
}
