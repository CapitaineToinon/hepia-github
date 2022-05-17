import { Fruit } from "../Fruits/Fruit";

export class CaloriesEnrichi {
	static #table = new Map<string, number>([
		["pomme", 5000],
		["banane", 1000],
	]);

	public static returnCalories(fruit: Fruit): number {
		return CaloriesEnrichi.#table.get(fruit.nom) ?? -1;
	}
}
