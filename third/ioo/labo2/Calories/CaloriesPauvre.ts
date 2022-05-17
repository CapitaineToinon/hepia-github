import { Fruit } from "../Fruits/Fruit";

export class CaloriesPauvre {
	static #table = new Map<string, number>([
		["pomme", 50],
		["banane", 60],
	]);

	public static getCalories(fruit: Fruit): number {
		return CaloriesPauvre.#table.get(fruit.nom);
	}
}
