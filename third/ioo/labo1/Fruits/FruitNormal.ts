import { CaloriesNormal } from "../Calories/CaloriesNormal";
import { FruitType } from "./FruitType";
import { Fruit } from "./Fruit";

export class FruitNormal extends FruitType {
	constructor() {
		super({ nom: "FruitNormal" });
	}

	getCalories(fruit: Fruit): number {
		return CaloriesNormal.getCal(fruit);
	}
}
