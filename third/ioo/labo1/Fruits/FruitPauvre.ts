import { CaloriesPauvre } from "../Calories/CaloriesPauvre";
import { FruitType } from "./FruitType";
import { Fruit } from "./Fruit";

export class FruitPauvre extends FruitType {
	constructor() {
		super({ nom: "FruitPauvre" });
	}

	getCalories(fruit: Fruit): number {
		return CaloriesPauvre.getCalories(fruit);
	}
}
